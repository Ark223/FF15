
#include "Process.h"

namespace Evade
{
    Process* Process::m_instance = nullptr;

    void Process::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Process* Process::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Process();
            m_instance->api = API::Get();
            m_instance->data = Data::Get();
            m_instance->program = Program::Get();
            m_instance->hero = API::Get()->GetHero();
        }
        return m_instance;
    }

    Linq<OnProcSpellArgs> Process::AddSkillshots(Skillshot* skillshot)
    {
        API* api = this->api;
        auto result = Linq<OnProcSpellArgs>();
        const auto& active_data = skillshot->Get();

        float range = active_data.Range;
        const auto& caster = active_data.Caster;
        const Vector& dir = active_data.Direction;
        const Vector& dest = active_data.DestPos;
        const Vector& ending = active_data.EndPos;
        const Vector& start = active_data.StartPos;
        const Vector& perp = active_data.Perpendicular;

        const std::string& name = active_data.SkillshotName;
        const DetectionType type = active_data.DetectionType;
        const auto& data = this->data->GetSkillshots().at(name);
        bool proc_type = type == DetectionType::ON_PROCESS_SPELL;

        // Find and handle skillshot's relationships
        const auto& connectors = this->data->GetConnectors();
        if (auto it = connectors.find(name); it != connectors.end())
        {
            const auto& [key, connector] = *it;
            std::string next_name = connector.PrimaryName;
            if (connector.ConnectType == ConnectionType::EXTEND_LENGTH)
            {
                result.Append({ caster, next_name, ending.Clone(),
                    ending + dir, skillshot->TotalLifeTime(), false });
            }
            else if (connector.ConnectType == ConnectionType::FOLLOW_ORIGIN)
            {
                result.Append({ caster, next_name, start, ending.Clone() });
            }
        }

        // Rotated skillshots fired in a cone pattern
        if (data.MultiAngle != 0 && proc_type)
        {
            int cone_angle = data.MultiAngle;
            int mis_count = data.MultiCount / 2;
            float step = (float)cone_angle / mis_count;
            for (int i = mis_count; i >= -mis_count; --i)
            {
                float angle = M_RAD(i * step);
                if (ABS(angle) <= 0.01f) continue;
                Vector rotated = ending.Rotate(angle, start);
                result.Append({ caster, name, start, rotated });
            }
        }

        // Heimerdinger W
        else if (name == "HeimerdingerW" && proc_type)
        {
            Vector offsets[4] =
            {
                Vector(-180.0f, 110.0f), Vector(-105.0f, 30.0f),
                Vector(105.0f, 30.0f), Vector(180.0f, 110.0f)
            };
            for (const Vector& offset : offsets)
            {
                Vector pos = start + perp * offset.x + dir * offset.y;
                result.Append({ caster, name, pos, dest.Clone() });
            }
        }

        // Heimerdinger RE
        else if (name == "HeimerdingerEUlt")
        {
            float time = skillshot->TotalLifeTime();
            Vector land_one = ending + dir * 270.0f;
            Vector land_two = land_one + dir * 270.0f;
            result.Append({ caster, name, ending, land_one, time });
            result.Append({ caster, name, ending, land_two, time });
        }

        // Hwei EE
        else if (name == "HweiEE")
        {
            float side_range = 350.0f, phi = M_RAD(105.0f);

            // Jaw snap one
            Vector rot_one = dir.Rotate(phi);
            Vector start_one = ending + rot_one * side_range;
            Vector ending_one = ending - rot_one * side_range;
            result.Append({ caster, name, start_one, ending_one });

            // Jaw snap two
            Vector rot_two = dir.Rotate(-phi);
            Vector start_two = ending + rot_two * side_range;
            Vector ending_two = ending - rot_two * side_range;
            result.Append({ caster, name, start_two, ending_two });
        }

        // Jarvan IV EQ
        else if (name == "JarvanIVDragonStrike")
        {
            auto& skillshots = this->program->GetSkillshots();
            auto flags = skillshots.Where([&](Skillshot* skillshot)
            {
                const auto& unit = skillshot->Get().Caster;
                const std::string& id = skillshot->Get().SkillshotName;
                std::string eq_spell_name = "JarvanIVDemacianStandard";
                return api->Compare(unit, caster) && id == eq_spell_name;
            });
            if (flags.Any())
            {
                const Vector& flag = flags.First()->Get().Position;
                bool collides = flag.DistSqrToSegment(start, ending) <= 215 * 215;
                if (collides) result.Append({ caster, "JarvanIVEQ", start, flag });
            }
        }

        // Mel E Field
        else if (name == "MelE")
        {
            Vector field_start = ending - dir * 525.0f;
            result.Append({ caster, "MelEFieldTail", field_start, field_start });
            result.Append({ caster, "MelEFieldTail", ending, ending, 0.45f });
        }

        // Sylas Q
        else if (name == "SylasQ")
        {
            Vector start_one = start - perp * 125.0f;
            Vector start_two = start + perp * 125.0f;
            result.Append({ caster, "SylasQ2", start_one, ending });
            result.Append({ caster, "SylasQ2", start_two, ending });
        }

        // Syndra E
        else if (name == "SyndraE")
        {
            // Gather all active Syndra's spheres 
            auto& skillshots = this->program->GetSkillshots();
            auto& seeds = this->program->GetObjects().at("Seed");
            auto spheres = skillshots.Where([&](Skillshot* skillshot)
            {
                const auto& unit = skillshot->Get().Caster;
                const std::string& id = skillshot->Get().SkillshotName;
                return api->Compare(unit, caster) && id == "SyndraQ";
            })
            .Select<Vector>([](Skillshot* skillshot)
            {
                return skillshot->Get().Position;
            })
            .Concat(seeds.Select<Vector>([&](auto& object)
            {
                return api->GetPosition(object);
            }));

            // Predict any possible missile trajectory
            std::string id = "SyndraESphereMissile";
            for (const Vector& sphere : spheres)
            {
                if (!skillshot->IsDangerous(sphere)) continue;
                float distance = start.Distance(sphere) - 200.0f;
                float extension = distance > 0 ? 1300.0f : 1000.0f;
                Vector destination = start.Extend(sphere, extension);
                result.Append({ caster, id, sphere, destination });
            }
        }

        // Taric E
        else if (name == "TaricE")
        {
            auto units = api->GetEnemyHeroes(FLT_MAX, Vector(), true);
            auto candidates = units.Where([&](const auto& unit)
            {
                bool is_owner = api->Compare(unit, caster);
                return !is_owner && api->HasBuff(unit, 0x9F0DE030);
            });
            if (candidates.Any())
            {
                const auto& unit = candidates.First();
                Vector pos = api->GetPosition(unit);
                result.Append({ unit, name, pos, dest });
            }
        }

        // Velkoz Q Implosion
        else if (name == "VelkozQImplosion")
        {
            auto& skillshots = this->program->GetSkillshots();
            auto missile = skillshots.FirstOrDefault([&](Skillshot* skillshot)
            {
                const auto& s_caster = skillshot->Get().Caster;
                const std::string& s_name = skillshot->Get().SkillshotName;
                bool same_caster = this->api->Compare(caster, s_caster);
                return same_caster && s_name == "VelkozQ";
            });
            if (missile != nullptr)
            {
                const Vector& split = missile->Get().Perpendicular;
                Vector split_one = start + split, split_two = start - split;
                result.Append({ caster, "VelkozQSplit", start, split_one });
                result.Append({ caster, "VelkozQSplit", start, split_two });
                skillshots.Delete(missile);
            }
        }

        // Xayah E
        else if (name == "XayahE")
        {
            auto& objects = this->program->GetObjects();
            objects.at("Feather").ForEach([&](auto& object)
            {
                Vector feather = api->GetPosition(object);
                result.Append({ caster, name, feather, start });
            });
        }

        // Ziggs Q
        else if (name == "ZiggsQ")
        {
            Vector bomb_one = start + dir * (range * 1.4f);
            float time = skillshot->TotalLifeTime() - 0.25f;
            float extension = ending.Distance(bomb_one) * 1.69f;
            Vector bomb_two = ending.Extend(bomb_one, extension);
            result.Append({ caster, name, ending, bomb_one, time + 0.495f });
            result.Append({ caster, name, bomb_one, bomb_two, time + 0.99f });
        }

        // Zed Q
        else if (name == "ZedQ" && proc_type)
        {
            auto& objects = this->program->GetObjects();
            objects.at("Shadow").ForEach([&](auto& object)
            {
                Vector shadow = api->GetPosition(object);
                result.Append({ caster, name, shadow, dest });
            });
        }

        return result;
    }

    Skillshot* Process::CreateSkillshot(ActiveData& data, bool test)
    {
        int slot = data.SkillshotSlot;
        int type = (int)data.DamageType;
        auto& dmg_func = data.DamageFunc;
        auto& unit = data.Caster;

        // Populate data with various statistics
        StatData stats{};
        API* api = this->api;
        float time = api->GetTime();
        stats.Armor = api->GetArmor(unit);
        stats.BonusArmor = api->GetBonusArmor(unit);
        stats.BonusSpellBlock = api->GetBonusMagicResistance(unit);
        stats.BonusAbilityDamage = api->GetBonusAbilityDamage(unit);
        stats.BonusAttackDamage = api->GetBonusAttackDamage(unit);
        stats.BonusHealth = api->GetBonusHealth(unit);
        stats.BonusMana = api->GetBonusMana(unit);
        stats.Health = api->GetHealth(unit);
        stats.Lethality = api->GetLethality(unit);
        stats.MaxHealth = api->GetMaxHealth(unit);
        stats.MaxMana = api->GetMaxMana(unit);
        stats.SpellBlock = api->GetMagicResistance(unit);
        stats.SpellLevel = api->GetSpellLevel(unit, slot) - 1;
        stats.TargetHealth = api->GetHealth(this->hero);
        stats.TargetMaxHealth = api->GetMaxHealth(this->hero);
        stats.TotalAbilityDamage = api->GetTotalAbilityDamage(unit);
        stats.TotalAttackDamage = api->GetTotalAttackDamage(unit);
        stats.UnitLevel = api->GetLevel(unit) - 1;
        stats.UnitData = unit;

        // Set the additional properties for skillshot
        data.ArcStep = 25.0f; data.StartTime = time;

        // Calculate adjusted damage based on its type
        data.Damage = test ? 0.0f : api->CalcDamage(
            this->hero, unit, dmg_func(stats), type);

        // Create and return the appropriate skillshot
        switch (data.SkillshotType)
        {
            case SkillshotType::CIRCLE: return new Circle(data);
            case SkillshotType::CONE: return new Cone(data);
            case SkillshotType::LINE: return new Line(data);
            case SkillshotType::POLYGON: return new Polygon(data);
            case SkillshotType::RING: return new Ring(data);
        }
        return nullptr;
    }

    void Process::PredictCollision(Skillshot* skillshot, bool fix)
    {
        // Retrieve data from active skillshot
        const auto& active_data = skillshot->Get();

        float delay = active_data.Delay;
        float range = active_data.Range;
        float radius = active_data.Radius;
        const Vector& start = active_data.StartPos;
        const Vector& ending = active_data.EndPos;
        const Vector& hero_pos = this->program->GetHeroPos();
        auto minions = this->api->GetAllyMinions(range, hero_pos);

        // Fast collision detection algorithm for stationary positions
        auto results = minions.Select<Vector>([&](const auto& minion)
        {
            if (this->api->IsMoving(minion)) return Vector();
            const Vector& pos = this->api->GetPosition(minion);
            Vector closest = pos.ClosestOnSegment(start, ending);
            float dist = closest.DistanceSquared(pos);
            float hitbox = this->api->GetHitbox(minion);
            bool inside = dist <= (hitbox + radius) * (hitbox + radius);
            return (inside && start != closest) ? closest : Vector();
        })
        .Where([](const Vector& point)
        {
            return point.IsValid();
        })
        .OrderBy<float>([&](const Vector& point)
        {
            return point.DistanceSquared(start);
        });

        // Update skillshot properties
        if (results.Count() == 0) return;
        const Vector& point = results.First();
        float distance = point.Distance(start);
        skillshot->Set().FixedRange = false;
        skillshot->Set().Range = distance;
        skillshot->Set().DestPos = point;
        if (fix) skillshot->FixOrigin();
    }

    void Process::UpdateAcceleration(Skillshot* skillshot)
    {
        const std::string& name = skillshot->Get().SkillshotName;
        const auto& data = this->data->GetSkillshots().at(name);
        float accel = data.Acceleration, speed = data.InitialSpeed;
        float max_speed = data.MaxSpeed, min_speed = data.MinSpeed;

        float time = this->api->GetTime();
        float delay = skillshot->Get().Delay;
        float start_time = skillshot->Get().StartTime;

        // As per formula: s = v0 * t + 1/2 * a * t ^ 2
        float delta = MAX(0.0f, -start_time + time - delay);
        speed = MAX(50.0f, speed + 0.5f * accel * delta);
        speed = MAX(min_speed, MIN(max_speed, speed));
        skillshot->Set().Speed = speed;
    }

    bool Process::UpdateData(Skillshot* skillshot, int level)
    {
        API* api = API::Get(); Poly poly;
        CollisionFlag flag = CollisionFlag::MINION;
        const auto& active_data = skillshot->Get();

        float delay = active_data.Delay;
        float speed = active_data.Speed;
        float range = active_data.Range;
        float radius = active_data.Radius;

        const auto& caster = active_data.Caster;
        const Vector& dir = active_data.Direction;
        const Vector& dest = active_data.DestPos;
        const Vector& ending = active_data.EndPos;
        const Vector& start = active_data.StartPos;
        const Vector& perp = active_data.Perpendicular;

        const Collisions& flags = active_data.Collisions;
        const std::string& name = active_data.SkillshotName;
        const DetectionType type = active_data.DetectionType;
        bool proc_type = type == DetectionType::ON_PROCESS_SPELL;
        
        // Predict the collision between a minion and a skillshot
        if (std::find(flags.begin(), flags.end(), flag) != flags.end())
        {
            this->PredictCollision(skillshot, true);
        }

        // Handle custom skillshots and adjust properties
        if (name == "Volley" && level > 0)
        {
            float angles[] = { 30, 35, 40, 45, 50 };
            float angle = M_RAD(angles[level - 1]) / 2.0f;
            float offset = -radius * 2.0f / std::sinf(angle);
            skillshot->Set().Delay = delay + offset / speed;
            skillshot->Set().ConeAngle = 2.0f * angle;
            skillshot->Set().Offset = offset;
            skillshot->FixOrigin();
        }
        else if (name == "GravesRExplosion")
        {
            poly.Add(ending - perp * (radius + 240.0f));
            poly.Add(ending + perp * (radius + 240.0f));
            poly.Add(start + perp * radius);
            poly.Add(start - perp * radius);
            skillshot->Set().Polygon = poly;
        }
        else if (name == "HeimerdingerW" && level > 0)
        {
            skillshot->Set().Offset = -200.0f;
            skillshot->Set().Range = 1125.0f;
        }
        else if (name == "IreliaEMissile")
        {
            auto& objects = this->program->GetObjects();
            if (objects["Blade"].Count() == 0)
            {
                delete skillshot; return false;
            }
            auto object = objects["Blade"].Last();
            Vector blade = api->GetPosition(object);
            float distance = start.Distance(ending);
            delay = distance / speed + 0.4f;
            skillshot->Set().Delay = delay;
            skillshot->Set().Range = 12500.0f;
            skillshot->Set().Speed = FLT_MAX;
            skillshot->Set().StartPos = blade;
        }
        else if (name == "MordekaiserE") // Check distance
        {
            Vector new_start = start.Append(ending, 250.0f);
            Vector new_dest = new_start.Extend(start, 850.0f);
            float distance = new_start.Distance(new_dest);
            skillshot->Set().Range =  distance;
            skillshot->Set().DestPos = new_dest;
            skillshot->Set().StartPos = new_start;
        }
        else if (name == "LuxR" && !proc_type)
        {
            skillshot->Translate(-dir * 150.0f);
        }
        else if (name == "PykeR")
        {
            float len = 250.0f;
            poly.Add(ending + Vector(len + radius, -len + radius));
            poly.Add(ending + Vector(len - radius, -len - radius));
            poly.Add(ending + Vector(0.0f, -2.0f * radius));
            poly.Add(ending + Vector(-len + radius, -len - radius));
            poly.Add(ending + Vector(-len - radius, -len + radius));
            poly.Add(ending + Vector(-2.0f * radius, 0.0f));
            poly.Add(ending + Vector(-len - radius, len - radius));
            poly.Add(ending + Vector(-len + radius, len + radius));
            poly.Add(ending + Vector(0.0f, 2.0f * radius));
            poly.Add(ending + Vector(len - radius, len + radius));
            poly.Add(ending + Vector(len + radius, len - radius));
            poly.Add(ending + Vector(2.0f * radius, 0.0f));
            skillshot->Set().Polygon = poly;
        }
        else if (name == "Tremors2" && level > 0) // Rammus R
        {
            speed = api->GetMovementSpeed(caster) * 1.5f;
            skillshot->Set().Range = MAX(800.0f, speed);
        }
        else if (name == "LeeSinR" || name == "RellW2")
        {
            skillshot->Set().StartPos = ending;
            skillshot->Set().EndPos = ending + dir * range;
        }
        else if (name == "ShenE")
        {
            skillshot->Set().DestPos = start + dir * 750.0f;
            skillshot->Set().Range = MAX(300.0f, range) + 150.0f;
        }
        else if (name == "SionQ")
        {
            poly.Add(ending - perp * (radius + 110.0f));
            poly.Add(ending + perp * (radius + 110.0f));
            poly.Add(start + perp * radius);
            poly.Add(start - perp * radius);
            skillshot->Set().Polygon = poly;
        }
        else if (name == "SettW")
        {
            poly.Add(ending - perp * radius);
            poly.Add(ending + perp * radius);
            poly.Add(start + perp * 150.0f);
            poly.Add(start - perp * 150.0f);
            skillshot->Set().Polygon = poly;
        }
        else if (name == "SkarnerR")
        {
            poly.Add(ending + perp * (radius - 75.0f));
            poly.Add(ending - perp * (radius - 75.0f));
            poly.Add(start - perp * radius);
            poly.Add(start + perp * radius);
            skillshot->Set().Polygon = poly;
        }
        else if (name == "SylasQ")
        {
            float distance = MAX(175.0f, range);
            Vector pos = start + dir * distance;
            skillshot->Set().DestPos = pos;
            skillshot->Set().Range = distance;
        }
        else if (name == "TahmKenchQ" && level > 0)
        {
            float distance = 11.25f * api->GetHitbox(caster);
            skillshot->Set().Range = MAX(range, distance);
        }
        else if (name == "TaricE" || name == "SejuaniW")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Range = range + speed;
            skillshot->Set().Radius = radius + speed;
        }
        else if (name == "WarwickR")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Range = 2.5f * speed;
        }
        else if (name == "VexR" && level > 0)
        {
            skillshot->Set().Range = 1500.0f + 500.0f * level;
        }
        else if (name == "YorickE")
        {
            poly.Add(ending + perp * (radius + 25.0f) + dir * 400.0f);
            poly.Add(ending - perp * (radius + 25.0f) + dir * 400.0f);
            poly.Add(ending - perp * radius - dir * 125.0f);
            poly.Add(ending + perp * radius - dir * 125.0f);
            skillshot->Set().Polygon = poly;
        }
        else if (name == "ZoeQMis2")
        {
            skillshot->Set().DestPos = ending + dir * 350.0f;
            skillshot->Set().Range = range + 350.0f;
        }
        else if (name == "ZoeE" && proc_type)
        {
            skillshot->Set().Range = 800.0f;
        }

        // Adjust skillshot's origin points
        if (range != skillshot->Get().Range)
        {
            skillshot->FixOrigin();
            range = skillshot->Get().Range;
        }

        // Adjust various numeric properties
        if (name == "AurelionE" || name == "AurelionR")
        {
            int stacks = api->GetBuffCount(caster, 0x42E9ABCB);
            skillshot->Set().Radius = std::sqrtf(75625 + 286.625f * stacks);
        }
        else if (name == "AurelionSolR2")
        {
            int stacks = api->GetBuffCount(caster, 0x42E9ABCB);
            skillshot->Set().Radius = std::sqrtf(151251 + 477.423f * stacks);
        }
        else if (name == "BriarE" && proc_type)
        {
            float windup = api->GetAttackWindup(caster);
            skillshot->Set().Delay = delay + windup;
        }
        else if (name == "CaitlynW" && level > 0)
        {
            float durations[] = { 30, 35, 40, 45, 50 };
            skillshot->Set().ExtraTime = durations[level - 1];
        }
        else if (name == "CamilleEDash2")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Speed = speed + 1050.0f;
        }
        else if (name == "GalioQ")
        {
            range = MIN(825.0f, range) / 1000.0f;
            skillshot->Set().Delay = 0.7f * range + 0.45f;
        }
        else if (name == "GravesQDetonation")
        {
            skillshot->Set().Delay = delay + 1.0f;
        }
        else if (name == "HweiEW")
        {
            skillshot->Set().Delay = delay + 0.7f;
        }
        else if (name == "HowlingGaleSpell") // Janna Q
        {
            skillshot->Set().Speed = 0.8f * range;
        }
        else if (name == "JarvanIVEQ") // Jarvan EQ
        {
            skillshot->Set().Speed = range / 0.25f;
        }
        else if (name == "JinxW" && proc_type)
        {
            float mod = api->GetAttackSpeedMod(caster) - 1.0f;
            skillshot->Set().Delay = 0.6f - 0.08f * mod;
        }
        else if (name == "UFSlash") // Malphite R
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Speed = speed + 1500.0f;
        }
        else if (name == "NunuWSnowballMissile")
        {
            radius = 0.125f * range + 6.25f;
            speed = 0.30f * range + 235.0f;
            skillshot->Set().Radius = MAX(75.0f, radius);
            skillshot->Set().Speed = MAX(400.0f, speed);
        }
        else if (name == "RenektonSliceAndDice")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Speed = speed + 760.0f;
        }
        else if (name == "ShenE")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Speed = speed + 800.0f;
        }
        else if (name == "SyndraESphereMissile" && proc_type)
        {
            Vector position = api->GetPosition(caster);
            float distance = position.Distance(start);
            delay = 0.25f + distance / 2500.0f;
            skillshot->Set().Delay = delay;
        }
        else if (name == "TahmKenchQ")
        {
            skillshot->Set().Speed = range * speed / 900.0f;
        }
        else if (name == "UrgotE")
        {
            speed = api->GetMovementSpeed(caster);
            skillshot->Set().Speed = speed + 1200.0f;
        }
        else if (name == "WarwickR")
        {
            skillshot->Set().Speed = range / 0.6f;
        }
        else if (name == "VelkozWEruption")
        {
            auto missile = this->program->GetSkillshots().Last();
            float life_time = missile->TotalLifeTime();
            skillshot->Set().Delay = life_time + 0.75f;
        }
        else if (name == "VelkozE" && proc_type)
        {
            range = MIN(800.0f, range);
            float extra_delay = 0.25f + (range > 150.0f ?
                0.3f * (range - 150.0f) / 650.0f : 0.0f);
            skillshot->Set().Delay = delay + extra_delay;
        }
        else if (name == "VexE")
        {
            float hitbox = radius + 0.125f * range;
            skillshot->Set().Radius = MIN(300.0f, hitbox);
        }
        else if (name == "XayahQ" && proc_type)
        {
            float mod = api->GetAttackSpeedMod(caster) - 1.0f;
            float extra_delay = MAX(0.1f, delay - 0.07f * mod);
            skillshot->Set().Delay = 0.15f + extra_delay;
        }
        else if (name == "ZiggsQ" && level <= 0)
        {
            radius = delay > 1.245f ? 240.0f : 180.0f;
            skillshot->Set().Radius = radius;
            skillshot->Set().Speed = FLT_MAX;
        }
        skillshot->Update(true);
        return true;
    }

    Linq<Skillshot*> Process::UpdateConsidered()
    {
        bool can_evade = this->program->CanEvade();
        bool dodge_on = this->program->GetValue<bool>("Dodge");
        if (!can_evade || !dodge_on) return Linq<Skillshot*>();

        bool only_danger = this->program->DodgeDangerous();
        bool dodge_fog = this->program->GetValue<bool>("DodgeFog");
        float hp_percent = api->GetHealthRatio(this->hero) * 100.0f;

        // Filter skillshots to find those to evade
        auto& supported = this->data->GetSkillshots();
        auto& skillshots = this->program->GetSkillshots();
        return skillshots.Where([&](Skillshot* skillshot)
        {
            auto& name = skillshot->Get().SkillshotName;
            bool processed = skillshot->Get().Processed;
            bool fog_of_war = skillshot->Get().FogOfWar;
            bool fog_supp = supported.at(name).FogSupport;

            bool s_fog = fog_supp && this->program->GetValue<bool>("D|" + name + "|Fog");
            bool s_dangerous = this->program->GetValue<bool>("D|" + name + "|Dangerous");
            bool s_dodge_on = this->program->GetValue<bool>("D|" + name + "|Dodge");
            int s_hp_percent = this->program->GetValue<int>("D|" + name + "|Health");

            return s_dodge_on && !processed && (!fog_of_war || (s_fog && dodge_fog))
                && hp_percent <= s_hp_percent && (!only_danger || s_dangerous);
        });
    }

    void Process::UpdateSkillshots()
    {
        API* api = API::Get();

        // Remove all skillshots that have expired
        auto& skillshots = this->program->GetSkillshots();
        skillshots.DeleteAll([&](Skillshot* skillshot)
        {
            const auto& caster = skillshot->Get().Caster;
            const Vector& ending = skillshot->Get().EndPos;
            const Vector& position = skillshot->Get().Position;
            const std::string& name = skillshot->Get().SkillshotName;
            const SkillshotType type = skillshot->Get().SkillshotType;

            uint32_t h1 = 0xC76B603D, h2 = 0xF8F52C78; // Sion Q buffs
            bool sion_stop = name == "SionQ" && api->IsValid(caster) &&
                (api->HasBuff(caster, h1) || api->HasBuff(caster, h2));
            return skillshot->IsExpired() || type == SkillshotType::LINE
                && position.DistanceSquared(ending) <= 100 || sion_stop;
        });

        // Iterate through each skillshot to update its state
        skillshots.ForEach([&](Skillshot* skillshot)
        {
            bool force_update = false;
            const auto& active_data = skillshot->Get();

            float delay = active_data.Delay;
            float range = active_data.Range;
            float speed = active_data.Speed;
            float radius = active_data.Radius;

            const auto& caster = active_data.Caster;
            const auto& object = active_data.ObjectPtr;
            const Vector& dir = active_data.Direction;
            const Vector& pos = active_data.Position;
            const Vector& start = active_data.StartPos;
            const Vector& origin = active_data.OriginPos;

            const std::string& name = active_data.SkillshotName;
            const DetectionType type = active_data.DetectionType;
            const auto& data = this->data->GetSkillshots().at(name);
            bool proc_type = type == DetectionType::ON_PROCESS_SPELL;

            float timer = active_data.StartTime;
            float elapsed = api->GetTime() - timer;
            float distance = start.Distance(pos);

            // Update skillshot's hitbox by following the caster
            if (data.FollowCaster && api->IsValid(caster))
            {
                force_update = true;
                Vector dir = api->GetPosition(caster) - start;
                float speed = api->GetMovementSpeed(caster);
                float left = MAX(0.0f, 1.0f - elapsed);
                if (name == "TaricE")
                {
                    skillshot->Set().Radius = 70.0f + left * speed;
                    skillshot->Set().Range = 610.0f + left * speed;
                    skillshot->FixOrigin();
                }
                else if (name == "SejuaniW")
                {
                    skillshot->Set().Radius = 60.0f + left * speed;
                    skillshot->Set().Range = 600.0f + left * speed;
                    skillshot->FixOrigin();
                }
                if (elapsed <= delay) skillshot->Translate(dir);
            }

            // Redirect skillshot back to the caster's position
            else if (data.BackToCaster && api->IsValid(caster))
            {
                skillshot->Set().DestPos = api->GetPosition(caster);
                skillshot->Set().Range = 12500.0f;
                skillshot->FixOrigin();
            }

            // Adjust trajectory if the caster moved during a cast
            else if (api->IsValid(caster) && proc_type && start == pos
                && api->IsWindingUp(caster) && elapsed < data.Windup &&
                api->GetPosition(caster).DistanceSquared(origin) > 2500)
            {
                this->program->RequestUpdateOnce(skillshot->GetId());
                skillshot->Set().StartPos = api->GetPosition(caster);
                skillshot->FixOrigin();
            }

            // Adjust Pyke E hitbox based on movement speed after initial delay
            else if (name == "PykeE" && api->IsValid(caster) && elapsed > 0.275f)
            {
                force_update = true;
                float left = MAX(0.0f, 1.0f - elapsed);
                float speed = api->GetMovementSpeed(caster);

                skillshot->Set().DestPos = api->GetPosition(caster);
                float dist = skillshot->Get().DestPos.Distance(pos);
                float buffer = (left + dist / 3000.0f) * speed;

                skillshot->Set().Radius = 110.0f + buffer;
                skillshot->Set().FixedRange = false;
                skillshot->Set().Range = 12500.0f;
                skillshot->FixOrigin();
            }
            
            // Cut global skillshots
            else if (data.IsGlobal)
            {
                force_update = true;
                float hitbox = api->GetHitbox(this->hero);
                float move_speed = api->GetMovementSpeed(this->hero);
                float threshold = (radius + hitbox) / move_speed + 1.5f;
                float offset = MIN(range - distance, threshold * speed);
                skillshot->Set().EndPos = pos + dir * offset;
            }

            // Adjust skillshot's speed dynamically based on various properties
            if (name == "EnchantedCrystalArrow" && proc_type && elapsed > 0.25f)
            {
                skillshot->Set().Speed = elapsed < 3.0f + 0.25f
                    ? (10.0f * std::sqrtf(5625.0f + distance) + 750.0f)
                    : (2100.0f * distance) / (distance + 900.0f);
            }
            else if (name == "JinxR" && proc_type && elapsed > 0.6f)
            {
                skillshot->Set().Speed = distance < 1350.0f ?
                    1700.0f : 2200.0f - 743250.0f / distance;
            }
            else if (data.FixSpeed && !proc_type && api->IsValid(object))
            {
                MissileClient missile = (MissileClient)object;
                skillshot->Set().Speed = api->GetMissileSpeed(missile);
                skillshot->Set().StartPos = api->GetPosition(missile);
                skillshot->Set().StartTime = api->GetTime();
                skillshot->FixOrigin();
            }
            else if (data.Acceleration != 0.0f)
            {
                this->UpdateAcceleration(skillshot);
            }

            // Apply final changes to hitbox
            skillshot->Update(force_update);
        });
    }
}
