
#include "Data.h"

namespace Evade
{
    Data* Data::m_instance = nullptr;

    Data::Data()
    {
        this->InitSkillshots();
        this->InitEvadingSpells();
        this->InitShieldSpells();
        this->InitAntiRecalcs();
        this->InitDashingSpells();
        this->InitSpellQueue();
        this->InitStoppers();
    }

    void Data::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Data* Data::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Data();
        }
        return m_instance;
    }

    // Data initialization

    void Data::InitSkillshots()
    {
        this->skillshots = SkillshotTable();

        #pragma region Global

        SkillshotData data;
        data.ChampionName = "Global";
        data.SkillshotName = "Debug";
        data.DisplayName = "Debug Skillshot";
        data.IconName = "Dummy.png";
        data.Dangerous = true;
        data.SoftCC = true;
        data.HardCC = true;
        data.Collisions = { };
        data.Damage = [](const auto& info) { return 0.0f; };
        data.SkillshotType = SkillshotType::NONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion
    }

    void Data::InitEvadingSpells()
    {
        this->evaders = Linq<EvadingSpellData>();
    }

    void Data::InitShieldSpells()
    {
        this->shields = Linq<ShieldSpellData>(
        {
            {"Annie", "Molten Shield", "AnnieE", EvadingType::DAMAGE_ABSORPTION, 'E', FLT_INF, 800},
            {"Janna", "Eye of the Storm", "EyeOfTheStorm", EvadingType::DAMAGE_ABSORPTION, 'E', FLT_INF, 800},
            {"Karma", "Inspire", "KarmaSolKimShield", EvadingType::DAMAGE_ABSORPTION, 'E', FLT_INF, 800},
            {"Lulu", "Help, Pix!", "LuluE", EvadingType::DAMAGE_ABSORPTION, 'E', FLT_INF, 650},
            {"Lux", "Prismatic Barrier", "LuxPrismaticWave", EvadingType::DAMAGE_ABSORPTION, 'W', 2200, 1175},
            {"Morgana", "Black Shield", "MorganaE", EvadingType::CC_IMMUNITY, 'E', FLT_INF, 800},
            {"Orianna", "Command: Protect", "OrianaRedactCommand", EvadingType::DAMAGE_ABSORPTION, 'E', 1200, 1100},
            {"Renata", "Loyalty Program", "RenataE", EvadingType::DAMAGE_ABSORPTION, 'E', 1450, 800},
            {"Seraphine", "Surround Sound", "SeraphineW", EvadingType::DAMAGE_ABSORPTION, 'W', FLT_INF, 800},
            {"Sona", "Aria of Perseverance", "SonaW", EvadingType::DAMAGE_ABSORPTION, 'W', FLT_INF, 1000},
            {"Taric", "Bastion", "TaricW", EvadingType::DAMAGE_ABSORPTION, 'W', FLT_INF, 800}
        });
    }

    void Data::InitDashingSpells()
    {
        this->dashes =
        {
            {"Hecarim", {"HecarimRDash", "hecarimultsound"}},
            {"Vi", {"ViQDash", "ViQDash"}},
            {"Zac", {"ZacEDash", "zacemove"}}
        };
    }

    void Data::InitAntiRecalcs()
    {
        this->anti_recalcs =
        {
            {"AniviaQCenter", {"FlashFrost"}},
            {"EkkoQ2", {"EkkoQ"}},
            {"GravesQDetonation", {"GravesQLineMis"}},
            {"GravesChargeShot", {"GravesRExplosion"}},
            {"GravesRExplosion", {"GravesChargeShot"}},
            {"HweiQQ", {"HweiQQExplosion"}},
            {"KarmaQMissileMantra", {"KarmaQExplosion"}},
            {"LeonaSolarFlare", {"LeonaREpicenter"}},
            {"LeonaREpicenter", {"LeonaSolarFlare"}},
            {"LilliaERollingMissile", {"LilliaE"}},
            {"MalzaharQSecond", {"MalzaharQ"}},
            {"MordekaiserQ2", {"MordekaiserQ"}},
            {"OrianaIzunaCenter", {"OrianaIzuna"}},
            {"RenataEExplosion", {"RenataE"}},
            {"RumbleRLand", {"RumbleRMissile"}},
            {"SettWInner", {"SettW"}},
            {"SwainEDetonation", {"SwainE"}},
            {"SyndraESphereMissile", {"SyndraE"}},
            {"VexQAccelerated", {"VexQ"}},
            {"ZoeEc", {"ZoeE"}}
        };
    }

    void Data::InitStoppers()
    {
        this->stoppers =
        {
            {"Caitlyn", {"CaitlynR"}},
            {"Fiora", {"FioraW"}},
            {"Galio", {"GalioR"}},
            {"Janna", {"ReapTheWhirlwind"}},
            {"Karthus", {"karthusfallenonecastsound"}},
            {"Katarina", {"katarinarsound"}},
            {"Kayle", {"KayleR"}},
            {"KogMaw", {"KogMawIcathianSurprise"}},
            {"Lissandra", {"lissandrarself"}},
            {"Malzahar", {"MalzaharRSound"}},
            {"MasterYi", {"Meditate"}},
            {"MissFortune", {"missfortunebulletsound"}},
            {"Olaf", {"OlafRagnarok"}},
            {"Pantheon", {"PantheonR"}},
            {"Shen", {"shenstandunitedlock"}},
            {"Sion", {"sionpassivezombie"}},
            {"Tryndamere", {"UndyingRage"}},
            {"TwistedFate", {"Gate"}},
            {"Velkoz", {"VelkozR"}},
            {"Xerath", {"XerathLocusOfPower2"}},
            {"Yuumi", {"YuumiWAttach"}},
            {"Zac", {"ZacE"}}
        };
    }

    void Data::InitSpellQueue()
    {
        this->queue =
        {
            {"EkkoQ", {"EkkoQ2", ConnectionType::EXTEND_LENGTH}},
            {"FlashFrost", {"AniviaQCenter", ConnectionType::EXTEND_LENGTH}},
            {"GravesQLineMis", {"GravesQDetonation", ConnectionType::EXTEND_LENGTH}},
            {"GravesChargeShot", {"GravesRExplosion", ConnectionType::EXTEND_LENGTH}},
            {"HweiQQ", {"HweiQQExplosion", ConnectionType::EXTEND_LENGTH}},
            {"KarmaQMissileMantra", {"KarmaQExplosion", ConnectionType::EXTEND_LENGTH}},
            {"LeonaSolarFlare", {"LeonaREpicenter", ConnectionType::FOLLOW_ORIGIN}},
            {"MalzaharQ", {"MalzaharQSecond", ConnectionType::FOLLOW_ORIGIN}},
            {"MordekaiserQ", {"MordekaiserQ2", ConnectionType::FOLLOW_ORIGIN}},
            {"NaafiriE", {"NaafiriEFlurry", ConnectionType::FOLLOW_ORIGIN}},
            {"OrianaIzuna", {"OrianaIzunaCenter", ConnectionType::FOLLOW_ORIGIN}},
            {"OrnnQ", {"OrnnQPillar", ConnectionType::EXTEND_LENGTH}},
            {"QiyanaQ_Grass", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"QiyanaQ_Water", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"QiyanaQ_Rock", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"RellW_Dismount", {"RellW2", ConnectionType::FOLLOW_ORIGIN}},
            {"RenataE", {"RenataEExplosion", ConnectionType::EXTEND_LENGTH}},
            {"RumbleRMissile", {"RumbleRLand", ConnectionType::FOLLOW_ORIGIN}},
            {"SettW", {"SettWInner", ConnectionType::FOLLOW_ORIGIN}},
            {"SmolderW", {"SmolderW2", ConnectionType::EXTEND_LENGTH}},
            {"SwainE", {"SwainEDetonation", ConnectionType::EXTEND_LENGTH}},
            {"VelkozW", {"VelkozWEruption", ConnectionType::FOLLOW_ORIGIN}},
            {"VeigarEventHorizon", {"VeigarCage", ConnectionType::FOLLOW_ORIGIN}},
            {"VexQ", {"VexQAccelerated", ConnectionType::EXTEND_LENGTH}},
            {"YoneQ3", {"YoneQ3Dash", ConnectionType::FOLLOW_ORIGIN}}
        };
    }

    // Getters

    SkillshotTable Data::GetSkillshots() const { return this->skillshots; }

    Linq<EvadingSpellData> Data::GetEvadingSpells() const { return this->evaders; }

    Linq<ShieldSpellData> Data::GetShieldSpells() const { return this->shields; }

    Miscellaneous Data::GetAntiRecalcs() const { return this->anti_recalcs; }

    Miscellaneous Data::GetDashingSpells() const { return this->dashes; }

    Miscellaneous Data::GetStoppers() const { return this->stoppers; }

    Miscellaneous Data::GetSpellQueue() const { return this->queue; }
}
