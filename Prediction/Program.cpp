
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

    void Program::OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo info)
    {
        Program::Get()->OnProcessSpellInternal(unit, info);
    }

    // Component initialization

    void Program::InitComponents()
    {
        // The neural network estimates the probability of a successful spell hit
        // Features: hit ratio, mean angle, path length and count, reaction time
        this->network->DefineLayout({5, 12, 1}, {new Mish(), new Sigmoid()});

        // Initialize pretrained weights
        this->network->InitWeights(
        {
           -1.5316844,  -1.71065,    -0.437382,   -2.0465446,  -0.8413319,
            1.8386623,   2.0452807,   0.00204372,  0.37771302,  1.7089669,
            1.5989374,  -6.924992,    1.0819958,   0.14347865, -3.259547,
           -1.2896303,  -1.7596085,   0.75078315,  0.7985319,   0.0083707,
            0.90909845, -2.0539536,   1.1982433,   0.6704966,  -0.8932713,
           -0.5481159,  -1.4807124,   2.078577,   -5.377486,   -0.3128848,
           -0.6607381,   0.021364,   -2.8196864,   0.29695556, -0.31326964,
           -0.7418757,   1.4304196,  -0.507849,    0.623478,   -0.68479514,
           -0.79684275, -0.67258763,  0.17905003,  0.00089716, -0.87963396,
            0.42509755, -3.4426699,   1.0750163,   1.7820777,   1.1495491,
            0.17524263,  0.32351404, -1.1114081,   0.74665314,  1.1425725,
            0.0106201,  -0.5294857,   0.88444936,  0.6734694,  -0.31335336,
            0.46347517,  0.17226617,  1.1833903,   1.2675507,   0.53423476,
           -0.7106298,  -0.38013995,  0.01372068,  0.6730857,  -0.31730786,
           -1.1679577,   0.47920567, -1.4393647,  -0.50676185, -1.5521555,
           -1.0837605,   2.9035027,   0.99544626,  1.1460985,  -0.00762801,
           -1.110289,    2.3377445,   2.079283,   -3.154874,   -0.2554172
        });

        // Initialize a path history for each enemy (used for hit chance)
        this->api->GetEnemyHeroes(0, Vector(), false).ForEach([&](auto& unit)
        {
            this->UpdatePaths(unit, this->utils->GetWaypoints(unit), false);
        });
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
        this->settings["Draw"] = this->config->AddCheckbox(core, "Draw", "Draw Waypoints", true);
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
        this->settings["D|HitChance"] = this->config->AddList(demo, "D|HitChance", "Hit Chance", hit_chances, 2);
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

    AoeSolution Program::GetAoeSolution(const std::vector<Vector>& candidates,
        const PredictionInput& input, const Vector& star_point) const
    {
        return AoeSolution();
    }

    AoeSolution Program::GetAoeSolution(const std::vector<Obj_AI_Base>& candidates,
        const PredictionInput& input, const Obj_AI_Base& star_unit) const
    {
        return AoeSolution();
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
                Path waypoints = {Segment(Vector(), 0.0f)};
                this->UpdatePaths(unit, waypoints, true);
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

            if (!this->GetValue<bool>("Draw")) return;
            const Path& path = this->GetWaypoints(unit);
            const Vector& pos = path.back().EndPos;

            float height = this->api->GetHeight(pos);
            this->api->DrawCircle(pos, 10.0f, height, colors[0]);
            this->utils->DrawPath(path, height, colors.data());
        });

        // Simulation test for debug
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
