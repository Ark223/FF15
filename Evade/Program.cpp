
#include "Program.h"
#include "Process.h"
#include "Utilities.h"

namespace Evade
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
        Process::Destroy();
        Utilities::Destroy();
        Config::Destroy("Evade");
        delete this->my_hero;
    }

    void Program::RegisterCore()
    {
        this->api = API::Get(this->m_api);
        this->my_hero = this->api->GetHero();
        this->process = Process::Get();
        this->utils = Utilities::Get();
        this->data = Data::Get();
        this->InitComponents();
    }

    void Program::RegisterEvents()
    {
        if (this->libs_loaded == false) return;
        this->api->RegisterEvent(EventType::ON_TICK, OnTickWrapper);
        this->api->RegisterEvent(EventType::ON_DRAW, OnDrawWrapper);
        this->api->RegisterEvent(EventType::ON_PROCESS_SPELL, OnProcessSpellWrapper);
        this->api->RegisterEvent(EventType::ON_CREATE_OBJECT, OnCreateObjectWrapper);
        this->api->RegisterEvent(EventType::ON_DELETE_OBJECT, OnDeleteObjectWrapper);
        this->api->RegisterEvent(EventType::ON_BUFF_GAIN, OnBuffGainWrapper);
        this->api->RegisterEvent(EventType::ON_WND_PROC, OnWndProcWrapper);
        this->api->RegisterEvent(EventType::ON_ISSUE_ORDER, OnIssueOrderWrapper);
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

    void Program::OnTickWrapper()
    {
        Program::Get()->OnTick();
    }

    void Program::OnDrawWrapper()
    {
        Program::Get()->OnDraw();
    }

    void Program::OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo info)
    {
        Program::Get()->OnProcessSpellInternal(unit, info);
    }

    void Program::OnCreateObjectWrapper(Object object, uint32_t object_id)
    {
        Program::Get()->OnCreateObject(object, object_id);
    }

    void Program::OnDeleteObjectWrapper(Object object)
    {
        Program::Get()->OnDeleteObjectInternal(object);
    }

    void Program::OnBuffGainWrapper(Obj_AI_Base unit, Buff buff)
    {
        Program::Get()->OnBuffGain(unit, buff);
    }

    void Program::OnWndProcWrapper(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        Program::Get()->OnWndProc(msg, wparam);
    }

    void Program::OnIssueOrderWrapper(Vector3 pos, int order, int id, const char* plugin)
    {
        Program::Get()->OnIssueOrderInternal(pos, order, plugin);
    }

    // Component initialization

    void Program::InitComponents()
    {
        // Find summoner spells such as flash & ghost
        auto slot_d = this->api->GetSpellName(this->my_hero, 4);
        auto slot_f = this->api->GetSpellName(this->my_hero, 5);

        size_t npos = std::string::npos;
        auto find_slot = [&](const std::string& spell)
        {
            return (slot_d.find(spell) != npos) ? 4 :
                (slot_f.find(spell) != npos ? 5 : -1);
        };

        this->flash_slot = find_slot("Flash");
        this->ghost_slot = find_slot("Ghost");

        // Initialize object collectors
        this->objects["Blade"] = {};
        this->objects["Feather"] = {};
        this->objects["Seed"] = {};
        this->objects["Shadow"] = {};

        // Initialize slot converter
        this->char_to_slot = [](char c) -> int
        {
            return (c == 'Q') ? 0 : (c == 'W') ? 1 :
                (c == 'E') ? 2 : (c == 'R') ? 3 : -1;
        };
    }

    // Menu constructor

    void Program::BuildMenu()
    {
        // Check if menu library is loaded
        this->libs_loaded = this->config->IsLoaded();
        if (this->libs_loaded == false) return;

        auto menu = this->config->CreateMenu("Evade", "Evade");
        this->config->AddLabel(menu, "Settings", "Settings", true);
        std::string char_name = this->api->GetCharacterName(this->my_hero);

        // Core settings
        auto core = this->config->AddSubMenu(menu, "Core", "<< Core >>");
        this->settings["Buffer"] = this->config->AddSlider(core, "Buffer", "Area Hitbox Buffer", 0, 0, 20, 5);
        this->settings["Delta"] = this->config->AddSlider(core, "Delta", "Collision Time Delta", 0, 0, 100, 5);
        this->settings["Interval"] = this->config->AddSlider(core, "Interval", "Force Click Interval", 35, 25, 50, 1);
        this->settings["Points"] = this->config->AddSlider(core, "Points", "Maximum Evade Points", 32, 8, 32, 1);
        this->settings["Reaction"] = this->config->AddSlider(core, "Reaction", "Minimum Reaction Time", 150, 0, 250, 10);
        this->settings["Retries"] = this->config->AddSlider(core, "Retries", "Solution Finder Retries", 1, 0, 3, 1);
        this->settings["Search"] = this->config->AddSlider(core, "Search", "Threat Search Range", 1000, 0, 1500, 100);

        // Main settings
        auto main = this->config->AddSubMenu(menu, "Main", "<< Main >>");

        // Standard options
        this->config->AddLabel(main, "Standard", "Standard Options", true);
        this->settings["Dodge"] = this->config->AddToggle(main, "Dodge", "Dodge Skillshots", (int)('\\'), true);
        this->settings["Draw"] = this->config->AddCheckbox(main, "Draw", "Draw Skillshots", true);
        this->settings["Path"] = this->config->AddCheckbox(main, "Path", "Draw Evade Path", true);
        this->settings["Status"] = this->config->AddCheckbox(main, "Status", "Draw Evade Status", true);
        this->settings["DetectCol"] = this->config->AddCheckbox(main, "DetectCol", "Detect Unit Collision", true);
        this->settings["DetectFog"] = this->config->AddCheckbox(main, "DetectFog", "Detect From Fog of War", true);
        this->settings["DodgeFog"] = this->config->AddCheckbox(main, "DodgeFog", "Dodge From Fog of War", true);

        // Dodge only dangerous
        this->config->AddLabel(main, "OnlyDangerous", "Dodge Only Dangerous", true);
        this->settings["DoDKey"] = this->config->AddKey(main, "DoDKey", "Process Key", (int)('J'));
        this->settings["DoDMode"] = this->config->AddList(main, "DoDMode", "Toggle Mode", { "Hold", "One Tap" }, 0);

        // Unit initialization
        auto N = [&](const auto& u) { return this->api->GetCharacterName(u); };

        auto ally_units = this->api->GetAllyHeroes(FLT_MAX, Vector(), false);
        auto enemy_units = this->api->GetEnemyHeroes(FLT_MAX, Vector(), false);
        auto allies = ally_units.Select<std::string>([&](const auto& u) { return N(u); });
        auto enemies = enemy_units.Select<std::string>([&](const auto& u) { return N(u); });

        bool mel = enemies.Any([](const auto& n) { return n == "Mel"; });
        bool sylas = enemies.Any([](const auto& n) { return n == "Sylas"; });
        bool viego = enemies.Any([](const auto& n) { return n == "Viego"; });
        bool connect = mel || sylas || viego || this->api->GetMapId() == 30;
        auto units = ((connect == true) ? allies.Concat(enemies) : enemies);
        units = units.OrderBy<std::string>([](const auto& n) { return n; });

        // Spell settings
        auto spells = this->config->AddSubMenu(menu, "Spells", "<< Spells >>");

        // Supported skillshots
        this->config->AddLabel(spells, "Skillshots", "Supported Skillshots", true);

        const auto& skillshots = this->data->GetSkillshots();
        for (const auto& unit : units.Distinct().Concat({ "Global" })) // Linq<std::string>({ char_name, "Global" }))
        {
            auto unit_menu = this->config->AddSubMenu(spells, "D|" + unit, unit);

            for (const auto& [name, data] : skillshots)
            {
                if (data.ChampionName == unit)
                {
                    std::string id = "D|" + name, spell_slot = std::string(1, data.SkillshotSlot) + " - ";
                    auto sets = this->config->AddSubMenu(unit_menu, id, (name != "Debug" ? spell_slot : "") + data.DisplayName);
                    this->settings[id + "|Dodge"] = this->config->AddCheckbox(sets, id + "|Dodge", "Dodge Skillshot", true);
                    this->settings[id + "|Draw"] = this->config->AddCheckbox(sets, id + "|Draw", "Draw Skillshot", true);
                    this->settings[id + "|Dangerous"] = this->config->AddCheckbox(sets, id + "|Dangerous", "Dangerous", data.Dangerous);
                    if (data.FogSupport) this->settings[id + "|Fog"] = this->config->AddCheckbox(sets, id + "|Fog", "Dodge From Fog", true);
                    this->settings[id + "|Override"] = this->config->AddCheckbox(sets, id + "|Override", "Override Reaction", false);
                    this->settings[id + "|Reaction"] = this->config->AddSlider(sets, id + "|Reaction", "Reaction Time", 0, 0, 250, 5);
                    this->settings[id + "|Danger"] = this->config->AddSlider(sets, id + "|Danger", "Danger Level", data.DangerLevel, 1, 5, 1);
                    this->settings[id + "|Health"] = this->config->AddSlider(sets, id + "|Health", "Min Health %", 100, 0, 100, 1);
                    if (data.MissileName != "") this->missiles.Append({ unit, name, data.MissileName });
                    if (data.ParticleName != "") this->particles.Append({ unit, name, data.ParticleName });
                }
            }
        }

        // Evading spellbook
        this->config->AddLabel(spells, "Spellbook", "Evading Spellbook", true);

        const auto& evade_spellbook = this->data->GetEvadingSpells();
        for (const auto& unit : { char_name, std::string("Local") })
        {
            auto unit_menu = this->config->AddSubMenu(spells, "E|" + unit, unit);

            for (const auto& [name, data] : evade_spellbook)
            {
                if (data.ChampionName == unit)
                {
                    std::string id = "E|" + name, spell_slot = std::string(1, data.SpellSlot);
                    int slot = name == "Flash" ? this->flash_slot : name == "Ghost" ? this->ghost_slot : -1;
                    auto sets = this->config->AddSubMenu(unit_menu, id, spell_slot + " - " + data.DisplayName);
                    this->settings[id + "|UseSpell"] = this->config->AddCheckbox(sets, id + "|UseSpell", "Use Spell", true);
                    this->settings[id + "|Danger"] = this->config->AddSlider(sets, id + "|Danger", "Danger Level", data.DangerLevel, 1, 5, 1);
                    this->evading_spells.Append({ name, data.SpellSlot != 'G' ? this->char_to_slot(data.SpellSlot) : slot });
                }
            }
        }

        // Ally shielding
        this->config->AddLabel(spells, "Shielding", "Ally Shielding", true);

        const auto& shield_spellbook = this->data->GetShieldSpells();
        for (const auto& [name, data] : shield_spellbook)
        {
            if (data.ChampionName == char_name)
            {
                std::string id = "S|" + name, spell_slot = std::string(1, data.SpellSlot);
                auto sets = this->config->AddSubMenu(spells, id, spell_slot + " - " + data.DisplayName);
                this->settings[id + "|UseSpell"] = this->config->AddCheckbox(sets, id + "|UseSpell", "Use Spell", true);
                this->settings[id + "|Danger"] = this->config->AddSlider(sets, id + "|Danger", "Danger Level", 1, 1, 5, 1);
                this->shield_spell = { name, this->char_to_slot(data.SpellSlot) };
            }
        }

        // Color settings
        auto colors = this->config->AddSubMenu(menu, "Color", "<< Colors >>");

        this->config->AddLabel(colors, "Arrow", "Evade Path Arrow", true);

        auto arrow_prim = this->config->AddSubMenu(colors, "Primary", "Primary");
        this->settings["ArrowPrimA"] = this->config->AddSlider(arrow_prim, "ArrowPrimA", "Alpha", 224, 0, 255, 1);
        this->settings["ArrowPrimR"] = this->config->AddSlider(arrow_prim, "ArrowPrimR", "Red", 224, 0, 255, 1);
        this->settings["ArrowPrimG"] = this->config->AddSlider(arrow_prim, "ArrowPrimG", "Green", 224, 0, 255, 1);
        this->settings["ArrowPrimB"] = this->config->AddSlider(arrow_prim, "ArrowPrimB", "Blue", 224, 0, 255, 1);

        auto arrow_sec = this->config->AddSubMenu(colors, "Secondary", "Secondary");
        this->settings["ArrowSecA"] = this->config->AddSlider(arrow_sec, "ArrowSecA", "Alpha", 224, 0, 255, 1);
        this->settings["ArrowSecR"] = this->config->AddSlider(arrow_sec, "ArrowSecR", "Red", 192, 0, 255, 1);
        this->settings["ArrowSecG"] = this->config->AddSlider(arrow_sec, "ArrowSecG", "Green", 192, 0, 255, 1);
        this->settings["ArrowSecB"] = this->config->AddSlider(arrow_sec, "ArrowSecB", "Blue", 192, 0, 255, 1);

        this->config->AddLabel(colors, "States", "Skillshot States", true);

        auto disabled = this->config->AddSubMenu(colors, "Disabled", "Disabled");
        this->settings["DisabledA"] = this->config->AddSlider(disabled, "DisabledA", "Alpha", 160, 0, 255, 1);
        this->settings["DisabledR"] = this->config->AddSlider(disabled, "DisabledR", "Red", 160, 0, 255, 1);
        this->settings["DisabledG"] = this->config->AddSlider(disabled, "DisabledG", "Green", 160, 0, 255, 1);
        this->settings["DisabledB"] = this->config->AddSlider(disabled, "DisabledB", "Blue", 160, 0, 255, 1);

        auto enabled = this->config->AddSubMenu(colors, "Enabled", "Enabled");
        this->settings["EnabledA"] = this->config->AddSlider(enabled, "EnabledA", "Alpha", 255, 0, 255, 1);
        this->settings["EnabledR"] = this->config->AddSlider(enabled, "EnabledR", "Red", 255, 0, 255, 1);
        this->settings["EnabledG"] = this->config->AddSlider(enabled, "EnabledG", "Green", 255, 0, 255, 1);
        this->settings["EnabledB"] = this->config->AddSlider(enabled, "EnabledB", "Blue", 255, 0, 255, 1);

        auto ignored = this->config->AddSubMenu(colors, "Ignored", "Ignored");
        this->settings["IgnoredA"] = this->config->AddSlider(ignored, "IgnoredA", "Alpha", 255, 0, 255, 1);
        this->settings["IgnoredR"] = this->config->AddSlider(ignored, "IgnoredR", "Red", 224, 0, 255, 1);
        this->settings["IgnoredG"] = this->config->AddSlider(ignored, "IgnoredG", "Green", 224, 0, 255, 1);
        this->settings["IgnoredB"] = this->config->AddSlider(ignored, "IgnoredB", "Blue", 0, 0, 255, 1);

        auto uncertain = this->config->AddSubMenu(colors, "Uncertain", "Uncertain");
        this->settings["UncertainA"] = this->config->AddSlider(uncertain, "UncertainA", "Alpha", 255, 0, 255, 1);
        this->settings["UncertainR"] = this->config->AddSlider(uncertain, "UncertainR", "Red", 255, 0, 255, 1);
        this->settings["UncertainG"] = this->config->AddSlider(uncertain, "UncertainG", "Green", 140, 0, 255, 1);
        this->settings["UncertainB"] = this->config->AddSlider(uncertain, "UncertainB", "Blue", 0, 0, 255, 1);

        auto undodgeable = this->config->AddSubMenu(colors, "Undodgeable", "Undodgeable");
        this->settings["UndodgeableA"] = this->config->AddSlider(undodgeable, "UndodgeableA", "Alpha", 255, 0, 255, 1);
        this->settings["UndodgeableR"] = this->config->AddSlider(undodgeable, "UndodgeableR", "Red", 255, 0, 255, 1);
        this->settings["UndodgeableG"] = this->config->AddSlider(undodgeable, "UndodgeableG", "Green", 32, 0, 255, 1);
        this->settings["UndodgeableB"] = this->config->AddSlider(undodgeable, "UndodgeableB", "Blue", 16, 0, 255, 1);

        // Debug settings
        auto debug = this->config->AddSubMenu(menu, "Debug", "<< Debug >>");
        this->settings["DebugOn"] = this->config->AddCheckbox(debug, "DebugOn", "Enable Debug", false);
        this->settings["DebugBox"] = this->config->AddCheckbox(debug, "DebugBox", "Include Hitbox", true);
        this->settings["DebugInf"] = this->config->AddCheckbox(debug, "DebugInf", "Infinite Speed", false);
        this->settings["DebugSummon"] = this->config->AddKey(debug, "DebugSummon", "Summon Skillshot", (int)('L'));
        this->settings["DebugAngle"] = this->config->AddSlider(debug, "DebugAngle", "Angle", 50, 20, 180, 5);
        this->settings["DebugDelay"] = this->config->AddSlider(debug, "DebugDelay", "Delay", 250, 0, 3000, 25);
        this->settings["DebugRadius"] = this->config->AddSlider(debug, "DebugRadius", "Radius", 70, 20, 300, 5);
        this->settings["DebugRange"] = this->config->AddSlider(debug, "DebugRange", "Range", 1000, 500, 1500, 25);
        this->settings["DebugSpeed"] = this->config->AddSlider(debug, "DebugSpeed", "Speed", 1800, 500, 2500, 25);
        this->settings["DebugType"] = this->config->AddList(debug, "DebugType", "Type", { "Circular", "Conic", "Linear" }, 2);

        // Version
        this->config->AddLabel(menu, "Creator", "Creator: Uncle Ark", true);
        this->config->AddLabel(menu, "Version", "Version: " + EVADE_VERSION, true);
    }

    // Core methods

    bool Program::RecalculatePath(float delay)
    {
        this->ResetState(delay);
        return true;
    }

    void Program::RequestUpdateOnce(int id, float delay)
    {
        if (id <= this->update_id) return;
        this->ResetState(delay);
        this->update_id = id;
    }

    void Program::ResetSkillshots()
    {
        this->skillshots.ForEach([](Skillshot* skillshot)
        {
            skillshot->Set().Undodgeable = false;
        });
    }

    void Program::ResetState(float delay)
    {
        float timer = this->api->GetTime();
        this->recalc_timer = timer + delay;
        this->dont_use_spell = false;
        this->recalc_path = true;
        this->undodgeable = false;
        this->ResetSkillshots();
    }

    void Program::SwitchOff(bool instant)
    {
        this->safe_pos = Vector();
        if (!instant) return;
        this->evade_timer = 0.0f;
        this->evade_pos = Vector();
        this->safe_order_count = 0;
        this->undodgeable = false;
        this->ResetSkillshots();
    }

    // Public API methods

    bool Program::IsEnabled() const
    {
        return this->GetValue<bool>("Dodge");
    }

    bool Program::IsEvading() const
    {
        return this->evade_pos.IsValid();
    }

    bool Program::IsInsideHardCC() const
    {
        return this->dangerous.Any([](Skillshot* skillshot)
        {
            return skillshot->Get().HardCC;
        });
    }

    bool Program::IsInsideSoftCC() const
    {
        return this->dangerous.Any([](Skillshot* skillshot)
        {
            return skillshot->Get().SoftCC;
        });
    }

    bool Program::IsSafe(float x, float y) const
    {
        return !this->IsDangerous(x, y);
    }

    bool Program::IsDangerous(float x, float y) const
    {
        return this->skillshots.Any([&](Skillshot* skillshot)
        {
            return skillshot->IsDangerous(Vector(x, y));
        });
    }

    float Program::TimeToHit(float x, float y) const
    {
        return this->skillshots.Aggregate<float>(-1.0f, [&](float result, auto skillshot)
        {
            float time = skillshot->TimeToHit(Vector(x, y));
            return result < 0 ? time : (time < 0 ? result : MIN(result, time));
        });
    }

    float Program::TimeToHit() const
    {
        return this->dangerous.Aggregate<float>(-1.0f, [&](float result, auto skillshot)
        {
            float time = MAX(0.0f, skillshot->TimeToHit(this->hero_pos, true));
            return result < 0 ? time : MIN(result, time);
        });
    }

    float Program::CalculateDamage() const
    {
        const auto& data = this->data->GetSkillshots();
        std::unordered_map<std::string, bool> excluded;

        return this->dangerous.Aggregate<float>(0.0f, [&](float total, auto skillshot)
        {
            const auto& caster = skillshot->Get().Caster;
            const std::string& name = skillshot->Get().SkillshotName;
            std::string id = std::to_string(this->api->GetObjectId(caster));

            // Skip calculation if relationship was already handled before
            if (excluded.find(name + id) != excluded.end()) return total;

            // Mark related skillshots to prevent duplicate damage
            for (const auto& exclusion : data.at(name).Exclusions)
            {
                excluded[exclusion + id] = true;
            }
            return total + skillshot->Get().Damage;
        });
    }

    int Program::GetDangerLevel() const
    {
        return this->dangerous.Aggregate(0, [](int danger, auto skillshot)
        {
            return MAX(danger, skillshot->Get().DangerLevel);
        });
    }

    // Events

    void Program::OnTick()
    {
        // Remove invalid or dead objects from collections
        for (auto& [key, collection] : this->objects)
        {
            collection.DeleteAll([&](auto& object)
            {
                return !this->api->IsValid(object)
                    || !this->api->IsAlive(object);
            });
        }

        // Activate 'Dodge Only Dangerous' mode
        if (this->GetValue<int>("DoDMode") == 0)
        {
            bool pushed = this->GetValue<bool>("DoDKey");
            this->dodge_dangerous = pushed;
        }

        // Reset evasion if hero has shifted significantly within a tick
        if (this->hero_pos.DistanceSquared(this->prev_pos) > 10000)
        {
            this->ResetState(0.0f);
        }

        this->prev_pos = this->hero_pos.Clone();
        this->hero_pos = this->api->GetPosition(this->my_hero);
        float speed = this->api->GetMovementSpeed(this->my_hero);
        float timer = this->api->GetTime();

        // Reset evasion in case of higher movement speed
        if (this->undodgeable && speed > this->move_speed)
        {
            this->ResetState(0.0f);
        }

        // Toggle evasion based on the movement state
        this->move_speed = speed;
        bool can_evade = this->utils->CanEvade();
        if (can_evade != this->can_evade)
        {
            if (can_evade && !this->can_evade)
            {
                this->ResetState(0.0f);
            }
            else if (!can_evade && this->can_evade)
            {
                this->SwitchOff(true);
            }
            this->can_evade = can_evade;
        }

        // If the hero is out of dangerous area, reset everything
        if (!this->safe_pos.IsValid() && this->evade_pos.IsValid())
        {
            this->ResetState(0.0f);
            this->SwitchOff(true);
        }

        // Turn off evasion if its disabled
        if (!this->GetValue<bool>("Dodge"))
        {
            return this->SwitchOff(true);
        }

        Vector destination = this->api->GetPathEndPos(this->my_hero);
        float interval = (float)this->GetValue<int>("Interval") / 100.0f;

        // Force a click to trigger the OnIssueOrder event, which is used
        // to override movement when trying to cross through skillshots
        if (!this->evade_pos.IsValid() && this->hero_pos != destination &&
            this->skillshots.Any() && timer - this->order_timer > interval)
        {
            float height = this->api->GetHeight(this->my_hero);
            this->api->SendMoveCommand(destination, height);
            this->order_timer = timer;
        }

        // Update and get all skillshots considered for evasion
        this->considered = this->process->UpdateConsidered();

        // Retrieve all skillshots that our hero is currently inside of
        this->dangerous = this->skillshots.Where([&](Skillshot* skillshot)
        {
            return skillshot->IsDangerous(this->hero_pos);
        });

        // Retrieve skillshots that provide no threat to our hero
        auto safe_area = this->skillshots.Except(this->dangerous);

        // Create a copy of the skillshots considered for processing
        auto skillshots = Linq<Skillshot*>(this->considered.ToArray());

        // Turn off evasion if its no longer required
        if (skillshots.All([&](Skillshot* skillshot)
        {
            bool undodgeable = skillshot->Get().Undodgeable;
            return undodgeable || safe_area.Contains(skillshot);
        }))
        {
            this->safety_timer = timer;
            this->utils->ShieldAllies(this->shield_spell);
            return this->SwitchOff(false);
        }

        // Determine the reaction delay for evasion
        float delay = skillshots.Min([&](Skillshot* skillshot)
        {
            int reaction = this->GetValue<int>("Reaction");
            const std::string& name = skillshot->Get().SkillshotName;
            bool s_override = this->GetValue<bool>("D|" + name + "|Override");
            int s_reaction = this->GetValue<int>("D|" + name + "|Reaction");
            return (float)(s_override ? s_reaction : reaction) / 1000.0f;
        });

        // Calculate hash value for the set of skillshots
        uint32_t hash = skillshots.Aggregate<uint32_t>(0,
            [](uint32_t hash_value, Skillshot* skillshot)
        {
            const int unique_id = skillshot->GetId();
            return ((hash_value << 5) - hash_value) + unique_id;
        });

        // Reset evasion if hash code has recently changed
        // Change indicates an addition/removal of a skillshot
        if (this->hash_code != hash) this->ResetState(delay);

        this->hash_code = hash;
        float time_diff = this->recalc_timer - timer;
        if (!this->recalc_path || time_diff > 0) return;

        // Check if any skillshot has a dynamic displacement
        float health = this->api->GetHealth(this->my_hero);
        bool dynamic = skillshots.Any([&](Skillshot* skillshot)
        {
            const std::string& name = skillshot->Get().SkillshotName;
            const auto& data = this->data->GetSkillshots().at(name);
            if (data.Acceleration != 0 || data.BackToCaster ||
                data.FastEvade || data.FollowCaster) return true;
            return this->update_id >= skillshot->GetId();
        });

        // Sort skillshots by specific names first, then by danger level and damage
        // The lowest priority is assigned to skillshots that include multiple parts
        auto splitted = { "LeonaSolarFlare", "SettW", "SyndraE", "VeigarEventHorizon" };
        skillshots = skillshots.OrderBy<bool>([&splitted](Skillshot* skillshot)
        {
            const std::string& name = skillshot->Get().SkillshotName;
            auto begin = splitted.begin(), ending = splitted.end();
            return std::find(begin, ending, name) == ending;
        })
        .ThenByDescending<bool>([&health](Skillshot* skillshot)
        {
            return health > skillshot->Get().Damage;
        })
        .ThenBy<int>([](Skillshot* skillshot)
        {
            return skillshot->Get().DangerLevel;
        })
        .ThenByDescending<int>([](Skillshot* skillshot)
        {
            return skillshot->GetId();
        });

        // Update item slots and prepare a new evading solution
        this->utils->UpdateItemSlots(this->evading_spells);
        this->SwitchOff(true); this->recalc_path = false;

        // Attempt to find a safe solution
        int retries = this->GetValue<int>("Retries"), iter = 0;
        while (skillshots.Count() > 0 && iter++ <= retries)
        {
            if (iter > 1 && !this->dangerous.Intersect(skillshots).Any())
            {
                break;
            }
            auto solution = this->utils->FindSolution(skillshots, dynamic);
            if (this->utils->ShouldEvade(skillshots, solution, dynamic))
            {
                this->safe_pos = solution.Destination.Clone();
                this->evade_pos = this->utils->Execute(solution);
                this->utils->EvadeToSpot(this->evade_pos);
                return;
            }
            else if (solution.IsValid)
            {
                return;
            }
            skillshots.First()->Set().Undodgeable = true;
            skillshots.RemoveAt(0);
        }

        // No solutions found, mark skillshots as undodgeable
        this->considered.ForEach([](Skillshot* skillshot)
        {
            skillshot->Set().Undodgeable = true;
        });
        this->undodgeable = true;
    }

    void Program::OnDraw()
    {
        this->safe_order_count++;
        this->process->UpdateSkillshots();

        // Force core to start evading toward the spot
        if (this->utils->EvadeToSpot(this->evade_pos))
            this->evade_timer = this->api->GetTime();

        // Draw the path line that the hero is following through
        if (this->evade_pos.IsValid() && this->GetValue<bool>("Path"))
        {
            int p_alpha = this->GetValue<int>("ArrowPrimA");
            int p_red = this->GetValue<int>("ArrowPrimR");
            int p_green = this->GetValue<int>("ArrowPrimG");
            int p_blue = this->GetValue<int>("ArrowPrimB");

            int s_alpha = this->GetValue<int>("ArrowSecA");
            int s_red = this->GetValue<int>("ArrowSecR");
            int s_green = this->GetValue<int>("ArrowSecG");
            int s_blue = this->GetValue<int>("ArrowSecB");

            float height = this->api->GetHeight(this->evade_pos);
            Vector hero_pos = this->api->GetPosition(this->my_hero);
            uint32_t c1 = this->api->ARGB(p_alpha, p_red, p_green, p_blue);
            uint32_t c2 = this->api->ARGB(s_alpha, s_red, s_green, s_blue);
            this->utils->DrawArrow(hero_pos, this->evade_pos, height, c1, c2);
        }

        // Display evasion status based on the toggled options
        if (this->GetValue<bool>("Status"))
        {
            bool dodge_dang = this->dodge_dangerous;
            bool dodge_on = this->GetValue<bool>("Dodge");
            auto text = dodge_on == false ? "Evade: OFF" :
                dodge_dang ? "Evade: Dangerous" : "Evade: ON";
            uint32_t color = dodge_on == false ? 0xC0FF3030 :
                dodge_dang == true ? 0xC0FFFF00 : 0xC0FFFFFF;
            float height = this->api->GetHeight(this->my_hero);
            auto pos = this->api->ToScreen(hero_pos, height);
            auto text_pos = Vector2(pos.x, pos.y + 50.0f);
            this->api->DrawCenteredText(text_pos, text, color);
        }

        if (!this->GetValue<bool>("Draw")) return;
        this->skillshots.ForEach([&](Skillshot* skillshot)
        {
            int alpha, red, green, blue;
            const std::string& name = skillshot->Get().SkillshotName;
            if (!this->GetValue<bool>("D|" + name + "|Draw")) return;

            // Assign the appropriate color for a skillshot
            if (!this->GetValue<bool>("D|" + name + "|Dodge")
                || !this->GetValue<bool>("Dodge"))
            {
                alpha = this->GetValue<int>("DisabledA");
                red = this->GetValue<int>("DisabledR");
                green = this->GetValue<int>("DisabledG");
                blue = this->GetValue<int>("DisabledB");
            }
            else if (skillshot->Get().Undodgeable)
            {
                alpha = this->GetValue<int>("UndodgeableA");
                red = this->GetValue<int>("UndodgeableR");
                green = this->GetValue<int>("UndodgeableG");
                blue = this->GetValue<int>("UndodgeableB");
            }
            else if (!this->can_evade)
            {
                alpha = this->GetValue<int>("UncertainA");
                red = this->GetValue<int>("UncertainR");
                green = this->GetValue<int>("UncertainG");
                blue = this->GetValue<int>("UncertainB");
            }
            else if (!this->considered.Contains(skillshot))
            {
                alpha = this->GetValue<int>("IgnoredA");
                red = this->GetValue<int>("IgnoredR");
                green = this->GetValue<int>("IgnoredG");
                blue = this->GetValue<int>("IgnoredB");
            }
            else
            {
                alpha = this->GetValue<int>("EnabledA");
                red = this->GetValue<int>("EnabledR");
                green = this->GetValue<int>("EnabledG");
                blue = this->GetValue<int>("EnabledB");
            }

            // Set colors for a primary hitbox and offset hitbox
            uint32_t c1 = this->api->ARGB(alpha, red, green, blue);
            uint32_t c2 = ((c1 >> 24) / 15 << 24) | (c1 & 0x00FFFFFF);

            // Draw skillshot's total hitbox with the assigned colors
            skillshot->Draw(std::vector<uint32_t>{c1, c2}.data());
        });
    }

    void Program::OnProcessSpell(const Obj_AI_Base& unit, std::string& spell_name,
        const int spell_level, const Vector& start_pos, const Vector& end_pos,
        uint32_t target_id, float cast_delay, float extra_delay, bool proc_next)
    {
        if (!this->api->IsValid(unit)) return;
        float timer = this->api->GetTime();
        size_t npos = std::string::npos;

        // Process any actions if they were done by our hero
        if (this->api->Compare(unit, this->my_hero))
        {
            if (spell_name.find("Attack") != npos)
            {
                this->attack_timer = timer; return;
            }
        }

        // Continue processing if the unit is a hero
        if (!this->api->IsHero(unit)) return;

        // Skip processing if the hero is an ally
        auto caster = this->api->AsHero(unit);
        if (!this->api->IsEnemy(caster)) return;

        // Track cast time of a skillshot stolen by either Sylas or Viego
        std::string caster_name = this->api->GetCharacterName(caster);
        bool stealer = caster_name == "Sylas" || caster_name == "Viego";
        if (stealer && spell_name.find(caster_name) != 0)
        {
            if (caster_name == "Sylas") this->sylas_timer = timer;
            if (caster_name == "Viego") this->viego_timer = timer;
        }

        const auto& patterns = this->data->GetPatterns();
        const auto& skillshots = this->data->GetSkillshots();

        // Find any matching pattern in the spell name to map to its primary name
        auto it = std::find_if(patterns.begin(), patterns.end(), [&](auto& pair)
        {
            return spell_name.find(pair.first) != npos;
        });
        if (it != patterns.end())
        {
            spell_name = it->second.PrimaryName;
        }

        // Check if the spell is part of our supported skillshots
        if (skillshots.find(spell_name) == skillshots.end()) return;
        const auto& data = skillshots.at(spell_name);

        // Skip processing if Yasuo is dashing
        bool dashing = this->api->IsDashing(caster);
        if (spell_name.find("YasuoQ") != npos && dashing) return;

        // Remove all Anivia Q's if the spell was recast
        if (spell_name == "FlashFrost")
        {
            size_t count = this->skillshots.Count();
            this->skillshots.DeleteAll([&](Skillshot* skillshot)
            {
                const auto& s_caster = skillshot->Get().Caster;
                const std::string& s_name = skillshot->Get().SkillshotName;
                bool anivia = s_name == "AniviaQ2" || s_name == "FlashFrost";
                return anivia && this->api->Compare(caster, s_caster);
            });
            if (this->skillshots.Count() != count) return;
        }

        // Ignore duplicates and similar skillshots from the same caster
        if (data.IgnoreAlive && this->skillshots.Any([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get().Caster;
            const std::string& s_name = skillshot->Get().SkillshotName;
            bool same_caster = this->api->Compare(caster, s_caster);
            return same_caster == true && spell_name == s_name;
        }))
        return;

        bool visible = this->api->IsVisible(caster);
        Vector dir = this->api->GetDirection(caster);
        Vector unit_pos = this->api->GetPosition(caster);
        int slot = this->char_to_slot(data.SkillshotSlot);
        int danger = this->GetValue<int>("D|" + spell_name + "|Danger");
        Vector dest_pos = data.UseDirection ? start_pos + dir : end_pos;
        DetectionType detect_type = DetectionType::ON_PROCESS_SPELL;

        float cone_angle = M_RAD(data.ConeAngle);
        float delay = data.UseWindup ? cast_delay : data.Delay;
        float height = this->api->GetHeight(end_pos), hitbox = 0.0f;
        bool adjust_height = height == 0.0f || ABS(height) > 100.0f;
        height = adjust_height ? this->api->GetHeight(caster) : height;
        if (data.AddHitbox) hitbox = this->api->GetHitbox(this->my_hero);

        auto params = ActiveData(dest_pos, unit_pos.Clone(), start_pos,
            spell_name, caster_name, caster, nullptr, danger, slot,
            data.FixedRange, !visible, data.Invert, data.HardCC,
            data.SoftCC, data.Rotate90, height, hitbox, data.Offset,
            data.Range, data.SideRange, data.Radius, data.InnerRadius,
            data.OuterRadius, cone_angle, delay + extra_delay,
            data.ExtraTime, data.Speed, data.Collisions, detect_type,
            data.SkillshotType, data.Damage, data.DamageType);

        // Create a new skillshot instance based on the collected data
        auto skillshot = this->process->CreateSkillshot(params, false);

        // Skip skillshot which is way too far from our hero
        if (data.Range < 25000.0f && !data.IsTrap)
        {
            float range = skillshot->Get().Range;
            const Vector& ending = skillshot->Get().EndPos;
            float dist = this->hero_pos.DistanceSquared(ending);
            bool too_far = dist > (range + 2500) * (range + 2500);
            if (too_far) { delete skillshot; return; }
        }

        // Update and register the skillshot for processing
        this->process->UpdateData(skillshot, spell_level);
        this->skillshots.Append(skillshot);
        if (!proc_next) return;

        // Add extra skillshots that originate from main one
        auto extra = this->process->AddSkillshots(skillshot);
        extra.ForEach([&](const OnProcSpellArgs& data)
        {
            std::string next_name = data.SpellName;
            this->OnProcessSpell(data.Caster, next_name, 0,
                data.StartPos, data.EndPos.Clone(), target_id,
                cast_delay, data.ExtraDelay, data.ProcessNext);
        });

        // Remove primary skillshot after processing related skillshots
        if (data.SkipAncestor) this->skillshots.Delete(skillshot);
    }

    void Program::OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info)
    {
        auto name = this->api->GetSpellCastName(info);
        int level = this->api->GetSpellCastLevel(info);
        Vector end_pos = this->api->GetSpellCastEndPos(info);
        Vector start_pos = this->api->GetSpellCastStartPos(info);
        uint32_t target_id = this->api->GetSpellCastTargetId(info);
        float cast_delay = this->api->GetSpellCastWindupTime(info);
        this->OnProcessSpell(unit, name, level, start_pos,
            end_pos, target_id, cast_delay, 0.0f, true);
    }

    void Program::OnCreateObject(const Object& object, uint32_t object_id)
    {
        if (!this->api->IsValid(object)) return;
        float timer = this->api->GetTime();

        ParticleEmitter particle = nullptr;
        MissileClient missile = nullptr;
        Object unit = nullptr;
        std::string spell_name;

        // Handle cases for both missile and particle
        if (this->api->IsMissile(object))
        {
            missile = this->api->AsMissile(object);
            if (!this->api->IsValid(missile)) return;

            // Retrieve spell name based on missile name
            spell_name = this->api->GetMissileName(missile);
            auto info = this->missiles.FirstOrDefault([&](auto& info)
            {
                std::regex pattern(info.MissileName);
                return std::regex_search(spell_name, pattern);
            });

            // Exit if spell name was not found
            if (info.SpellName.empty()) return;
            spell_name = info.SpellName;

            // Get the valid owner of the missile
            unit = this->api->GetMissileOwner(missile);
            if (!this->api->IsValid(unit)) return;
        }
        else if (this->api->IsParticle(object))
        {
            particle = this->api->AsParticle(object);
            if (!this->api->IsValid(particle)) return;

            // Retrieve spell name based on particle name
            std::string object_name = this->api->GetObjectName(object);
            auto info = this->particles.FirstOrDefault([&](auto& info)
            {
                size_t result = object_name.find(info.CharName);
                if (result == std::string::npos) return false;
                std::regex pattern(info.ParticleName);
                return std::regex_search(object_name, pattern);
            });

            // Exit if spell name was not found
            if (info.SpellName.empty()) return;
            spell_name = info.SpellName;

            // Get the valid owner of the particle object
            unit = this->utils->GetObjectOwner(object_name);
            if (!this->api->IsValid(unit)) return;
        }
        else if (this->api->IsMinion(object))
        {
            auto minion = this->api->AsMinion(object);
            std::string name = this->api->GetObjectName(minion);
            
            // Store object in the list for further processing
            if (this->objects.find(name) != this->objects.end())
            {
                this->objects[name].Append(minion);
            }
            return;
        }
        else return;

        // Continue processing if the unit is a hero
        if (!this->api->IsHero(unit)) return;

        // Skip processing if the hero is an ally
        auto caster = this->api->AsHero(unit);
        if (!this->api->IsEnemy(caster)) return;

        // Mel should never be able to replicate particles, return
        std::string caster_name = this->api->GetCharacterName(caster);
        if (particle != nullptr && caster_name == "Mel") return;

        // Check if Sylas or Viego recently casted a stolen skillshot
        bool stealer = caster_name == "Sylas" || caster_name == "Viego";
        if (particle && stealer && spell_name.find(caster_name) != 0)
        {
            bool sylas_cast = timer - this->sylas_timer < 0.1f;
            bool viego_cast = timer - this->viego_timer < 0.1f;
            if (caster_name == "Sylas" && !sylas_cast) return;
            if (caster_name == "Viego" && !viego_cast) return;
        }

        // Store duplicates and similar skillshots for further processing
        auto duplicates = this->skillshots.Where([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get().Caster;
            const std::string& s_name = skillshot->Get().SkillshotName;
            bool same_caster = this->api->Compare(caster, s_caster);
            return same_caster == true && spell_name == s_name;
        });

        // Save an object to track the missile skillshot
        Skillshot* duplicate = duplicates.FirstOrDefault();
        if (missile && duplicate && !duplicate->Get().ObjectPtr)
        {
            duplicate->Set().ObjectPtr = (Object)missile;
        };

        // Ignore duplicated instances from the same caster
        auto& data = this->data->GetSkillshots().at(spell_name);
        bool check_dups = data.IgnoreAlive || !data.Exception;
        if (check_dups && duplicates.Any([&](Skillshot* skillshot)
        {
            DetectionType s_type = skillshot->Get().DetectionType;
            bool proc_type = s_type == DetectionType::ON_PROCESS_SPELL;
            return data.IgnoreAlive || !data.Exception && proc_type;
        }))
        return;

        // Remove all similar triggered skillshots that should be overridden by new one
        if (data.Overrider != "") this->skillshots.DeleteAll([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get().Caster;
            const std::string& s_name = skillshot->Get().SkillshotName;
            const DetectionType s_type = skillshot->Get().DetectionType;
            bool proc_type = s_type == DetectionType::ON_PROCESS_SPELL;
            bool can_remove = data.Overrider == s_name && proc_type;
            return can_remove && this->api->Compare(caster, s_caster);
        });

        bool visible = this->api->IsVisible(caster);
        if (!visible && !this->GetValue<bool>("DetectFog")) return;
        bool multi = this->data->GetConnectors().count(spell_name) > 0;
        bool fixed_range = data.Range >= 25000.0f || data.FixedRange &&
            !multi && data.Acceleration == 0.0f && data.Range != 12500.0f;

        bool irelia_mis = spell_name == "IreliaEMissile";
        int spell_slot = this->char_to_slot(data.SkillshotSlot);
        int danger = this->GetValue<int>("D|" + spell_name + "|Danger");
        DetectionType detect_type = DetectionType::ON_OBJECT_CREATED;
        Object obj_ptr = missile ? (Object)missile : (Object)particle;

        Vector object_pos = this->api->GetPosition(object), dir;
        Vector start_pos = object_pos, end_pos = object_pos.Clone();
        if (missile) end_pos = this->api->GetMissileEndPos(missile);
        if (missile) start_pos = this->api->GetMissileStartPos(missile);
        if (data.UseEmitter) dir = this->api->GetDirection(particle);
        if (dir.IsValid()) end_pos = start_pos + dir * data.Range;

        float cone_angle = M_RAD(data.ConeAngle), speed = data.Speed;
        float delay = missile ? data.MissileDelay : data.ParticleDelay;
        float height = this->api->GetHeight(end_pos), hitbox = 0.0f;
        bool adjust_height = height == 0.0f || ABS(height) > 100.0f;
        height = adjust_height ? this->api->GetHeight(caster) : height;
        if (data.AddHitbox) hitbox = this->api->GetHitbox(this->my_hero);
        if (irelia_mis) speed = this->api->GetMissileSpeed(missile);

        auto params = ActiveData(end_pos, start_pos.Clone(), start_pos,
            spell_name, caster_name, caster, obj_ptr, danger, spell_slot,
            fixed_range, !visible, data.Invert, data.HardCC, data.SoftCC,
            data.Rotate90, height, hitbox, 0.0f, data.Range, data.SideRange,
            data.Radius, data.InnerRadius, data.OuterRadius, cone_angle,
            delay, data.ExtraTime, speed, data.Collisions, detect_type,
            data.SkillshotType, data.Damage, data.DamageType);

        // Create a new skillshot instance based on the collected data
        auto skillshot = this->process->CreateSkillshot(params, false);

        // Skip skillshot which is way too far from our hero
        if (data.Range < 25000.0f && !data.IsTrap)
        {
            float range = skillshot->Get().Range;
            const Vector& ending = skillshot->Get().EndPos;
            float dist = this->hero_pos.DistanceSquared(ending);
            bool too_far = dist > (range + 2500) * (range + 2500);
            if (too_far) { delete skillshot; return; }
        }

        // Update and register skillshot for further processing
        if (!this->process->UpdateData(skillshot, 0)) return;
        this->skillshots.Append(skillshot);

        // Add extra skillshots that originate from main one
        auto extra = this->process->AddSkillshots(skillshot);
        extra.ForEach([&](const OnProcSpellArgs& data)
        {
            std::string next_name = data.SpellName;
            this->OnProcessSpell(data.Caster, next_name,
                0, data.StartPos, data.EndPos.Clone(), 0,
                0.0f, data.ExtraDelay, data.ProcessNext);
        });

        // Remove primary skillshot after processing related skillshots
        if (data.SkipAncestor) this->skillshots.Delete(skillshot);
    }

    void Program::OnDeleteObject(const Object& object, uint32_t object_id)
    {
        bool collision = this->GetValue<bool>("DetectCol");
        const auto& skillshots = this->data->GetSkillshots();

        // Remove all tracked skillshots that are already dead
        this->skillshots.DeleteAll([&](Skillshot* skillshot)
        {
            if (!skillshot->Get().ObjectPtr) return false;
            const auto& object_ptr = skillshot->Get().ObjectPtr;
            auto& data = skillshots.at(skillshot->Get().SkillshotName);

            bool check = data.MissileName.empty() || collision == true;
            if (!(data.IsTrap || data.HookObject && check)) return false;
            return this->api->GetObjectId(object_ptr) == object_id;
        });
    }

    void Program::OnDeleteObjectInternal(const Object& object)
    {
        uint32_t object_id = this->api->GetObjectId(object);
        this->OnDeleteObject(object, object_id);
    }

    void Program::OnBuffGain(const Obj_AI_Base& unit, const Buff& buff)
    {
        const auto& buffs = this->data->GetDashBuffs();
        std::string name = this->api->GetBuffName(buff);
        if (buffs.find(name) == buffs.end()) return;

        // Ensure the caster is currently dashing
        auto caster = this->api->AsHero(unit);
        if (!this->api->IsDashing(caster)) return;
        if (!this->api->IsEnemy(caster)) return;

        // Exit if a similar spell is already detected
        auto& spell_name = buffs.at(name).PrimaryName;
        if (this->skillshots.Any([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get().Caster;
            const std::string& s_name = skillshot->Get().SkillshotName;
            bool same_caster = this->api->Compare(caster, s_caster);
            return same_caster == true && spell_name == s_name;
        }))
        return;

        std::string proc_name = spell_name;
        Vector start = this->api->GetDashStartPos(caster);
        Vector ending = this->api->GetDashEndPos(caster);
        this->OnProcessSpell(caster, proc_name, 0, start,
            ending, (uint32_t)0, 0.0f, 0.0f, false);
    }

    void Program::OnWndProc(const uint64_t msg, const uint64_t wparam)
    {
        float timer = this->api->GetTime();
        if (msg == 0x100) this->key_pushed = true;

        // Toggle 'Dodge Only Dangerous' mode
        if (this->GetValue<int>("DoDMode") == 1 &&
            this->GetValue<bool>("DoDKey") && this->key_pushed)
        {
            this->dodge_dangerous = !this->dodge_dangerous;
            this->key_pushed = false;
        }

        // Detect any keyboard action that may impact pathing
        if (msg == 0x204 && wparam == 0x002 || msg == 0x102 &&
            wparam == 0x062 || msg == 0x102 && wparam == 0x073)
        {
            this->order_timer = timer;
        }

        if (!this->key_pushed) return;
        bool debug_on = this->GetValue<bool>("DebugOn");
        bool summon_on = this->GetValue<bool>("DebugSummon");
        bool can_summon = timer - this->summon_timer > 0.25f;
        if (!debug_on || !summon_on || !can_summon) return;

        this->key_pushed = false;
        const auto& unit = this->my_hero;
        float height = this->api->GetHeight(unit);
        float hitbox = this->api->GetHitbox(unit);
        auto char_name = this->api->GetCharacterName(unit);
        std::string skill_name = std::string("Debug");

        bool box = this->GetValue<bool>("DebugBox");
        bool infinite = this->GetValue<bool>("DebugInf");
        int level = this->GetValue<int>("D|Debug|Danger");
        int type = this->GetValue<int>("DebugType") + 1;

        float extra = (float)this->GetValue<int>("Buffer");
        float speed = (float)this->GetValue<int>("DebugSpeed");
        float range = (float)this->GetValue<int>("DebugRange");
        float radius = (float)this->GetValue<int>("DebugRadius");
        float delay = (float)this->GetValue<int>("DebugDelay") / 1000.0f;
        float cone_angle = M_RAD((float)this->GetValue<int>("DebugAngle"));

        DamageFunc damage_func = [](const StatData& info) { return 0.0f; };
        auto collisions = std::vector({ CollisionFlag::WIND_WALL });
        DetectionType detect_type = DetectionType::ON_WND_PROC;
        DamageType damage_type = DamageType::TRUE_DAMAGE;
        SkillshotType skill_type = (SkillshotType)type;
        Vector dest = Vector(2700.0f, 1800.0f - range);
        Vector pos = Vector(2700.0f, 1800.0f);

        ActiveData params = ActiveData(dest, pos, pos, skill_name,
            char_name, unit, nullptr, level, 0, false, false, false,
            true, true, false, height, (box ? hitbox : 0.0f) + extra,
            0.0f, range, 0.0f, radius, 0.0f, 0.0f, cone_angle, delay,
            0.0f, infinite == true ? FLT_MAX : speed, collisions,
            detect_type, skill_type, damage_func, damage_type);

        // Create a new simulated skillshot and add it to the table
        auto skillshot = this->process->CreateSkillshot(params, true);
        this->skillshots.Append(skillshot);
        this->summon_timer = timer;
    }

    void Program::OnIssueOrder(const Vector& pos, int order, const std::string& plugin)
    {
        using Intersection = std::pair<Skillshot*, Vector>;
        const Vector& evade_pos = this->evade_pos;
        const Vector& hero_pos = this->hero_pos;

        // Skip override if evasion is turned off
        bool dodge_on = this->GetValue<bool>("Dodge");
        if (!this->can_evade || !dodge_on) return;

        // Block any interference while evading
        if (evade_pos.IsValid())
        {
            if (plugin == "Evade") return;
            return this->api->BlockOrder();
        }

        // Update timer if significant order is detected
        else if (order >= 1 && order <= 3 || order == 10)
        {
            this->order_timer = this->api->GetTime();
        }

        // Exit if its not a move order or theres no skillshots
        if (order != 2 || !this->skillshots.Any()) return;

        // Stop execution if generated path is invalid
        Linq<Vector> path = this->api->GetPath(pos);
        if (path.Count() < 2) return;

        // Retrieve all skillshots that hero might be about to cross
        auto skillshots = this->skillshots.Except(this->dangerous);

        // Find the closest intersection with any skillshot along the path to desired destination
        Intersection result = skillshots.Select<Linq<Intersection>>([&](Skillshot* skillshot)
        {
            return path.Take(path.Count() - 1).Zip(path.Skip(1))

            .Select<Linq<Intersection>>([skillshot](const std::pair<Vector, Vector>& segment)
            {
                return skillshot->PathIntersection({ segment.first, segment.second })

                .Select<Intersection>([skillshot](const Vector& point)
                {
                    return Intersection(skillshot, point);
                });
            })
            .Aggregate<Linq<Intersection>>(Linq<Intersection>(), [](auto total, const auto& points)
            {
                total.AddRange(points); return total;
            });
        })
        .Aggregate<Linq<Intersection>>(Linq<Intersection>(), [](auto total, const auto& points)
        {
            total.AddRange(points); return total;
        })
        .Where([](const Intersection& intersection)
        {
            return intersection.second.IsValid();
        })
        .OrderBy<float>([&path](const Intersection& intersection)
        {
            return path.First().DistanceSquared(intersection.second);
        })
        .FirstOrDefault(Intersection(nullptr, Vector()));

        // Exit if no intersections found
        if (result.first == nullptr) return;

        Vector mouse_pos = this->api->GetCursorPos(), move_pos;
        float latency = this->api->GetLatency() / 2.0f + 0.034f;
        float distance = this->move_speed * (latency + 0.4f);
        Skillshot* skillshot = result.first;
        Vector& intersection = result.second;

        // Check if the intersection is too far from our hero
        float length = hero_pos.DistanceSquared(intersection);
        if (length > distance * distance || length > 250000) return;

        float out_radius = skillshot->Get().OuterRadius;
        const Vector& position = skillshot->Get().Position;
        SkillshotType type = skillshot->Get().SkillshotType;

        // Calculate a safe position to move to based on the skillshot type
        if (type == SkillshotType::CIRCLE || type == SkillshotType::RING &&
            hero_pos.DistanceSquared(position) > out_radius * out_radius)
        {
            Vector direction = (intersection - position).Normalize();
            Vector out_pos = intersection + direction * SAFETY_BUFFER;
            Vector perpendicular = direction.Perpendicular();
            Vector spot_a = out_pos + perpendicular * distance;
            Vector spot_b = out_pos - perpendicular * distance;
            float length_a = mouse_pos.DistanceSquared(spot_a);
            float length_b = mouse_pos.DistanceSquared(spot_b);
            move_pos = length_a < length_b ? spot_a : spot_b;
        }
        else if (type != SkillshotType::NONE && type != SkillshotType::RING)
        {
            const Poly& polygon = skillshot->Get().OffsetPoly;
            auto segment = this->utils->FindSegment(polygon, intersection);
            float length_a = mouse_pos.DistanceSquared(segment.first);
            float length_b = mouse_pos.DistanceSquared(segment.second);
            if (length_a > length_b) segment.first.SwapXY(segment.second);
            Vector direction = (segment.first - segment.second).Normalize();
            Vector out_pos = hero_pos.ClosestOnSegment(segment.first, segment.second);
            move_pos = out_pos.Extend(hero_pos, SAFETY_BUFFER) + direction * distance;
        }
        bool valid = move_pos.IsValid();

        // Adjust move position if current one is unsafe or invalid
        bool unsafe = valid && skillshots.Any([&](auto& skillshot)
        {
            return skillshot->IsDangerous(move_pos);
        });
        if (unsafe || !valid || !this->api->IsReachable(move_pos))
        {
            move_pos = intersection.Extend(hero_pos, SAFETY_BUFFER);
        }

        // Override the order to move to safe position
        float height = this->api->GetHeight(this->my_hero);
        this->api->SetOverrideOrder(move_pos, height);
    }

    void Program::OnIssueOrderInternal(const Vector3& pos, int order, const char* plugin)
    {
        std::string plugin_name = plugin;
        this->OnIssueOrder(this->api->ToVector(pos), order, plugin_name);
    }
}
