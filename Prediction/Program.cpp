
#include "Program.h"
#include "Utilities.h"

namespace Prediction
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
        this->settings["Waypoints"] = this->config->AddCheckbox(core, "Waypoints", "Draw Waypoints", true);
        this->settings["Buffer"] = this->config->AddSlider(core, "Buffer", "Collision Buffer", 20, 0, 50, 5);

        // Simulation settings
        auto simulation = this->config->AddSubMenu(menu, "Simulation", "<< Simulation >>");
        this->settings["S|Draw"] = this->config->AddCheckbox(simulation, "S|Draw", "Draw Positions", true);
        this->settings["S|Hitbox"] = this->config->AddCheckbox(simulation, "S|Hitbox", "Add Unit Hitbox", true);
        this->settings["S|Missile"] = this->config->AddCheckbox(simulation, "S|Missile", "Simulate Missile", true);
        this->settings["S|Infinite"] = this->config->AddCheckbox(simulation, "S|Infinite", "Infinite Speed", true);
        this->settings["S|Range"] = this->config->AddSlider(simulation, "S|Range", "Range", 1000, 100, 1500, 5);
        this->settings["S|Radius"] = this->config->AddSlider(simulation, "S|Radius", "Radius", 70, 20, 300, 5);
        this->settings["S|Type"] = this->config->AddList(simulation, "S|Type", "Type", { "Circle", "Line" }, 1);

        // Measurement settings
        auto measurement = this->config->AddSubMenu(menu, "Measurement", "<< Measurement >>");
        this->settings["M|Draw"] = this->config->AddCheckbox(measurement, "M|Draw", "Draw Skillshot", true);
        this->settings["M|Fixed"] = this->config->AddCheckbox(measurement, "M|Fixed", "Fixed Range", true);
        this->settings["M|Range"] = this->config->AddSlider(measurement, "M|Range", "Range", 1000, 100, 1500, 5);
        this->settings["M|Radius"] = this->config->AddSlider(measurement, "M|Radius", "Radius", 70, 20, 300, 5);
        this->settings["M|Type"] = this->config->AddList(measurement, "M|Type", "Type", { "Circle", "Line" }, 1);

        // Version
        this->config->AddLabel(menu, "Creator", "Creator: Uncle Ark", true);
        this->config->AddLabel(menu, "Version", "Version: " + PRED_VERSION, true);
    }

    // Core methods

    void Program::CleanUpHistory(Linq<PathData>& paths, float timer, float duration)
    {
        // Remove outdated entries that exceed the duration threshold
        for (int index = (int)paths.Count() - 2; index >= 0; --index)
            if (timer - paths.ElementAt(index).UpdateTime > duration)
                paths.RemoveAt(index);
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

    // Public API methods

    Path Program::GetWaypoints(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);

        if (this->dashes.find(id) != this->dashes.end())
            return this->dashes[id].Waypoints;

        if (this->paths.find(id) != this->paths.end())
            return this->paths[id].Last().Waypoints;

        return this->utils->GetWaypoints(unit);
    }

    float Program::GetBlinkDuration(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        const DashData& data = this->dashes[id];
        return data.StartTime + data.Delay - timer;
    }

    float Program::GetInvisibilityTime(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->paths.count(id) == 0) return 0.0f;
        float mia_time = this->paths[id].Last().MiaTime;
        return mia_time < 0 ? 0.0f : timer - mia_time;
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

    float Program::GetPathChangeTime(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->paths.count(id) == 0) return 0.0f;
        return timer - this->paths[id].Last().UpdateTime;
    }

    float Program::GetWindupTime(const Obj_AI_Base& unit) const
    {
        float timer = this->api->GetTime();
        uint32_t id = this->api->GetObjectId(unit);
        if (this->windups.count(id) == 0) return 0.0f;
        return MAX(0.0f, this->windups[id] - timer);
    }

    bool Program::IsBlinking(const Obj_AI_Base& unit) const
    {
        uint32_t id = this->api->GetObjectId(unit);
        if (this->dashes.count(id) == 0) return false;
        return this->dashes[id].IsBlink;
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
        float height = this->api->GetHeight(this->my_hero);
        Vector hero_pos = this->api->GetPosition(this->my_hero);

        // Update target's waypoints for dash spells and handle blinks
        for (auto it = this->dashes.begin(); it != this->dashes.end(); )
        {
            DashData& data = it->second;
            if (!data.IsBlink)
            {
                float length = data.Speed * (timer - data.StartTime);
                data.Waypoints = Geometry::CutPath(data.TotalPath, length);
                bool should_erase = (data.Waypoints[0].Length == 0.0f);
                it = should_erase ? this->dashes.erase(it) : std::next(it);
            }
            else
            {
                bool should_erase = (timer - data.StartTime > data.Delay);
                it = should_erase ? this->dashes.erase(it) : std::next(it);
            }
        }

        // Update each enemy's path if visible; if not, mark the loss of vision
        this->api->GetEnemyHeroes(0, Vector(), false).ForEach([&](auto& unit)
        {
            uint32_t id = this->api->GetObjectId(unit);
            if (!this->api->IsVisible(unit))
            {
                PathData& last = this->paths[id].ToArray().back();
                if (last.MiaTime < 0) last.MiaTime = timer; return;
            }
            Path waypoints = this->utils->GetWaypoints(unit);
            this->CleanUpHistory(this->paths[id], timer, 1.0f);
            this->UpdatePaths(unit, waypoints, false);
        });

        // Measurement option
        if (this->GetValue<bool>("M|Draw"))
        {
            bool fixed = this->GetValue<bool>("M|Fixed");
            float range = (float)this->GetValue<int>("M|Range");
            float radius = (float)this->GetValue<int>("M|Radius");

            Vector mouse_pos = this->api->GetCursorPos();
            Vector direction = (mouse_pos - hero_pos);
            float distance = direction.Length();
            direction = direction / distance;

            // Determine actual drawing distance based on mode
            distance = fixed ? range : MIN(range, distance);
            Vector dest_pos = hero_pos + direction * distance;
            Vector perpend = direction.Perpendicular() * radius;

            if (this->GetValue<int>("M|Type") == 1)
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

    void Program::OnNewPath(const Obj_AI_Base& unit, Linq<Vector> path)
    {
        Path waypoints = Path();
        float speed = this->api->GetMoveSpeed(unit);

        if (path.Count() <= 1)
        {
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

    void Program::OnProcessSpell(const Obj_AI_Base& unit, std::string& spell_name,
        const Vector& start_pos, const Vector& end_pos, const float cast_delay)
    {
        if (!this->api->IsHero(unit)) return;
        if (!this->api->IsEnemy(unit)) return;
        uint32_t id = this->api->GetObjectId(unit);
        float timer = this->api->GetTime();

        // Store the windup time for immobile target
        this->windups[id] = timer + MAX(0.0f, cast_delay);

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
        data.StartTime = timer;

        // Track the dash for target
        this->dashes[id] = data;
    }

    void Program::OnNewPathInternal(const Obj_AI_Base& unit, std::vector<Vector3>& paths)
    {
        this->OnNewPath(unit, Linq(paths).Select<Vector>([&](const auto& point)
        {
            return this->api->ToVector(point);
        }));
    }

    void Program::OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info)
    {
        auto name = this->api->GetSpellCastName(info);
        Vector end_pos = this->api->GetSpellCastEndPos(info);
        Vector start_pos = this->api->GetSpellCastStartPos(info);
        float cast_delay = this->api->GetSpellCastWindupTime(info);
        this->OnProcessSpell(unit, name, start_pos, end_pos, cast_delay);
    }
}
