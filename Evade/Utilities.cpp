
#include "Utilities.h"

namespace Evade
{
    Utilities* Utilities::m_instance = nullptr;

    void Utilities::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Utilities* Utilities::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Utilities();
            m_instance->api = API::Get();
            m_instance->data = Data::Get();
            m_instance->program = Program::Get();
            m_instance->hero = API::Get()->GetHero();
        }
        return m_instance;
    }

    static bool operator==(const SpellInfo& lhs, const SpellInfo& rhs)
    {
        return lhs.SpellName == rhs.SpellName;
    }

    // Utility methods

    bool Utilities::CanEvade() const
    {
        API* api = this->api;
        Data* data = this->data;
        const auto& hero = this->hero;
        BuffType stun = BuffType::Stun;

        // Avoid interrupting specific ultimate spells
        std::string hero_name = api->GetCharacterName(hero);
        std::string spell_name = api->GetSpellName(hero, 3);
        if (spell_name == "NunuR_Recast") return false;
        if (spell_name == "JhinRShot") return false;

        // Consider active buffs stopping the movement
        const auto& stoppers = data->GetStoppers();
        auto it = stoppers.find(hero_name);
        if (it != stoppers.end())
        {
            uint32_t buff = it->second.BuffHash;
            if (api->HasBuff(hero, buff)) return false;
        }

        // Final checks to confirm evasion ability
        if (api->IsDead(hero)) return false;
        if (api->IsImmortal(hero)) return false;
        if (api->HasBuffType(hero, stun)) return false;

        bool kaisa = hero_name == "Kaisa";
        if (kaisa) kaisa = api->HasBuff(hero, 0xDB58751F);
        if (api->IsDashing(hero) && !kaisa) return false;

        bool attacking = api->IsAttacking(hero);
        bool winding_up = api->IsWindingUp(hero);
        if (attacking == true) winding_up = false;
        if (winding_up && !kaisa) return false;

        return true;
    }

    void Utilities::DrawArrow(const Vector& start, const Vector& ending,
        float height, uint32_t color_a, uint32_t color_b)
    {
        size_t iter = 0;
        float length = start.Distance(ending);
        float bar_size = 30.0f, head_size = 25.0f;
        Vector direction = (ending - start).Normalize();

        // Draw the arrow body
        while (length > 0)
        {
            float seg_size = bar_size;
            if (!iter) seg_size *= 2.0f;

            float size = MIN(seg_size, length);
            Vector bar_one = start + direction * length;
            Vector bar_two = bar_one - direction * size;

            uint32_t argb = iter % 2 == 0 ? color_a : color_b;
            this->api->DrawLine(bar_one, bar_two, height, argb);
            length -= (seg_size + bar_size / 2.0f); iter++;
        }

        direction *= head_size;
        Vector perpendicular = direction.Perpendicular();
        Vector head_one = ending - direction + perpendicular;
        Vector head_two = ending - direction - perpendicular;

        // Draw the arrowhead lines at the end of arrow
        this->api->DrawLine(ending, head_one, height, color_a);
        this->api->DrawLine(ending, head_two, height, color_a);
    }

    bool Utilities::EvadeToSpot(const Vector& spot) const
    {
        // Return if spot is not defined
        if (!spot.IsValid()) return false;

        // No need to repeat an action if we already follow the path
        Vector dest = this->api->GetPathEndPos(this->hero), offset;
        if (dest.DistanceSquared(spot) < 25 * 25) return false;

        // Adjust evading position to prevent the core
        // from blocking orders due to repeated requests
        switch (this->program->GetOrderCount())
        {
            case 0: offset.x = 5.0f; break;
            case 1: offset.x = -5.0f; break;
            case 2: offset.y = 5.0f; break;
            case 3: offset.y = -5.0f; break;
        }

        // Throttle command rate to avoid disconnects
        float evade_timer = this->program->GetEvadeTimer();
        float elapsed = this->api->GetTime() - evade_timer;
        if (offset == Vector() && elapsed < 0.05f) return false;

        // Finalize by moving toward the target location
        float height = this->api->GetHeight(this->hero);
        if (this->api->IsAttacking(this->hero))
        {
            // Cancel current auto attack during evasion
            Vector hero_pos = this->program->GetHeroPos();
            this->api->SendMoveCommand(hero_pos, height);
        }
        this->api->SendMoveCommand(spot + offset, height);
        return true;
    }

    Vector Utilities::Execute(Solution& solution) const
    {
        // No solution means the skillshots are undodgeable
        if (!solution.IsValid) return Vector();

        const auto& name = solution.SpellInfo.SpellName;
        int spell_slot = solution.SpellInfo.SpellSlot;
        Vector& destination = solution.Destination;
        const auto& target = solution.Target;

        float height = this->api->GetHeight(this->hero);
        const Vector& hero_pos = this->program->GetHeroPos();
        auto& evading_spells = this->data->GetEvadingSpells();
        auto evading_type = evading_spells.at(name).EvadingType;
        bool buff = evading_type == EvadingType::MOVEMENT_BUFF;

        // No spell usage, extend and follow the path
        if (spell_slot == -1 && destination.IsValid())
        {
            destination = this->ExtendPath(destination, 100.0f);
        }

        // Cast evading spell towards destination point
        else if (spell_slot != -1 && target == nullptr)
        {
            float cait_dash_range = 390.0f;
            float length = !buff ? SAFETY_BUFFER : 100.0f;
            destination = this->ExtendPath(destination, length);
            Vector cast_pos = name != "CaitlynE" ? destination
                : destination.Append(hero_pos, cait_dash_range);
            this->api->CastSpell(spell_slot, cast_pos, height);
        }

        // Cast evading spell at the target (game object)
        else if (spell_slot != -1 && target != nullptr)
        {
            this->api->CastSpell(spell_slot, target);
        }

        return destination.Clone();
    }

    Vector Utilities::ExtendPath(const Vector& destination, float buffer) const
    {
        const Vector& hero_pos = this->program->GetHeroPos();
        Vector direction = (destination - hero_pos).Normalize();
        Vector safe_pos = destination + direction * SAFETY_BUFFER;
        if (buffer == SAFETY_BUFFER) return safe_pos;

        // Verify if the extended path can be traversed
        Vector extended = destination + direction * buffer;
        bool reachable = this->api->IsReachable(extended);
        return reachable ? extended : safe_pos;
    }

    Linq<Vector> Utilities::FindSafeSpots(Linq<Skillshot*>& skillshots,
        Path& path, float max_range, float dash_range, float delta, bool fixed)
    {
        Linq<Vector> results;
        Vector dir = path.Velocity.Clone();
        Vector vertical = Vector(0.0f, 1.0f);

        float delay = path.Delay;
        float latency = this->api->GetLatency() / 2.0f;
        bool attacking = this->api->IsAttacking(this->hero);
        latency += 0.034f + (attacking ? 0.034f : 0.0f);

        bool infinite = IsInfinite(path.Speed);
        float speed = this->program->GetMoveSpeed();
        int points = this->program->GetValue<int>("Points");
        float step = 360.0f / (float)points, angle = 360.0f;

        // Search around the starting position
        while ((angle -= step) > -0.01f)
        {
            float phi = M_RAD(MAX(0.0f, angle));
            Vector rotated = vertical.Rotate(phi);
            path.EndPos = path.StartPos + rotated * max_range;
            if (!infinite) path.Velocity = rotated * path.Speed;

            // Grab the closest safe intersection point to follow along the path
            // or skip calculation part if pathing has a predefined distance
            Vector intersection = fixed ? Linq<Vector>({ path.EndPos }).First()
                : skillshots.Select<Linq<Vector>>([&](Skillshot* skillshot)
            {
                return skillshot->PathIntersection({ path.StartPos, path.EndPos });
            })
            .Aggregate<Linq<Vector>>(Linq<Vector>(), [](auto total, auto& points)
            {
                total.AddRange(points); return total;
            })
            .OrderBy<float>([&path](const Vector& point)
            {
                return point.DistanceSquared(path.StartPos);
            })
            .FirstOrDefault([&skillshots, &rotated](const Vector &point)
            {
                if (skillshots.Count() == 1) return true;

                return skillshots.All([&point, &rotated](Skillshot* skillshot)
                {
                    return skillshot->IsSafe(point + rotated * SAFETY_BUFFER);
                });
            });

            // No valid intersections - path length too small?
            if (!intersection.IsValid()) continue;

            // Extend path by extra length (buffer)
            Vector destination = intersection + rotated;
            path.EndPos = intersection + rotated * SAFETY_BUFFER;

            // Skip if the end position collides or is in walls
            if (!this->api->IsReachable(path.EndPos)) continue;

            // Additional delay approximation due to latency
            float angle = dir.LengthSquared() < 0.5f ? 0.0f :
                std::cosf(Geometry::AngleBetween(rotated, dir));
            path.Delay = delay + latency * (1.0f - angle);

            // Check if any skillshot makes the path unsafe
            if (skillshots.Any([&](Skillshot* skillshot)
            {
                return skillshot->IsPathDangerous(path, true);
            }
            )) continue;

            float dist = path.StartPos.Distance(destination);
            if (dash_range > 0 && dist - dash_range > 0)
            {
                // Define the hero's next path after the dash completes
                float duration = dash_range / path.Speed + path.Delay;
                Path new_path = Path(speed, duration + 0.125f, delta,
                    path.StartPos + rotated * dash_range, path.EndPos);

                // Confirm if the next path is actually safe
                if (skillshots.Any([&](Skillshot* skillshot)
                {
                    return skillshot->IsPathDangerous(new_path);
                }
                )) continue;
            }
            else
            {
                // Ensure target safety when path is not splitted
                if (skillshots.Any([&](Skillshot* skillshot)
                {
                    return skillshot->IsDangerous(path.EndPos);
                }
                )) continue;
            }

            // Add final solution to results
            results.Append(destination);
        }
        return results;
    }

    std::pair<Vector, Vector> Utilities::FindSegment(const Poly& poly, Vector& point)
    {
        int size = poly.Size();
        float closest_distance = FLT_MAX;
        std::pair<Vector, Vector> segment;

        // Iterate through each edge of the polygon
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            // Calculate the distance from the point to segment
            const Vector& p1 = poly.Get(i), p2 = poly.Get(j);
            float dist_sqr = point.DistSqrToSegment(p1, p2);

            // Update the segment if a closer one is found
            if (dist_sqr < closest_distance)
            {
                closest_distance = dist_sqr;
                segment = { p1, p2 };
            }
        }
        return segment;
    }

    Solution Utilities::FindSolution(Linq<Skillshot*>& skillshots, bool any_dynamic)
    {
        API* api = this->api;
        using Threat = std::pair<Vector, float>;
        SpellInfo spell; Solution solution;

        Vector mouse = api->GetCursorPos();
        Vector path_end = api->GetPathEndPos(this->hero);
        const Vector& hero_pos = this->program->GetHeroPos();
        Vector direction = (path_end - hero_pos).Normalize();
        CollectionType col_type = CollectionType::DANGEROUS;

        int buffer = this->program->GetValue<int>("Delta");
        float latency = api->GetLatency() / 2.0f + 0.034f;
        float move_speed = this->program->GetMoveSpeed();
        float range = (float)this->program->GetValue<int>("Search");
        auto enemies = api->GetEnemyHeroes(range, hero_pos, true);

        // Find the maximum danger level among unsafe skillshots
        int level = skillshots.Max([](Skillshot* skillshot)
        {
            return skillshot->Get().DangerLevel;
        });

        // Retrieve all active evading spells that can be used for evasion
        const auto& evading_spells = this->data->GetEvadingSpells();
        auto active_spells = !this->program->CanUseSpell() ? Linq<SpellInfo>()
            : this->program->GetEvadingSpells().Where([&](const auto& info)
        {
            std::string id = "E|" + info.SpellName + "|";
            const auto& data = evading_spells.at(info.SpellName);
            int danger = this->program->GetValue<int>(id + "Danger");
            bool use_spell = this->program->GetValue<bool>(id + "UseSpell");
            return info.SpellSlot != -1 && data.Condition(api) && use_spell
                && danger <= level && api->CanUseSpell(info.SpellSlot);
        });

        // Filter spells to those that provide movement buffs or dashes
        auto move_buffers = active_spells.Where([&](const auto& info)
        {
            const auto& data = evading_spells.at(info.SpellName);
            return data.EvadingType == EvadingType::DASH_FIXED ||
                data.EvadingType == EvadingType::DASH_TARGETED ||
                data.EvadingType == EvadingType::MOVEMENT_BUFF;
        });

        // Retrieve remaining skillshots that are not being processed
        auto unhandled = this->program->GetSkillshots().Except(skillshots);

        // Thanks to this indicator we prefer evading outside the Veigar's cage
        bool any_cage = skillshots.Any([](Skillshot* skillshot)
        {
            const std::string& name = skillshot->Get().SkillshotName;
            return name == "VeigarCage" || name == "VeigarEventHorizon";
        });

        // Find skillshots that can change its hitbox during its lifetime
        bool any_resizable = skillshots.Any([](Skillshot* skillshot)
        {
            const std::string& name = skillshot->Get().SkillshotName;
            return name == "PykeE" || name == "SejuaniW" || name == "TaricE";
        });

        // Loop through available evasion options
        size_t size = move_buffers.Count() + 1;
        for (size_t index = 0; index <= size; index++)
        {
            bool using_spell = index > 0 && index < size;
            spell = move_buffers.ElementAtOrDefault(index - 1);
            const auto& spell_data = evading_spells.at(spell.SpellName);
            
            float delta = index < size ? 0.15f : any_cage ? 0.05f : 0.0f;
            float delay = using_spell ? MAX(0.0f, spell_data.Delay) : 0.0f;
            float speed = using_spell ? spell_data.Speed(api) : move_speed;
            float dash_range = using_spell ? spell_data.Range : 0.0f;

            bool fixed_range = using_spell && (any_dynamic == true &&
                spell_data.EvadingType == EvadingType::DASH_TARGETED ||
                spell_data.EvadingType == EvadingType::DASH_FIXED);

            // Calculate the potential paths that we want to consider
            Path path = Path(speed, delay + buffer / 1000.0f, delta);
            if (any_resizable && index == size) path.Speed *= 1.25f;
            path.StartPos = hero_pos; path.Velocity = direction;
            Linq<Vector> spots = this->FindSafeSpots(skillshots,
                path, 500.0f, dash_range, delta, fixed_range);

            // Remove risky spots where the direction vectors of
            // linear skillshot and a path are close to be parallel
            spots.RemoveAll([&](const Vector& spot)
            {
                Vector spot_dir = (spot - hero_pos).Normalize();
                return skillshots.Any([&spot_dir](Skillshot* skillshot)
                {
                    SkillshotType type = skillshot->Get().SkillshotType;
                    if (type != SkillshotType::LINE) return false;

                    const Vector& dir = skillshot->Get().Direction;
                    float angle = Geometry::AngleBetween(dir, spot_dir);
                    return angle < 0.2618f || angle > 2.8798f; // 15 degrees
                });
            });

            // An extra feature to check for potential threats to the spots
            // Find the threat closest to the direction indicated by mouse
            auto threats = enemies.Select<Threat>([&](auto& enemy)
            {
                Vector position = api->GetPosition(enemy);
                float distance = position.DistanceSquared(mouse);
                return std::make_pair(position, distance);
            })
            .OrderBy<float>([](auto& pair) { return pair.second; });

            // Remove spots that are not aligned with running away from threat
            if (threats.Any()) spots.RemoveAll([&](const Vector& spot)
            {
                const Vector& enemy = threats.Last().first;
                float angle_a = hero_pos.AngleBetween(mouse, enemy, true);
                float angle_b = hero_pos.AngleBetween(spot, enemy, true);
                return angle_a > M_PI_2_F && angle_b < M_PI_2_F;
            });

            // Decide whether to evade using the shortest path
            // or by heading toward the mouse position
            bool short_path = any_dynamic || index == size;
            spots = spots.OrderBy<float>([&](const Vector& spot)
            {
                return short_path ? spot.DistanceSquared(hero_pos)
                    : hero_pos.AngleBetween(spot, mouse, true);
            });

            // Find the best spot that is safe from all skillshots
            Vector spot = spots.FirstOrDefault([&](const Vector& spot)
            {
                return unhandled.All([&](Skillshot* skillshot)
                {
                    return skillshot->IsSafe(spot);
                });
            }, spots.FirstOrDefault());

            // Return the valid spot if its 100% safe
            if (spot.IsValid())
            {
                Vector start = direction.IsValid() ? path.StartPos :
                    path.StartPos + direction * (latency * move_speed);
                float time = start.Distance(spot) / speed + delay;
                solution = { spot, spell, nullptr, time, true };
                if (index != size) return solution;
            }
        }

        // Retrieve considered skillshots that are dodgeable
        auto& dangerous = this->program->GetSkillshots(col_type);
        auto candidates = dangerous.Intersect(skillshots);
        Linq<Skillshot*> blocked = Linq<Skillshot*>();

        // Process spells which do not influence pathing
        for (const auto& info : active_spells.Except(move_buffers))
        {
            const auto& data = evading_spells.at(info.SpellName);
            float delay = data.Delay + latency;

            // Use spell that provides crowd control immunity
            if (data.EvadingType == EvadingType::CC_IMMUNITY)
            {
                blocked = candidates.Where([](Skillshot* skillshot)
                {
                    bool soft_cc = skillshot->Get().SoftCC;
                    bool hard_cc = skillshot->Get().HardCC;
                    return soft_cc || hard_cc;
                });

                if (blocked.Count() == 0) continue;
                blocked.ForEach([](Skillshot* skillshot)
                {
                    skillshot->Set().Processed = true;
                });
            }

            // Use spell to absorb damage or make hero invulnerable
            if (data.EvadingType == EvadingType::ABSORPTION ||
                data.EvadingType == EvadingType::INVULNERABILITY)
            {
                candidates.ForEach([](Skillshot* skillshot)
                {
                    skillshot->Set().Processed = true;
                });
            }

            // Use wind wall to block incoming projectiles
            if (data.EvadingType == EvadingType::WIND_WALL)
            {
                CollisionFlag flag = CollisionFlag::WIND_WALL;
                blocked = candidates.Where([&flag](Skillshot* skillshot)
                {
                    const Collisions& flags = skillshot->Get().Collisions;
                    return std::find(flags.begin(), flags.end(), flag) != flags.end();
                });

                if (blocked.Count() == 0) continue;
                blocked.ForEach([](Skillshot* skillshot)
                {
                    skillshot->Set().Processed = true;
                });
            }

            // Finalize evading solution
            if (!data.IsTargeted && blocked.Any())
            {
                Skillshot* skillshot = blocked.Last();
                const Vector& position = skillshot->Get().Position;
                Vector cast_position = hero_pos.Extend(position, 100.0f);
                return { cast_position, info, nullptr, delay, true };
            }
            return { Vector(), info, this->hero, delay, true };
        }

        return solution; // Return the best available solution (suboptimal)
    }

    Obj_AI_Hero Utilities::GetObjectOwner(const std::string& name)
    {
        // Guess the owner as owner id provided by core is not always valid
        auto enemies = this->api->GetEnemyHeroes(FLT_MAX, Vector(), false);
        enemies = enemies.Where([&](const auto& enemy)
        {
            std::string enemy_name = this->api->GetCharacterName(enemy);
            bool stealer = enemy_name == "Sylas" || enemy_name == "Viego";
            return name.find(enemy_name) == 0 || stealer == true;
        })
        .OrderByDescending<bool>([&](const auto& enemy)
        {
            std::string enemy_name = this->api->GetCharacterName(enemy);
            return enemy_name != "Sylas" && enemy_name != "Viego";
        })
        .ThenBy<float>([&](const auto& enemy)
        {
            const Vector& hero_pos = this->program->GetHeroPos();
            Vector enemy_pos = this->api->GetPosition(enemy);
            return hero_pos.DistanceSquared(enemy_pos);
        });
        return enemies.FirstOrDefault();
    }

    void Utilities::ShieldAllies(const SpellInfo& shield_spell)
    {
        const auto& name = shield_spell.SpellName;
        int slot = shield_spell.SpellSlot;
        std::string id = "S|" + name + "|";

        // Check if shielding spell can be used
        if (slot == -1 || name.empty()) return;
        if (!this->program->GetValue<bool>(id + "UseSpell")
            || this->program->GetEvadePos().IsValid()
            || !this->api->CanUseSpell(slot)) return;

        int minimum = this->program->GetValue<int>(id + "Danger");
        const auto& spell = this->data->GetShieldSpells().at(name);
        const Vector& hero_pos = this->program->GetHeroPos();
        auto& skillshots = this->program->GetSkillshots();
        float range = spell.Range, speed = spell.Speed;

        // Retrieve allies within range and prioritize them by health
        auto allies = this->api->GetAllyHeroes(range, hero_pos, true);
        allies = allies.OrderBy<float>([&](const auto& unit)
        {
            return this->api->GetHealth(unit);
        });

        // Iterate over each found ally
        for (const auto& ally : allies)
        {
            if (this->api->Compare(ally, this->hero)) continue;
            Vector ally_pos = this->api->GetPosition(ally);

            // Filter skillshots that are dangerous to the ally
            auto active = skillshots.Where([&](Skillshot* skillshot)
            {
                bool soft_cc = skillshot->Get().SoftCC;
                bool hard_cc = skillshot->Get().HardCC;
                bool immunity = (int)spell.ShieldType == 1;
                return (!immunity || soft_cc || hard_cc)
                    && skillshot->IsDangerous(ally_pos);
            });

            if (active.Count() == 0) continue;
            bool targeted = spell.IsTargeted;
            float height = this->api->GetHeight(ally);
            float distance = ally_pos.Distance(hero_pos);
            float time = distance / spell.Speed + spell.Delay;

            // Find the maximum danger level and minimum hit time
            int danger = active.Max([](Skillshot* skillshot)
            {
                return skillshot->Get().DangerLevel;
            });
            float hit_time = active.Min([&](Skillshot* skillshot)
            {
                return skillshot->TimeToHit(ally_pos, true);
            });

            // Check conditions and cast the spell if necessary
            if (danger < minimum || hit_time <= time) continue;
            if (targeted) return this->api->CastSpell(slot, ally);
            return this->api->CastSpell(slot, ally_pos, height);
        }
    }

    bool Utilities::ShouldEvade(Linq<Skillshot*>& skillshots, Solution& solution, bool any_dynamic) const
    {
        // Return if no available solution
        if (!solution.IsValid) return false;

        // Constantly recalculate path against dynamic skillshots
        if (any_dynamic) return this->program->RecalculatePath(0.25f);

        bool valid = solution.Destination.IsValid();
        CollectionType type = CollectionType::DANGEROUS;
        auto& dangerous = this->program->GetSkillshots(type);
        const Vector& position = this->program->GetHeroPos();

        float attack_timer = this->program->GetAttackTimer();
        float elapsed = this->api->GetTime() - attack_timer;
        float windup = this->api->GetAttackWindup(this->hero);

        // Calculate time required for any skillshot to hit
        float time = skillshots.Min([&](Skillshot* skillshot)
        {
            bool inside = dangerous.Contains(skillshot);
            if (inside) return skillshot->TimeToHit(position, true);
            return (valid && !skillshot->PathIntersection({ position,
                solution.Destination }).Any()) ? FLT_MAX : 0.0f;
        });

        // Delay pathfinding if evasion is not needed yet
        float cooldown = MAX(0.0f, windup - elapsed);
        float left = elapsed < windup ? cooldown : windup;
        return (time - solution.TimeToReach <= left + 0.25f) ||
            (this->program->RecalculatePath(left + 0.1f), false);
    }

    float Utilities::TimeToRecall(float extra_delay)
    {
        float time = this->api->GetBuffTime(this->hero, 0xA31133D0);
        if (time == 0.0f)
        {
            time = this->api->GetBuffTime(this->hero, 0x0AAC64F9);
        }
        return time > 0 ? time + extra_delay : 0.0f;
    }

    void Utilities::UpdateItemSlots(Linq<SpellInfo>& spells)
    {
        for (SpellInfo& spell : spells)
        {
            int slot = spell.SpellSlot;
            if (slot >= 0 && slot <= 3) continue;
            if (spell.SpellName == "Flash") continue;
            if (spell.SpellName == "Ghost") continue;

            spell.SpellSlot = this->api->GetItemSlot(
                this->hero, std::stoi(spell.SpellName));
        }
    }
}
