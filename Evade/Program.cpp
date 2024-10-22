
#include "Program.h"
#include "Process.h"
#include "Utilities.h"

namespace Evade
{
    Program* Program::m_instance = nullptr;

    Program::Program(PublicAPI* api) : m_api(api)
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
        this->m_api->set_onGameUpdate(OnTickWrapper);
        this->m_api->set_onDraw(OnDrawWrapper);
        this->m_api->set_onProcessSpell(OnProcessSpellWrapper);
        this->m_api->set_onGameObjCreate(OnCreateObjectWrapper);
        this->m_api->set_onBuffAdd(OnBuffGainWrapper);
        this->m_api->set_onWndProc(OnWndProcWrapper);
        this->m_api->set_onIssueOrder(OnIssueOrderWrapper);
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

    void Program::OnProcessSpellWrapper(GameObject* unit, SpellCastInfo* info)
    {
        Program::Get()->OnProcessSpellInternal(unit, info);
    }

    void Program::OnCreateObjectWrapper(GameObject* unit, uint32_t id)
    {
        Program::Get()->OnCreateObject(unit, id);
    }

    void Program::OnBuffGainWrapper(BuffInstance* buff)
    {
        Program::Get()->OnBuffGain(buff);
    }

    void Program::OnWndProcWrapper(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        Program::Get()->OnWndProc(msg, wparam);
    }

    void Program::OnIssueOrderWrapper(VecInt3 pos, int order, int target_id)
    {
        Program::Get()->OnIssueOrder(pos, order);
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
        this->objects["Feather"] = {};
        this->objects["Seed"] = {};
        this->objects["Shadow"] = {};

        // Initialize slot converter
        this->char_to_slot = [](char c) -> int
        {
            return (int)std::string_view("QWER").find(c);
        };
    }

    // Menu constructor

    void Program::BuildMenu()
    {
        // Check if menu library is loaded
        this->libs_loaded = this->config->IsLoaded();
        if (this->libs_loaded == false) return;

        auto menu = this->config->CreateMenu("Evade", "Evade");
        std::string char_name = this->api->GetCharacterName(this->my_hero);

        // Core settings
        auto core = this->config->AddSubMenu(menu, "Core", "<< Core >>");
        this->settings["Buffer"] = this->config->AddSlider(core, "Buffer", "Area Hitbox Buffer", 0, 0, 20, 5);
        this->settings["Delta"] = this->config->AddSlider(core, "Delta", "Collision Time Delta", 0, 0, 100, 5);
        this->settings["Interval"] = this->config->AddSlider(core, "Interval", "Force Click Interval", 40, 25, 50, 1);
        this->settings["Points"] = this->config->AddSlider(core, "Points", "Maximum Evade Points", 35, 8, 50, 1);
        this->settings["Reaction"] = this->config->AddSlider(core, "Reaction", "Minimum Reaction Time", 0, 0, 250, 5);
        this->settings["Retries"] = this->config->AddSlider(core, "Retries", "Solution Finder Retries", 1, 0, 3, 1);
        this->settings["Search"] = this->config->AddSlider(core, "Search", "Threat Search Range", 1000, 0, 1500, 100);

        // Main settings
        auto main = this->config->AddSubMenu(menu, "Main", "<< Main >>");

        // Standard options
        this->config->AddLabel(main, "Standard", "Standard Options", true);
        this->settings["Dodge"] = this->config->AddCheckbox(main, "Dodge", "Dodge Skillshots", true);
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

        auto ally_units = this->api->GetAllyHeroes(FLT_INF, Vector(), false);
        auto enemy_units = this->api->GetEnemyHeroes(FLT_INF, Vector(), false);
        auto allies = ally_units.Select<std::string>([&](const auto& u) { return N(u); });
        auto enemies = enemy_units.Select<std::string>([&](const auto& u) { return N(u); });

        bool sylas = enemies.Any([](const auto& n) { return n == "Sylas"; });
        bool viego = enemies.Any([](const auto& n) { return n == "Viego"; });
        auto units = ((sylas || viego) ? allies.Concat(enemies) : enemies);
        units = units.OrderBy<std::string>([](const auto& n) { return n; });

        // Spell settings
        auto spells = this->config->AddSubMenu(menu, "Spells", "<< Spells >>");

        // Supported skillshots
        this->config->AddLabel(spells, "Skillshots", "Supported Skillshots", true);

        const auto& skillshots = this->data->GetSkillshots();
        for (const auto& unit : units.Distinct().Concat({ "Global" }))
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

        // Block settings
        auto blocks = this->config->AddSubMenu(menu, "Block", "<< Blocks >>");
        this->settings["BlockQ"] = this->config->AddCheckbox(blocks, "BlockQ", "Block Q On Evade", true);
        this->settings["BlockW"] = this->config->AddCheckbox(blocks, "BlockW", "Block W On Evade", true);
        this->settings["BlockE"] = this->config->AddCheckbox(blocks, "BlockE", "Block E On Evade", true);
        this->settings["BlockR"] = this->config->AddCheckbox(blocks, "BlockR", "Block R On Evade", true);

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

    void Program::RecalculatePath(float delay)
    {
        this->ResetState(delay);
    }

    void Program::RequestUpdateOnce(int id, float delay)
    {
        if (id <= this->update_id) return;
        this->ResetState(delay);
        this->update_id = id;
    }

    void Program::ResetSkillshots()
    {
        this->considered.ForEach([](Skillshot* skillshot)
        {
            skillshot->Set("Undodgeable", false);
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
        this->evade_pos = Vector();
        this->safe_order_count = 0;
        this->undodgeable = false;
        this->ResetSkillshots();
    }

    // Events

    void Program::OnTick()
    {
        // Remove invalid or dead objects from collections
        for (auto& [key, collection] : this->objects)
        {
            collection.RemoveAll([&](auto object)
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

        // Turn off evasion if user has disabled it
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

        // Create a copy of the skillshots considered for processing
        auto skillshots = Linq<Skillshot*>(this->considered.ToArray());

        // Turn off evasion if its no longer required
        if (skillshots.All([&](Skillshot* skillshot)
        {
            bool undodgeable = skillshot->Get<bool>("Undodgeable");
            return undodgeable || skillshot->IsSafe(this->hero_pos);
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
            const auto& name = skillshot->Get<std::string>("SkillshotName");
            bool s_override = this->GetValue<bool>("D|" + name + "|Override");
            int s_reaction = this->GetValue<int>("D|" + name + "|Reaction");
            return (float)(s_override ? s_reaction : reaction) / 1000.0f;
        });

        // Calculate hash value for the set of skillshots
        uint32_t hash = skillshots.Aggregate<uint32_t>(0,
            [](uint32_t hash_value, Skillshot* skillshot)
        {
            int unique_id = skillshot->Get<int>("UniqueId");
            return (hash_value * 31 + unique_id) % 0xFFFFFFFF;
        });

        // Reset evasion if hash code has recently changed
        // Change indicates an addition/removal of a skillshot
        if (this->hash_code != hash) this->ResetState(delay);

        this->hash_code = hash;
        float time_diff = this->recalc_timer - timer;
        if (!this->recalc_path || time_diff > 0) return;

        // Check if any of the skillshots has a dynamic displacement
        bool dynamic = skillshots.Any([&](Skillshot* skillshot)
        {
            const Vector& start = skillshot->Get<Vector>("StartPos");
            const Vector& origin = skillshot->Get<Vector>("OriginPos");
            const auto& name = skillshot->Get<std::string>("SkillshotName");
            const auto& data = this->data->GetSkillshots().at(name);
            if (data.BackToCaster || data.FollowCaster) return true;
            return this->update_id >= skillshot->Get<int>("UniqueId");
        });

        // Sort skillshots by specific names first, then by danger level and damage
        // Lowest priority is assigned to skillshots that include multiple parts
        auto splitted = { "LeonaSolarFlare", "SettW", "SyndraE", "VeigarCage" };
        skillshots = skillshots.OrderBy<Features>([&](Skillshot* skillshot)
        {
            const auto& name = skillshot->Get<std::string>("SkillshotName");
            auto special = std::find(splitted.begin(), splitted.end(), name);
            int priority = special != splitted.end() ? 1 : 0;
            int danger = skillshot->Get<int>("DangerLevel");
            float damage = skillshot->Get<float>("Damage");
            return std::make_tuple(priority, danger, damage);
        });

        // Update item slots and prepare a new evading solution
        this->utils->UpdateItemSlots(this->evading_spells);
        this->SwitchOff(true); this->recalc_path = false;

        // Attempt to find a safe solution
        int retries = this->GetValue<int>("Retries");
        while (skillshots.Count() > 0 && retries-- >= 0)
        {
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
            skillshots.First()->Set("Undodgeable", true);
            skillshots.RemoveAt(0);
        }

        // No solution found
        this->undodgeable = true;
    }

    void Program::OnDraw()
    {
        this->safe_order_count++;
        this->utils->EvadeToSpot(this->evade_pos);

        // Update and get all skillshots considered for evasion
        this->considered = this->process->UpdateSkillshots();

        // Retrieve all skillshots that our hero is currently inside of
        this->dangerous = this->skillshots.Where([&](Skillshot* skillshot)
        {
            return skillshot->IsDangerous(this->hero_pos);
        });

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
            auto text_pos = VecInt2(pos.x, pos.y + 50.0f);
            this->api->DrawCenteredText(text_pos, text, color);
        }

        if (!this->GetValue<bool>("Draw")) return;
        this->skillshots.ForEach([&](Skillshot* skillshot)
        {
            int alpha, red, green, blue;
            const auto& name = skillshot->Get<std::string>("SkillshotName");
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
            else if (skillshot->Get<bool>("Undodgeable"))
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
            uint32_t colors[2] = { c1, c2 }; skillshot->Draw(colors);
        });
    }

    void Program::OnProcessSpell(GameObject* unit, std::string& spell_name,
        const int spell_level, const Vector& start_pos, const Vector& end_pos,
        uint32_t target_id, float cast_delay, float extra_delay, bool proc_next)
    {
        if (!this->api->IsValid(unit)) return;
        size_t npos = std::string::npos;

        // Process any actions if they were done by our hero
        if (this->api->Compare(unit, this->my_hero))
        {
            if (spell_name.find("Attack") != npos)
            {
                float timer = this->api->GetTime();
                this->attack_timer = timer; return;
            }
        }

        // Continue processing if the unit is a hero
        if (!this->api->IsHero(unit)) return;

        // Skip processing if the hero is an ally
        auto caster = this->api->AsHero(unit);
        if (!this->api->IsEnemy(caster)) return;

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
            this->skillshots.RemoveAll([&](Skillshot* skillshot)
            {
                const auto& s_caster = skillshot->Get<Obj_AI_Hero*>("Caster");
                const auto& s_name = skillshot->Get<std::string>("SkillshotName");
                bool anivia = s_name == "AniviaQCenter" || s_name == "FlashFrost";
                return anivia && this->api->Compare(caster, s_caster);
            });
            if (this->skillshots.Count() != count) return;
        }

        // Ignore duplicates and similar skillshots from the same caster
        if (data.IgnoreAlive && this->skillshots.Any([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get<Obj_AI_Hero*>("Caster");
            const auto& s_name = skillshot->Get<std::string>("SkillshotName");
            return this->api->Compare(caster, s_caster) && spell_name == s_name;
        }))
        return;

        bool visible = this->api->IsVisible(caster);
        Vector unit_pos = this->api->GetPosition(caster);
        int slot = this->char_to_slot(data.SkillshotSlot);
        int danger = this->GetValue<int>("D|" + spell_name + "|Danger");
        std::string caster_name = this->api->GetCharacterName(caster);
        DetectionType detect_type = DetectionType::ON_PROCESS_SPELL;

        float cone_angle = data.ConeAngle * M_PI / 180.0f;
        float delay = data.UseWindup ? cast_delay : data.Delay;
        float height = this->api->GetHeight(end_pos), hitbox = 0.0f;
        if (ABS(height) > 100) height = this->api->GetHeight(caster);
        if (data.AddHitbox) hitbox = this->api->GetHitbox(this->my_hero);

        Base instance =
        {
            {"SoftCC", data.SoftCC}, {"HardCC", data.HardCC},
            {"FogOfWar", !visible}, {"DangerLevel", danger},
            {"FixedRange", data.FixedRange}, {"Invert", data.Invert},
            {"Rotate90", data.Rotate90}, {"Offset", data.Offset},
            {"Range", data.Range}, {"SideRange", data.SideRange},
            {"Radius", data.Radius}, {"InnerRadius", data.InnerRadius},
            {"OuterRadius", data.OuterRadius}, {"ConeAngle", cone_angle},
            {"ExtraDuration", data.ExtraTime}, {"Delay", delay + extra_delay},
            {"Speed", data.Speed}, {"Hitbox", hitbox}, {"DestPos", end_pos},
            {"OriginPos", unit_pos.Clone()}, {"StartPos", start_pos},
            {"Collisions", data.Collisions}, {"DamageFunc", data.Damage},
            {"DamageType", data.DamageType}, {"DetectionType", detect_type},
            {"SkillshotType", data.SkillshotType}, {"SkillshotSlot", slot},
            {"SkillshotName", spell_name}, {"CasterName", caster_name},
            {"Caster", caster}, {"Height", height}
        };

        // Create a new skillshot instance based on the collected data
        auto skillshot = this->process->CreateSkillshot(instance, false);

        // Skip skillshot which is way too far from our hero
        if (data.Range < 25000 && !data.IsTrap)
        {
            float range = skillshot->Get<float>("Range");
            Vector ending = skillshot->Get<Vector>("EndPos");
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
        if (data.SkipAncestor) this->skillshots.Remove(skillshot);
    }

    void Program::OnProcessSpellInternal(GameObject* unit, SpellCastInfo* info)
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

    void Program::OnCreateObject(GameObject* object, uint32_t id)
    {
        if (!this->api->IsValid(object)) return;

        MissileClient* missile = nullptr;
        GameObject* unit = nullptr;
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
                return std::regex_match(spell_name, pattern);
            });

            // Exit if spell name was not found
            if (info.SpellName == "") return;
            spell_name = info.SpellName;

            // Get the valid owner of the missile
            unit = this->api->GetMissileOwner(missile);
            if (!this->api->IsValid(unit)) return;
        }
        else if (this->api->IsParticle(object))
        {
            // Retrieve spell name based on particle name
            spell_name = this->api->GetObjectName(object);
            auto info = this->particles.FirstOrDefault([&](auto& info)
            {
                size_t result = spell_name.find(info.CharName);
                if (result == std::string::npos) return false;
                std::regex pattern(info.ParticleName);
                return std::regex_match(spell_name, pattern);
            });

            // Exit if spell name was not found
            if (info.SpellName == "") return;
            spell_name = info.SpellName;

            // Get the valid owner of the particle
            unit = this->api->GetObjectOwner(object);
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

        const auto& data = this->data->GetSkillshots().at(spell_name);
        bool check_dups = data.IgnoreAlive || !data.Exception;

        // Ignore duplicates and similar skillshots from the same caster
        if (check_dups && this->skillshots.Any([&](Skillshot* skillshot)
        {
            DetectionType proc_type = DetectionType::ON_PROCESS_SPELL;
            const auto& s_caster = skillshot->Get<Obj_AI_Hero*>("Caster");
            const auto& s_name = skillshot->Get<std::string>("SkillshotName");
            const auto& s_type = skillshot->Get<DetectionType>("DetectionType");
            bool exists = this->api->Compare(caster, s_caster) && spell_name == s_name;
            return exists && (data.IgnoreAlive || !data.Exception && s_type == proc_type);
        }))
        return;

        // Remove all similar triggered skillshots that should be overridden by new one
        if (data.Overrider != "") this->skillshots.RemoveAll([&](Skillshot* skillshot)
        {
            DetectionType proc_type = DetectionType::ON_PROCESS_SPELL;
            const auto& s_caster = skillshot->Get<Obj_AI_Hero*>("Caster");
            const auto& s_name = skillshot->Get<std::string>("SkillshotName");
            const auto& s_type = skillshot->Get<DetectionType>("DetectionType");
            bool can_remove = data.Overrider == s_name && s_type == proc_type;
            return can_remove && this->api->Compare(caster, s_caster);
        });

        bool visible = this->api->IsVisible(caster);
        bool multi = this->data->GetConnectors().count(spell_name) > 0;
        bool fixed_range = data.Range >= 25000.0f || data.FixedRange &&
            (!(multi || data.Acceleration != 0.0f || spell_name == "ZoeE"));

        int slot = this->char_to_slot(data.SkillshotSlot);
        int danger = this->GetValue<int>("D|" + spell_name + "|Danger");
        std::string caster_name = this->api->GetCharacterName(caster);
        DetectionType detect_type = DetectionType::ON_OBJECT_CREATED;

        Vector end_pos = this->api->GetPosition(object);
        Vector start_pos = this->api->GetPosition(caster), dir;
        if (missile) end_pos = this->api->GetMissileEndPos(missile);
        if (missile) start_pos = this->api->GetMissileStartPos(missile);
        if (data.UseEmitter) dir = this->api->GetObjectDirection(object);
        if (dir.IsValid()) end_pos = start_pos + dir;

        float cone_angle = data.ConeAngle * M_PI / 180.0f;
        float delay = missile ? data.MissileDelay : data.ParticleDelay;
        float height = this->api->GetHeight(end_pos), hitbox = 0.0f;
        if (ABS(height) > 100) height = this->api->GetHeight(caster);
        if (data.AddHitbox) hitbox = this->api->GetHitbox(this->my_hero);

        Base instance =
        {
            {"SoftCC", data.SoftCC}, {"HardCC", data.HardCC},
            {"FogOfWar", !visible}, {"DangerLevel", danger},
            {"FixedRange", fixed_range}, {"Invert", data.Invert},
            {"Rotate90", data.Rotate90}, {"Offset", data.Offset},
            {"Range", data.Range}, {"SideRange", data.SideRange},
            {"Radius", data.Radius}, {"InnerRadius", data.InnerRadius},
            {"OuterRadius", data.OuterRadius}, {"ConeAngle", cone_angle},
            {"ExtraDuration", data.ExtraTime}, {"Delay", MAX(0.0f, delay)},
            {"Speed", data.Speed}, {"Hitbox", hitbox}, {"DestPos", end_pos},
            {"OriginPos", start_pos.Clone()}, {"StartPos", start_pos},
            {"Collisions", data.Collisions}, {"DamageFunc", data.Damage},
            {"DamageType", data.DamageType}, {"DetectionType", detect_type},
            {"SkillshotType", data.SkillshotType}, {"SkillshotSlot", slot},
            {"SkillshotName", spell_name}, {"CasterName", caster_name},
            {"Caster", caster}, {"Height", height}
        };

        // Create a new skillshot instance based on the collected data
        auto skillshot = this->process->CreateSkillshot(instance, false);

        // Update and register skillshot for processing
        this->process->UpdateData(skillshot, 0);
        this->skillshots.Append(skillshot);
    }

    void Program::OnBuffGain(BuffInstance* buff)
    {
        const auto& dashes = this->data->GetCustomDashes();
        std::string name = this->api->GetBuffName(buff);
        if (dashes.find(name) == dashes.end()) return;

        // Retrieve the buff owner from buff instance
        auto owner = this->api->GetBuffOwner(buff);
        if (!this->api->IsValid(owner)) return;

        // Ensure the caster is currently dashing
        auto caster = this->api->AsHero(owner);
        if (!this->api->IsDashing(caster)) return;
        if (!this->api->IsEnemy(caster)) return;

        // Exit if a similar spell is already detected
        auto& spell_name = dashes.at(name).PrimaryName;
        if (this->skillshots.Any([&](Skillshot* skillshot)
        {
            const auto& s_caster = skillshot->Get<Obj_AI_Hero*>("Caster");
            const auto& s_name = skillshot->Get<std::string>("SkillshotName");
            return this->api->Compare(caster, s_caster) && spell_name == s_name;
        }))
        return;

        // A valid dash should take exactly two waypoints
        auto waypoints = this->api->GetWaypoints(caster);
        if (waypoints.Count() < 2) return;

        std::string proc_name = spell_name;
        const Vector& end_pos = waypoints.Last();
        const Vector& start_pos = waypoints.First();
        this->OnProcessSpell(caster, proc_name, 0,
            start_pos, end_pos, 0, 0.0f, 0.0f, false);
    }

    void Program::OnWndProc(UINT msg, WPARAM wparam)
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

        // Remove skillshots using a double-click
        if (msg == 0x202 && wparam == 0x000)
        {
            if (timer - this->click_timer < 0.25f)
            {
                const Vector& mouse_pos = this->api->GetCursorPos();
                this->skillshots.RemoveAll([&](Skillshot* skillshot)
                {
                    return skillshot->IsDangerous(mouse_pos);
                });
            }
            this->click_timer = timer;
        }

        if (!this->key_pushed) return;
        bool debug_on = this->GetValue<bool>("DebugOn");
        bool summon_on = this->GetValue<bool>("DebugSummon");
        bool can_summon = timer - this->summon_timer > 0.25f;
        if (!debug_on || !summon_on || !can_summon) return;

        this->key_pushed = false;
        auto unit = this->my_hero;
        float height = this->api->GetHeight(unit);
        float hitbox = this->api->GetHitbox(unit);
        auto name = this->api->GetCharacterName(unit);

        bool box = this->GetValue<bool>("DebugBox");
        bool infinite = this->GetValue<bool>("DebugInf");
        int level = this->GetValue<int>("D|Debug|Danger");
        int type = this->GetValue<int>("DebugType") + 1;

        float extra = (float)this->GetValue<int>("Buffer");
        float speed = (float)this->GetValue<int>("DebugSpeed");
        float range = (float)this->GetValue<int>("DebugRange");
        float radius = (float)this->GetValue<int>("DebugRadius");
        float delay = (float)this->GetValue<int>("DebugDelay") / 1000.0f;
        float angle = (float)this->GetValue<int>("DebugAngle") * M_PI / 180.0f;

        DamageFunc damage_func = [](const StatData& info) { return 0.0f; };
        auto collision_table = std::vector({ CollisionFlag::WIND_WALL });
        DetectionType detect_type = DetectionType::ON_WND_PROC;
        SkillshotType skillshot_type = (SkillshotType)type;
        DamageType damage_type = DamageType::TRUE_DAMAGE;
        Vector dest = Vector(2700.0f, 1800.0f - range);
        Vector pos = Vector(2700.0f, 1800.0f);

        Base data =
        {
            {"SoftCC", true}, {"HardCC", true}, {"DangerLevel", level},
            {"FixedRange", false}, {"Range", range}, {"Caster", unit},
            {"Height", height}, {"Radius", radius}, {"Delay", delay},
            {"ConeAngle", angle}, {"Speed", infinite ? FLT_INF : speed},
            {"Hitbox", (box ? hitbox : 0.0f) + extra}, {"DestPos", dest},
            {"OriginPos", pos}, {"StartPos", pos}, {"CasterName", name},
            {"Collisions", collision_table}, {"DamageFunc", damage_func},
            {"DamageType", damage_type}, {"DetectionType", detect_type},
            {"SkillshotType", skillshot_type}, {"SkillshotSlot", 0},
            {"SkillshotName", std::string("Debug")}
        };

        // Create a new simulated skillshot and add it to the table
        auto skillshot = this->process->CreateSkillshot(data, true);
        this->skillshots.Append(skillshot);
        this->summon_timer = timer;
    }

    void Program::OnIssueOrder(VecInt3 pos, int order)
    {
        using Intersection = std::pair<Skillshot*, Vector>;
        Vector order_pos = this->api->ToVector(pos);
        const Vector& evade_pos = this->evade_pos;
        const Vector& hero_pos = this->hero_pos;

        // Skip override if evasion is turned off
        bool dodge_on = this->GetValue<bool>("Dodge");
        if (!this->can_evade || !dodge_on) return;

        // Update timer if significant order is detected
        if (order == 1 || order == 3 || order == 10)
        {
            this->order_timer = this->api->GetTime();
        }

        // Block any interference while evading
        if (evade_pos.IsValid())
        {
            float distance = order_pos.DistanceSquared(evade_pos);
            bool should_block = distance > 400 && order == 2
                || (order == 1 || order == 3 || order == 10);
            if (should_block) this->api->BlockOrder(); return;
        }

        // Exit if its not a move order or theres no skillshots
        if (order != 2 || !this->skillshots.Any()) return;

        // Stop execution if the generated path is invalid
        Linq<Vector> path = this->api->GetPath(order_pos);
        if (path.Count() < 2) return;

        // Retrieve all skillshots that hero might be about to cross
        auto skillshots = this->skillshots.Except(this->dangerous);

        // Find the closest intersection with any skillshot along the path to desired destination
        Intersection result = skillshots.Select<Linq<Intersection>>([&](Skillshot* skillshot)
        {
            return path.Take(path.Count() - 1).Zip(path.Skip(1))

            .Select<Linq<Intersection>>([skillshot](const std::pair<Vector, Vector>& segment)
            {
                return skillshot->PathIntersection(segment.first, segment.second)

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

        float out_radius = skillshot->Get<float>("OuterRadius");
        const Vector& position = skillshot->Get<Vector>("Position");
        const auto& type = skillshot->Get<SkillshotType>("SkillshotType");

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
            const Poly& polygon = skillshot->Get<Poly>("OffsetPoly");
            auto segment = this->utils->FindSegment(polygon, intersection);
            float length_a = mouse_pos.DistanceSquared(segment.first);
            float length_b = mouse_pos.DistanceSquared(segment.second);
            if (length_a > length_b) segment.first.Swap(segment.second);
            Vector direction = (segment.first - segment.second).Normalize();
            Vector out_pos = hero_pos.ClosestOnSegment(segment.first, segment.second);
            move_pos = out_pos.Extend(hero_pos, SAFETY_BUFFER) + direction * distance;
        }
        bool valid = move_pos.IsValid();

        // Adjust move position if current one is unsafe or invalid
        bool unsafe = valid && skillshots.Any([&](auto skillshot)
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
}
