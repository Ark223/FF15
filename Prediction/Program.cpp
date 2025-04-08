
#include "Program.h"
#include "Utilities.h"

namespace IPrediction
{
    Program* Program::m_instance = nullptr;

    Program::Program(PublicAPI api) : m_api(api)
    {
        this->m_instance = this;
        this->RegisterCore();
        this->RegisterMenu();
        this->RegisterEvents();
    }

    Program::~Program()
    {
        API::Destroy();
        Data::Destroy();
        Network::Destroy();
        Utilities::Destroy();
        Config::Destroy("Prediction");
        delete this->my_hero;
    }

    void Program::RegisterCore()
    {
        this->api = API::Get(this->m_api);
        this->my_hero = this->api->GetHero();
        this->network = Network::Get();
        this->utils = Utilities::Get();
        this->data = Data::Get();
        this->InitComponents();
    }

    void Program::RegisterEvents()
    {
        if (this->libs_loaded == false) return;
        this->api->RegisterEvent(EventType::OnDraw, OnDrawWrapper);
        this->api->RegisterEvent(EventType::OnNewPath, OnNewPathWrapper);
        this->api->RegisterEvent(EventType::OnProcessSpell, OnProcessSpellWrapper);
        this->api->RegisterEvent(EventType::OnCreateObject, OnCreateObjectWrapper);
    }

    void Program::RegisterMenu()
    {
        this->config = Config::Get(this->m_api);
        this->BuildMenu();
    }

    Program* Program::Get()
    {
        return m_instance;
    }

    template<typename T>
    T Program::GetValue(const std::string& id) const
    {
        return this->config->GetValue<T>(this->settings.at(id), id);
    }

    template bool Program::GetValue<bool>(const std::string&) const;

    template int Program::GetValue<int>(const std::string&) const;

    // Event wrappers

    void Program::OnDrawWrapper()
    {
        Program::Get()->OnDraw();
    }

    void Program::OnNewPathWrapper(Obj_AI_Base unit, std::vector<Vector3> paths)
    {
        Program::Get()->OnNewPathInternal(unit, paths);
    }

    void Program::OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo cast_info)
    {
        Program::Get()->OnProcessSpellInternal(unit, cast_info);
    }

    void Program::OnCreateObjectWrapper(Object object, uint32_t network_id)
    {
        Program::Get()->OnCreateObject(object, network_id);
    }

    // Component initialization

    void Program::InitComponents()
    {
        // The neural network estimates the probability of a successful spell hit
        // Features include hit ratio, pathing data, reaction time and spell type
        this->network->DefineLayout({6, 12, 1}, {new Mish(), new Sigmoid()});

        // Initialize pretrained weights
        this->network->InitWeights(
        {
           -1.2928982,   3.6424842,   1.1371002,  -4.7275963,  -4.6449633,
           -6.5053873,   6.408297,   -0.02636885,  0.00060714, -0.7487647,
           -19.735334,  -5.0507536,   0.21929236,  3.233195,   -0.6834893,
            1.1046116,  -1.7405959,   0.34342524, -1.3933556,  -0.06029257,
            0.0059832,   1.2787944,   2.6550598,   3.296719,   -6.201479,
           -1.3988853,   0.5596696,   0.05239796, -1.4400576,   3.0194254,
            0.83777446,  0.2357904,   0.22133717, -1.1004771,   0.33932444,
           -4.133593,   -1.0927055,  -7.8071833,  -2.948673,    0.3265651,
            0.22033252,  1.278157,   -0.5937438,   0.16694003,  0.04887361,
            3.6764305,   1.8805927,  -0.00623076, -2.0989063,   1.2777847,
           -5.969112,   -0.69277287,  0.4858697,  -2.7215195,   1.2116735,
           -0.12191653, -0.10571268, -0.400049,    0.5129176,   0.38956302,
            0.03270679,  0.8492915,  -0.20229079, -9.570694,   -0.13596986,
            0.8401494,   0.08398294, -0.11911385, -0.1622787,  -10.254427,
           -0.4965595,   3.0646186,   0.4243737,  -2.4033813,  -0.41528255,
            3.003503,    2.901044,    2.0250735,  -4.096801,   -0.11865114,
           -0.06944592, -2.1206882,   4.547432,   -1.1856408,   4.019871,
            0.5642771,  -1.2142243,  -0.7026102,  -1.1749012,  -0.39492846,
            1.6351405,  -0.00229616,  0.01537126,  0.69714636, -0.29531154,
           -0.54395574,  1.0644993,   0.2848414,  -0.3592015,   0.9421906
        });

        // Initialize a path history for each enemy (used for hit chance)
        this->api->GetEnemyHeroes(0, Vector(), false).ForEach([&](auto& unit)
        {
            this->UpdatePaths(unit, this->utils->GetWaypoints(unit), false);
        });

        // Initialize extra exclusion spells to prevent false spell hits
        this->exclusions = {"Item2420", "SummonerFlash", "ZhonyasHourglass"};
    }

    // Menu constructor

    void Program::BuildMenu()
    {
        // Check if menu library is loaded
        this->libs_loaded = this->config->IsLoaded();
        if (this->libs_loaded == false) return;

        auto menu = this->config->CreateMenu("Prediction", "Prediction");
        this->config->AddLabel(menu, "Settings", "Settings", true);

        // Core settings
        auto core = this->config->AddSubMenu(menu, "Core", "<< Core >>");
        this->settings["Waypoints"] = this->config->AddCheckbox(core, "Waypoints", "Draw Waypoints", true);
        this->settings["Windwalls"] = this->config->AddCheckbox(core, "Windwalls", "Draw Wind Walls", true);
        this->settings["Buffer"] = this->config->AddSlider(core, "Buffer", "Collision Buffer", 20, 0, 50, 5);

        // Live Demo settings
        auto demo = this->config->AddSubMenu(menu, "Demo", "<< Live Demo >>");

        this->settings["D|Run"] = this->config->AddCheckbox(demo, "D|Run", "Run Live Demo", false);
        this->settings["D|Cast"] = this->config->AddCheckbox(demo, "D|Cast", "Cast Skillshot", false);
        this->settings["D|Hitbox"] = this->config->AddCheckbox(demo, "D|Hitbox", "Include Hitbox", true);
        this->settings["D|Infinite"] = this->config->AddCheckbox(demo, "D|Infinite", "Infinite Speed", false);
        this->settings["D|Collision"] = this->config->AddCheckbox(demo, "D|Collision", "Unit Collision", true);

        this->settings["D|Delay"] = this->config->AddSlider(demo, "D|Delay", "Delay", 250, 0, 1000, 25);
        this->settings["D|Range"] = this->config->AddSlider(demo, "D|Range", "Range", 1200, 200, 2000, 25);
        this->settings["D|Speed"] = this->config->AddSlider(demo, "D|Speed", "Speed", 2000, 600, 3000, 25);
        this->settings["D|Radius"] = this->config->AddSlider(demo, "D|Radius", "Radius", 60, 20, 300, 5);
        this->settings["D|Type"] = this->config->AddList(demo, "D|Type", "Spell Type", { "Line", "Circle" }, 0);
        this->settings["D|Slot"] = this->config->AddList(demo, "D|Slot", "Spell Slot", { "Q", "W", "E", "R" }, 0);

        std::vector<std::string> cast_rates = { "Instant", "Moderate", "Precise" };
        std::vector<std::string> hit_chances = { "Low", "Normal", "High", "VeryHigh", "Extreme", "Guaranteed" };
        this->settings["D|HitChance"] = this->config->AddList(demo, "D|HitChance", "Hit Chance", hit_chances, 1);
        this->settings["D|CastRate"] = this->config->AddList(demo, "D|CastRate", "Cast Rate", cast_rates, 1);

        // Measurement settings
        auto measurement = this->config->AddSubMenu(menu, "Measurement", "<< Measurement >>");
        this->settings["M|Draw"] = this->config->AddCheckbox(measurement, "M|Draw", "Draw Skillshot", false);
        this->settings["M|Fixed"] = this->config->AddCheckbox(measurement, "M|Fixed", "Fixed Range", true);
        this->settings["M|Range"] = this->config->AddSlider(measurement, "M|Range", "Range", 1200, 200, 2000, 25);
        this->settings["M|Radius"] = this->config->AddSlider(measurement, "M|Radius", "Radius", 60, 20, 300, 5);
        this->settings["M|Type"] = this->config->AddList(measurement, "M|Type", "Type", { "Line", "Circle" }, 0);

        // Version
        this->config->AddLabel(menu, "Creator", "Creator: Uncle Ark", true);
        this->config->AddLabel(menu, "Version", "Version: " + PRED_VERSION, true);
    }

    // Core methods

    void Program::CleanUpHistory(Linq<PathData>& paths, float timer, float duration)
    {
        // Remove outdated entries that exceed the duration threshold
        for (int index = (int)paths.Count() - 2; index >= 0; --index)
        {
            if (timer - paths.ElementAt(index).UpdateTime > duration)
                paths.RemoveAt(index);
        }
    }

    void Program::UpdatePaths(const Obj_AI_Base& unit, const Path& path, bool changed)
    {
        uint32_t id = this->api->GetObjectId(unit);
        auto linq = Linq<PathData>({ PathData(path) });
        PathData& data = linq.ToArray().front();

        // Initialize path data during plugin load
        if (this->paths.find(id) == this->paths.end())
            return (void)this->paths.emplace(id, linq);

        // Add or update path depending on change
        if (changed) this->paths[id].AddRange(linq);
        PathData& last = this->paths[id].ToArray().back();
        data.UpdateTime = last.UpdateTime; last = data;
    }

    // Public utility methods

    float Program::GetCollisionBuffer() const
    {
        return (float)this->GetValue<int>("Buffer");
    }

    float Program::GetMiaDuration(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->paths.count(id) == 0) return 0.0f;
        float mia_time = this->paths[id].Last().MiaTime;
        return mia_time < 0 ? 0.0f : timer - mia_time;
    }

    float Program::GetImmobilityTime(const Obj_AI_Base& unit) const
    {
        return this->utils->GetImmobilityTime(unit);
    }

    float Program::GetPathChangeTime(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->paths.count(id) == 0) return 0.0f;
        return timer - this->paths[id].Last().UpdateTime;
    }

    float Program::GetWindupDuration(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->windups.count(id) == 0) return 0.0f;
        return MAX(0.0f, this->windups[id] - timer);
    }

    Path Program::GetWaypoints(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        if (this->dashes.find(id) != this->dashes.end())
            return this->dashes[id].Waypoints;
        if (this->paths.find(id) != this->paths.end())
            return this->paths[id].Last().Waypoints;
        return this->utils->GetWaypoints(unit);
    }

    bool Program::IsBlinking(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        if (this->dashes.count(id) == 0) return false;
        return this->dashes[id].IsBlink;
    }

    bool Program::IsDashing(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        if (this->dashes.count(id) > 0) return true;
        return this->api->IsDashing(unit);
    }

    // Public core methods

    AoeSolution Program::GetAoeSolution(const Linq<Vector>& candidates,
        const PredictionInput& input, const Vector& star_point) const
    {
        return this->utils->GetAoeSolution(input, candidates, star_point);
    }

    AoeSolution Program::GetAoeSolution(const Linq<Obj_AI_Base>& candidates,
        const PredictionInput& input, const Obj_AI_Base& star_unit) const
    {
        Vector star = star_unit ? this->api->GetPosition(star_unit) : Vector();
        auto converter = [&](auto& unit) { return this->api->GetPosition(unit); };
        return this->GetAoeSolution(candidates.Select<Vector>(converter), input, star);
    }

    PredictionOutput Program::GetPrediction(const PredictionInput& input) const
    {
        auto output = this->utils->PredictOnPath(input);
        this->utils->GetHitChance(input, output);
        return output;
    }

    // Internal methods

    float Program::GetBlinkDuration(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->dashes.count(id) == 0) return 0.0f;
        const DashData& data = this->dashes[id];
        return !data.IsBlink ? 0.0f : data.Delay +
            data.StartTime - this->api->GetTime();
    }

    float Program::GetDashDuration(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        if (this->paths.count(id) == 0) return 0.0f;
        const PathData& data = this->paths[id].Last();
        if (this->dashes.find(id) != this->dashes.end())
        {
            const DashData& data = this->dashes[id];
            float end_time = data.StartTime + data.Delay;
            if (!data.IsBlink) end_time += data.Range / data.Speed;
            return MAX(0.0f, end_time - this->api->GetTime());
        }
        return this->api->IsDashing(unit) && data.PathLength > 0 ?
            data.PathLength / data.Waypoints.back().Speed : 0.0f;
    }

    float Program::GetMeanAngleDiff(const Obj_AI_Base& unit) const
    {
        float angle = 0.0f;
        uint32_t id = this->api->GetObjectId(unit);
        size_t size = this->paths[id].Count();
        for (size_t i = 0; i < size - 1; ++i)
        {
            const PathData& pa = this->paths[id].ElementAt(i);
            const PathData& pb = this->paths[id].ElementAt(i + 1);
            angle += Geometry::Angle(pa.Direction, pb.Direction);
        }
        return (size > 1) ? angle / (float)(size - 1) : 0.0f;
    }

    bool Program::IsCastingDash(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        return this->dashes.find(id) != this->dashes.end();
    }

    // Events

    void Program::OnDraw()
    {
        float timer = this->api->GetTime();
        Vector hero_pos = this->api->GetPosition(this->my_hero);
        auto colors = std::vector<uint32_t>{0xC0FFFFFF, 0x30FFFFFF};

        // Remove cached windwalls that have exceeded duration
        this->wind_walls.RemoveAll([&timer](const auto& info)
        {
            return timer - info.StartTime > 4.0f;
        });

        // Process each enemy to update paths for dash spells and blinks
        for (auto it = this->dashes.begin(); it != this->dashes.end(); )
        {
            DashData& data = it->second;
            if (!data.IsBlink)
            {
                float fly_time = data.Range / data.Speed;
                float length = data.Speed * (timer - data.StartTime);
                data.Waypoints = Geometry::CutPath(data.TotalPath, length);

                float total_time = fly_time + data.Delay + data.ExtraTime;
                bool should_erase = (timer - data.StartTime > total_time);
                it = should_erase ? this->dashes.erase(it) : std::next(it);
            }
            else
            {
                float total_time = data.Delay + data.ExtraTime;
                bool should_erase = (timer - data.StartTime > total_time);
                it = should_erase ? this->dashes.erase(it) : std::next(it);
            }
        }

        // Process each enemy to update and render standard movement paths
        this->api->GetEnemyHeroes(0, Vector(), false).ForEach([&](auto& unit)
        {
            // Remove old entries from path history
            uint32_t id = this->api->GetObjectId(unit);
            this->CleanUpHistory(this->paths[id], timer, 1.0f);

            if (this->api->IsDead(unit))
            {
                // Clean up dead target
                this->paths[id].Clear();
                this->UpdatePaths(unit, {Segment()}, true);
            }
            else if (!this->api->IsVisible(unit))
            {
                // Mark the time when unit was last seen visible
                PathData& last = this->paths[id].ToArray().back();
                if (last.MiaTime < 0) last.MiaTime = timer;
            }
            else
            {
                // If visible, update paths with current waypoints
                Path waypoints = this->utils->GetWaypoints(unit);
                this->UpdatePaths(unit, waypoints, false);
            }

            if (!this->GetValue<bool>("Waypoints")) return;
            const Path& path = this->GetWaypoints(unit);
            const Vector& start = path.front().StartPos;
            const Vector& ending = path.back().EndPos;

            float height = this->api->GetHeight(ending);
            this->api->DrawCircle(start, 10.0f, height, colors[0]);
            this->utils->DrawPath(path, height, colors.data());
        });

        // Draw registered and active windwalls
        if (this->GetValue<bool>("Windwalls"))
        {
            this->wind_walls.ForEach([&](const auto& info)
            {
                float height = this->api->GetHeight(info.Polygon[0]);
                this->api->DrawPolygon(info.Polygon, height, 0xFFFFFFFF);
            });
        }

        // Live demo of prediction core
        if (this->GetValue<bool>("D|Run"))
        {
            PredictionInput input = PredictionInput();
            bool collision = this->GetValue<bool>("D|Collision");
            bool infinite = this->GetValue<bool>("D|Infinite");
            int spell_type = this->GetValue<int>("D|Type");

            input.AddHitbox = this->GetValue<bool>("D|Hitbox");
            input.Delay = (float)this->GetValue<int>("D|Delay");
            input.Range = (float)this->GetValue<int>("D|Range");
            input.Speed = (float)this->GetValue<int>("D|Speed");
            input.Radius = (float)this->GetValue<int>("D|Radius");

            input.SourceObject = this->my_hero;
            input.SpellType = (SpellType)(spell_type + 1);
            input.Speed = infinite ? FLT_MAX : input.Speed;
            input.CollisionFlags = collision ? 0xF : 0x0;
            input.Delay = input.Delay / 1000.0f;

            // Iterate over enemy heroes and display valid prediction details
            this->api->GetEnemyHeroes(0, Vector(), false).ForEach([&](auto& unit)
            {
                input.TargetObject = unit;
                PredictionOutput output = this->GetPrediction(input);
                if (output.HitChance == HitChance::Impossible) return;

                std::ostringstream oss;
                oss << "Confidence: " << output.Confidence << "\n"
                    << "TimeToHit: " << output.TimeToHit << "\n"
                    << "Intercept: " << output.Intercept << "\n"
                    << "Distance: " << output.Distance << "\n";
                std::string text = oss.str();

                const Vector& cast_pos = output.CastPosition;
                const Vector& pred_pos = output.TargetPosition;
                float height = this->api->GetHeight(pred_pos);
                float hitbox = this->api->GetHitbox(unit);
                if (!input.AddHitbox) hitbox = 5.0f;

                // Draw predicted target position and cast position
                this->api->DrawCircle(pred_pos, 5.0f, height, 0xFF0096FF);
                this->api->DrawCircle(cast_pos, 5.0f, height, 0xFFD4AF37);
                this->api->DrawCircle(pred_pos, hitbox, height, 0xFF0096FF);
                this->api->DrawCircle(cast_pos, input.Radius, height, 0xFFD4AF37);

                // Display remaining prediction output in a text format
                Vector2 screen_pos = this->api->ToScreen(cast_pos, height);
                screen_pos = Vector2(screen_pos.x, screen_pos.y + 30.0f);
                this->api->DrawCenteredText(screen_pos, text.c_str(), 0xC0FFFFFF);

                // Check if spell casting is enabled and ready
                if (!this->GetValue<bool>("D|Cast")) return;
                int spell_slot = this->GetValue<int>("D|Slot");
                if (!this->api->CanUseSpell(spell_slot)) return;
                if (this->GetMiaDuration(unit) > 0.25f) return;
                height = this->api->GetHeight(cast_pos);

                // Validate cast rate and hit chance thresholds before casting
                auto rate = (CastRate)(this->GetValue<int>("D|CastRate") + 1);
                auto chance = (HitChance)(this->GetValue<int>("D|HitChance") + 3);
                if (output.CastRate < rate || output.HitChance < chance) return;
                this->api->CastSpell(spell_slot, output.CastPosition, height);
            });
        }

        // Skillshot measurement option
        if (this->GetValue<bool>("M|Draw"))
        {
            bool fixed = this->GetValue<bool>("M|Fixed");
            float range = (float)this->GetValue<int>("M|Range");
            float radius = (float)this->GetValue<int>("M|Radius");
            float height = this->api->GetHeight(this->my_hero);

            Vector mouse_pos = this->api->GetCursorPos();
            Vector direction = (mouse_pos - hero_pos);
            float distance = direction.Length();
            direction = direction / distance;

            // Determine actual drawing distance based on mode
            distance = fixed ? range : MIN(range, distance);
            Vector dest_pos = hero_pos + direction * distance;
            Vector perpend = direction.Perpendicular() * radius;

            if (this->GetValue<int>("M|Type") == 0)
            {
                // Draw a rectangle to simulate path for linear skillshot
                std::vector<Vector> box = {dest_pos - perpend, dest_pos
                    + perpend, hero_pos + perpend, hero_pos - perpend};
                this->api->DrawPolygon(box, height, 0xFFFFFFFF);
            }
            else
            {
                // Draw a circle at the destination for circular skillshot
                this->api->DrawCircle(dest_pos, radius, height, 0xFFFFFFFF);
            }
        }
    }

    void Program::OnNewPath(const Obj_AI_Base& unit, Linq<Vector> path, float speed)
    {
        float threshold = 50.0f;
        Path waypoints = Path();
        if (path.Count() <= 1)
        {
            uint32_t id = this->api->GetObjectId(unit);
            const auto& last = this->paths[id].Last();
            if (last.PathLength <= threshold) return;
            Vector position = this->api->GetPosition(unit);
            waypoints.push_back(Segment(position, speed));
        }
        for (size_t index = 0; index < path.Count() - 1; ++index)
        {
            const Vector& start = path.ElementAt(index);
            const Vector& ending = path.ElementAt(index + 1);
            waypoints.push_back(Segment(start, ending, speed));
        }
        this->UpdatePaths(unit, waypoints, true);
    }

    void Program::OnProcessSpell(const Obj_AI_Base& unit, const std::string& spell_name,
        const Vector& start_pos, const Vector& end_pos, const float cast_delay)
    {
        uint32_t id = this->api->GetObjectId(unit);
        float timer = this->api->GetTime();

        // Store the windup time for immobile target
        if (this->exclusions.count(spell_name) == 0)
            this->windups[id] = timer + cast_delay;

        // Process and register the custom dash spell
        const auto& spells = this->data->GetDashSpells();
        if (spells.find(spell_name) == spells.end()) return;
        DashData data = DashData(spells.at(spell_name));

        Vector direction = end_pos - start_pos;
        float distance = direction.Length();
        bool within = distance < data.Range;
        direction = direction / distance;

        // Adjust dash range if it's not fixed and within range
        if (!data.FixedRange && within) data.Range = distance;

        Vector ending = start_pos + direction * data.Range;
        Vector start = start_pos + direction * data.Offset;
        start -= (direction * (data.Delay * data.Speed));
        data.Range -= data.Offset; data.Offset = 0.0f;

        auto path = {Segment(start, ending, data.Speed)};
        data.TotalPath = path; data.Waypoints = path;
        data.StartTime = timer; data.ExtraTime = 0.1f;

        // Track the dash for target
        this->dashes[id] = data;
    }

    void Program::OnCreateObject(const Object& game_object, const uint32_t network_id)
    {
        if (!this->api->IsValid(game_object)) return;
        if (!this->api->IsMissile(game_object)) return;
        const size_t npos = std::string::npos;

        // Track windwalls for later collision detection
        auto missile = this->api->AsMissile(game_object);
        std::string name = this->api->GetMissileName(missile);
        if (name.find("YasuoW_VisualMis") == npos) return;

        // Missile should have a valid owner to process
        auto owner = this->api->GetMissileOwner(missile);
        if (!this->api->IsValid(owner)) return;

        // Make sure the owner is not an ally
        auto hero = this->api->AsHero(owner);
        if (!this->api->IsEnemy(hero)) return;

        float timer = this->api->GetTime();
        size_t level = this->api->GetSpellLevel(hero, 1);
        float extension = 160.0f + 35.0f * (level - 1);

        Vector position = this->api->GetMissileStartPos(missile);
        Vector destination = this->api->GetMissileEndPos(missile);
        Vector direction = (destination - position).Normalize();
        Vector perpend = direction.Perpendicular() * extension;

        // Build wall as a rectangle
        std::vector<Vector> wall{};
        wall.push_back(position + direction * 300.0f - perpend);
        wall.push_back(position + direction * 300.0f + perpend);
        wall.push_back(position + direction * 350.0f + perpend);
        wall.push_back(position + direction * 350.0f - perpend);

        // Register windwall geometry and timestamp for further processing
        this->wind_walls.Append({direction, perpend /= extension, wall, timer});
    }

    void Program::OnNewPathInternal(const Obj_AI_Base& unit, std::vector<Vector3>& paths)
    {
        if (!this->api->IsHero(unit)) return;
        if (!this->api->IsEnemy(unit)) return;
        float speed = this->api->GetPathSpeed(unit);
        auto converter = [&](const auto& p) { return this->api->ToVector(p); };
        this->OnNewPath(unit, Linq(paths).Select<Vector>(converter), speed);
    }

    void Program::OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info)
    {
        if (!this->api->IsHero(unit)) return;
        if (!this->api->IsEnemy(unit)) return;
        auto name = this->api->GetSpellCastName(info);
        Vector end_pos = this->api->GetSpellCastEndPos(info);
        Vector start_pos = this->api->GetSpellCastStartPos(info);
        float cast_delay = this->api->GetSpellCastWindupTime(info);
        this->OnProcessSpell(unit, name, start_pos, end_pos, cast_delay);
    }
}
