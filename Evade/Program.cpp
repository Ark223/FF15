
#include "Program.h"

namespace Evade
{
    Program* Program::m_instance = nullptr;

    Program::Program(PublicAPI* api) : m_api(api)
    {
        this->m_instance = this;
        this->RegisterAPI();
        this->RegisterData();
        this->RegisterMyHero();
        this->RegisterMenu();
        this->RegisterEvents();
    }

    Program::~Program()
    {
        API::Destroy();
        Data::Destroy();
        Config::Destroy("Evade");
        delete this->my_hero;
    }

    void Program::RegisterAPI()
    {
        this->api = API::Get(this->m_api);
    }

    void Program::RegisterData()
    {
        this->data = Data::Get();
    }

    void Program::RegisterEvents()
    {
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

    void Program::RegisterMyHero()
    {
        this->my_hero = this->api->GetMyHero();
    }

    Program* Program::Get()
    {
        return m_instance;
    }

    // Event Wrappers

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
        Program::Get()->OnWndProc(wnd, msg, wparam, lparam);
    }

    void Program::OnIssueOrderWrapper(VecInt3 pos, int order, int target_id)
    {
        Program::Get()->OnIssueOrder(pos, order, target_id);
    }

    // Menu constructor

    void Program::BuildMenu()
    {
        auto menu = this->config->CreateMenu("Evade", "Evade");

        // Core settings
        auto core = this->config->AddSubMenu(menu, "Core", "Core Settings");
        this->settings["Fast"] = this->config->AddCheckbox(core, "Fast", "Fast Evade Mode", false);
        this->settings["Buffer"] = this->config->AddSlider(core, "Buffer", "Collision Time Buffer", 0, 0, 100, 5);
        this->settings["Points"] = this->config->AddSlider(core, "Points", "Maximum Evade Points", 35, 8, 50, 1);
        this->settings["Reaction"] = this->config->AddSlider(core, "Reaction", "Minimum Reaction Time", 0, 0, 250, 5);

        // Main settings
        auto main = this->config->AddSubMenu(menu, "Main", "Main Settings");

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

        bool sylas = enemies.Any([](const std::string& s) { return s == "Sylas"; });
        bool viego = enemies.Any([](const std::string& s) { return s == "Viego"; });
        auto units = ((sylas || viego) ? allies.Concat(enemies) : enemies).Distinct();
        units = units.OrderBy<std::string>([](const auto& s) { return s; });

        // Dodgeable Spells
        this->config->AddLabel(main, "DodgeableSpells", "Dodgeable Spells", true);

        for (const auto& unit : units)
        {

        }
    }

    // Events

    void Program::OnTick()
    {
        // TODO
    }

    void Program::OnDraw()
    {
        // TODO
    }

    void Program::OnProcessSpell(GameObject* unit, const std::string& name,
        int level, const Vector& start_pos, const Vector& end_pos,
        uint32_t target_id, float cast_delay, float cast_time)
    {
        // TODO
    }

    void Program::OnProcessSpellInternal(GameObject* unit, SpellCastInfo* info)
    {
        auto name = this->api->GetSpellCastName(info);
        int level = this->api->GetSpellCastLevel(info);
        Vector start_pos = this->api->GetSpellCastStart(info);
        Vector end_pos = this->api->GetSpellCastDestination(info);
        uint32_t target_id = this->api->GetSpellCastOwnerId(info);
        float cast_delay = this->api->GetSpellCastWindupTime(info);
        float cast_time = this->api->GetSpellCastStartTime(info);
        this->OnProcessSpell(unit, name, level, start_pos,
            end_pos, target_id, cast_delay, cast_time);
    }

    void Program::OnCreateObject(GameObject* unit, uint32_t id)
    {
        // TODO
    }

    void Program::OnBuffGain(BuffInstance* buff)
    {
        // TODO
    }

    void Program::OnWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        // TODO
    }

    void Program::OnIssueOrder(VecInt3 pos, int order, int target_id)
    {
        // TODO
    }
}
