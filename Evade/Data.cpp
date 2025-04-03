
#include "Data.h"

namespace Evade
{
    Data* Data::m_instance = nullptr;

    Data::Data()
    {
        this->InitSkillshots();
        this->InitEvadingSpells();
        this->InitShieldSpells();
        this->InitConnectors();
        this->InitDashBuffs();
        this->InitPatterns();
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
        data.Collisions = { CollisionFlag::WIND_WALL };
        data.Detectors = { DetectionType::ON_WND_PROC };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::NONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Aatrox

        // Aatrox Q1

        data = SkillshotData();
        data.ChampionName = "Aatrox";
        data.DisplayName = "The Darkin Blade [First]";
        data.SkillshotName = "AatroxQ";
        data.IconName = "AatroxQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FollowCaster = true;
        data.HardCC = true;
        data.Delay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 130.0f;
        data.Range = 625.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 16, 40, 64, 88, 112 };
            float ad_scale[] = { 96, 108, 120, 132, 144 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Aatrox Q3

        data = SkillshotData();
        data.ChampionName = "Aatrox";
        data.DisplayName = "The Darkin Blade [Third]";
        data.SkillshotName = "AatroxQ3";
        data.IconName = "AatroxQ3.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FollowCaster = true;
        data.HardCC = true;
        data.Delay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 300.0f;
        data.Range = 200.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 24, 60, 96, 132, 168 };
            float ad_scale[] = { 144, 162, 180, 198, 216 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Aatrox W

        data = SkillshotData();
        data.ChampionName = "Aatrox";
        data.DisplayName = "Infernal Chains";
        data.MissileName = "AatroxW";
        data.SkillshotName = "AatroxW";
        data.IconName = "AatroxW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 825.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 80, 100, 120, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ahri

        // Ahri Q Initial

        data = SkillshotData();
        data.ChampionName = "Ahri";
        data.DisplayName = "Orb of Deception [Initial]";
        data.MissileName = "AhriOrbMissile";
        data.SkillshotName = "AhriQ";
        data.IconName = "AhriQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 970.0f;
        data.Speed = 2500.0f;
        data.Acceleration = -3200.0f;
        data.InitialSpeed = 2500.0f;
        data.MinSpeed = 400.0f;
        data.MaxSpeed = 2500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 65, 90, 115, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ahri Q Return

        data = SkillshotData();
        data.ChampionName = "Ahri";
        data.DisplayName = "Orb of Deception [Return]";
        data.MissileName = "AhriQReturnMissile";
        data.SkillshotName = "AhriQReturnMissile";
        data.IconName = "AhriQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.BackToCaster = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 100.0f;
        data.Range = 12500.0f;
        data.Speed = 2600.0f;
        data.Acceleration = 1900.0f;
        data.InitialSpeed = 60.0f;
        data.MinSpeed = 60.0f;
        data.MaxSpeed = 2600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 65, 90, 115, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ahri E

        data = SkillshotData();
        data.ChampionName = "Ahri";
        data.DisplayName = "Seduce";
        data.MissileName = "AhriEMissile";
        data.SkillshotName = "AhriE";
        data.IconName = "AhriE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1000.0f;
        data.Speed = 1550.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Akali

        // Akali E

        data = SkillshotData();
        data.ChampionName = "Akali";
        data.DisplayName = "Shuriken Flip";
        data.MissileName = "AkaliEMis";
        data.SkillshotName = "AkaliE";
        data.IconName = "AkaliE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 825.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 140, 210, 280, 350 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.TotalAttackDamage;
            bonus_damage += 1.1f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Alistar

        // Alistar Q

        data = SkillshotData();
        data.ChampionName = "Alistar";
        data.DisplayName = "Pulverize";
        data.SkillshotName = "Pulverize";
        data.IconName = "Pulverize.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 375.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ambessa

        // Ambessa Q2

        data = SkillshotData();
        data.ChampionName = "Ambessa";
        data.DisplayName = "Sundering Slam";
        data.SkillshotName = "AmbessaQ2";
        data.IconName = "AmbessaQ2.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.225f;
        data.Windup = 0.225f;
        data.Radius = 40.0f;
        data.Range = 650.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 75, 100, 125, 150 };
            float hp_scale[] = { 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = hp_scale[info.SpellLevel];
            bonus_damage += 0.04f * info.BonusAttackDamage;
            bonus_damage *= info.TargetMaxHealth / 100.0f;
            bonus_damage += 0.9f * info.BonusAttackDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ambessa R

        data = SkillshotData();
        data.ChampionName = "Ambessa";
        data.DisplayName = "Public Execution";
        data.SkillshotName = "AmbessaR";
        data.IconName = "AmbessaR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.55f;
        data.Windup = 0.55f;
        data.Radius = 65.0f;
        data.Range = 1250.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Amumu

        // Amumu Q

        data = SkillshotData();
        data.ChampionName = "Amumu";
        data.DisplayName = "Bandage Toss";
        data.MissileName = "SadMummyBandageToss";
        data.SkillshotName = "BandageToss";
        data.IconName = "BandageToss.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 1100.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 95, 120, 145, 170 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.85f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Amumu R

        data = SkillshotData();
        data.ChampionName = "Amumu";
        data.DisplayName = "Curse of the Sad Mummy";
        data.SkillshotName = "CurseoftheSadMummy";
        data.IconName = "CurseoftheSadMummy.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 550.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Anivia

        // Anivia Q1

        data = SkillshotData();
        data.ChampionName = "Anivia";
        data.DisplayName = "Flash Frost [Missile]";
        data.MissileName = "FlashFrostSpell";
        data.SkillshotName = "FlashFrost";
        data.IconName = "FlashFrost.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 225.0f;
        data.Range = 1100.0f;
        data.Speed = 950.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 70, 90, 110, 130 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.25f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Anivia Q2

        data = SkillshotData();
        data.ChampionName = "Anivia";
        data.DisplayName = "Flash Frost [Destination]";
        data.SkillshotName = "AniviaQ2";
        data.IconName = "FlashFrost.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 225.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 95, 130, 165, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Annie

        // Annie W

        data = SkillshotData();
        data.ChampionName = "Annie";
        data.DisplayName = "Incinerate";
        data.SkillshotName = "AnnieW";
        data.IconName = "AnnieW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.ConeAngle = 49.52f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 600.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.85f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Annie R

        data = SkillshotData();
        data.ChampionName = "Annie";
        data.DisplayName = "Summon: Tibbers";
        data.SkillshotName = "AnnieR";
        data.IconName = "AnnieR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 250.0f;
        data.Range = 600.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 275, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Aphelios

        // Aphelios Q Calibrum

        data = SkillshotData();
        data.ChampionName = "Aphelios";
        data.DisplayName = "Moonshot";
        data.MissileName = "ApheliosCalibrumQ";
        data.SkillshotName = "ApheliosCalibrumQ";
        data.IconName = "ApheliosCalibrumQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 60.0f;
        data.Range = 1450.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 60, 76.67f, 76.67f,
                93.33f, 93.33f, 110, 110, 126.67f, 126.67f,
                143.33f, 143.33f, 160, 160, 160, 160, 160, 160 };
            float ad_scale[] = { 42, 42, 45, 45, 48, 48, 51, 51,
                    54, 54, 57, 57, 60, 60, 60, 60, 60, 60 };
            float base_damage = base_scale[info.UnitLevel];
            float bonus_damage = ad_scale[info.UnitLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Aphelios Q Infernum

        data = SkillshotData();
        data.ChampionName = "Aphelios";
        data.DisplayName = "Infernum [Duskwave]";
        data.MissileName = "ApheliosInfernumQMis";
        data.SkillshotName = "ApheliosInfernumQ";
        data.IconName = "ApheliosInfernumQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Radius = 40.0f;
        data.Range = 850.0f;
        data.Speed = 1850.0f;
        data.DangerLevel = 1;
        data.MultiAngle = 20;
        data.MultiCount = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 25, 25, 31.67f, 31.67f,
                38.33f, 38.33f, 45, 45, 51.67f, 51.67f,
                58.33f, 58.33f, 65, 65, 65, 65, 65, 65 };
            float ad_scale[] = { 56, 56, 60, 60, 64, 64, 68,
                68, 72, 72, 76, 76, 80, 80, 80, 80, 80, 80 };
            float base_damage = base_scale[info.UnitLevel];
            float bonus_damage = ad_scale[info.UnitLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Aphelios R

        data = SkillshotData();
        data.ChampionName = "Aphelios";
        data.DisplayName = "Moonlight Vigil";
        data.MissileName = "ApheliosRMis";
        data.SkillshotName = "ApheliosR";
        data.IconName = "ApheliosR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 125.0f;
        data.Range = 1300.0f;
        data.Speed = 2050.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 125, 175, 225 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.2f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ashe

        // Ashe W

        data = SkillshotData();
        data.ChampionName = "Ashe";
        data.DisplayName = "Volley";
        data.SkillshotName = "Volley";
        data.IconName = "Volley.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.ConeAngle = 30.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 20.0f;
        data.Range = 1275.0f;
        data.Offset = 75.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 95, 130, 165, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Ashe R

        data = SkillshotData();
        data.ChampionName = "Ashe";
        data.DisplayName = "Enchanted Crystal Arrow";
        data.MissileName = "EnchantedCrystalArrow";
        data.SkillshotName = "EnchantedCrystalArrow";
        data.IconName = "EnchantedCrystalArrow.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FastEvade = true;
        data.FixedRange = true;
        data.FixSpeed = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsGlobal = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 130.0f;
        data.Range = 25000.0f;
        data.Speed = 1650.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 400, 600 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Aurora

        // Aurora Q

        data = SkillshotData(); // Check
        data.ChampionName = "Aurora";
        data.DisplayName = "Twofold Hex";
        data.MissileName = "AuroraQ";
        data.SkillshotName = "AuroraQ";
        data.IconName = "AuroraQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 90.0f;
        data.Range = 900.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 65, 90, 115, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Aurora E

        data = SkillshotData();
        data.ChampionName = "Aurora";
        data.DisplayName = "The Weirding";
        data.SkillshotName = "AuroraE";
        data.IconName = "AuroraE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 87.5f;
        data.Range = 825.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Bard

        // Bard Q

        data = SkillshotData();
        data.ChampionName = "Bard";
        data.DisplayName = "Cosmic Binding";
        data.MissileName = "BardQMissile";
        data.SkillshotName = "BardQ";
        data.IconName = "BardQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1150.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Bard R

        data = SkillshotData();
        data.ChampionName = "Bard";
        data.DisplayName = "Tempered Fate";
        data.MissileName = "BardRMissile";
        data.SkillshotName = "BardR";
        data.IconName = "BardR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 350.0f;
        data.Range = 3400.0f;
        data.Speed = 2100.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Belveth

        // Belveth W

        data = SkillshotData();
        data.ChampionName = "Belveth";
        data.DisplayName = "Above and Below";
        data.ParticleName = "_W_Tell";
        data.SkillshotName = "BelvethW";
        data.IconName = "BelvethW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.ParticleDelay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 100.0f;
        data.Range = 715.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.BonusAttackDamage;
            bonus_damage += 1.25f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Blitzcrank

        // Blitzcrank Q

        data = SkillshotData();
        data.ChampionName = "Blitzcrank";
        data.DisplayName = "Rocket Grab";
        data.MissileName = "RocketGrabMissile";
        data.SkillshotName = "RocketGrab";
        data.IconName = "RocketGrab.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1050.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 105, 150, 195, 240, 285 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Blitzcrank R

        data = SkillshotData();
        data.ChampionName = "Blitzcrank";
        data.DisplayName = "Static Field";
        data.SkillshotName = "StaticField";
        data.IconName = "StaticField.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 275, 400, 525 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Brand

        // Brand Q

        data = SkillshotData();
        data.ChampionName = "Brand";
        data.DisplayName = "Sear";
        data.MissileName = "BrandQMissile";
        data.SkillshotName = "BrandQ";
        data.IconName = "BrandQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1100.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 120, 165, 210, 255 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.6f * info.TotalAbilityDamage;
            return 1.25f * (base_damage + bonus_damage);
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Brand W

        data = SkillshotData();
        data.ChampionName = "Brand";
        data.DisplayName = "Pillar of Flame";
        data.ParticleName = "_W_POF_charge";
        data.SkillshotName = "BrandW";
        data.IconName = "BrandW.png";
        data.SkillshotSlot = 'W';
        data.FogSupport = true;
        data.Delay = 0.9f;
        data.ParticleDelay = 0.65f;
        data.Windup = 0.25f;
        data.Radius = 260.0f;
        data.Range = 900.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 55, 80, 105, 130, 155 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Braum

        // Braum Q

        data = SkillshotData();
        data.ChampionName = "Braum";
        data.DisplayName = "Winter's Bite";
        data.MissileName = "BraumQMissile";
        data.SkillshotName = "BraumQ";
        data.IconName = "BraumQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1050.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 125, 175, 225, 275 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.025f * info.MaxHealth;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Braum R

        data = SkillshotData();
        data.ChampionName = "Braum";
        data.DisplayName = "Glacial Fissure";
        data.MissileName = "BraumRMissile";
        data.SkillshotName = "BraumRWrapper";
        data.IconName = "BraumRWrapper.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 115.0f;
        data.Range = 1200.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 300, 450 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Briar

        // Briar E

        data = SkillshotData();
        data.ChampionName = "Briar";
        data.DisplayName = "Chilling Scream";
        data.MissileName = "BriarEMis";
        data.SkillshotName = "BriarE";
        data.IconName = "BriarE.png";
        data.Overrider = "BriarE";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.Radius = 190.0f;
        data.Range = 600.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 115, 150, 185, 220 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Briar R

        data = SkillshotData();
        data.ChampionName = "Briar";
        data.DisplayName = "Certain Death";
        data.MissileName = "BriarR(?!Self)";
        data.SkillshotName = "BriarR";
        data.IconName = "BriarR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.Windup = 1.0f;
        data.Radius = 160.0f;
        data.Range = 10000.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.5f * info.BonusAttackDamage;
            bonus_damage += 1.2f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Caitlyn

        // Caitlyn Q

        data = SkillshotData();
        data.ChampionName = "Caitlyn";
        data.DisplayName = "Piltover Peacemaker";
        data.MissileName = "CaitlynQ";
        data.SkillshotName = "CaitlynQ";
        data.IconName = "CaitlynQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.625f;
        data.Windup = 0.625f;
        data.Radius = 90.0f;
        data.Range = 1300.0f;
        data.Speed = 2200.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 90, 130, 170, 210 };
            float ad_scale[] = { 125, 145, 165, 185, 205 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Caitlyn W

        data = SkillshotData();
        data.ChampionName = "Caitlyn";
        data.DisplayName = "Yordle Snap Trap";
        data.ParticleName = "_W_Trap_idle_enemy";
        data.SkillshotName = "CaitlynWParticle";
        data.IconName = "CaitlynW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.IsTrap = true;
        data.Delay = 1.05f;
        data.ParticleDelay = 1.05f;
        data.ExtraTime = 50.0f;
        data.Radius = 15.0f;
        data.Range = 800.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors = { DetectionType::ON_OBJECT_CREATED };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Caitlyn E

        data = SkillshotData();
        data.ChampionName = "Caitlyn";
        data.DisplayName = "Entrapment";
        data.MissileName = "CaitlynEMissile";
        data.SkillshotName = "CaitlynE";
        data.IconName = "CaitlynE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.15f;
        data.Windup = 0.15f;
        data.Radius = 70.0f;
        data.Range = 750.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 130, 180, 230, 280 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Camille

        // Camille E

        data = SkillshotData();
        data.ChampionName = "Camille";
        data.DisplayName = "Hookshot [Second]";
        data.SkillshotName = "CamilleEDash2";
        data.IconName = "CamilleE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 130.0f;
        data.Range = 800.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Cassiopeia

        // Cassiopeia Q

        data = SkillshotData();
        data.ChampionName = "Cassiopeia";
        data.DisplayName = "Noxious Blast";
        data.ParticleName = "_Q_Hit";
        data.SkillshotName = "CassiopeiaQ";
        data.IconName = "CassiopeiaQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.Delay = 0.7f;
        data.ParticleDelay = 0.45f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 850.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 110, 145, 180, 215 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Cassiopeia R

        data = SkillshotData();
        data.ChampionName = "Cassiopeia";
        data.DisplayName = "Petrifying Gaze";
        data.SkillshotName = "CassiopeiaR";
        data.IconName = "CassiopeiaR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.ConeAngle = 80.0f;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Range = 950.0f;
        data.Offset = -100.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Chogath

        // Chogath Q

        data = SkillshotData();
        data.ChampionName = "Chogath";
        data.DisplayName = "Rupture";
        data.ParticleName = "_Q_Enemy";
        data.SkillshotName = "Rupture";
        data.IconName = "Rupture.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 1.15f;
        data.ParticleDelay = 0.65f;
        data.Windup = 0.5f;
        data.Radius = 250.0f;
        data.Range = 950.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 140, 200, 260, 320 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Chogath W

        data = SkillshotData();
        data.ChampionName = "Chogath";
        data.DisplayName = "Feral Scream";
        data.SkillshotName = "FeralScream";
        data.IconName = "FeralScream.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.ConeAngle = 56.0f;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Range = 650.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 130, 180, 230, 280 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Corki

        // Corki Q

        data = SkillshotData();
        data.ChampionName = "Corki";
        data.DisplayName = "Phosphorus Bomb";
        data.MissileName = "PhosphorusBombMissile";
        data.SkillshotName = "PhosphorusBomb";
        data.IconName = "PhosphorusBomb.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 275.0f;
        data.Range = 825.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.2f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Corki R1

        data = SkillshotData();
        data.ChampionName = "Corki";
        data.DisplayName = "Missile Barrage [Standard]";
        data.MissileName = "MissileBarrageMissile";
        data.SkillshotName = "MissileBarrage";
        data.IconName = "MissileBarrage.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.175f;
        data.Windup = 0.175f;
        data.Radius = 40.0f;
        data.Range = 1300.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 170, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Corki R2

        data = SkillshotData();
        data.ChampionName = "Corki";
        data.DisplayName = "Missile Barrage [Big One]";
        data.MissileName = "MissileBarrageMissile2";
        data.SkillshotName = "MissileBarrage2";
        data.IconName = "MissileBarrage.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.175f;
        data.Windup = 0.175f;
        data.Radius = 40.0f;
        data.Range = 1500.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 160, 320, 480 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Darius

        // Darius Q

        data = SkillshotData();
        data.ChampionName = "Darius";
        data.DisplayName = "Decimate";
        data.SkillshotName = "DariusCleave";
        data.IconName = "DariusCleave.png";
        data.SkillshotSlot = 'Q';
        data.FollowCaster = true;
        data.Delay = 0.75f;
        data.InnerRadius = 240.0f;
        data.OuterRadius = 460.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 80, 110, 140, 170 };
            float ad_scale[] = { 100, 110, 120, 130, 140 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::RING;
        this->skillshots[data.SkillshotName] = data;

        // Darius E

        data = SkillshotData();
        data.ChampionName = "Darius";
        data.DisplayName = "Apprehend";
        data.SkillshotName = "DariusAxeGrabCone";
        data.IconName = "DariusAxeGrabCone.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.ConeAngle = 50.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 535.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Diana

        // Diana Q

        data = SkillshotData();
        data.ChampionName = "Diana";
        data.DisplayName = "Crescent Strike";
        data.SkillshotName = "DianaQ";
        data.IconName = "DianaQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 800.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Diana R

        data = SkillshotData();
        data.ChampionName = "Diana";
        data.DisplayName = "Moonfall";
        data.SkillshotName = "DianaR";
        data.IconName = "DianaR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FollowCaster = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 475.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region DrMundo

        // DrMundo Q

        data = SkillshotData();
        data.ChampionName = "DrMundo";
        data.DisplayName = "Infected Cleaver";
        data.MissileName = "DrMundoQ";
        data.SkillshotName = "DrMundoQ";
        data.IconName = "DrMundoQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1050.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 130, 180, 230, 280 };
            float hp_scale[] = { 20, 22.5f, 25, 27.5f, 30 };
            float base_damage = base_scale[info.SpellLevel];
            float hp_damage = hp_scale[info.SpellLevel];
            hp_damage *= info.TargetHealth / 100.0f;
            return MAX(base_damage, hp_damage);
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Draven

        // Draven E

        data = SkillshotData();
        data.ChampionName = "Draven";
        data.DisplayName = "Stand Aside";
        data.MissileName = "DravenDoubleShotMissile";
        data.SkillshotName = "DravenDoubleShot";
        data.IconName = "DravenDoubleShot.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 130.0f;
        data.Range = 1100.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 110, 145, 180, 215 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Draven R

        data = SkillshotData();
        data.ChampionName = "Draven";
        data.DisplayName = "Whirling Death";
        data.MissileName = "DravenR";
        data.SkillshotName = "DravenRCast";
        data.IconName = "DravenRCast.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsGlobal = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 160.0f;
        data.Range = 25000.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 350, 550, 750 };
            float ad_scale[] = { 220, 260, 300 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ekko

        // Ekko Q1

        data = SkillshotData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Timewinder";
        data.MissileName = "EkkoQMis",
        data.SkillshotName = "EkkoQ";
        data.IconName = "EkkoQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Radius = 60.0f;
        data.Range = 700.0f;
        data.Speed = 1650.0f;
        data.DangerLevel = 1;
        data.Exclusions =
        {
            "EkkoQ2"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 95, 110, 125, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.3f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ekko Q2

        data = SkillshotData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Temporal Sickness";
        data.SkillshotName = "EkkoQ2";
        data.IconName = "EkkoQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Radius = 100.0f;
        data.Range = 400.0f;
        data.Speed = 200.0f;
        data.DangerLevel = 1;
        data.Exclusions =
        {
            "EkkoQ"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 95, 110, 125, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.3f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ekko Q Return

        data = SkillshotData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Timewinder [Return]";
        data.MissileName = "EkkoQReturn";
        data.SkillshotName = "EkkoQReturn";
        data.IconName = "EkkoQ.png";
        data.Overrider = "EkkoQ2";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.BackToCaster = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Radius = 100.0f;
        data.Range = 12500.0f;
        data.Speed = 2300.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 65, 90, 115, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ekko W

        data = SkillshotData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Parallel Convergence";
        data.MissileName = "EkkoWMis";
        data.SkillshotName = "EkkoW";
        data.IconName = "EkkoW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 3.25f;
        data.MissileDelay = 1.0f;
        data.Windup = 0.25f;
        data.Radius = 375.0f;
        data.Range = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Ekko R

        data = SkillshotData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Chronobreak";
        data.ParticleName = "_R_ChargeIndicator";
        data.SkillshotName = "EkkoRParticle";
        data.IconName = "EkkoR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.Delay = 0.5f;
        data.ParticleDelay = 0.5f;
        data.Radius = 375.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 350, 500 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Elise

        // Elise E

        data = SkillshotData();
        data.ChampionName = "Elise";
        data.DisplayName = "Cocoon";
        data.MissileName = "EliseHumanE";
        data.SkillshotName = "EliseHumanE";
        data.IconName = "EliseHumanE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 1100.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Evelynn

        // Evelynn Q

        data = SkillshotData();
        data.ChampionName = "Evelynn";
        data.DisplayName = "Hate Spike";
        data.MissileName = "^EvelynnQ$";
        data.SkillshotName = "EvelynnQ";
        data.IconName = "EvelynnQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 800.0f;
        data.Speed = 2400.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 25, 30, 35, 40, 45 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.25f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Evelynn R

        data = SkillshotData();
        data.ChampionName = "Evelynn";
        data.DisplayName = "Last Caress";
        data.SkillshotName = "EvelynnR";
        data.IconName = "EvelynnR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FixedRange = true;
        data.ConeAngle = 180.0f;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Range = 500.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 125, 250, 375 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.75f * info.BonusAttackDamage;
            float total_damage = base_damage + bonus_damage;
            float ratio = info.TargetHealth / info.TargetMaxHealth;
            return total_damage * (ratio < 0.3f ? 2.4f : 1.0f);
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ezreal

        // Ezreal Q

        data = SkillshotData();
        data.ChampionName = "Ezreal";
        data.DisplayName = "Mystic Shot";
        data.MissileName = "EzrealQ";
        data.SkillshotName = "EzrealQ";
        data.IconName = "EzrealQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1200.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.3f * info.TotalAttackDamage;
            bonus_damage += 0.15f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ezreal W

        data = SkillshotData();
        data.ChampionName = "Ezreal";
        data.DisplayName = "Essence Flux";
        data.MissileName = "EzrealW";
        data.SkillshotName = "EzrealW";
        data.IconName = "EzrealW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 1200.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 135, 190, 245, 300 };
            float ap_scale[] = { 70, 75, 80, 85, 90 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ap_scale[info.SpellLevel];
            bonus_damage *= info.TotalAbilityDamage / 100.0f;
            bonus_damage += 1.0f * info.BonusAttackDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ezreal R

        data = SkillshotData();
        data.ChampionName = "Ezreal";
        data.DisplayName = "Trueshot Barrage";
        data.MissileName = "EzrealR";
        data.SkillshotName = "EzrealR";
        data.IconName = "EzrealR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsGlobal = true;
        data.Delay = 1.0f;
        data.Windup = 1.0f;
        data.Radius = 160.0f;
        data.Range = 25000.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 350, 550, 750 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.BonusAttackDamage;
            bonus_damage += 0.9f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Fiora

        // Fiora W

        data = SkillshotData();
        data.ChampionName = "Fiora";
        data.DisplayName = "Riposte";
        data.SkillshotName = "FioraW";
        data.IconName = "FioraW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 70.0f;
        data.Range = 750.0f;
        data.Speed = 3200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 110, 150, 190, 230, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Fizz

        // Fizz R

        data = SkillshotData();
        data.ChampionName = "Fizz";
        data.DisplayName = "Chum the Waters";
        data.MissileName = "FizzRMissile";
        data.SkillshotName = "FizzR";
        data.IconName = "FizzR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 120.0f;
        data.Range = 1300.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 300, 400, 500 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Galio

        // Galio Q

        data = SkillshotData();
        data.ChampionName = "Galio";
        data.DisplayName = "Winds of War";
        data.SkillshotName = "GalioQ";
        data.IconName = "GalioQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 225.0f;
        data.Range = 825.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Galio E

        data = SkillshotData();
        data.ChampionName = "Galio";
        data.DisplayName = "Justice Punch";
        data.SkillshotName = "GalioE";
        data.IconName = "GalioE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.4f;
        data.Radius = 200.0f;
        data.Range = 750.0f;
        data.Offset = -250.0f;
        data.Speed = 2300.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 130, 170, 210, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Gnar

        // Gnar Q

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Boomerang Throw [Initial]";
        data.MissileName = "^GnarQMissile$";
        data.SkillshotName = "GnarQ";
        data.IconName = "GnarQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 1125.0f;
        data.Speed = 2500.0f;
        data.Acceleration = -2500.0f;
        data.InitialSpeed = 2500.0f;
        data.MinSpeed = 1400.0f;
        data.MaxSpeed = 2500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 5, 45, 85, 125, 165 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.25f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar Q Return

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Boomerang Throw [Return]";
        data.MissileName = "^GnarQMissileReturn$";
        data.SkillshotName = "GnarQMissileReturn";
        data.IconName = "GnarQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Radius = 75.0f;
        data.Range = 3000.0f;
        data.Speed = 1125.0f;
        data.Acceleration = 800.0f;
        data.InitialSpeed = 60.0f;
        data.MinSpeed = 60.0f;
        data.MaxSpeed = 2600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 5, 45, 85, 125, 165 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.25f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar Q Big Missile

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Boulder Toss [Missile]";
        data.MissileName = "GnarBigQMissile";
        data.SkillshotName = "GnarBigQ";
        data.IconName = "GnarBigQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 90.0f;
        data.Range = 1150.0f;
        data.Speed = 2100.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "GnarBigQLand"
        };
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 90, 135, 180, 225 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.4f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar Q Big Land

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Boulder Toss [Land]";
        data.SkillshotName = "GnarBigQLand";
        data.IconName = "GnarBigQ.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.SoftCC = true;
        data.Radius = 200.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "GnarBigQ"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 90, 135, 180, 225 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.4f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar W

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Wallop";
        data.ParticleName = "_W_Big_Beam";
        data.SkillshotName = "GnarBigW";
        data.IconName = "GnarBigW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.6f;
        data.ParticleDelay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 80.0f;
        data.Range = 550.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 75, 105, 135, 165 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar E

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Hop";
        data.SkillshotName = "GnarE";
        data.IconName = "GnarE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.SoftCC = true;
        data.Radius = 150.0f;
        data.Range = 475.0f;
        data.Speed = 900.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 85, 120, 155, 190 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.06f * info.MaxHealth;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar E Big

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Crunch";
        data.SkillshotName = "GnarBigE";
        data.IconName = "GnarBigE.png";
        data.SkillshotSlot = 'E';
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.Radius = 350.0f;
        data.Range = 675.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 115, 150, 185, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.06f * info.MaxHealth;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Gnar R

        data = SkillshotData();
        data.ChampionName = "Gnar";
        data.DisplayName = "GNAR!";
        data.SkillshotName = "GnarR";
        data.IconName = "GnarR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 475.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 300, 450, 600 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.75f * info.BonusAttackDamage;
            bonus_damage += 1.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Gragas

        // Gragas Q Missile

        data = SkillshotData();
        data.ChampionName = "Gragas";
        data.DisplayName = "Barrel Roll [Missile]";
        data.MissileName = "GragasQMissile";
        data.SkillshotName = "GragasQ";
        data.IconName = "GragasQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.ExtraTime = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 300.0f;
        data.Range = 850.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 120, 180, 240, 300, 360 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Gragas Q Explosion

        data = SkillshotData();
        data.ChampionName = "Gragas";
        data.DisplayName = "Barrel Roll [Explosion]";
        data.ParticleName = "_Q_Enemy";
        data.SkillshotName = "GragasQExplosion";
        data.IconName = "GragasQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.ExtraTime = 4.25f;
        data.Radius = 300.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 120, 180, 240, 300, 360 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Gragas E

        data = SkillshotData();
        data.ChampionName = "Gragas";
        data.DisplayName = "Body Slam";
        data.SkillshotName = "GragasE";
        data.IconName = "GragasE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 180.0f;
        data.Range = 800.0f;
        data.Speed = 900.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Gragas R

        data = SkillshotData();
        data.ChampionName = "Gragas";
        data.DisplayName = "Explosive Cask";
        data.MissileName = "GragasRBoom";
        data.SkillshotName = "GragasR";
        data.IconName = "GragasR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.Windup = 0.25f;
        data.Radius = 400.0f;
        data.Range = 1000.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Graves

        // Graves Q1

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "End of the Line [Initial]";
        data.MissileName = "GravesQLineMis";
        data.SkillshotName = "GravesQLineMis";
        data.IconName = "GravesQLineSpell.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.Radius = 40.0f;
        data.Range = 800.0f;
        data.Speed = 3000.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 65, 85, 105, 125 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Graves Q2

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "End of the Line [Detonation]";
        data.SkillshotName = "GravesQDetonation";
        data.IconName = "GravesQLineSpell.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Rotate90 = true;
        data.Radius = 100.0f;
        data.Range = 1.0f;
        data.SideRange = 500.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 65, 85, 105, 125 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Graves Q Return

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "End of the Line [Return]";
        data.MissileName = "GravesQReturn";
        data.SkillshotName = "GravesQReturn";
        data.IconName = "GravesQLineSpell.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 100.0f;
        data.Range = 900.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 120, 155, 190, 225 };
            float ad_scale[] = { 40, 65, 90, 115, 140 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Graves W

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "Smoke Grenade";
        data.MissileName = "GravesSmokeGrenadeBoom";
        data.SkillshotName = "GravesSmokeGrenade";
        data.IconName = "GravesSmokeGrenade.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 950.0f;
        data.Speed = 1650.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 110, 160, 210, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Graves R1

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "Charge Shot [Missile]";
        data.MissileName = "GravesChargeShotShot";
        data.SkillshotName = "GravesChargeShot";
        data.IconName = "GravesChargeShot.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 1100.0f;
        data.Speed = 2100.0f;
        data.DangerLevel = 4;
        data.Exclusions =
        {
            "GravesRExplosion"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 275, 425, 575 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Graves R2

        data = SkillshotData();
        data.ChampionName = "Graves";
        data.DisplayName = "Charge Shot [Explosion]";
        data.SkillshotName = "GravesRExplosion";
        data.IconName = "GravesChargeShot.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Radius = 100.0f;
        data.Range = 590.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Exclusions =
        {
            "GravesChargeShot"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 275, 425, 575 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Gwen

        // Gwen R

        data = SkillshotData();
        data.ChampionName = "Gwen";
        data.DisplayName = "Needlework";
        data.MissileName = "^GwenRMis$";
        data.SkillshotName = "GwenRMis";
        data.IconName = "GwenR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Radius = 60.0f;
        data.Range = 1350.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 60, 90 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.08f * info.TotalAbilityDamage;
            float hp_scale = 0.008f * info.TotalAbilityDamage / 100.0f;
            bonus_damage += info.TargetMaxHealth * (0.01f + hp_scale);
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Hecarim

        // Hecarim R Dash

        data = SkillshotData();
        data.ChampionName = "Hecarim";
        data.DisplayName = "Onslaught of Shadows [Dash]";
        data.SkillshotName = "HecarimRDash";
        data.IconName = "HecarimUlt.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Radius = 315.0f;
        data.Range = 1650.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors = { DetectionType::ON_NEW_PATH };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Hecarim R Shadows

        data = SkillshotData();
        data.ChampionName = "Hecarim";
        data.DisplayName = "Onslaught of Shadows [Riders]";
        data.MissileName = "HecarimUltMissile";
        data.SkillshotName = "HecarimShadows";
        data.IconName = "HecarimUlt.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.Radius = 40.0f;
        data.Range = 1650.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Exclusions =
        {
            "HecarimShadows"
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Heimerdinger

        // Heimerdinger W

        data = SkillshotData();
        data.ChampionName = "Heimerdinger";
        data.DisplayName = "Hextech Micro-Rockets";
        data.SkillshotName = "HeimerdingerW";
        data.IconName = "HeimerdingerW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 40.0f;
        data.Range = 1325.0f;
        data.Speed = 2050.0f;
        data.Acceleration = 4000.0f;
        data.InitialSpeed = 750.0f;
        data.MinSpeed = 750.0f;
        data.MaxSpeed = 3000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 72, 117, 162, 207, 252 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.99f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Heimerdinger E

        data = SkillshotData();
        data.ChampionName = "Heimerdinger";
        data.DisplayName = "CH-2 Electron Storm Grenade";
        data.MissileName = "HeimerdingerESpell";
        data.SkillshotName = "HeimerdingerE";
        data.IconName = "HeimerdingerE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 250.0f;
        data.Range = 970.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Heimerdinger RE

        data = SkillshotData();
        data.ChampionName = "Heimerdinger";
        data.DisplayName = "CH-3X Lightning Grenade";
        data.SkillshotName = "HeimerdingerEUlt";
        data.IconName = "HeimerdingerE.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 250.0f;
        data.Range = 970.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "HeimerdingerEUlt"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 200, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Hwei

        // Hwei QQ Fireball

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Devastating Fire [Fireball]";
        data.MissileName = "HweiQQ";
        data.SkillshotName = "HweiQQ";
        data.IconName = "HweiQQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 900.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "HweiQQExplosion"
        };
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float hp_scale[] = { 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = hp_scale[info.SpellLevel];
            bonus_damage *= info.TargetMaxHealth / 100.0f;
            bonus_damage += 0.75f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei QQ Explosion

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Devastating Fire [Explosion]";
        data.SkillshotName = "HweiQQExplosion";
        data.IconName = "HweiQQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Radius = 240.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "HweiQQ"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 80, 110, 140, 170 };
            float hp_scale[] = { 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = hp_scale[info.SpellLevel];
            bonus_damage *= info.TargetMaxHealth / 100.0f;
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei QW

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Severing Bolt";
        data.ParticleName = "_Q_W_warning";
        data.SkillshotName = "HweiQW";
        data.IconName = "HweiQW.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.Delay = 1.0f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.5f;
        data.Radius = 225.0f;
        data.Range = 1900.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.25f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei QE

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Molten Fissure";
        data.MissileName = "HweiQE(?!Tail)";
        data.SkillshotName = "HweiQE";
        data.IconName = "HweiQE.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.35f;
        data.Radius = 150.0f;
        data.Range = 1200.0f;
        data.Speed = 800.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 40, 60, 80, 100 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.3f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei EQ

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Grim Visage";
        data.MissileName = "HweiEQ";
        data.SkillshotName = "HweiEQ";
        data.IconName = "HweiEQ.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1100.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei EW

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Gaze of the Abyss";
        data.MissileName = "HweiEW";
        data.SkillshotName = "HweiEW";
        data.IconName = "HweiEW.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.ExtraTime = 2.5f;
        data.Windup = 0.25f;
        data.Radius = 350.0f;
        data.Range = 900.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei EE

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Crushing Maw";
        data.SkillshotName = "HweiEE";
        data.IconName = "HweiEE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.SkipAncestor = true;
        data.HardCC = true;
        data.Delay = 0.625f;
        data.Windup = 0.35f;
        data.Radius = 170.0f;
        data.Range = 800.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Hwei R

        data = SkillshotData();
        data.ChampionName = "Hwei";
        data.DisplayName = "Spiraling Despair";
        data.MissileName = "HweiR";
        data.SkillshotName = "HweiR";
        data.IconName = "HweiR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 90.0f;
        data.Range = 1300.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 230, 360, 490 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.95f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        // Illaoi Q

        data = SkillshotData();
        data.ChampionName = "Illaoi";
        data.DisplayName = "Tentacle Smash";
        data.ParticleName = "_Q_Indicator";
        data.SkillshotName = "IllaoiQParticle";
        data.IconName = "IllaoiQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 100.0f;
        data.Range = 800.0f;
        data.Offset = -50.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_damage = 9.0f * info.UnitLevel;
            float bonus_damage = 1.0f * info.TotalAttackDamage;
            bonus_damage += 0.4f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma region Illaoi

        // Illaoi E

        data = SkillshotData();
        data.ChampionName = "Illaoi";
        data.DisplayName = "Test of Spirit";
        data.MissileName = "IllaoiEMis";
        data.SkillshotName = "IllaoiE";
        data.IconName = "IllaoiE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 50.0f;
        data.Range = 950.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Irelia

        // Irelia W

        data = SkillshotData();
        data.ChampionName = "Irelia";
        data.DisplayName = "Defiant Dance";
        data.SkillshotName = "IreliaW2";
        data.IconName = "IreliaW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Radius = 120.0f;
        data.Range = 775.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 60, 90, 120, 150 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.2f * info.TotalAttackDamage;
            bonus_damage += 1.2f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Irelia E

        data = SkillshotData();
        data.ChampionName = "Irelia";
        data.DisplayName = "Flawless Duet";
        data.MissileName = "IreliaEMissile";
        data.SkillshotName = "IreliaEMissile";
        data.IconName = "IreliaE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Radius = 70.0f;
        data.Range = 12500.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Irelia R

        data = SkillshotData();
        data.ChampionName = "Irelia";
        data.DisplayName = "Vanguard's Edge";
        data.SkillshotName = "IreliaR";
        data.IconName = "IreliaR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 160.0f;
        data.Range = 1000.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 125, 200, 275 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ivern

        // Ivern Q

        data = SkillshotData();
        data.ChampionName = "Ivern";
        data.DisplayName = "Rootcaller";
        data.MissileName = "IvernQ";
        data.SkillshotName = "IvernQ";
        data.IconName = "IvernQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 1150.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Janna

        // Janna Q

        data = SkillshotData();
        data.ChampionName = "Janna";
        data.DisplayName = "Howling Gale";
        data.MissileName = "HowlingGaleSpell";
        data.SkillshotName = "HowlingGaleSpell";
        data.IconName = "HowlingGale.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Radius = 120.0f;
        data.Range = 1760.0f;
        data.Speed = 1167.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 135, 185, 235, 285 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region JarvanIV

        // JarvanIV Q

        data = SkillshotData();
        data.ChampionName = "JarvanIV";
        data.DisplayName = "Dragon Strike [Standard]";
        data.SkillshotName = "JarvanIVDragonStrike";
        data.IconName = "JarvanIVDragonStrike.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 70.0f;
        data.Range = 850.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 130, 170, 210, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.45f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // JarvanIV E

        data = SkillshotData();
        data.ChampionName = "JarvanIV";
        data.DisplayName = "Demacian Standard";
        data.ParticleName = "_E_tar_green";
        data.SkillshotName = "JarvanIVDemacianStandard";
        data.IconName = "JarvanIVDemacianStandard.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.Delay = 0.4f;
        data.ParticleDelay = 0.4f;
        data.ExtraTime = 8.0f;
        data.Radius = 200.0f;
        data.Range = 860.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // JarvanIV EQ

        data = SkillshotData();
        data.ChampionName = "JarvanIV";
        data.DisplayName = "Dragon Strike [Dash]";
        data.SkillshotName = "JarvanIVEQ";
        data.IconName = "JarvanIVDragonStrike.png";
        data.SkillshotSlot = 'Q';
        data.HardCC = true;
        data.Delay = 0.4f;
        data.Radius = 180.0f;
        data.Range = 850.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Jayce

        // Jayce Q Standard

        data = SkillshotData(); // Check effect radius
        data.ChampionName = "Jayce";
        data.DisplayName = "Shock Blast [Standard]";
        data.MissileName = "JayceShockBlastMis";
        data.SkillshotName = "JayceShockBlast";
        data.IconName = "JayceShockBlast.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.2143f;
        data.Windup = 0.2143f;
        data.Radius = 70.0f;
        data.Range = 1050.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 110, 160, 210, 260, 310 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Jayce Q Accelerated

        data = SkillshotData();
        data.ChampionName = "Jayce";
        data.DisplayName = "Shock Blast [Accelerated]";
        data.MissileName = "JayceShockBlastWallMis";
        data.SkillshotName = "JayceShockBlastWallMis";
        data.IconName = "JayceShockBlast.png";
        data.Overrider = "JayceShockBlast";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 70.0f;
        data.Range = 1600.0f;
        data.Speed = 2350.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 84, 154, 224, 294, 364, 434 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.96f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Jhin

        // Jhin W

        data = SkillshotData();
        data.ChampionName = "Jhin";
        data.DisplayName = "Deadly Flourish";
        data.ParticleName = "_W_indicator";
        data.SkillshotName = "JhinW";
        data.IconName = "JhinW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 45.0f;
        data.Range = 2550.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 95, 130, 165, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Jhin E

        data = SkillshotData();
        data.ChampionName = "Jhin";
        data.DisplayName = "Captive Audience";
        data.ParticleName = "_E_Trap_indicator";
        data.SkillshotName = "JhinEParticle";
        data.IconName = "JhinE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsTrap = true;
        data.SoftCC = true;
        data.Delay = 0.8f;
        data.ParticleDelay = 0.8f;
        data.ExtraTime = 180.0f;
        data.Radius = 160.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 80, 140, 200, 260 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.2f * info.TotalAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Jhin R

        data = SkillshotData();
        data.ChampionName = "Jhin";
        data.DisplayName = "Curtain Call";
        data.MissileName = "JhinRShotMis";
        data.SkillshotName = "JhinRShot";
        data.IconName = "JhinR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 3500.0f;
        data.Speed = 5000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 64, 128, 192 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.25f * info.TotalAttackDamage;
            float missing_hp = info.TargetMaxHealth - info.TargetHealth;
            float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
            float damage_mod = 1.0f + 3.0f * missing_hp_ratio;
            return (base_damage + bonus_damage) * damage_mod;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Jinx

        // Jinx W

        data = SkillshotData();
        data.ChampionName = "Jinx";
        data.DisplayName = "Zap!";
        data.ParticleName = "_W_Beam";
        data.SkillshotName = "JinxW";
        data.IconName = "JinxW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.SoftCC = true;
        data.Delay = 0.6f;
        data.ParticleDelay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 60.0f;
        data.Range = 1500.0f;
        data.Speed = 3300.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 10, 60, 110, 160, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.4f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Jinx E Toss

        data = SkillshotData();
        data.ChampionName = "Jinx";
        data.DisplayName = "Flame Chompers! [Toss]";
        data.MissileName = "JinxEHit";
        data.SkillshotName = "JinxEHit";
        data.IconName = "JinxE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 1.1f;
        data.MissileDelay = 1.1f;
        data.Radius = 115.0f;
        data.Range = 925.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 120, 170, 220, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Jinx E Explosion

        data = SkillshotData();
        data.ChampionName = "Jinx";
        data.DisplayName = "Flame Chompers! [Explosion]";
        data.ParticleName = "_E_Mine_Ready";
        data.SkillshotName = "JinxETrap";
        data.IconName = "JinxE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.ExtraTime = 4.5f;
        data.Radius = 115.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 120, 170, 220, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Jinx R

        data = SkillshotData();
        data.ChampionName = "Jinx";
        data.DisplayName = "Super Mega Death Rocket!";
        data.MissileName = "JinxR";
        data.SkillshotName = "JinxR";
        data.IconName = "JinxR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FastEvade = true;
        data.FixSpeed = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsGlobal = true;
        data.Delay = 0.6f;
        data.Windup = 0.6f;
        data.Radius = 140.0f;
        data.Range = 25000.0f;
        data.Speed = 2200.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 300, 450, 600 };
            float hp_scale[] = { 25.0f, 30.0f, 35.0f };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.55f * info.BonusAttackDamage;
            float hp_damage = hp_scale[info.SpellLevel] / 100.0f;
            hp_damage *= info.TargetMaxHealth - info.TargetHealth;
            return base_damage + bonus_damage + hp_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region KSante

        // KSante Q

        data = SkillshotData();
        data.ChampionName = "KSante";
        data.DisplayName = "Ntofo Strikes [Standard]";
        data.SkillshotName = "KSanteQ";
        data.IconName = "KSanteQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.UseWindup = true;
        data.Radius = 50.0f;
        data.Range = 465.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 100, 130, 160, 190 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.4f * info.BonusArmor;
            bonus_damage += 0.4f * info.BonusSpellBlock;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // KSante Q3

        data = SkillshotData();
        data.ChampionName = "KSante";
        data.DisplayName = "Ntofo Strikes [Empowered]";
        data.MissileName = "KSanteQ3Missile";
        data.SkillshotName = "KSanteQ3";
        data.IconName = "KSanteQ3.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.UseWindup = true;
        data.Radius = 70.0f;
        data.Range = 800.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 100, 130, 160, 190 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.4f * info.BonusArmor;
            bonus_damage += 0.4f * info.BonusSpellBlock;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kaisa

        // Kaisa W

        data = SkillshotData();
        data.ChampionName = "Kaisa";
        data.DisplayName = "Void Seeker";
        data.MissileName = "KaisaW";
        data.SkillshotName = "KaisaW";
        data.IconName = "KaisaW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 100.0f;
        data.Range = 3000.0f;
        data.Speed = 1750.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 55, 80, 105, 130 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.3f * info.TotalAttackDamage;
            bonus_damage += 0.45f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kalista

        // Kalista Q

        data = SkillshotData();
        data.ChampionName = "Kalista";
        data.DisplayName = "Pierce";
        data.MissileName = "KalistaMysticShotMisTrue";
        data.SkillshotName = "KalistaMysticShot";
        data.IconName = "KalistaMysticShot.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 40.0f;
        data.Range = 1200.0f;
        data.Speed = 2400.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 10, 75, 140, 205, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Kalista R

        data = SkillshotData();
        data.ChampionName = "Kalista";
        data.DisplayName = "Fate's Call";
        data.MissileName = "KalistaRMis";
        data.SkillshotName = "KalistaRMis";
        data.IconName = "KalistaRx.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Radius = 80.0f;
        data.Range = 1450.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 5;
        data.Collisions = { CollisionFlag::CHAMPION };
        data.Detectors = { DetectionType::ON_OBJECT_CREATED };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Karma

        // Karma Q

        data = SkillshotData();
        data.ChampionName = "Karma";
        data.DisplayName = "Inner Flame [Standard]";
        data.MissileName = "KarmaQMissile(?!Mantra)";
        data.SkillshotName = "KarmaQ";
        data.IconName = "KarmaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 950.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 110, 160, 210, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Karma RQ Missile

        data = SkillshotData();
        data.ChampionName = "Karma";
        data.DisplayName = "Inner Flame [Mantra]";
        data.MissileName = "KarmaQMissileMantra";
        data.SkillshotName = "KarmaQMissileMantra";
        data.IconName = "KarmaQ.png";
        data.Overrider = "KarmaQ";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Radius = 80.0f;
        data.Range = 950.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Karma RQ Explosion

        data = SkillshotData();
        data.ChampionName = "Karma";
        data.DisplayName = "Inner Flame [Explosion]";
        data.ParticleName = "_Q_impact_R_01";
        data.SkillshotName = "KarmaQExplosion";
        data.IconName = "KarmaQ.png";
        data.Overrider = "KarmaQExplosion";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.ExtraTime = 1.5f;
        data.Radius = 280.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 130, 220, 310 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Karthus

        // Karthus Q

        data = SkillshotData();
        data.ChampionName = "Karthus";
        data.DisplayName = "Lay Waste";
        data.ParticleName = "_Q_Ring";
        data.SkillshotName = "KarthusQ";
        data.IconName = "KarthusLayWasteA1.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.5f;
        data.ExtraTime = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 875.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 118, 156, 194, 232 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kassadin

        // Kassadin E

        data = SkillshotData();
        data.ChampionName = "Kassadin";
        data.DisplayName = "Force Pulse";
        data.SkillshotName = "ForcePulse";
        data.IconName = "ForcePulse.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.ConeAngle = 78.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Kassadin R

        data = SkillshotData();
        data.ChampionName = "Kassadin";
        data.DisplayName = "Rift Walk";
        data.SkillshotName = "RiftWalk";
        data.IconName = "RiftWalk.png";
        data.SkillshotSlot = 'R';
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 270.0f;
        data.Range = 500.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            API* api = API::Get();
            auto& caster = info.UnitData;
            uint32_t hash = api->FNV1A32("RiftWalk");

            float base_scale[] = { 70, 90, 110 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.02f * info.MaxMana;
            bonus_damage += 0.5f * info.TotalAbilityDamage;

            float extra_scale[] = { 35, 45, 55 };
            float extra_damage = extra_scale[info.SpellLevel];
            extra_damage += 0.01f * info.MaxMana;
            extra_damage += 0.1f * info.TotalAbilityDamage;
            extra_damage *= api->GetBuffCount(caster, hash);

            return base_damage + bonus_damage + extra_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kayle

        // Kayle Q

        data = SkillshotData();
        data.ChampionName = "Kayle";
        data.DisplayName = "Radiant Blast";
        data.MissileName = "KayleQMis";
        data.SkillshotName = "KayleQ";
        data.IconName = "KayleQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.UseWindup = true;
        data.Radius = 75.0f;
        data.Range = 900.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.6f * info.BonusAttackDamage;
            bonus_damage += 0.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kayn

        // Kayn Q

        data = SkillshotData();
        data.ChampionName = "Kayn";
        data.DisplayName = "Reaping Slash";
        data.SkillshotName = "KaynQ";
        data.IconName = "KaynQ.png";
        data.SkillshotSlot = 'Q';
        data.Delay = 0.15f;
        data.Radius = 350.0f;
        data.Range = 350.0f;
        data.Speed = 1050.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 200, 250, 300, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.7f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Kayn W Standard

        data = SkillshotData();
        data.ChampionName = "Kayn";
        data.DisplayName = "Blade's Reach [Standard]";
        data.ParticleName = ".*_W_cas$";
        data.SkillshotName = "KaynW";
        data.IconName = "KaynW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.HardCC = true;
        data.Delay = 0.55f;
        data.ParticleDelay = 0.55f;
        data.Radius = 80.0f;
        data.Range = 700.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 135, 180, 225, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Kayn W Assassin

        data = SkillshotData();
        data.ChampionName = "Kayn";
        data.DisplayName = "Blade's Reach [Assassin]";
        data.SkillshotName = "KaynAssW";
        data.IconName = "KaynW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.6f;
        data.Radius = 80.0f;
        data.Range = 900.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 135, 180, 225, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kennen

        // Kennen Q

        data = SkillshotData();
        data.ChampionName = "Kennen";
        data.DisplayName = "Shuriken Hurl";
        data.MissileName = "KennenShurikenHurlMissile1";
        data.SkillshotName = "KennenShurikenHurlMissile1";
        data.IconName = "KennenShurikenHurlMissile1.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.175f;
        data.Windup = 0.175f;
        data.Radius = 50.0f;
        data.Range = 950.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 125, 175, 225, 275 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Khazix

        // Khazix W Standard

        data = SkillshotData();
        data.ChampionName = "Khazix";
        data.DisplayName = "Void Spike [Standard]";
        data.MissileName = "KhazixWMissile";
        data.SkillshotName = "KhazixW";
        data.IconName = "KhazixW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1025.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 115, 145, 175, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Khazix W Long

        data = SkillshotData();
        data.ChampionName = "Khazix";
        data.DisplayName = "Void Spike [Threeway]";
        data.SkillshotName = "KhazixWLong";
        data.IconName = "KhazixW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1025.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 2;
        data.MultiAngle = 22;
        data.MultiCount = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 115, 145, 175, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Khazix E Standard

        data = SkillshotData();
        data.ChampionName = "Khazix";
        data.DisplayName = "Leap [Standard]";
        data.SkillshotName = "KhazixE";
        data.IconName = "KhazixE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Radius = 300.0f;
        data.Range = 700.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 100, 135, 170, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Khazix E Long

        data = SkillshotData();
        data.ChampionName = "Khazix";
        data.DisplayName = "Leap [Long]";
        data.SkillshotName = "KhazixELong";
        data.IconName = "KhazixE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Radius = 300.0f;
        data.Range = 900.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 100, 135, 170, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Kled

        // Kled Q

        data = SkillshotData();
        data.ChampionName = "Kled";
        data.DisplayName = "Beartrap on a Rope";
        data.MissileName = "KledQMissile";
        data.SkillshotName = "KledQ";
        data.IconName = "KledQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 45.0f;
        data.Range = 800.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 165, 240, 315, 390 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.95f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Kled E

        data = SkillshotData();
        data.ChampionName = "Kled";
        data.DisplayName = "Jousting";
        data.SkillshotName = "KledEDash";
        data.IconName = "KledE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.IgnoreAlive = true;
        data.Radius = 100.0f;
        data.Range = 550.0f;
        data.Speed = 945.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 35, 60, 85, 110, 135 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region KogMaw

        // KogMaw Q

        data = SkillshotData();
        data.ChampionName = "KogMaw";
        data.DisplayName = "Caustic Spittle";
        data.MissileName = "KogMawQ";
        data.SkillshotName = "KogMawQ";
        data.IconName = "KogMawQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.UseWindup = true;
        data.Radius = 70.0f;
        data.Range = 1200.0f;
        data.Speed = 1650.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // KogMaw E

        data = SkillshotData();
        data.ChampionName = "KogMaw";
        data.DisplayName = "Void Ooze";
        data.MissileName = "KogMawVoidOozeMissile";
        data.SkillshotName = "KogMawVoidOoze";
        data.IconName = "KogMawVoidOoze.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 120.0f;
        data.Range = 1360.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // KogMaw R

        data = SkillshotData();
        data.ChampionName = "KogMaw";
        data.DisplayName = "Living Artillery";
        data.ParticleName = "_R_cas_green";
        data.SkillshotName = "KogMawLivingArtillery";
        data.IconName = "KogMawLivingArtillery.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.Delay = 1.2f;
        data.ParticleDelay = 0.95f;
        data.Windup = 0.25f;
        data.Radius = 240.0f;
        data.Range = 1800.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 140, 180 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.75f * info.BonusAttackDamage;
            bonus_damage += 0.35f * info.TotalAbilityDamage;
            float missing_hp = info.TargetMaxHealth - info.TargetHealth;
            float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
            float damage_mod = MIN(0.833f * missing_hp_ratio, 0.50f);
            if (missing_hp_ratio >= 0.6f) { damage_mod = 1.0f; }
            return (base_damage + bonus_damage) * (1.0f + damage_mod);
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Leblanc

        // Leblanc W

        data = SkillshotData();
        data.ChampionName = "Leblanc";
        data.DisplayName = "Distortion [Standard]";
        data.SkillshotName = "LeblancW";
        data.IconName = "LeblancW.png";
        data.SkillshotSlot = 'W';
        data.Radius = 240.0f;
        data.Range = 600.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 115, 155, 195, 235 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Leblanc E

        data = SkillshotData();
        data.ChampionName = "Leblanc";
        data.DisplayName = "Ethereal Chains [Standard]";
        data.MissileName = "LeblancEMissile";
        data.SkillshotName = "LeblancE";
        data.IconName = "LeblancE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 950.0f;
        data.Speed = 1750.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 130, 190, 250, 310, 370 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Leblanc RW

        data = SkillshotData();
        data.ChampionName = "Leblanc";
        data.DisplayName = "Distortion [Mimic]";
        data.SkillshotName = "LeblancRW";
        data.IconName = "LeblancR.png";
        data.SkillshotSlot = 'R';
        data.Radius = 240.0f;
        data.Range = 600.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 300, 450 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Leblanc RE

        data = SkillshotData();
        data.ChampionName = "Leblanc";
        data.DisplayName = "Ethereal Chains [Mimic]";
        data.MissileName = "LeblancREMissile";
        data.SkillshotName = "LeblancRE";
        data.IconName = "LeblancR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 950.0f;
        data.Speed = 1750.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 210, 420, 630 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region LeeSin

        // LeeSin Q

        data = SkillshotData();
        data.ChampionName = "LeeSin";
        data.DisplayName = "Sonic Wave";
        data.MissileName = "LeeSinQOne";
        data.SkillshotName = "LeeSinQOne";
        data.IconName = "LeeSinQOne.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1200.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 55, 80, 105, 130, 155 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.15f * info.BonusAttackDamage;
            float missing_hp = info.TargetMaxHealth - info.TargetHealth;
            float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
            return (base_damage + bonus_damage) * (1.0f + missing_hp_ratio);
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // LeeSin R

        data = SkillshotData();
        data.ChampionName = "LeeSin";
        data.DisplayName = "Dragon's Rage";
        data.SkillshotName = "LeeSinR";
        data.IconName = "LeeSinR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = false;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Radius = 160.0f;
        data.Range = 800.0f;
        data.Speed = 865.0f;
        data.DangerLevel = 3;
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 175, 400, 625 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 2.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Leona

        // Leona E

        data = SkillshotData();
        data.ChampionName = "Leona";
        data.DisplayName = "Zenith Blade";
        data.MissileName = "LeonaZenithBladeMissile";
        data.SkillshotName = "LeonaZenithBlade";
        data.IconName = "LeonaZenithBlade.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 900.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 90, 130, 170, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Leona R Inner

        data = SkillshotData();
        data.ChampionName = "Leona";
        data.DisplayName = "Solar Flare [Inner]";
        data.SkillshotName = "LeonaREpicenter";
        data.IconName = "LeonaSolarFlare.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.9f;
        data.Radius = 175.0f;
        data.Range = 1200.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Exclusions =
        {
            "LeonaSolarFlare"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 225, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Leona R Outer

        data = SkillshotData();
        data.ChampionName = "Leona";
        data.DisplayName = "Solar Flare [Outer]";
        data.ParticleName = "_R_hit_aoe_";
        data.SkillshotName = "LeonaSolarFlare";
        data.IconName = "LeonaSolarFlare.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.9f;
        data.ParticleDelay = 0.65f;
        data.Windup = 0.25f;
        data.Radius = 325.0f;
        data.Range = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Exclusions =
        {
            "LeonaREpicenter"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 225, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Lillia

        // Lillia Q

        data = SkillshotData();
        data.ChampionName = "Lillia";
        data.DisplayName = "Blooming Blows";
        data.SkillshotName = "LilliaQ";
        data.IconName = "LilliaQ.png";
        data.SkillshotSlot = 'Q';
        data.FollowCaster = true;
        data.Delay = 0.25f;
        data.InnerRadius = 225.0f;
        data.OuterRadius = 485.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 90, 110, 130, 150 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MIXED;
        data.SkillshotType = SkillshotType::RING;
        this->skillshots[data.SkillshotName] = data;

        // Lillia W

        data = SkillshotData();
        data.ChampionName = "Lillia";
        data.DisplayName = "Watch Out! Eep!";
        data.ParticleName = "_W_AOE_indicator";
        data.SkillshotName = "LilliaW";
        data.IconName = "LilliaW.png";
        data.SkillshotSlot = 'W';
        data.FogSupport = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.Radius = 250.0f;
        data.Range = 500.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 240, 300, 360, 420, 480 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Lillia E Lob

        data = SkillshotData();
        data.ChampionName = "Lillia";
        data.DisplayName = "Swirlseed [Lob]";
        data.MissileName = "LilliaE(?!Rolling)";
        data.SkillshotName = "LilliaE";
        data.IconName = "LilliaE.png";
        data.SkillshotSlot = 'E';
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.4f;
        data.Radius = 150.0f;
        data.Range = 700.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Exclusions =
        {
            "LilliaERollingMissile"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Lillia E Roll

        data = SkillshotData();
        data.ChampionName = "Lillia";
        data.DisplayName = "Swirlseed [Roll]";
        data.MissileName = "LilliaERollingMissile(?!Short)";
        data.SkillshotName = "LilliaERollingMissile";
        data.Overrider = "LilliaERollingMissile";
        data.IconName = "LilliaE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsGlobal = true;
        data.SoftCC = true;
        data.Radius = 60.0f;
        data.Range = 12500.0f;
        data.Speed = 1150.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "LilliaE"
        };
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Lissandra

        // Lissandra Q Standard

        data = SkillshotData();
        data.ChampionName = "Lissandra";
        data.DisplayName = "Ice Shard [Initial]";
        data.MissileName = "LissandraQMissile";
        data.SkillshotName = "LissandraQ";
        data.IconName = "LissandraQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 75.0f;
        data.Range = 725.0f;
        data.Speed = 2200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 115, 150, 185, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Lissandra Q Shards

        data = SkillshotData();
        data.ChampionName = "Lissandra";
        data.DisplayName = "Ice Shard [Shards]";
        data.MissileName = "LissandraQShards";
        data.SkillshotName = "LissandraQShards";
        data.IconName = "LissandraQ.png";
        data.Overrider = "LissandraQ";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Radius = 90.0f;
        data.Range = 825.0f;
        data.Speed = 2200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 115, 150, 185, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Lissandra E

        data = SkillshotData();
        data.ChampionName = "Lissandra";
        data.DisplayName = "Glacial Path";
        data.MissileName = "LissandraEMissile";
        data.SkillshotName = "LissandraE";
        data.IconName = "LissandraE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 125.0f;
        data.Range = 1025.0f;
        data.Speed = 850.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Lucian

        // Lucian Q

        data = SkillshotData();
        data.ChampionName = "Lucian";
        data.DisplayName = "Piercing Light";
        data.SkillshotName = "LucianQ";
        data.IconName = "LucianQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 50.0f;
        data.Range = 1000.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 115, 145, 175, 205 };
            float ad_scale[] = { 60, 75, 90, 105, 120 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Lulu

        // Lulu Q

        data = SkillshotData();
        data.ChampionName = "Lulu";
        data.DisplayName = "Glitterlance";
        data.MissileName = "LuluQMissile";
        data.SkillshotName = "LuluQ";
        data.IconName = "LuluQ.png";
        data.Overrider = "LuluQ";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 950.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 95, 130, 165, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Lux

        // Lux Q

        data = SkillshotData();
        data.ChampionName = "Lux";
        data.DisplayName = "Light Binding";
        data.MissileName = "LuxLightBindingMis";
        data.SkillshotName = "LuxLightBinding";
        data.IconName = "LuxLightBinding.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1400.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Lux E Missile

        data = SkillshotData();
        data.ChampionName = "Lux";
        data.DisplayName = "Light Strike Kugel [Missile]";
        data.MissileName = "LuxLightStrikeKugel";
        data.SkillshotName = "LuxLightStrikeKugel";
        data.IconName = "LuxLightStrikeKugel.png";
        data.SkillshotSlot = 'E';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.ExtraTime = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 310.0f;
        data.Range = 1100.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 115, 165, 215, 265 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Lux E Explosion

        data = SkillshotData();
        data.ChampionName = "Lux";
        data.DisplayName = "Light Strike Kugel [Explosion]";
        data.ParticleName = "_E_tar_aoe_green";
        data.SkillshotName = "LuxAOEExplosion";
        data.IconName = "LuxLightStrikeKugel.png";
        data.Overrider = "LuxLightStrikeKugel";
        data.SkillshotSlot = 'E';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.ExtraTime = 5.25f;
        data.Radius = 310.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 115, 165, 215, 265 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Lux R

        data = SkillshotData();
        data.ChampionName = "Lux";
        data.DisplayName = "Malice Cannon";
        data.ParticleName = ".*_R_mis_beam$";
        data.SkillshotName = "LuxR";
        data.IconName = "LuxR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.IgnoreAlive = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.Delay = 1.0f;
        data.ParticleDelay = 1.0f;
        data.Windup = 1.0f;
        data.Radius = 100.0f;
        data.Range = 3500.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 300, 400, 500 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Malphite

        // Malphite E

        data = SkillshotData();
        data.ChampionName = "Malphite";
        data.DisplayName = "Ground Slam";
        data.SkillshotName = "Landslide";
        data.IconName = "Landslide.png";
        data.SkillshotSlot = 'E';
        data.SoftCC = true;
        data.Delay = 0.2419f;
        data.Windup = 0.2419f;
        data.Radius = 400.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.6f * info.TotalAbilityDamage;
            bonus_damage += 0.4f * info.Armor;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Malphite R

        data = SkillshotData();
        data.ChampionName = "Malphite";
        data.DisplayName = "Unstoppable Force";
        data.SkillshotName = "UFSlash";
        data.IconName = "UFSlash.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.HardCC = true;
        data.Radius = 325.0f;
        data.Range = 1000.0f;
        data.Speed = 1835.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Malzahar

        // Malzahar Q Left

        data = SkillshotData();
        data.ChampionName = "Malzahar";
        data.DisplayName = "Call of the Void [Left]";
        data.SkillshotName = "MalzaharQSecond";
        data.IconName = "MalzaharQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.SoftCC = true;
        data.Invert = true;
        data.Delay = 0.65f;
        data.Windup = 0.25f;
        data.Radius = 85.0f;
        data.Range = 900.0f;
        data.SideRange = 750.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Exclusions =
        {
            "MalzaharQ"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Malzahar Q Right

        data = SkillshotData();
        data.ChampionName = "Malzahar";
        data.DisplayName = "Call of the Void [Right]";
        data.SkillshotName = "MalzaharQ";
        data.IconName = "MalzaharQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.SoftCC = true;
        data.Rotate90 = true;
        data.Delay = 0.65f;
        data.Windup = 0.25f;
        data.Radius = 85.0f;
        data.Range = 900.0f;
        data.SideRange = 750.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Exclusions =
        {
            "MalzaharQSecond"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Maokai

        // Maokai Q

        data = SkillshotData();
        data.ChampionName = "Maokai";
        data.DisplayName = "Bramble Smash";
        data.MissileName = "MaokaiQMissile";
        data.SkillshotName = "MaokaiQ";
        data.IconName = "MaokaiQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 110.0f;
        data.Range = 600.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 110, 155, 200, 245 };
            float hp_scale[] = { 2.0f, 2.5f, 3.0f, 3.5f, 4.0f };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = hp_scale[info.SpellLevel];
            bonus_damage *= info.TargetMaxHealth / 100.0f;
            bonus_damage += 0.4f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Mel

        // Mel Q

        data = SkillshotData();
        data.ChampionName = "Mel";
        data.DisplayName = "Radiant Volley";
        data.MissileName = "MelQMissile1(?!0)";
        data.SkillshotName = "MelQ";
        data.IconName = "MelQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.ExtraTime = 0.7f;
        data.Radius = 250.0f;
        data.Range = 950.0f;
        data.Speed = 4500.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 78, 112, 152, 198, 250 };
            float ap_scale[] = { 51, 59.5f, 68, 76.5f, 85 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ap_scale[info.SpellLevel];
            bonus_damage *= info.TotalAbilityDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Mel E Orb

        data = SkillshotData();
        data.ChampionName = "Mel";
        data.DisplayName = "Solar Snare [Orb]";
        data.SkillshotName = "MelE";
        data.IconName = "MelE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 1050.0f;
        data.Offset = 100.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "MelEFieldTravel",
            "MelEFieldTail"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 105, 150, 195, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Mel E Field (Travel)

        data = SkillshotData();
        data.ChampionName = "Mel";
        data.DisplayName = "Solar Snare [Field, Travel]";
        data.SkillshotName = "MelEFieldTravel";
        data.IconName = "MelE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.675f;
        data.Radius = 250.0f;
        data.Range = 950.0f;
        data.Offset = 425.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "MelE",
            "MelEFieldTail"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 105, 150, 195, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Mel E Field (Tail)

        data = SkillshotData();
        data.ChampionName = "Mel";
        data.DisplayName = "Solar Snare [Field, Tail]";
        data.SkillshotName = "MelEFieldTail";
        data.IconName = "MelE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.SoftCC = true;
        data.Delay = 0.75f;
        data.Radius = 250.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "MelE",
            "MelEFieldTravel"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 105, 150, 195, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Milio

        // Milio Q Missile

        data = SkillshotData();
        data.ChampionName = "Milio";
        data.DisplayName = "Ultra Mega Fire Kick [Missile]";
        data.MissileName = "MilioQ";
        data.SkillshotName = "MilioQ";
        data.IconName = "MilioQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1200.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "MilioQHit"
        };
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 140, 200, 260, 320 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Milio Q Bounce

        data = SkillshotData();
        data.ChampionName = "Milio";
        data.DisplayName = "Ultra Mega Fire Kick [Bounce]";
        data.MissileName = "MilioQHit";
        data.SkillshotName = "MilioQHit";
        data.IconName = "MilioQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.8f;
        data.MissileDelay = 0.8f;
        data.Radius = 275.0f;
        data.Range = 12500.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Exclusions =
        {
            "MilioQ"
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 140, 200, 260, 320 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Mordekaiser

        // Mordekaiser Q1

        data = SkillshotData();
        data.ChampionName = "Mordekaiser";
        data.DisplayName = "Obliterate [P1]";
        data.SkillshotName = "MordekaiserQ";
        data.IconName = "MordekaiserQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 85.0f;
        data.Range = 275.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "MordekaiserQ2"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 110, 140, 170, 200 };
            float mod_scale[] = { 30, 35, 40, 45, 50 };
            float bonus_scale[] = { 0, 0, 0, 0, 0, 0, 0,
                0, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            float damage_mod = mod_scale[info.SpellLevel];
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            damage_mod = (1.0f + damage_mod / 100.0f);
            return (base_damage + bonus_damage) * damage_mod;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Mordekaiser Q2

        data = SkillshotData();
        data.ChampionName = "Mordekaiser";
        data.DisplayName = "Obliterate [P2]";
        data.SkillshotName = "MordekaiserQ2";
        data.IconName = "MordekaiserQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 125.0f;
        data.Range = 675.0f;
        data.Offset = 275.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "MordekaiserQ"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 110, 140, 170, 200 };
            float mod_scale[] = { 30, 35, 40, 45, 50 };
            float bonus_scale[] = { 0, 0, 0, 0, 0, 0, 0,
                0, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            float damage_mod = mod_scale[info.SpellLevel];
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            damage_mod = (1.0f + damage_mod / 100.0f);
            return (base_damage + bonus_damage) * damage_mod;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Mordekaiser E

        data = SkillshotData();
        data.ChampionName = "Mordekaiser";
        data.DisplayName = "Death's Grasp";
        data.SkillshotName = "MordekaiserE";
        data.IconName = "MordekaiserE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.Radius = 100.0f;
        data.Range = 700.0f;
        data.Speed = 3000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 75, 90, 105, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Morgana

        // Morgana Q

        data = SkillshotData();
        data.ChampionName = "Morgana";
        data.DisplayName = "Dark Binding";
        data.MissileName = "MorganaQ";
        data.SkillshotName = "MorganaQ";
        data.IconName = "MorganaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1300.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 135, 190, 245, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Naafiri

        // Naafiri Q

        data = SkillshotData();
        data.ChampionName = "Naafiri";
        data.DisplayName = "Darkin Daggers";
        data.MissileName = "NaafiriQ";
        data.SkillshotName = "NaafiriQ";
        data.IconName = "NaafiriQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 50.0f;
        data.Range = 900.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            API* api = API::Get();
            uint32_t hash = api->FNV1A32("NaafiriQBleed");

            float base_scale[] = { 35, 40, 45, 50, 55 };
            float bonus_scale[] = { 35, 60, 85, 110, 135 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            bonus_damage += 1.0f * info.BonusAttackDamage;
            
            float extra_damage = 0.0;
            if (api->HasBuff(api->GetHero(), hash))
            {
                float extra_scale[] = { 30, 45, 60, 75, 90 };
                extra_damage = base_scale[info.SpellLevel];
                extra_damage += 0.4f * info.BonusAttackDamage;
                float missing_hp = info.TargetMaxHealth - info.TargetHealth;
                float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
                extra_damage *= 1.0f + missing_hp_ratio;
            }
            return base_damage + bonus_damage + extra_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Naafiri E Dash

        data = SkillshotData();
        data.ChampionName = "Naafiri";
        data.DisplayName = "Eviscerate [Dash]";
        data.SkillshotName = "NaafiriE";
        data.IconName = "NaafiriE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Radius = 50.0f;
        data.Range = 350.0f;
        data.Speed = 950.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 15, 25, 35, 45, 55 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Naafiri E Flurry

        data = SkillshotData();
        data.ChampionName = "Naafiri";
        data.DisplayName = "Eviscerate [Flurry]";
        data.SkillshotName = "NaafiriEFlurry";
        data.IconName = "NaafiriE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.Radius = 225.0f;
        data.Range = 350.0f;
        data.Speed = 950.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nami

        // Nami Q

        data = SkillshotData();
        data.ChampionName = "Nami";
        data.DisplayName = "Aqua Prison";
        data.ParticleName = "_Q_indicator_green";
        data.SkillshotName = "NamiQ";
        data.IconName = "NamiQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 850.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 145, 200, 255, 310 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Nami R

        data = SkillshotData();
        data.ChampionName = "Nami";
        data.DisplayName = "Tidal Wave";
        data.MissileName = "NamiRMissile";
        data.SkillshotName = "NamiR";
        data.IconName = "NamiR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 250.0f;
        data.Range = 2750.0f;
        data.Speed = 850.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nautilus

        // Nautilus Q

        data = SkillshotData();
        data.ChampionName = "Nautilus";
        data.DisplayName = "Dredge Line";
        data.MissileName = "NautilusAnchorDragMissile";
        data.SkillshotName = "NautilusAnchorDrag";
        data.IconName = "NautilusAnchorDrag.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 90.0f;
        data.Range = 1150.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::TERRAIN_WALL,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 130, 175, 220, 265 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Neeko

        // Neeko Q

        data = SkillshotData();
        data.ChampionName = "Neeko";
        data.DisplayName = "Blooming Burst";
        data.MissileName = "NeekoQ";
        data.SkillshotName = "NeekoQ";
        data.IconName = "NeekoQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 250.0f;
        data.Range = 800.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 245, 340, 435, 530 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Neeko E

        data = SkillshotData(); // Check
        data.ChampionName = "Neeko";
        data.DisplayName = "Tangle-Barbs";
        data.MissileName = "NeekoE";
        data.SkillshotName = "NeekoE";
        data.IconName = "NeekoE.png";
        data.Overrider = "NeekoE";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 1000.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nidalee

        // Nidalee Q

        data = SkillshotData();
        data.ChampionName = "Nidalee";
        data.DisplayName = "Javelin Toss";
        data.MissileName = "JavelinToss";
        data.SkillshotName = "JavelinToss";
        data.IconName = "JavelinToss.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 40.0f;
        data.Range = 1500.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 210, 270, 330, 390, 450 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Nidalee W

        data = SkillshotData();
        data.ChampionName = "Nidalee";
        data.DisplayName = "Bushwhack";
        data.ParticleName = "_W_TC_";
        data.SkillshotName = "NidaleeWParticle";
        data.IconName = "Bushwhack.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.IsTrap = true;
        data.Delay = 1.35f;
        data.ParticleDelay = 1.35f;
        data.ExtraTime = 120.0f;
        data.Windup = 0.25f;
        data.Radius = 150.0f;
        data.Range = 900.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 80, 120, 160, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Nidalee E

        data = SkillshotData();
        data.ChampionName = "Nidalee";
        data.DisplayName = "Swipe";
        data.SkillshotName = "Swipe";
        data.IconName = "Swipe.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.ConeAngle = 180.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 310.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 130, 190, 250 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.7f * info.BonusAttackDamage;
            bonus_damage += 0.4f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nilah

        // Nilah Q

        data = SkillshotData();
        data.ChampionName = "Nilah";
        data.DisplayName = "Formless Blade";
        data.SkillshotName = "NilahQWrapper";
        data.IconName = "NilahQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FollowCaster = true;
        data.UseDirection = true;
        data.UseWindup = true;
        data.Radius = 75.0f;
        data.Range = 600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 10, 20, 30, 40, 50 };
            float ad_scale[] = { 180, 190, 200, 210, 220 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nocturne

        // Nocturne Q

        data = SkillshotData();
        data.ChampionName = "Nocturne";
        data.DisplayName = "Duskbringer";
        data.MissileName = "NocturneDuskbringer";
        data.SkillshotName = "NocturneDuskbringer";
        data.IconName = "NocturneDuskbringer.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1200.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 110, 155, 200, 245 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.85f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Nunu

        // Nunu W

        data = SkillshotData(); // Check
        data.ChampionName = "Nunu";
        data.DisplayName = "Biggest Snowball Ever!";
        data.MissileName = "NunuWSnowballMissile";
        data.SkillshotName = "NunuWSnowballMissile";
        data.IconName = "NunuW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Radius = 75.0f;
        data.Range = 1750.0f;
        data.Speed = 350.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 180, 225, 270, 315, 360 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Olaf

        // Olaf Q

        data = SkillshotData();
        data.ChampionName = "Olaf";
        data.DisplayName = "Undertow";
        data.MissileName = "OlafAxeThrow";
        data.SkillshotName = "OlafAxeThrowCast";
        data.IconName = "OlafAxeThrowCast.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 90.0f;
        data.Range = 1000.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 110, 160, 210, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Orianna

        // Orianna Q Ball

        data = SkillshotData();
        data.ChampionName = "Orianna";
        data.DisplayName = "Command: Attack [Ball]";
        data.MissileName = "OrianaIzuna";
        data.SkillshotName = "OrianaIzuna";
        data.IconName = "OrianaIzunaCommand.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Radius = 80.0f;
        data.Range = 12500.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "OrianaIzunaCenter"
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Orianna Q Destination

        data = SkillshotData();
        data.ChampionName = "Orianna";
        data.DisplayName = "Command: Attack [Destination]";
        data.SkillshotName = "OrianaIzunaCenter";
        data.IconName = "OrianaIzunaCommand.png";
        data.SkillshotSlot = 'Q';
        data.SoftCC = true;
        data.Radius = 175.0f;
        data.Range = 12500.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Exclusions =
        {
            "OrianaIzuna"
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Orianna R

        data = SkillshotData();
        data.ChampionName = "Orianna";
        data.DisplayName = "Command: Shockwave";
        data.ParticleName = "_R_VacuumIndicator";
        data.SkillshotName = "OrianaRParticle";
        data.IconName = "OrianaDetonateCommand.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.ParticleDelay = 0.5f;
        data.Radius = 415.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 250, 400, 550 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.95f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ornn

        // Ornn Q Rupture

        data = SkillshotData();
        data.ChampionName = "Ornn";
        data.DisplayName = "Volcanic Rupture [Rupture]";
        data.MissileName = "OrnnQ";
        data.SkillshotName = "OrnnQ";
        data.IconName = "OrnnQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.3f;
        data.Windup = 0.3f;
        data.Radius = 65.0f;
        data.Range = 750.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ornn Q Pillar

        data = SkillshotData();
        data.ChampionName = "Ornn";
        data.DisplayName = "Volcanic Rupture [Pillar]";
        data.SkillshotName = "OrnnQPillar";
        data.IconName = "OrnnQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 100.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Ornn E

        data = SkillshotData();
        data.ChampionName = "Ornn";
        data.DisplayName = "Searing Charge";
        data.SkillshotName = "OrnnE";
        data.IconName = "OrnnE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 150.0f;
        data.Range = 650.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.4f * info.BonusArmor;
            bonus_damage += 0.4f * info.BonusSpellBlock;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Ornn R

        data = SkillshotData();
        data.ChampionName = "Ornn";
        data.DisplayName = "Call of the Forge God";
        data.MissileName = "OrnnRWave2";
        data.SkillshotName = "OrnnRWave2";
        data.IconName = "OrnnR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Radius = 200.0f;
        data.Range = 5000.0f;
        data.Speed = 1650.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 125, 175, 225 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Pantheon

        // Pantheon Q Melee

        data = SkillshotData();
        data.ChampionName = "Pantheon";
        data.DisplayName = "Comet Spear [Melee]";
        data.SkillshotName = "PantheonQTap";
        data.IconName = "PantheonQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 600.0f;
        data.Offset = -40.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            if (info.TargetHealth / info.TargetMaxHealth > 0.2f)
            {
                float base_scale[] = { 70, 100, 130, 160, 190 };
                float base_damage = base_scale[info.SpellLevel];
                float bonus_damage = 1.15f * info.BonusAttackDamage;
                bonus_damage += 0.5f * info.TotalAbilityDamage;
                return base_damage + bonus_damage;
            }
            float base_scale[] = { 155, 230, 305, 380, 455 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 2.3f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Pantheon Q Range

        data = SkillshotData();
        data.ChampionName = "Pantheon";
        data.DisplayName = "Comet Spear [Range]";
        data.MissileName = "PantheonQMissile";
        data.SkillshotName = "PantheonQMissile";
        data.IconName = "PantheonQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 1200.0f;
        data.Speed = 2700.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            if (info.TargetHealth / info.TargetMaxHealth > 0.2f)
            {
                float base_scale[] = { 70, 100, 130, 160, 190 };
                float base_damage = base_scale[info.SpellLevel];
                return base_damage + 1.15f * info.BonusAttackDamage;
            }
            float base_scale[] = { 155, 230, 305, 380, 455 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 2.3f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Poppy

        // Poppy Q

        data = SkillshotData();
        data.ChampionName = "Poppy";
        data.DisplayName = "Hammer Shock";
        data.SkillshotName = "PoppyQ";
        data.IconName = "PoppyQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.332f;
        data.Windup = 0.332f;
        data.ExtraTime = 1.0f;
        data.Radius = 80.0f;
        data.Range = 400.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 110, 160, 210, 260 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 2.0f * info.BonusAttackDamage;
            bonus_damage += 0.18f * info.TargetMaxHealth;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Poppy R

        data = SkillshotData();
        data.ChampionName = "Poppy";
        data.DisplayName = "Keeper's Verdict";
        data.MissileName = "PoppyRMissile";
        data.SkillshotName = "PoppyRSpell";
        data.IconName = "PoppyR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 90.0f;
        data.Range = 1200.0f;
        data.Speed = 2500.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Pyke

        // Pyke Q

        data = SkillshotData();
        data.ChampionName = "Pyke";
        data.DisplayName = "Bone Skewer";
        data.MissileName = "PykeQRange";
        data.SkillshotName = "PykeQRange";
        data.IconName = "PykeQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.2f;
        data.Windup = 0.2f;
        data.Radius = 70.0f;
        data.Range = 1100.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 150, 200, 250, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.75f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Pyke E

        data = SkillshotData();
        data.ChampionName = "Pyke";
        data.DisplayName = "Phantom Undertow";
        data.SkillshotName = "PykeE";
        data.IconName = "PykeE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FastEvade = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.Radius = 110.0f;
        data.Range = 550.0f;
        data.Speed = 3000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 150, 200, 250, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Pyke R

        data = SkillshotData();
        data.ChampionName = "Pyke";
        data.DisplayName = "Death from Below";
        data.ParticleName = "_R_GroundIndicator";
        data.SkillshotName = "PykeR";
        data.IconName = "PykeR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.Delay = 0.5f;
        data.ParticleDelay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 80.0f;
        data.Range = 750.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 0, 0, 0, 0, 0, 0, 290, 330,
                370, 400, 430, 450, 470, 490, 510, 530, 540, 550 };
            float execution_damage = base_scale[info.UnitLevel];
            execution_damage += 0.8f * info.BonusAttackDamage;
            execution_damage += 1.5f * info.Lethality;
            return execution_damage >= info.TargetHealth
                ? 9999999.0f : 0.5f * execution_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Qiyana

        // Qiyana Q Standard

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Edge of Ixtal";
        data.SkillshotName = "QiyanaQ";
        data.IconName = "QiyanaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 525.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Qiyana Q Terrain

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Elemental Wrath [Terrain]";
        data.SkillshotName = "QiyanaQ_Rock";
        data.IconName = "QiyanaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 500.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.9f * info.BonusAttackDamage;
            float hp_ratio = info.TargetHealth / info.TargetMaxHealth;
            float bonus_damage_mod = hp_ratio < 0.5f ? 1.6f : 1.0f;
            return (base_damage + bonus_damage) * bonus_damage_mod;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Qiyana Q River

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Elemental Wrath [River]";
        data.SkillshotName = "QiyanaQ_Water";
        data.IconName = "QiyanaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 500.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Qiyana Q Grass

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Elemental Wrath [Grass]";
        data.SkillshotName = "QiyanaQ_Grass";
        data.IconName = "QiyanaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 500.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Qiyana Q Explosion

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Elemental Wrath [Explosion]";
        data.SkillshotName = "QiyanaQExplosion";
        data.IconName = "QiyanaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.5625f;
        data.Radius = 100.0f;
        data.Range = 925.0f;
        data.Offset = 500.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            API* api = API::Get();
            uint32_t hash = api->FNV1A32("QiyanaQ_Rock");

            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.9f * info.BonusAttackDamage;

            if (api->HasBuff(info.UnitData, hash))
            {
                float hp_ratio = info.TargetHealth / info.TargetMaxHealth;
                float bonus_damage_mod = hp_ratio < 0.5f ? 1.6f : 1.0f;
                return (base_damage + bonus_damage) * bonus_damage_mod;
            }
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Qiyana R

        data = SkillshotData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Supreme Display of Talent";
        data.MissileName = "QiyanaRMis";
        data.SkillshotName = "QiyanaR";
        data.IconName = "QiyanaR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 140.0f;
        data.Range = 950.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 200, 300 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.1f * info.TargetMaxHealth;
            bonus_damage += 1.25f * info.BonusAttackDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Quinn

        // Quinn Q

        data = SkillshotData();
        data.ChampionName = "Quinn";
        data.DisplayName = "Blinding Assault";
        data.MissileName = "QuinnQ";
        data.SkillshotName = "QuinnQ";
        data.IconName = "QuinnQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1050.0f;
        data.Speed = 1550.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 40, 60, 80, 100 };
            float ad_scale[] = { 80, 90, 100, 110, 120 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            bonus_damage += 0.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Rakan

        // Rakan Q

        data = SkillshotData();
        data.ChampionName = "Rakan";
        data.DisplayName = "Gleaming Quill";
        data.MissileName = "RakanQMis";
        data.SkillshotName = "RakanQ";
        data.IconName = "RakanQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 65.0f;
        data.Range = 900.0f;
        data.Speed = 1850.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Rakan W

        data = SkillshotData();
        data.ChampionName = "Rakan";
        data.DisplayName = "Grand Entrance";
        data.SkillshotName = "RakanW";
        data.IconName = "RakanW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Radius = 275.0f;
        data.Range = 650.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 120, 170, 220, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Rammus

        // Rammus R

        data = SkillshotData();
        data.ChampionName = "Rammus";
        data.DisplayName = "Soaring Slam";
        data.ParticleName = "_R_LandPositionIndicator";
        data.SkillshotName = "RammusRParticle";
        data.IconName = "Tremors2.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 0.83f;
        data.ParticleDelay = 0.83f;
        data.Radius = 400.0f;
        data.Range = 2000.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 175, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region RekSai

        // RekSai Q

        data = SkillshotData();
        data.ChampionName = "RekSai";
        data.DisplayName = "Prey Seeker";
        data.MissileName = "RekSaiQBurrowedMis";
        data.SkillshotName = "RekSaiQBurrowed";
        data.IconName = "RekSaiQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.125f;
        data.Windup = 0.125f;
        data.Radius = 65.0f;
        data.Range = 1650.0f;
        data.Speed = 1950.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 80, 110, 140, 170 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.25f * info.BonusAttackDamage;
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Rell

        // Rell Q

        data = SkillshotData();
        data.ChampionName = "Rell";
        data.DisplayName = "Shattering Strike";
        data.SkillshotName = "RellQ";
        data.IconName = "RellQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 75.0f;
        data.Range = 720.0f;
        data.Offset = -220.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Rell W Land

        data = SkillshotData();
        data.ChampionName = "Rell";
        data.DisplayName = "Ferromancy: Crash Down [Land]";
        data.SkillshotName = "RellW_Dismount";
        data.IconName = "RellW_Dismount.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.625f;
        data.Radius = 180.0f;
        data.Range = 450.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Rell W Dash

        data = SkillshotData();
        data.ChampionName = "Rell";
        data.DisplayName = "Ferromancy: Crash Down [Dash]";
        data.SkillshotName = "RellW2";
        data.IconName = "RellW_Dismount.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.625f;
        data.Radius = 200.0f;
        data.Range = 600.0f;
        data.Speed = 600.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Rell R

        data = SkillshotData();
        data.ChampionName = "Rell";
        data.DisplayName = "Magnet Storm";
        data.SkillshotName = "RellR";
        data.IconName = "RellR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FollowCaster = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 450.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 120, 200, 280 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.1f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Renata

        // Renata Q

        data = SkillshotData();
        data.ChampionName = "Renata";
        data.DisplayName = "Handshake";
        data.MissileName = "RenataQ";
        data.SkillshotName = "RenataQ";
        data.IconName = "RenataQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 900.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Renata E Missile

        data = SkillshotData();
        data.ChampionName = "Renata";
        data.DisplayName = "Loyalty Program [Missile]";
        data.MissileName = "RenataE";
        data.SkillshotName = "RenataE";
        data.IconName = "RenataE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 110.0f;
        data.Range = 800.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "RenataEExplosion"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 95, 125, 155, 185 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Renata E Explosion

        data = SkillshotData();
        data.ChampionName = "Renata";
        data.DisplayName = "Loyalty Program [Explosion]";
        data.SkillshotName = "RenataEExplosion";
        data.IconName = "RenataE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Radius = 225.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "RenataE"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 95, 125, 155, 185 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Renata R

        data = SkillshotData();
        data.ChampionName = "Renata";
        data.DisplayName = "Hostile Takeover";
        data.MissileName = "RenataRMissile";
        data.SkillshotName = "RenataR";
        data.IconName = "RenataR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 250.0f;
        data.Range = 2000.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 5;
        data.MultiAngle = 7;
        data.MultiCount = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Renekton

        // Renekton E

        data = SkillshotData();
        data.ChampionName = "Renekton";
        data.DisplayName = "Slice";
        data.SkillshotName = "RenektonSliceAndDice";
        data.IconName = "RenektonSliceAndDice.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.Radius = 100.0f;
        data.Range = 450.0f;
        data.Speed = 1115.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 70, 100, 130, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Rengar

        // Rengar E Standard

        data = SkillshotData();
        data.ChampionName = "Rengar";
        data.DisplayName = "Bola Strike [Standard]";
        data.MissileName = "RengarEMis";
        data.SkillshotName = "RengarE";
        data.IconName = "RengarE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1000.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 55, 100, 145, 190, 235 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Rengar E Empowered

        data = SkillshotData();
        data.ChampionName = "Rengar";
        data.DisplayName = "Bola Strike [Empowered]";
        data.MissileName = "RengarEEmpMis";
        data.SkillshotName = "RengarEEmp";
        data.IconName = "RengarE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1000.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_damage = 35.0f + 15.0f * (info.UnitLevel + 1);
            return base_damage + 0.8f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Riven

        // Riven R

        data = SkillshotData();
        data.ChampionName = "Riven";
        data.DisplayName = "Wind Slash";
        data.MissileName = "RivenWindslashMissile";
        data.SkillshotName = "RivenIzunaBlade";
        data.IconName = "RivenIzunaBlade.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 1100.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.MultiAngle = 9;
        data.MultiCount = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 150, 200 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.60f * info.BonusAttackDamage;
            float missing_hp = info.TargetMaxHealth - info.TargetHealth;
            float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
            float damage_mod = MIN(2.667f * missing_hp_ratio, 2.0f);
            return (base_damage + bonus_damage) * (1.0f + damage_mod);
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Rumble

        // Rumble E

        data = SkillshotData();
        data.ChampionName = "Rumble";
        data.DisplayName = "Electro Harpoon";
        data.MissileName = "RumbleGrenadeMissile";
        data.SkillshotName = "RumbleGrenade";
        data.IconName = "RumbleGrenade.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 950.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 110, 160, 210, 260, 310 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ryze

        // Ryze Q

        data = SkillshotData();
        data.ChampionName = "Ryze";
        data.DisplayName = "Overload";
        data.MissileName = "RyzeQ(?!Missile)";
        data.SkillshotName = "RyzeQWrapper";
        data.IconName = "RyzeQWrapper.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 55.0f;
        data.Range = 1000.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float scaling[] = { 75, 95, 115, 135, 155 };
            float base_damage = scaling[info.SpellLevel];
            float bonus_damage = 0.02f * info.BonusMana;
            bonus_damage += 0.55f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Samira

        // Samira Q

        data = SkillshotData();
        data.ChampionName = "Samira";
        data.DisplayName = "Flair";
        data.MissileName = "SamiraQGun";
        data.SkillshotName = "SamiraQGun";
        data.IconName = "SamiraQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.UseDirection = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 950.0f;
        data.Speed = 2600.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 0, 5, 10, 15, 20 };
            float ad_scale[] = { 95, 102.5f, 110, 117.5f, 125 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sejuani

        // Sejuani Q

        data = SkillshotData();
        data.ChampionName = "Sejuani";
        data.DisplayName = "Arctic Assault";
        data.SkillshotName = "SejuaniQ";
        data.IconName = "SejuaniQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 150.0f;
        data.Range = 650.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 140, 190, 240, 290 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Sejuani W

        data = SkillshotData();
        data.ChampionName = "Sejuani";
        data.DisplayName = "Winter's Wrath";
        data.SkillshotName = "SejuaniW";
        data.IconName = "SejuaniW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FollowCaster = true;
        data.SoftCC = true;
        data.Delay = 1.0f;
        data.Radius = 60.0f;
        data.Range = 600.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 5, 15, 25, 35, 45 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.04f * info.MaxHealth;
            bonus_damage += 0.2f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Sejuani R

        data = SkillshotData();
        data.ChampionName = "Sejuani";
        data.DisplayName = "Glacial Prison";
        data.MissileName = "SejuaniRMissile";
        data.SkillshotName = "SejuaniR";
        data.IconName = "SejuaniR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 120.0f;
        data.Range = 1300.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 400, 600, 800 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Senna

        // Senna Q

        data = SkillshotData();
        data.ChampionName = "Senna";
        data.DisplayName = "Piercing Darkness";
        data.SkillshotName = "SennaQCast";
        data.IconName = "SennaQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 50.0f;
        data.Range = 1400.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 60, 90, 120, 150 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Senna W

        data = SkillshotData();
        data.ChampionName = "Senna";
        data.DisplayName = "Last Embrace";
        data.MissileName = "SennaW";
        data.SkillshotName = "SennaW";
        data.IconName = "SennaW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1300.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Senna R

        data = SkillshotData();
        data.ChampionName = "Senna";
        data.DisplayName = "Dawning Shadow";
        data.MissileName = "SennaRWarningMis";
        data.SkillshotName = "SennaR";
        data.IconName = "SennaR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 1.0f;
        data.Windup = 1.0f;
        data.Radius = 160.0f;
        data.Range = 25000.0f;
        data.Speed = 20000.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 250, 400, 550 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.15f * info.BonusAttackDamage;
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Seraphine

        // Seraphine Q

        data = SkillshotData();
        data.ChampionName = "Seraphine";
        data.DisplayName = "High Note";
        data.MissileName = "SeraphineQInitialMissile";
        data.SkillshotName = "SeraphineQCast";
        data.IconName = "SeraphineQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.ExtraTime = 0.3f;
        data.Windup = 0.25f;
        data.Radius = 350.0f;
        data.Range = 900.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.5f * info.TotalAbilityDamage;
            float missing_hp = info.TargetMaxHealth - info.TargetHealth;
            float missing_hp_ratio = missing_hp / info.TargetMaxHealth;
            float damage_mod = MIN(0.75f * missing_hp_ratio, 0.75f);
            return (base_damage + bonus_damage) * (1.0f + damage_mod);
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Seraphine E

        data = SkillshotData();
        data.ChampionName = "Seraphine";
        data.DisplayName = "Beat Drop";
        data.MissileName = "SeraphineEMissile";
        data.SkillshotName = "SeraphineECast";
        data.IconName = "SeraphineE.png";
        data.Overrider = "SeraphineECast";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1300.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 100, 130, 160, 190 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Seraphine R

        data = SkillshotData();
        data.ChampionName = "Seraphine";
        data.DisplayName = "Encore";
        data.MissileName = "SeraphineR";
        data.SkillshotName = "SeraphineR";
        data.IconName = "SeraphineR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 160.0f;
        data.Range = 3500.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 200, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sett

        // Sett W Inner

        data = SkillshotData();
        data.ChampionName = "Sett";
        data.DisplayName = "Haymaker [Inner]";
        data.SkillshotName = "SettWInner";
        data.IconName = "SettW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.FixedRange = true;
        data.Delay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 100.0f;
        data.Range = 790.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Sett W Outer

        data = SkillshotData();
        data.ChampionName = "Sett";
        data.DisplayName = "Haymaker [Outer]";
        data.SkillshotName = "SettW";
        data.IconName = "SettW.png";
        data.SkillshotSlot = 'W';
        data.FixedRange = true;
        data.Delay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 400.0f;
        data.Range = 790.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        // Sett E

        data = SkillshotData();
        data.ChampionName = "Sett";
        data.DisplayName = "Facebreaker";
        data.SkillshotName = "SettE";
        data.IconName = "SettE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 140.0f;
        data.Range = 450.0f;
        data.Offset = -450.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 70, 90, 110, 130 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Shen

        // Shen E

        data = SkillshotData();
        data.ChampionName = "Shen";
        data.DisplayName = "Shadow Dash";
        data.SkillshotName = "ShenE";
        data.IconName = "ShenE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.HardCC = true;
        data.Radius = 60.0f;
        data.Range = 600.0f;
        data.Speed = 1150.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.15f * info.BonusHealth;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Shyvana

        // Shyvana E Standard

        data = SkillshotData();
        data.ChampionName = "Shyvana";
        data.DisplayName = "Flame Breath [Standard]";
        data.MissileName = "ShyvanaFireballMissile";
        data.SkillshotName = "ShyvanaFireball";
        data.IconName = "ShyvanaFireball.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 925.0f;
        data.Speed = 1575.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 125, 165, 205, 245 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.5f * info.BonusAttackDamage;
            bonus_damage += 0.7f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Shyvana E Dragon

        data = SkillshotData();
        data.ChampionName = "Shyvana";
        data.DisplayName = "Flame Breath [Dragon]";
        data.MissileName = "ShyvanaFireballDragonMissile";
        data.SkillshotName = "ShyvanaFireballDragon2";
        data.IconName = "ShyvanaFireball.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.FogSupport = true;
        data.Delay = 0.3333f;
        data.Windup = 0.3333f;
        data.Radius = 60.0f;
        data.Range = 975.0f;
        data.Speed = 1575.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "ShyvanaE2"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 125, 165, 205, 245 };
            float bonus_scale[] = { 75, 75, 75, 75, 75, 75, 80, 85,
                90, 95, 100, 105, 110, 115, 120, 125, 130, 135 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            bonus_damage += 1.0f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Shyvana E Explosion

        data = SkillshotData();
        data.ChampionName = "Shyvana";
        data.DisplayName = "Flame Breath [Explosion]";
        data.ParticleName = "_R_E_groundburn";
        data.SkillshotName = "ShyvanaE2";
        data.IconName = "ShyvanaFireball.png";
        data.Overrider = "ShyvanaE2";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.ExtraTime = 4.0f;
        data.Radius = 345.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "ShyvanaFireballDragon2"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 125, 165, 205, 245 };
            float bonus_scale[] = { 75, 75, 75, 75, 75, 75, 80, 85,
                90, 95, 100, 105, 110, 115, 120, 125, 130, 135 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            bonus_damage += 1.0f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Shyvana R

        data = SkillshotData();
        data.ChampionName = "Shyvana";
        data.DisplayName = "Transform Leap";
        data.SkillshotName = "ShyvanaTransformCast";
        data.IconName = "ShyvanaTransformCast.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 160.0f;
        data.Range = 850.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sion

        // Sion Q

        data = SkillshotData();
        data.ChampionName = "Sion";
        data.DisplayName = "Decimating Smash";
        data.SkillshotName = "SionQ";
        data.IconName = "SionQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.FixedRange = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.ExtraTime = 1.0f;
        data.Radius = 175.0f;
        data.Range = 820.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 155, 220, 285, 350 };
            float ad_scale[] = { 120, 150, 180, 210, 240 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        // Sion E

        data = SkillshotData();
        data.ChampionName = "Sion";
        data.DisplayName = "Roar of the Slayer";
        data.MissileName = "SionEMissile";
        data.SkillshotName = "SionE";
        data.IconName = "SionE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 80.0f;
        data.Range = 800.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 65, 100, 135, 170, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.55f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sivir

        // Sivir Q Standard

        data = SkillshotData();
        data.ChampionName = "Sivir";
        data.DisplayName = "Boomerang Blade [Initial]";
        data.MissileName = "SivirQMissile";
        data.SkillshotName = "SivirQ";
        data.IconName = "SivirQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.UseWindup = true;
        data.Radius = 90.0f;
        data.Range = 1250.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 85, 110, 135, 160 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.BonusAttackDamage;
            bonus_damage += 0.6f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Sivir Q Return

        data = SkillshotData();
        data.ChampionName = "Sivir";
        data.DisplayName = "Boomerang Blade [Return]";
        data.MissileName = "SivirQMissileReturn";
        data.SkillshotName = "SivirQMissileReturn";
        data.IconName = "SivirQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.BackToCaster = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 100.0f;
        data.Range = 12500.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 15, 30, 45, 60, 75 };
            float ad_scale[] = { 80, 85, 90, 95, 100 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.TotalAttackDamage / 100.0f;
            bonus_damage += 0.6f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Skarner

        // Skarner Q

        data = SkillshotData();
        data.ChampionName = "Skarner";
        data.DisplayName = "Upheaval";
        data.MissileName = "SkarnerQRockThrow";
        data.SkillshotName = "SkarnerQRockThrow";
        data.IconName = "SkarnerQRockThrow.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.UseWindup = true;
        data.Radius = 90.0f;
        data.Range = 1050.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 10, 20, 30, 40, 50 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.03f * info.BonusHealth;
            bonus_damage += 0.08f * info.TargetMaxHealth;
            bonus_damage += 0.8f * info.BonusAttackDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Skarner R

        data = SkillshotData();
        data.ChampionName = "Skarner";
        data.DisplayName = "Impale";
        data.SkillshotName = "SkarnerR";
        data.IconName = "SkarnerR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.65f;
        data.Windup = 0.65f;
        data.Radius = 175.0f;
        data.Range = 675.0f;
        data.Offset = -50.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Smolder

        // Smolder W1

        data = SkillshotData();
        data.ChampionName = "Smolder";
        data.DisplayName = "Achooo! [Initial]";
        data.MissileName = "SmolderW";
        data.SkillshotName = "SmolderW";
        data.IconName = "SmolderW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.35f;
        data.Windup = 0.35f;
        data.Radius = 115.0f;
        data.Range = 1050.0f;
        data.Speed = 1400.0f;
        data.Acceleration = -1600.0f;
        data.InitialSpeed = 2000.0f;
        data.MinSpeed = 400.0f;
        data.MaxSpeed = 2000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Smolder W2

        data = SkillshotData();
        data.ChampionName = "Smolder";
        data.DisplayName = "Achooo! [Slowed]";
        data.SkillshotName = "SmolderW2";
        data.IconName = "SmolderW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 125.0f;
        data.Range = 450.0f;
        data.Speed = 400.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Smolder R

        data = SkillshotData();
        data.ChampionName = "Smolder";
        data.DisplayName = "MMOOOMMMM!";
        data.MissileName = "SmolderRMomMissile(?!Sweet)";
        data.SkillshotName = "SmolderR";
        data.IconName = "SmolderR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.Windup = 0.25f;
        data.Radius = 240.0f;
        data.Range = 3650.0f;
        data.Offset = -600.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 300, 400 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.1f * info.BonusAttackDamage;
            bonus_damage += 1.0f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sona

        // Sona R

        data = SkillshotData();
        data.ChampionName = "Sona";
        data.DisplayName = "Crescendo";
        data.MissileName = "SonaRMissile";
        data.SkillshotName = "SonaR";
        data.IconName = "SonaR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 140.0f;
        data.Range = 1000.0f;
        data.Speed = 2400.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Soraka

        // Soraka Q

        data = SkillshotData();
        data.ChampionName = "Soraka";
        data.DisplayName = "Starcall";
        data.MissileName = "SorakaQMissile";
        data.SkillshotName = "SorakaQ";
        data.IconName = "SorakaQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 265.0f;
        data.Range = 800.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 120, 155, 190, 225 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.35f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Swain

        // Swain Q

        data = SkillshotData();
        data.ChampionName = "Swain";
        data.DisplayName = "Death's Hand";
        data.SkillshotName = "SwainQ";
        data.IconName = "SwainQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.ConeAngle = 40.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 750.0f;
        data.Speed = 5000.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 120, 180, 240, 300, 360 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Swain W

        data = SkillshotData();
        data.ChampionName = "Swain";
        data.DisplayName = "Vision of Empire";
        data.ParticleName = "_W_AOE_";
        data.SkillshotName = "SwainW";
        data.IconName = "SwainW.png";
        data.SkillshotSlot = 'W';
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 1.5f;
        data.ParticleDelay = 1.5f;
        data.Windup = 0.25f;
        data.Radius = 325.0f;
        data.Range = 7500.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Swain E Standard

        data = SkillshotData();
        data.ChampionName = "Swain";
        data.DisplayName = "Nevermove [Initial]";
        data.MissileName = "SwainE";
        data.SkillshotName = "SwainE";
        data.IconName = "SwainE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 90.0f;
        data.Range = 950.0f;
        data.Speed = 1125.0f;
        data.Acceleration = 1125.0f;
        data.InitialSpeed = 1125.0f;
        data.MinSpeed = 1125.0f;
        data.MaxSpeed = 1800.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "SwainEDetonation"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Swain E Return

        data = SkillshotData();
        data.ChampionName = "Swain";
        data.DisplayName = "Nevermove [Return]";
        data.MissileName = "SwainEReturnMissile";
        data.SkillshotName = "SwainEReturnMissile";
        data.IconName = "SwainE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.BackToCaster = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Radius = 90.0f;
        data.Range = 12500.0f;
        data.Speed = 2000.0f;
        data.Acceleration = 2000.0f;
        data.InitialSpeed = 2000.0f;
        data.MinSpeed = 2000.0f;
        data.MaxSpeed = 2800.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Swain E Detonation

        data = SkillshotData();
        data.ChampionName = "Swain";
        data.DisplayName = "Nevermove [Detonation]";
        data.SkillshotName = "SwainEDetonation";
        data.IconName = "SwainE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 100.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "SwainE"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Sylas

        // Sylas Q Explosion

        data = SkillshotData();
        data.ChampionName = "Sylas";
        data.DisplayName = "Chain Lash [Explosion]";
        data.SkillshotName = "SylasQ";
        data.IconName = "SylasQ.png";
        data.SkillshotSlot = 'Q';
        data.Dangerous = true;
        data.SoftCC = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.ExtraTime = 0.6f;
        data.Radius = 180.0f;
        data.Range = 725.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 175, 250, 325, 400 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Sylas Q Chains

        data = SkillshotData();
        data.ChampionName = "Sylas";
        data.DisplayName = "Chain Lash [Chains]";
        data.SkillshotName = "SylasQ2";
        data.IconName = "SylasQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 70.0f;
        data.Range = 725.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 40, 60, 80, 100, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Sylas E

        data = SkillshotData();
        data.ChampionName = "Sylas";
        data.DisplayName = "Abduct";
        data.MissileName = "SylasE2Mis";
        data.SkillshotName = "SylasE2";
        data.IconName = "SylasE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 850.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 130, 180, 230, 280 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Syndra

        // Syndra Q

        data = SkillshotData();
        data.ChampionName = "Syndra";
        data.DisplayName = "Dark Sphere";
        data.ParticleName = "_Q_2021_aoe_gather";
        data.SkillshotName = "SyndraQ";
        data.IconName = "SyndraQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.Delay = 0.6f;
        data.ParticleDelay = 0.6f;
        data.Radius = 210.0f;
        data.Range = 800.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 110, 145, 180, 215 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Syndra W

        data = SkillshotData();
        data.ChampionName = "Syndra";
        data.DisplayName = "Force of Will";
        data.SkillshotName = "SyndraWCast";
        data.IconName = "SyndraW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Radius = 225.0f;
        data.Range = 950.0f;
        data.Speed = 1450.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Syndra E

        data = SkillshotData();
        data.ChampionName = "Syndra";
        data.DisplayName = "Scatter the Weak [Cone]";
        data.SkillshotName = "SyndraE";
        data.IconName = "SyndraE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.ConeAngle = 56.0f;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Range = 800.0f;
        data.Speed = 2500.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "SyndraESphereMissile"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 115, 155, 195, 235 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Syndra QE

        data = SkillshotData();
        data.ChampionName = "Syndra";
        data.DisplayName = "Scatter the Weak [Sphere]";
        data.MissileName = "SyndraESphereMissile";
        data.SkillshotName = "SyndraESphereMissile";
        data.IconName = "SyndraQ.png";
        data.Overrider = "SyndraESphereMissile";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Radius = 100.0f;
        data.Range = 950.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Exclusions =
        {
            "SyndraE"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 115, 155, 195, 235 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region TahmKench

        // TahmKench Q

        data = SkillshotData();
        data.ChampionName = "TahmKench";
        data.DisplayName = "Tongue Lash";
        data.MissileName = "TahmKenchQMissile";
        data.SkillshotName = "TahmKenchQ";
        data.IconName = "TahmKenchQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 900.0f;
        data.Speed = 2800.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 120, 165, 210, 255 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.TotalAbilityDamage;
            float extra_damage = 6 + 42 / 17.0f * info.UnitLevel;
            extra_damage += 0.015f * info.TotalAbilityDamage;
            extra_damage *= info.BonusHealth / 100.0f;
            extra_damage += 0.04f * info.BonusHealth;
            return base_damage + bonus_damage + extra_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // TahmKench W

        data = SkillshotData();
        data.ChampionName = "TahmKench";
        data.DisplayName = "Abyssal Dive";
        data.ParticleName = "_W_ImpactWarning";
        data.SkillshotName = "TahmKenchWParticle";
        data.IconName = "TahmKenchW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.5f;
        data.Windup = 0.75f;
        data.Radius = 275.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 135, 170, 205, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Taliyah

        // Taliyah Q

        data = SkillshotData();
        data.ChampionName = "Taliyah";
        data.DisplayName = "Threaded Volley";
        data.MissileName = "TaliyahQMis";
        data.SkillshotName = "TaliyahQ";
        data.IconName = "TaliyahQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 100.0f;
        data.Range = 1000.0f;
        data.Speed = 3600.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 56, 74.5f, 93, 111.5f, 130 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Taliyah W

        data = SkillshotData();
        data.ChampionName = "Taliyah";
        data.DisplayName = "Seismic Shove";
        data.ParticleName = "_W_indicator_";
        data.SkillshotName = "TaliyahWVC";
        data.IconName = "TaliyahWVC.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 0.8f;
        data.ParticleDelay = 0.8f;
        data.Windup = 0.25f;
        data.Radius = 225.0f;
        data.Range = 900.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Talon

        // Talon W

        data = SkillshotData();
        data.ChampionName = "Talon";
        data.DisplayName = "Rake";
        data.SkillshotName = "TalonW";
        data.IconName = "TalonW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 75.0f;
        data.Range = 825.0f;
        data.Speed = 3000.0f;
        data.DangerLevel = 2;
        data.MultiAngle = 11;
        data.MultiCount = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 60, 70, 80, 90 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.4f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Taric

        // Taric E

        data = SkillshotData();
        data.ChampionName = "Taric";
        data.DisplayName = "Dazzle";
        data.SkillshotName = "TaricE";
        data.IconName = "TaricE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FollowCaster = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.Radius = 70.0f;
        data.Range = 610.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 130, 170, 210, 250 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.5f * info.BonusArmor;
            bonus_damage += 0.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Teemo

        // Teemo R

        data = SkillshotData();
        data.ChampionName = "Teemo";
        data.DisplayName = "Noxious Trap";
        data.ParticleName = "_R_CollisionBox_Ring";
        data.SkillshotName = "TeemoRParticle";
        data.IconName = "TeemoR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.IsTrap = true;
        data.Delay = 0.65f;
        data.ParticleDelay = 0.65f;
        data.ExtraTime = 300.0f;
        data.Windup = 0.25f;
        data.Radius = 160.0f;
        data.Range = 900.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED,
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 200, 325, 450 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Thresh

        // Thresh Q

        data = SkillshotData();
        data.ChampionName = "Thresh";
        data.DisplayName = "Death Sentence";
        data.MissileName = "ThreshQMissile";
        data.SkillshotName = "ThreshQ";
        data.IconName = "ThreshQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 70.0f;
        data.Range = 1100.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 155, 210, 265, 320 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Thresh E

        data = SkillshotData();
        data.ChampionName = "Thresh";
        data.DisplayName = "Flay";
        data.SkillshotName = "ThreshE";
        data.IconName = "ThreshE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Radius = 110.0f;
        data.Range = 525.0f;
        data.Offset = -550.0f;
        data.Speed = 2000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 120, 165, 210, 255 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Tristana

        // Tristana W

        data = SkillshotData();
        data.ChampionName = "Tristana";
        data.DisplayName = "Rocket Jump";
        data.SkillshotName = "TristanaW";
        data.IconName = "TristanaW.png";
        data.SkillshotSlot = 'W';
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 350.0f;
        data.Range = 900.0f;
        data.Speed = 1100.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.0f * info.BonusAttackDamage;
            bonus_damage += 0.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Tryndamere

        // Tryndamere E

        data = SkillshotData();
        data.ChampionName = "Tryndamere";
        data.DisplayName = "Spinning Slash";
        data.SkillshotName = "TryndamereE";
        data.IconName = "TryndamereE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FixedRange = true;
        data.Radius = 225.0f;
        data.Range = 660.0f;
        data.Speed = 910.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 105, 135, 165, 195 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.3f * info.BonusAttackDamage;
            bonus_damage += 0.8f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region TwistedFate

        // TwistedFate Q

        data = SkillshotData();
        data.ChampionName = "TwistedFate";
        data.DisplayName = "Wild Cards";
        data.MissileName = "SealFateMissile";
        data.SkillshotName = "WildCards";
        data.IconName = "WildCards.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 40.0f;
        data.Range = 1450.0f;
        data.Speed = 1000.0f;
        data.DangerLevel = 1;
        data.MultiAngle = 28;
        data.MultiCount = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 105, 150, 195, 240 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.5f * info.BonusAttackDamage;
            bonus_damage += 0.85f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Urgot

        // Urgot Q

        data = SkillshotData();
        data.ChampionName = "Urgot";
        data.DisplayName = "Corrosive Charge";
        data.ParticleName = "_Q_Indicator_Ring";
        data.SkillshotName = "UrgotQ";
        data.IconName = "UrgotQ.png";
        data.SkillshotSlot = 'Q';
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.4f;
        data.ParticleDelay = 0.4f;
        data.Windup = 0.4f;
        data.Radius = 210.0f;
        data.Range = 800.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 25, 70, 115, 160, 205 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Urgot E

        data = SkillshotData();
        data.ChampionName = "Urgot";
        data.DisplayName = "Disdain";
        data.SkillshotName = "UrgotE";
        data.IconName = "UrgotE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.45f;
        data.Windup = 0.45f;
        data.Radius = 100.0f;
        data.Range = 450.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 120, 150, 180, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Urgot R

        data = SkillshotData();
        data.ChampionName = "Urgot";
        data.DisplayName = "Fear Beyond Death";
        data.MissileName = "UrgotR";
        data.SkillshotName = "UrgotR";
        data.IconName = "UrgotR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 80.0f;
        data.Range = 2500.0f;
        data.Speed = 3200.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::CHAMPION
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 100, 225, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Varus

        // Varus Q

        data = SkillshotData();
        data.ChampionName = "Varus";
        data.DisplayName = "Piercing Arrow";
        data.MissileName = "VarusQMissile";
        data.SkillshotName = "VarusQMissile";
        data.IconName = "VarusQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 75.0f;
        data.Range = 1595.0f;
        data.Speed = 1900.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 160, 230, 300, 370 };
            float ad_scale[] = { 130, 140, 150, 160, 170 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ad_scale[info.SpellLevel];
            bonus_damage *= info.BonusAttackDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Varus E

        data = SkillshotData();
        data.ChampionName = "Varus";
        data.DisplayName = "Hail of Arrows";
        data.SkillshotName = "VarusE";
        data.IconName = "VarusE.png";
        data.SkillshotSlot = 'E';
        data.SoftCC = true;
        data.Delay = 0.77f;
        data.Windup = 0.2419f;
        data.Radius = 300.0f;
        data.Range = 925.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Varus R

        data = SkillshotData();
        data.ChampionName = "Varus";
        data.DisplayName = "Chain of Corruption";
        data.MissileName = "VarusRMissile";
        data.SkillshotName = "VarusR";
        data.IconName = "VarusR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.2419f;
        data.Windup = 0.2419f;
        data.Radius = 120.0f;
        data.Range = 1370.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Veigar

        // Veigar Q

        data = SkillshotData();
        data.ChampionName = "Veigar";
        data.DisplayName = "Baleful Strike";
        data.MissileName = "VeigarBalefulStrikeMis";
        data.SkillshotName = "VeigarBalefulStrike";
        data.IconName = "VeigarBalefulStrike.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1050.0f;
        data.Speed = 2200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float ap_scale[] = { 50, 55, 60, 65, 70 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ap_scale[info.SpellLevel];
            bonus_damage *= info.TotalAbilityDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Veigar W

        data = SkillshotData();
        data.ChampionName = "Veigar";
        data.DisplayName = "Dark Matter";
        data.ParticleName = "_W_cas_green";
        data.SkillshotName = "VeigarDarkMatter";
        data.IconName = "VeigarDarkMatter.png";
        data.SkillshotSlot = 'W';
        data.FogSupport = true;
        data.Delay = 1.2f;
        data.ParticleDelay = 1.2f;
        data.Windup = 0.25f;
        data.Radius = 240.0f;
        data.Range = 950.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 140, 195, 250, 305 };
            float ap_scale[] = { 60, 70, 80, 90, 100 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ap_scale[info.SpellLevel];
            bonus_damage *= info.TotalAbilityDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Veigar E Inner

        data = SkillshotData();
        data.ChampionName = "Veigar";
        data.DisplayName = "Event Horizon [Inner]";
        data.SkillshotName = "VeigarEventHorizon";
        data.IconName = "VeigarEventHorizon.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ExtraTime = 3.0f;
        data.Windup = 0.25f;
        data.Radius = 290.1f;
        data.Range = 725.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Veigar E Outer

        data = SkillshotData();
        data.ChampionName = "Veigar";
        data.DisplayName = "Event Horizon [Outer]";
        data.SkillshotName = "VeigarCage";
        data.IconName = "VeigarEventHorizon.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ExtraTime = 3.0f;
        data.Windup = 0.25f;
        data.InnerRadius = 290.0f;
        data.OuterRadius = 410.0f;
        data.Range = 725.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::RING;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Velkoz

        // Velkoz Q Standard

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Plasma Fission [Initial]";
        data.MissileName = "VelkozQMissile(?!Split)";
        data.SkillshotName = "VelkozQ";
        data.IconName = "VelkozQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 50.0f;
        data.Range = 1200.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Velkoz Q Implosion

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Plasma Fission [Implosion]";
        data.ParticleName = "_Q_SplitImplosion";
        data.SkillshotName = "VelkozQImplosion";
        data.IconName = "VelkozQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.SkipAncestor = true;
        data.SoftCC = true;
        data.Radius = 45.0f;
        data.Range = 1100.0f;
        data.Speed = 2100.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Velkoz Q Split

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Plasma Fission [Split]";
        data.MissileName = "VelkozQMissileSplit";
        data.SkillshotName = "VelkozQSplit";
        data.IconName = "VelkozQ.png";
        data.Overrider = "VelkozQSplit";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.Radius = 45.0f;
        data.Range = 1100.0f;
        data.Speed = 2100.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Velkoz W Missile

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Void Rift [Missile]";
        data.MissileName = "VelkozWMissile";
        data.SkillshotName = "VelkozWMissile";
        data.IconName = "VelkozW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.Radius = 87.5f;
        data.Range = 1200.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 50, 70, 90, 110 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Velkoz W Eruption

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Void Rift [Eruption]";
        data.SkillshotName = "VelkozWEruption";
        data.IconName = "VelkozW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Delay = 0.1f;
        data.Radius = 87.5f;
        data.Range = 1200.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 75, 105, 135, 165 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.25f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Velkoz E

        data = SkillshotData();
        data.ChampionName = "Velkoz";
        data.DisplayName = "Tectonic Disruption";
        data.ParticleName = "_E_AOE_";
        data.SkillshotName = "VelkozE";
        data.IconName = "VelkozE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.FastEvade = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.ParticleDelay = 0.55f;
        data.Windup = 0.25f;
        data.Radius = 225.0f;
        data.Range = 800.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 100, 130, 160, 190 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.3f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Vex

        // Vex Q1

        data = SkillshotData();
        data.ChampionName = "Vex";
        data.DisplayName = "Mistral Bolt [Initial]";
        data.SkillshotName = "VexQ";
        data.IconName = "VexQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Delay = 0.15f;
        data.Windup = 0.15f;
        data.Radius = 180.0f;
        data.Range = 500.0f;
        data.Speed = 700.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "VexQAccelerated"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Vex Q2

        data = SkillshotData();
        data.ChampionName = "Vex";
        data.DisplayName = "Mistral Bolt [Accelerated]";
        data.SkillshotName = "VexQAccelerated";
        data.IconName = "VexQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.Radius = 80.0f;
        data.Range = 700.0f;
        data.Speed = 3200.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "VexQ"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 115, 160, 205, 250 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Vex E

        data = SkillshotData();
        data.ChampionName = "Vex";
        data.DisplayName = "Looming Darkness";
        data.MissileName = "VexE";
        data.SkillshotName = "VexE";
        data.IconName = "VexE.png";
        data.SkillshotSlot = 'E';
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 200.0f;
        data.Range = 800.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 70, 90, 110, 130 };
            float ap_scale[] = { 40, 45, 50, 55, 60 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = ap_scale[info.SpellLevel];
            bonus_damage *= info.TotalAbilityDamage / 100.0f;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Vex R

        data = SkillshotData();
        data.ChampionName = "Vex";
        data.DisplayName = "Shadow Surge";
        data.MissileName = "VexR";
        data.SkillshotName = "VexR";
        data.IconName = "VexR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 130.0f;
        data.Range = 3000.0f;
        data.Speed = 1600.0f;
        data.DangerLevel = 4;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 225, 375, 525 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Vi

        // Vi Q

        data = SkillshotData();
        data.ChampionName = "Vi";
        data.DisplayName = "Vault Breaker";
        data.SkillshotName = "ViQDash";
        data.IconName = "ViQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Radius = 55.0f;
        data.Range = 725.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_NEW_PATH
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 90, 140, 190, 240, 290 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.6f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Viego

        // Viego Q

        data = SkillshotData();
        data.ChampionName = "Viego";
        data.DisplayName = "Blade of the Ruined King";
        data.SkillshotName = "ViegoQ";
        data.IconName = "ViegoQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 62.5f;
        data.Range = 600.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 60, 90, 120, 150 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Viego W

        data = SkillshotData();
        data.ChampionName = "Viego";
        data.DisplayName = "Spectral Maw";
        data.MissileName = "ViegoWMis";
        data.SkillshotName = "ViegoWMis";
        data.IconName = "ViegoW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Radius = 60.0f;
        data.Range = 900.0f;
        data.Speed = 1300.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 135, 190, 245, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Viego R

        data = SkillshotData();
        data.ChampionName = "Viego";
        data.DisplayName = "Heartbreaker";
        data.ParticleName = "_R_Tell";
        data.SkillshotName = "ViegoR";
        data.IconName = "ViegoR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.ParticleDelay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 300.0f;
        data.Range = 500.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float hp_scale[] = { 12.0f, 16.0f, 20.0f };
            float damage = hp_scale[info.SpellLevel] / 100.0f;
            damage += (0.05f * info.BonusAttackDamage / 100.0f);
            return damage * (info.TargetMaxHealth - info.TargetHealth);
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Viktor

        // Viktor W

        data = SkillshotData();
        data.ChampionName = "Viktor";
        data.DisplayName = "Graviton Field";
        data.ParticleName = "_W_AOE_green";
        data.SkillshotName = "ViktorWParticle";
        data.IconName = "ViktorW.png";
        data.SkillshotSlot = 'W';
        data.Exception = true;
        data.FastEvade = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.ExtraTime = 3.75f;
        data.Windup = 0.25f;
        data.Radius = 300.0f;
        data.Range = 800.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Viktor E Standard

        data = SkillshotData();
        data.ChampionName = "Viktor";
        data.DisplayName = "Death Ray [Standard]";
        data.MissileName = "ViktorEMissile(?!2)";
        data.SkillshotName = "ViktorEMissile";
        data.IconName = "ViktorE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 80.0f;
        data.Range = 600.0f;
        data.Speed = 1050.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Viktor E Evolved

        data = SkillshotData();
        data.ChampionName = "Viktor";
        data.DisplayName = "Death Ray [Evolved]";
        data.MissileName = "ViktorEAugMissile";
        data.SkillshotName = "ViktorEAugMissile";
        data.IconName = "ViktorE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 80.0f;
        data.Range = 600.0f;
        data.Speed = 1050.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Viktor E Aftershock

        data = SkillshotData();
        data.ChampionName = "Viktor";
        data.DisplayName = "Death Ray [Aftershock]";
        data.MissileName = "ViktorEMissile2";
        data.SkillshotName = "ViktorEMissile2";
        data.IconName = "ViktorE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 80.0f;
        data.Range = 600.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 50, 80, 110, 140 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Vladimir

        // Vladimir R

        data = SkillshotData();
        data.ChampionName = "Vladimir";
        data.DisplayName = "Hemoplague";
        data.SkillshotName = "VladimirHemoplague";
        data.IconName = "VladimirHemoplague.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.Delay = 0.15f;
        data.Radius = 375.0f;
        data.Range = 625.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 150, 250, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Volibear

        // Volibear E

        data = SkillshotData();
        data.ChampionName = "Volibear";
        data.DisplayName = "Sky Splitter";
        data.ParticleName = "_E_AOE_Warning";
        data.SkillshotName = "VolibearE";
        data.IconName = "VolibearE.png";
        data.SkillshotSlot = 'E';
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.SoftCC = true;
        data.Delay = 2.0f;
        data.ParticleDelay = 2.0f;
        data.Radius = 325.0f;
        data.Range = 1200.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 110, 140, 170, 200 };
            float hp_scale[] = { 11, 12, 13, 14, 15 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = hp_scale[info.SpellLevel];
            bonus_damage *= info.TargetMaxHealth / 100.0f;
            bonus_damage += 0.8f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Volibear R

        data = SkillshotData();
        data.ChampionName = "Volibear";
        data.DisplayName = "Stormbringer";
        data.ParticleName = "_R_AOE_Warning";
        data.SkillshotName = "VolibearR";
        data.IconName = "VolibearR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.SoftCC = true;
        data.Delay = 0.9f;
        data.ParticleDelay = 0.9f;
        data.Radius = 300.0f;
        data.Range = 700.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 300, 500, 700 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 2.5f * info.BonusAttackDamage;
            bonus_damage += 1.25f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Warwick

        // Warwick R

        data = SkillshotData(); // Check
        data.ChampionName = "Warwick";
        data.DisplayName = "Infinite Duress";
        data.SkillshotName = "WarwickR";
        data.IconName = "WarwickR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.Delay = 0.1f;
        data.Windup = 0.1f;
        data.Radius = 205.0f;
        data.Range = 2000.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 5;
        data.Collisions =
        {
            CollisionFlag::CHAMPION
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 175, 350, 525 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.67f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Xayah

        // Xayah Q

        data = SkillshotData();
        data.ChampionName = "Xayah";
        data.DisplayName = "Double Daggers";
        data.MissileName = "XayahQ";
        data.SkillshotName = "XayahQ";
        data.IconName = "XayahQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.UseWindup = true;
        data.Radius = 50.0f;
        data.Range = 1100.0f;
        data.Speed = 3000.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 45, 60, 75, 90, 105 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Xayah E

        data = SkillshotData();
        data.ChampionName = "Xayah";
        data.DisplayName = "Bladecaller";
        data.SkillshotName = "XayahE";
        data.IconName = "XayahE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.BackToCaster = true;
        data.Dangerous = true;
        data.SkipAncestor = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Radius = 80.0f;
        data.Range = 12500.0f;
        data.Speed = 4000.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 55, 65, 75, 85, 95 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Xerath

        // Xerath Q

        data = SkillshotData();
        data.ChampionName = "Xerath";
        data.DisplayName = "Arcanopulse";
        data.ParticleName = "_Q_cas(?!_charge)";
        data.SkillshotName = "XerathArcanopulse2";
        data.IconName = "XerathArcanopulseChargeUp.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 70.0f;
        data.Range = 1450.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Xerath W

        data = SkillshotData();
        data.ChampionName = "Xerath";
        data.DisplayName = "Arcane Barrage";
        data.ParticleName = "_W_aoe_green";
        data.SkillshotName = "XerathArcaneBarrage2";
        data.IconName = "XerathArcaneBarrage2.png";
        data.SkillshotSlot = 'W';
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.SoftCC = true;
        data.Delay = 0.8f;
        data.ParticleDelay = 0.8f;
        data.Windup = 0.25f;
        data.Radius = 275.0f;
        data.Range = 1000.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 85, 120, 155, 190 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.65f * info.TotalAbilityDamage;
            return (base_damage + bonus_damage) * 1.667f;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Xerath E

        data = SkillshotData();
        data.ChampionName = "Xerath";
        data.DisplayName = "Mage Spear";
        data.MissileName = "XerathMageSpearMissile";
        data.SkillshotName = "XerathMageSpear";
        data.IconName = "XerathMageSpear.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 60.0f;
        data.Range = 1125.0f;
        data.Speed = 1400.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 100, 130, 160, 190 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Xerath R

        data = SkillshotData();
        data.ChampionName = "Xerath";
        data.DisplayName = "Rite of the Arcane";
        data.MissileName = "XerathLocusPulse";
        data.SkillshotName = "XerathRMissileWrapper";
        data.IconName = "XerathLocusOfPower2.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FastEvade = true;
        data.FogSupport = true;
        data.Delay = 0.6f;
        data.Radius = 200.0f;
        data.Range = 5000.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 170, 220, 270 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region XinZhao

        // XinZhao W

        data = SkillshotData();
        data.ChampionName = "XinZhao";
        data.DisplayName = "Wind Becomes Lightning";
        data.SkillshotName = "XinZhaoW";
        data.IconName = "XinZhaoW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.SoftCC = true;
        data.Delay = 0.5f;
        data.Windup = 0.5f;
        data.Radius = 40.0f;
        data.Range = 1000.0f;
        data.Speed = 6250.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 125, 170, 215, 260 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.2f * info.TotalAttackDamage;
            bonus_damage += 0.65f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Yasuo

        // Yasuo Q1

        data = SkillshotData();
        data.ChampionName = "Yasuo";
        data.DisplayName = "Steel Tempest";
        data.SkillshotName = "YasuoQ1Wrapper";
        data.IconName = "YasuoQ1Wrapper.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 40.0f;
        data.Range = 450.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Yasuo Q2

        data = SkillshotData();
        data.ChampionName = "Yasuo";
        data.DisplayName = "Steel Wind Rising";
        data.SkillshotName = "YasuoQ2Wrapper";
        data.IconName = "YasuoQ2Wrapper.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 40.0f;
        data.Range = 450.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Yasuo Q3

        data = SkillshotData();
        data.ChampionName = "Yasuo";
        data.DisplayName = "Gathering Storm";
        data.MissileName = "YasuoQ3Mis";
        data.SkillshotName = "YasuoQ3Wrapper";
        data.IconName = "YasuoQ3Wrapper.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.UseWindup = true;
        data.Radius = 90.0f;
        data.Range = 1150.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Yone

        // Yone Q

        data = SkillshotData();
        data.ChampionName = "Yone";
        data.DisplayName = "Mortal Steel [Sword]";
        data.SkillshotName = "YoneQ";
        data.IconName = "YoneQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.Radius = 40.0f;
        data.Range = 450.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Yone Q3 Missile

        data = SkillshotData();
        data.ChampionName = "Yone";
        data.DisplayName = "Mortal Steel [Storm]";
        data.MissileName = "YoneQ3Missile";
        data.SkillshotName = "YoneQ3";
        data.IconName = "YoneQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.UseWindup = true;
        data.Radius = 80.0f;
        data.Range = 1050.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 2;
        data.Exclusions =
        {
            "YoneQ3Dash"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Yone Q3 Dash

        data = SkillshotData();
        data.ChampionName = "Yone";
        data.DisplayName = "Mortal Steel [Dash]";
        data.SkillshotName = "YoneQ3Dash";
        data.IconName = "YoneQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.HardCC = true;
        data.UseWindup = true;
        data.Radius = 100.0f;
        data.Range = 450.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Exclusions =
        {
            "YoneQ3"
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 20, 45, 70, 95, 120 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.05f * info.TotalAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Yone W

        data = SkillshotData();
        data.ChampionName = "Yone";
        data.DisplayName = "Spirit Cleave";
        data.SkillshotName = "YoneW";
        data.IconName = "YoneW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.UseWindup = true;
        data.ConeAngle = 80.0f;
        data.Range = 600.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CONE;
        this->skillshots[data.SkillshotName] = data;

        // Yone R

        data = SkillshotData();
        data.ChampionName = "Yone";
        data.DisplayName = "Fate Sealed";
        data.ParticleName = "_R_Cas_Warning";
        data.SkillshotName = "YoneR";
        data.IconName = "YoneR.png";
        data.SkillshotSlot = 'R';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.75f;
        data.Radius = 112.5f;
        data.Range = 1000.0f;
        data.DangerLevel = 5;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Yorick

        // Yorick E

        data = SkillshotData(); // Check
        data.ChampionName = "Yorick";
        data.DisplayName = "Mourning Mist";
        data.SkillshotName = "YorickE";
        data.IconName = "YorickE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.SoftCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 125.0f;
        data.Range = 700.0f;
        data.Speed = 1800.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::POLYGON;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Yuumi

        // Yuumi Q

        data = SkillshotData();
        data.ChampionName = "Yuumi";
        data.DisplayName = "Prowling Projectile [Unattached]";
        data.MissileName = "YuumiQSkillShot";
        data.SkillshotName = "YuumiQSkillShot";
        data.IconName = "YuumiQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.Delay = 0.15f;
        data.Radius = 35.0f;
        data.Range = 1150.0f;
        data.Speed = 850.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 90, 120, 150, 180, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.2f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zac

        // Zac Q

        data = SkillshotData();
        data.ChampionName = "Zac";
        data.DisplayName = "Stretching Strikes";
        data.MissileName = "ZacQMissile";
        data.SkillshotName = "ZacQ";
        data.IconName = "ZacQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.33f;
        data.Windup = 0.33f;
        data.Radius = 80.0f;
        data.Range = 951.0f;
        data.Speed = 2800.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 110, 140, 170, 200 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 0.06f * info.MaxHealth;
            bonus_damage += 0.6f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zac E

        data = SkillshotData();
        data.ChampionName = "Zac";
        data.DisplayName = "Elastic Slingshot";
        data.SkillshotName = "ZacEDash";
        data.IconName = "ZacE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Exception = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Radius = 250.0f;
        data.Range = 1800.0f;
        data.Speed = 1500.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_NEW_PATH
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 105, 150, 195, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.8f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zed

        // Zed Q

        data = SkillshotData();
        data.ChampionName = "Zed";
        data.DisplayName = "Razor Shuriken";
        data.MissileName = "ZedQMissile";
        data.SkillshotName = "ZedQ";
        data.IconName = "ZedQ.png";
        data.Overrider = "ZedQ";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 50.0f;
        data.Range = 925.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 80, 120, 160, 200, 240 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 1.0f * info.BonusAttackDamage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zeri

        // Zeri W Laser

        data = SkillshotData();
        data.ChampionName = "Zeri";
        data.DisplayName = "Ultrashock Laser [Laser]";
        data.ParticleName = "_W_SecondImpact_Beam";
        data.SkillshotName = "ZeriWParticle";
        data.IconName = "ZeriW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.UseEmitter = true;
        data.Delay = 0.85f;
        data.ParticleDelay = 0.85f;
        data.Radius = 100.0f;
        data.Range = 1500.0f;
        data.DangerLevel = 2;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 70, 110, 150, 190 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.3f * info.TotalAttackDamage;
            bonus_damage += 0.25f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zeri W Missile

        data = SkillshotData();
        data.ChampionName = "Zeri";
        data.DisplayName = "Ultrashock Laser [Pulse]";
        data.MissileName = "ZeriW";
        data.SkillshotName = "ZeriW";
        data.IconName = "ZeriW.png";
        data.SkillshotSlot = 'W';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.SoftCC = true;
        data.UseWindup = true;
        data.Radius = 40.0f;
        data.Range = 1200.0f;
        data.Speed = 2500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 30, 70, 110, 150, 190 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = 1.3f * info.TotalAttackDamage;
            bonus_damage += 0.25f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::PHYSICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Ziggs

        // Ziggs Q

        data = SkillshotData();
        data.ChampionName = "Ziggs";
        data.DisplayName = "Bouncing Bomb";
        data.SkillshotName = "ZiggsQ";
        data.IconName = "ZiggsQ.png";
        data.SkillshotSlot = 'Q';
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 180.0f;
        data.Range = 850.0f;
        data.Speed = 1700.0f;
        data.DangerLevel = 1;
        data.Exclusions =
        {
            "ZiggsQ"
        };
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 85, 135, 185, 235, 285 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        // Ziggs W

        data = SkillshotData();
        data.ChampionName = "Ziggs";
        data.DisplayName = "Satchel Charge";
        data.MissileName = "ZiggsW";
        data.SkillshotName = "ZiggsW";
        data.IconName = "ZiggsW.png";
        data.SkillshotSlot = 'W';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 325.0f;
        data.Range = 1000.0f;
        data.Speed = 1750.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 105, 140, 175, 210 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.5f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zilean

        // Zilean Q

        data = SkillshotData();
        data.ChampionName = "Zilean";
        data.DisplayName = "Time Bomb";
        data.ParticleName = "_Q_Cas_Warning";
        data.SkillshotName = "ZileanQ";
        data.IconName = "ZileanQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FogSupport = true;
        data.IgnoreAlive = true;
        data.HardCC = true;
        data.Delay = 0.75f;
        data.ParticleDelay = 0.75f;
        data.Windup = 0.25f;
        data.Radius = 140.0f;
        data.Range = 900.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 75, 115, 165, 230, 300 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.9f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zoe

        // Zoe Q1

        data = SkillshotData();
        data.ChampionName = "Zoe";
        data.DisplayName = "Paddle Star [First]";
        data.MissileName = "ZoeQMissile";
        data.SkillshotName = "ZoeQ";
        data.IconName = "ZoeQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 50.0f;
        data.Range = 800.0f;
        data.Speed = 1200.0f;
        data.DangerLevel = 1;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 50, 80, 110, 140, 170 };
            float bonus_scale[] = { 7, 8, 10, 12, 14, 16, 18,
                20, 22, 24, 26, 29, 32, 35, 38, 42, 46, 50 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            bonus_damage += 0.6f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zoe Q2

        data = SkillshotData();
        data.ChampionName = "Zoe";
        data.DisplayName = "Paddle Star [Second]";
        data.MissileName = "ZoeQMis2(?!Warning)";
        data.SkillshotName = "ZoeQMis2";
        data.IconName = "ZoeQ.png";
        data.SkillshotSlot = 'Q';
        data.AddHitbox = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.Radius = 70.0f;
        data.Range = 12500.0f;
        data.Speed = 2500.0f;
        data.DangerLevel = 2;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 125, 200, 275, 350, 425 };
            float bonus_scale[] = { 17.5, 20, 25, 30, 35, 40, 45,
                50, 55, 60, 65, 72.5, 80, 87.5, 95, 105, 115, 125 };
            float base_damage = base_scale[info.SpellLevel];
            float bonus_damage = bonus_scale[info.SpellLevel];
            bonus_damage += 1.5f * info.TotalAbilityDamage;
            return base_damage + bonus_damage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zoe E Missile

        data = SkillshotData();
        data.ChampionName = "Zoe";
        data.DisplayName = "Sleepy Trouble Bubble [Missile]";
        data.MissileName = "ZoeEMis(?!Audio)";
        data.SkillshotName = "ZoeE";
        data.IconName = "ZoeE.png";
        data.Overrider = "ZoeE";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.Exception = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.3f;
        data.Windup = 0.3f;
        data.Radius = 50.0f;
        data.Range = 12500.0f;
        data.Speed = 1850.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::CHAMPION,
            CollisionFlag::MINION,
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 70, 110, 150, 190, 230 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.45f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zoe E Ground

        data = SkillshotData();
        data.ChampionName = "Zoe";
        data.DisplayName = "Sleepy Trouble Bubble [Ground]";
        data.MissileName = "ZoeEc";
        data.SkillshotName = "ZoeEc";
        data.IconName = "ZoeE.png";
        data.SkillshotSlot = 'E';
        data.Dangerous = true;
        data.Exception = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 1.0f;
        data.MissileDelay = 1.0f;
        data.ExtraTime = 5.0f;
        data.Radius = 250.0f;
        data.Range = 50.0f;
        data.DangerLevel = 3;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_OBJECT_CREATED
        };
        data.Damage = [](const auto&) { return 0.0f; };
        data.DamageType = DamageType::TRUE_DAMAGE;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        #pragma region Zyra

        // Zyra Q

        data = SkillshotData();
        data.ChampionName = "Zyra";
        data.DisplayName = "Deadly Spines";
        data.SkillshotName = "ZyraQ";
        data.IconName = "ZyraQ.png";
        data.SkillshotSlot = 'Q';
        data.Rotate90 = true;
        data.Delay = 0.9f;
        data.Windup = 0.25f;
        data.Radius = 140.0f;
        data.Range = 800.0f;
        data.SideRange = 900.0f;
        data.DangerLevel = 1;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 100, 140, 180, 220 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.65f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zyra E

        data = SkillshotData();
        data.ChampionName = "Zyra";
        data.DisplayName = "Grasping Roots";
        data.MissileName = "ZyraE";
        data.SkillshotName = "ZyraE";
        data.IconName = "ZyraE.png";
        data.SkillshotSlot = 'E';
        data.AddHitbox = true;
        data.Dangerous = true;
        data.FixedRange = true;
        data.FogSupport = true;
        data.HookObject = true;
        data.HardCC = true;
        data.Delay = 0.25f;
        data.Windup = 0.25f;
        data.Radius = 70.0f;
        data.Range = 1100.0f;
        data.Speed = 1150.0f;
        data.DangerLevel = 3;
        data.Collisions =
        {
            CollisionFlag::WIND_WALL
        };
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 60, 95, 130, 165, 200 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.6f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::LINE;
        this->skillshots[data.SkillshotName] = data;

        // Zyra R

        data = SkillshotData();
        data.ChampionName = "Zyra";
        data.DisplayName = "Stranglethorns";
        data.ParticleName = "_R_Cas_Center";
        data.SkillshotName = "ZyraR";
        data.IconName = "ZyraR.png";
        data.SkillshotSlot = 'R';
        data.Dangerous = true;
        data.FogSupport = true;
        data.HardCC = true;
        data.Delay = 2.0f;
        data.ExtraTime = 0.25f;
        data.ParticleDelay = 1.75f;
        data.Windup = 0.25f;
        data.Radius = 500.0f;
        data.Range = 700.0f;
        data.DangerLevel = 4;
        data.Collisions = {};
        data.Detectors =
        {
            DetectionType::ON_ACTIVE_SPELL,
            DetectionType::ON_OBJECT_CREATED,
            DetectionType::ON_PROCESS_SPELL
        };
        data.Damage = [](const StatData& info)
        {
            float base_scale[] = { 180, 265, 350 };
            float base_damage = base_scale[info.SpellLevel];
            return base_damage + 0.7f * info.TotalAbilityDamage;
        };
        data.DamageType = DamageType::MAGICAL;
        data.SkillshotType = SkillshotType::CIRCLE;
        this->skillshots[data.SkillshotName] = data;

        #pragma endregion

        // Missing: Akshan Q, Aurelion Sol R, Azir R, FiddleSticks E, Rumble R, Yuumi Q
    }

    void Data::InitEvadingSpells()
    {
        this->evaders = EvadingSpellTable();
        this->evaders[""] = EvadingSpellData();

        #pragma region Local

        // Summoner Flash

        EvadingSpellData data;
        data.ChampionName = "Local";
        data.DisplayName = "Summoner Flash";
        data.IconName = "SummonerFlash.png";
        data.SpellName = "Flash";
        data.SpellSlot = 'G';
        data.Range = 400.0f;
        data.DangerLevel = 5;
        data.Speed = [](API*) { return FLT_MAX; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Aatrox

        // Aatrox E

        data = EvadingSpellData();
        data.ChampionName = "Aatrox";
        data.DisplayName = "Umbral Dash";
        data.IconName = "AatroxE.png";
        data.SpellName = "AatroxE";
        data.SpellSlot = 'E';
        data.Range = 300.0f;
        data.DangerLevel = 3;
        data.Speed = [](API*) { return 800.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Ahri

        // Ahri W

        data = EvadingSpellData();
        data.ChampionName = "Ahri";
        data.DisplayName = "Fox-Fire";
        data.IconName = "AhriW.png";
        data.SpellName = "AhriW";
        data.SpellSlot = 'W';
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 1.4f * api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        // Ahri R

        data = EvadingSpellData();
        data.ChampionName = "Ahri";
        data.DisplayName = "Spirit Rush";
        data.IconName = "AhriR.png";
        data.SpellName = "AhriR";
        data.SpellSlot = 'R';
        data.Range = 500.0f;
        data.DangerLevel = 4;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 1200.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Annie

        // Annie E

        data = EvadingSpellData();
        data.ChampionName = "Annie";
        data.DisplayName = "Molten Shield";
        data.IconName = "AnnieE.png";
        data.SpellName = "AnnieE";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetLevel(hero) - 1;
            float speed = api->GetMovementSpeed(hero);
            return (1.2f + 0.3f / 17.0f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Belveth

        // Belveth Q

        data = EvadingSpellData();
        data.ChampionName = "Belveth";
        data.DisplayName = "Void Surge";
        data.IconName = "BelvethQ.png";
        data.SpellName = "BelvethQ";
        data.SpellSlot = 'Q';
        data.Range = 400.0f;
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 0);
            float speed = api->GetMovementSpeed(hero);
            return 750.0f + 50.0f * level + speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Blitzcrank

        // Blitzcrank W

        data = EvadingSpellData();
        data.ChampionName = "Blitzcrank";
        data.DisplayName = "Overdrive";
        data.IconName = "Overdrive.png";
        data.SpellName = "Overdrive";
        data.SpellSlot = 'W';
        data.DangerLevel = 2;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 1);
            float speed = api->GetMovementSpeed(hero);
            return (1.55f + 0.05f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Braum

        // Braum E

        data = EvadingSpellData();
        data.ChampionName = "Braum";
        data.DisplayName = "Unbreakable";
        data.IconName = "BraumE.png";
        data.SpellName = "BraumE";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::WIND_WALL;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Caitlyn

        // Caitlyn E

        data = EvadingSpellData();
        data.ChampionName = "Caitlyn";
        data.DisplayName = "90 Caliber Net";
        data.IconName = "CaitlynE.png";
        data.SpellName = "CaitlynE";
        data.SpellSlot = 'E';
        data.Delay = 0.15f;
        data.Range = 390.0f;
        data.DangerLevel = 2;
        data.Speed = [](API*) { return 600.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Corki

        // Corki W

        data = EvadingSpellData();
        data.ChampionName = "Corki";
        data.DisplayName = "Valkyrie";
        data.IconName = "CarpetBomb.png";
        data.SpellName = "CarpetBomb";
        data.SpellSlot = 'W';
        data.Range = 600.0f;
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 650.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Draven

        // Draven W

        data = EvadingSpellData();
        data.ChampionName = "Draven";
        data.DisplayName = "Blood Rush";
        data.IconName = "DravenFury.png";
        data.SpellName = "DravenFury";
        data.SpellSlot = 'W';
        data.DangerLevel = 2;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 1);
            float speed = api->GetMovementSpeed(hero);
            return (1.45f + 0.05f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Ekko

        // Ekko E

        data = EvadingSpellData();
        data.ChampionName = "Ekko";
        data.DisplayName = "Phase Dive";
        data.IconName = "EkkoE.png";
        data.SpellName = "EkkoE";
        data.SpellSlot = 'E';
        data.Range = 325.0f;
        data.DangerLevel = 1;
        data.Speed = [](API*) { return 1150.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Ezreal

        // Ezreal E

        data = EvadingSpellData();
        data.ChampionName = "Ezreal";
        data.DisplayName = "Arcane Shift";
        data.IconName = "EzrealE.png";
        data.SpellName = "EzrealE";
        data.SpellSlot = 'E';
        data.Delay = 0.25f;
        data.Range = 475.0f;
        data.DangerLevel = 3;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Fiora

        // Fiora Q

        data = EvadingSpellData();
        data.ChampionName = "Fiora";
        data.DisplayName = "Lunge";
        data.IconName = "FioraQ.png";
        data.SpellName = "FioraQ";
        data.SpellSlot = 'Q';
        data.Range = 400.0f;
        data.DangerLevel = 1;
        data.Speed = [](API*) { return 900.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        // Fiora W

        data = EvadingSpellData();
        data.ChampionName = "Fiora";
        data.DisplayName = "Riposte";
        data.IconName = "FioraW.png";
        data.SpellName = "FioraW";
        data.SpellSlot = 'W';
        data.DangerLevel = 1;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::CC_IMMUNITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Fizz

        // Fizz E

        data = EvadingSpellData();
        data.ChampionName = "Fizz";
        data.DisplayName = "Playful";
        data.IconName = "FizzE.png";
        data.SpellName = "FizzE";
        data.SpellSlot = 'E';
        data.DangerLevel = 3;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::INVULNERABILITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Garen

        // Garen Q

        data = EvadingSpellData();
        data.ChampionName = "Garen";
        data.DisplayName = "Decisive Strike";
        data.IconName = "GarenQ.png";
        data.SpellName = "GarenQ";
        data.SpellSlot = 'Q';
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 1.35f * api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Gnar

        // Gnar E

        data = EvadingSpellData();
        data.ChampionName = "Gnar";
        data.DisplayName = "Hop";
        data.IconName = "GnarE.png";
        data.SpellName = "GnarE";
        data.SpellSlot = 'E';
        data.Range = 475.0f;
        data.DangerLevel = 3;
        data.Speed = [](API*) { return 900.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Graves

        // Graves E

        data = EvadingSpellData();
        data.ChampionName = "Graves";
        data.DisplayName = "Quickdraw";
        data.IconName = "GravesMove.png";
        data.SpellName = "GravesMove";
        data.SpellSlot = 'E';
        data.Range = 375.0f;
        data.DangerLevel = 1;
        data.Speed = [](API*) { return 1225.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Kaisa

        // Kaisa E

        data = EvadingSpellData();
        data.ChampionName = "Kaisa";
        data.DisplayName = "Supercharge";
        data.IconName = "KaisaE.png";
        data.SpellName = "KaisaE";
        data.SpellSlot = 'E';
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            float move_speed = api->GetMovementSpeed(hero);
            float attack_speed = api->GetAttackSpeedMod(hero);
            float speed_mod = MIN(attack_speed - 1.0f, 1.0f);
            return 1.3f * (1.0f + speed_mod) * move_speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Karma

        // Karma E

        data = EvadingSpellData();
        data.ChampionName = "Karma";
        data.DisplayName = "Inspire";
        data.IconName = "KarmaSolKimShield.png";
        data.SpellName = "KarmaSolKimShield";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 1.4f * api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Kassadin

        // Kassadin R

        data = EvadingSpellData();
        data.ChampionName = "Kassadin";
        data.DisplayName = "Rift Walk";
        data.IconName = "RiftWalk.png";
        data.SpellName = "RiftWalk";
        data.SpellSlot = 'R';
        data.Delay = 0.25f;
        data.Range = 500.0f;
        data.DangerLevel = 2;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Kayle

        // Kayle W

        data = EvadingSpellData();
        data.ChampionName = "Kayle";
        data.DisplayName = "Celestial Blessing";
        data.IconName = "KayleW.png";
        data.SpellName = "KayleW";
        data.SpellSlot = 'W';
        data.Delay = 0.25f;
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 1);
            float speed = api->GetMovementSpeed(hero);
            float damage = api->GetTotalAbilityDamage(hero);
            float base_mod = 1.0f + 0.2f + 0.04f * level;
            return (base_mod + 0.0008f * damage) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Kennen

        // Kennen E

        data = EvadingSpellData();
        data.ChampionName = "Kennen";
        data.DisplayName = "Lightning Rush";
        data.IconName = "KennenLightningRush.png";
        data.SpellName = "KennenLightningRush";
        data.SpellSlot = 'E';
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 2.0f * api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Khazix

        // Khazix E

        data = EvadingSpellData();
        data.ChampionName = "Khazix";
        data.DisplayName = "Leap";
        data.IconName = "KhazixE.png";
        data.SpellName = "KhazixE";
        data.SpellSlot = 'E';
        data.Range = 700.0f;
        data.DangerLevel = 3;
        data.Speed = [](API*) { return 1300.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Kindred

        // Kindred Q

        data = EvadingSpellData();
        data.ChampionName = "Kindred";
        data.DisplayName = "Dance of Arrows";
        data.IconName = "KindredQ.png";
        data.SpellName = "KindredQ";
        data.SpellSlot = 'Q';
        data.Range = 300.0f;
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 500.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Leblanc

        // Leblanc W

        data = EvadingSpellData();
        data.ChampionName = "Leblanc";
        data.DisplayName = "Distortion";
        data.IconName = "LeblancW.png";
        data.SpellName = "LeblancW";
        data.SpellSlot = 'W';
        data.Range = 600.0f;
        data.DangerLevel = 2;
        data.Speed = [](API*) { return 1450.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Lucian

        // Lucian E

        data = EvadingSpellData();
        data.ChampionName = "Lucian";
        data.DisplayName = "Relentless Pursuit";
        data.IconName = "LucianE.png";
        data.SpellName = "LucianE";
        data.SpellSlot = 'E';
        data.Range = 425.0f;
        data.DangerLevel = 2;
        data.Speed = [](API*) { return 1350.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Mel

        // Mel W

        data = EvadingSpellData();
        data.ChampionName = "Mel";
        data.DisplayName = "Rebuttal";
        data.IconName = "MelW.png";
        data.SpellName = "MelW";
        data.SpellSlot = 'W';
        data.DangerLevel = 3;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::INVULNERABILITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Morgana

        // Morgana E

        data = EvadingSpellData();
        data.ChampionName = "Morgana";
        data.DisplayName = "Black Shield";
        data.IconName = "MorganaE.png";
        data.SpellName = "MorganaE";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::CC_IMMUNITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Neeko

        // Neeko W

        data = EvadingSpellData();
        data.ChampionName = "Neeko";
        data.DisplayName = "Shapesplitter";
        data.IconName = "NeekoW.png";
        data.SpellName = "NeekoW";
        data.SpellSlot = 'W';
        data.DangerLevel = 2;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 1);
            float speed = api->GetMovementSpeed(hero);
            return (1.15f + 0.05f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Nocturne

        // Nocturne W

        data = EvadingSpellData();
        data.ChampionName = "Nocturne";
        data.DisplayName = "Shroud of Darkness";
        data.IconName = "NocturneShroudofDarkness.png";
        data.SpellName = "NocturneShroudofDarkness";
        data.SpellSlot = 'W';
        data.DangerLevel = 2;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::INVULNERABILITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Qiyana

        // Qiyana W

        data = EvadingSpellData();
        data.ChampionName = "Qiyana";
        data.DisplayName = "Terrashape";
        data.IconName = "QiyanaW.png";
        data.SpellName = "QiyanaW";
        data.SpellSlot = 'W';
        data.Range = 300.0f;
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 440.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Renekton

        // Renekton E

        data = EvadingSpellData();
        data.ChampionName = "Renekton";
        data.DisplayName = "Slice / Dice";
        data.IconName = "RenektonSliceAndDice.png";
        data.SpellName = "RenektonSliceAndDice";
        data.SpellSlot = 'E';
        data.Range = 450.0f;
        data.DangerLevel = 2;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 760.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Riven

        // Riven E

        data = EvadingSpellData();
        data.ChampionName = "Riven";
        data.DisplayName = "Valor";
        data.IconName = "RivenFeint.png";
        data.SpellName = "RivenFeint";
        data.SpellSlot = 'E';
        data.Range = 250.0f;
        data.DangerLevel = 1;
        data.Speed = [](API*) { return 1200.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Samira

        // Samira W

        data = EvadingSpellData();
        data.ChampionName = "Samira";
        data.DisplayName = "Blade Whirl";
        data.IconName = "SamiraW.png";
        data.SpellName = "SamiraW";
        data.SpellSlot = 'W';
        data.Delay = 0.01f;
        data.DangerLevel = 1;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::WIND_WALL;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Sejuani

        // Sejuani Q

        data = EvadingSpellData();
        data.ChampionName = "Sejuani";
        data.DisplayName = "Arctic Assault";
        data.IconName = "SejuaniQ.png";
        data.SpellName = "SejuaniQ";
        data.SpellSlot = 'Q';
        data.Range = 650.0f;
        data.DangerLevel = 2;
        data.Speed = [](API*) { return 1000.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_TARGETED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Seraphine

        // Seraphine W

        data = EvadingSpellData();
        data.ChampionName = "Seraphine";
        data.DisplayName = "Surround Sound";
        data.IconName = "SeraphineW.png";
        data.SpellName = "SeraphineW";
        data.SpellSlot = 'W';
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            float speed = api->GetMovementSpeed(hero);
            float damage = api->GetTotalAbilityDamage(hero);
            return (1.2f + 0.02f * damage / 100.0f) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Shaco

        // Shaco Q

        data = EvadingSpellData();
        data.ChampionName = "Shaco";
        data.DisplayName = "Deceive";
        data.IconName = "Deceive.png";
        data.SpellName = "Deceive";
        data.SpellSlot = 'Q';
        data.Delay = 0.125f;
        data.Range = 400.0f;
        data.DangerLevel = 2;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Shen

        // Shen E

        data = EvadingSpellData();
        data.ChampionName = "Shen";
        data.DisplayName = "Shadow Dash";
        data.IconName = "ShenE.png";
        data.SpellName = "ShenE";
        data.SpellSlot = 'E';
        data.Range = 600.0f;
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 800.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Sivir

        // Sivir E

        data = EvadingSpellData();
        data.ChampionName = "Sivir";
        data.DisplayName = "Spell Shield";
        data.IconName = "SivirE.png";
        data.SpellName = "SivirE";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::INVULNERABILITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Smolder

        // Smolder E

        data = EvadingSpellData();
        data.ChampionName = "Smolder";
        data.DisplayName = "Flap, Flap, Flap";
        data.IconName = "SmolderE.png";
        data.SpellName = "SmolderE";
        data.SpellSlot = 'E';
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 1.75f * api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Sona

        // Sona E

        data = EvadingSpellData();
        data.ChampionName = "Sona";
        data.DisplayName = "Song of Celerity";
        data.IconName = "SonaE.png";
        data.SpellName = "SonaE";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            float speed = api->GetMovementSpeed(hero);
            float damage = api->GetTotalAbilityDamage(hero);
            return (1.2f + 0.02f * damage / 100.0f) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Shen

        // Shen E

        data = EvadingSpellData();
        data.ChampionName = "Shen";
        data.DisplayName = "Abscond";
        data.IconName = "SylasE.png";
        data.SpellName = "SylasE";
        data.SpellSlot = 'E';
        data.Range = 400.0f;
        data.DangerLevel = 3;
        data.Condition = [](API* api)
        {
            const auto& hero = api->GetHero();
            return api->GetSpellName(hero, 2) == "SylasE";
        };
        data.Speed = [](API* api) { return 1450.0f; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Teemo

        // Teemo W

        data = EvadingSpellData();
        data.ChampionName = "Teemo";
        data.DisplayName = "Move Quick";
        data.IconName = "TeemoW.png";
        data.SpellName = "TeemoW";
        data.SpellSlot = 'W';
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 1);
            float speed = api->GetMovementSpeed(hero);
            return (1.08f + 0.04f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Vayne

        // Vayne Q

        data = EvadingSpellData();
        data.ChampionName = "Vayne";
        data.DisplayName = "Tumble";
        data.IconName = "VayneTumble.png";
        data.SpellName = "VayneTumble";
        data.SpellSlot = 'Q';
        data.Range = 250.0f;
        data.DangerLevel = 1;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 500.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Vi

        // Vi Q

        data = EvadingSpellData();
        data.ChampionName = "Vi";
        data.DisplayName = "Vault Breaker";
        data.IconName = "ViQ.png";
        data.SpellName = "ViQ";
        data.SpellSlot = 'Q';
        data.Range = 250.0f;
        data.DangerLevel = 1;
        data.Speed = [](API*) { return 1450.0f; };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Vladimir

        // Vladimir W

        data = EvadingSpellData();
        data.ChampionName = "Vladimir";
        data.DisplayName = "Sanguine Pool";
        data.IconName = "VladimirSanguinePool.png";
        data.SpellName = "VladimirSanguinePool";
        data.SpellSlot = 'W';
        data.DangerLevel = 3;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::INVULNERABILITY;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Yasuo

        // Yasuo W

        data = EvadingSpellData();
        data.ChampionName = "Yasuo";
        data.DisplayName = "Wind Wall";
        data.IconName = "YasuoW.png";
        data.SpellName = "YasuoW";
        data.SpellSlot = 'W';
        data.Delay = 0.0125f;
        data.DangerLevel = 1;
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::WIND_WALL;
        this->evaders[data.SpellName] = data;
        
        #pragma endregion

        #pragma region Zeri

        // Zeri E

        data = EvadingSpellData();
        data.ChampionName = "Zeri";
        data.DisplayName = "Spark Surge";
        data.IconName = "ZeriE.png";
        data.SpellName = "ZeriE";
        data.SpellSlot = 'E';
        data.Range = 300.0f;
        data.DangerLevel = 3;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            return 600.0f + api->GetMovementSpeed(hero);
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::DASH_FIXED;
        this->evaders[data.SpellName] = data;

        #pragma endregion

        #pragma region Zilean

        // Zilean E

        data = EvadingSpellData();
        data.ChampionName = "Zilean";
        data.DisplayName = "Time Warp";
        data.IconName = "TimeWarp.png";
        data.SpellName = "TimeWarp";
        data.SpellSlot = 'E';
        data.DangerLevel = 1;
        data.IsTargeted = true;
        data.Speed = [](API* api)
        {
            const auto& hero = api->GetHero();
            int level = api->GetSpellLevel(hero, 2);
            float speed = api->GetMovementSpeed(hero);
            return (1.25f + 0.15f * level) * speed;
        };
        data.Condition = [](API*) { return true; };
        data.EvadingType = EvadingType::MOVEMENT_BUFF;
        this->evaders[data.SpellName] = data;

        #pragma endregion
    }

    void Data::InitShieldSpells()
    {
        this->shields = ShieldSpellTable();
        this->shields[""] = ShieldSpellData();

        #pragma region Annie

        // Annie E

        ShieldSpellData data;
        data.ChampionName = "Annie";
        data.DisplayName = "Molten Shield";
        data.IconName = "AnnieE.png";
        data.SpellName = "AnnieE";
        data.SpellSlot = 'E';
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Janna

        // Janna E

        data = ShieldSpellData();
        data.ChampionName = "Janna";
        data.DisplayName = "Eye of the Storm";
        data.IconName = "EyeOfTheStorm.png";
        data.SpellName = "EyeOfTheStorm";
        data.SpellSlot = 'E';
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Karma

        // Karma E

        data = ShieldSpellData();
        data.ChampionName = "Karma";
        data.DisplayName = "Inspire";
        data.IconName = "KarmaSolKimShield.png";
        data.SpellName = "KarmaSolKimShield";
        data.SpellSlot = 'E';
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Lux

        // Lux W

        data = ShieldSpellData();
        data.ChampionName = "Lux";
        data.DisplayName = "Prismatic Barrier";
        data.IconName = "LuxPrismaticWave.png";
        data.SpellName = "LuxPrismaticWave";
        data.SpellSlot = 'W';
        data.Delay = 0.25f;
        data.Range = 1175.0f;
        data.Speed = 2400.0f;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Morgana

        // Morgana E

        data = ShieldSpellData();
        data.ChampionName = "Morgana";
        data.DisplayName = "Black Shield";
        data.IconName = "MorganaE.png";
        data.SpellName = "MorganaE";
        data.SpellSlot = 'E';
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::CC_IMMUNITY;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Orianna

        // Orianna E

        data = ShieldSpellData();
        data.ChampionName = "Orianna";
        data.DisplayName = "Command: Protect";
        data.IconName = "OrianaRedactCommand.png";
        data.SpellName = "OrianaRedactCommand";
        data.SpellSlot = 'E';
        data.Range = 1120.0f;
        data.Speed = 1850.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion

        #pragma region Seraphine

        // Seraphine W

        data = ShieldSpellData();
        data.ChampionName = "Seraphine";
        data.DisplayName = "Surround Sound";
        data.IconName = "SeraphineW.png";
        data.SpellName = "SeraphineW";
        data.SpellSlot = 'W';
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion
        
        #pragma region Taric

        // Taric W

        data = ShieldSpellData();
        data.ChampionName = "Taric";
        data.DisplayName = "Bastion";
        data.IconName = "TaricW.png";
        data.SpellName = "TaricW";
        data.SpellSlot = 'W';
        data.Delay = 0.25f;
        data.Range = 800.0f;
        data.IsTargeted = true;
        data.ShieldType = EvadingType::ABSORPTION;
        this->shields[data.SpellName] = data;

        #pragma endregion
    }

    void Data::InitConnectors()
    {
        this->connectors =
        {
            {"EkkoQ", {"EkkoQ2", ConnectionType::EXTEND_LENGTH}},
            {"FlashFrost", {"AniviaQ2", ConnectionType::EXTEND_LENGTH}},
            {"GnarBigQ", {"GnarBigQLand", ConnectionType::EXTEND_LENGTH}},
            {"GravesQLineMis", {"GravesQDetonation", ConnectionType::EXTEND_LENGTH}},
            {"GravesChargeShot", {"GravesRExplosion", ConnectionType::EXTEND_LENGTH}},
            {"HweiQQ", {"HweiQQExplosion", ConnectionType::EXTEND_LENGTH}},
            {"KarmaQMissileMantra", {"KarmaQExplosion", ConnectionType::EXTEND_LENGTH}},
            {"LeonaSolarFlare", {"LeonaREpicenter", ConnectionType::FOLLOW_ORIGIN}},
            {"LilliaE", {"LilliaERollingMissile", ConnectionType::EXTEND_LENGTH}},
            {"MalzaharQ", {"MalzaharQSecond", ConnectionType::FOLLOW_ORIGIN}},
            {"MelE", {"MelEFieldTravel", ConnectionType::FOLLOW_ORIGIN}},
            {"MordekaiserQ", {"MordekaiserQ2", ConnectionType::FOLLOW_ORIGIN}},
            {"NaafiriE", {"NaafiriEFlurry", ConnectionType::FOLLOW_ORIGIN}},
            {"OrianaIzuna", {"OrianaIzunaCenter", ConnectionType::FOLLOW_ORIGIN}},
            {"OrnnQ", {"OrnnQPillar", ConnectionType::EXTEND_LENGTH}},
            {"QiyanaQ_Grass", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"QiyanaQ_Water", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"QiyanaQ_Rock", {"QiyanaQExplosion", ConnectionType::FOLLOW_ORIGIN}},
            {"RellW_Dismount", {"RellW2", ConnectionType::FOLLOW_ORIGIN}},
            {"RenataE", {"RenataEExplosion", ConnectionType::EXTEND_LENGTH}},
            {"SettW", {"SettWInner", ConnectionType::FOLLOW_ORIGIN}},
            {"ShyvanaFireballDragon2", {"ShyvanaE2", ConnectionType::EXTEND_LENGTH}},
            {"SmolderW", {"SmolderW2", ConnectionType::EXTEND_LENGTH}},
            {"SwainE", {"SwainEDetonation", ConnectionType::EXTEND_LENGTH}},
            {"VelkozWMissile", {"VelkozWEruption", ConnectionType::FOLLOW_ORIGIN}},
            {"VeigarEventHorizon", {"VeigarCage", ConnectionType::FOLLOW_ORIGIN}},
            {"VexQ", {"VexQAccelerated", ConnectionType::EXTEND_LENGTH}},
            {"YoneQ3", {"YoneQ3Dash", ConnectionType::FOLLOW_ORIGIN}}
        };
    }

    void Data::InitDashBuffs()
    {
        this->dash_buffs =
        {
            {"hecarimultsound", {"HecarimRDash"}},
            {"ViQDash", {"ViQDash"}},
            {"zacemove", {"ZacEDash"}}
        };
    }

    void Data::InitPatterns()
    {
        this->patterns =
        {
            {"LayWasteA", {"KarthusQ"}},
            {"NaafiriQ", {"NaafiriQ"}},
            {"SyndraQ", {"SyndraQ"}},
            {"SyndraE5", {"SyndraE"}},
            {"Volley", {"Volley"}}
        };
    }

    void Data::InitStoppers()
    {
        API* api = API::Get();
        this->stoppers =
        {
            {"Caitlyn", {api->FNV1A32("CaitlynR")}},
            {"Fiora", {api->FNV1A32("FioraW")}},
            {"Galio", {api->FNV1A32("GalioR")}},
            {"Janna", {api->FNV1A32("ReapTheWhirlwind")}},
            {"Karthus", {api->FNV1A32("karthusfallenonecastsound")}},
            {"Katarina", {api->FNV1A32("katarinarsound")}},
            {"Kayle", {api->FNV1A32("KayleR")}},
            {"KogMaw", {api->FNV1A32("KogMawIcathianSurprise")}},
            {"Lissandra", {api->FNV1A32("lissandrarself")}},
            {"Malzahar", {api->FNV1A32("MalzaharRSound")}},
            {"MasterYi", {api->FNV1A32("Meditate")}},
            {"MissFortune", {api->FNV1A32("missfortunebulletsound")}},
            {"Olaf", {api->FNV1A32("OlafRagnarok")}},
            {"Pantheon", {api->FNV1A32("PantheonR")}},
            {"Shen", {api->FNV1A32("shenstandunitedlock")}},
            {"Sion", {api->FNV1A32("sionpassivezombie")}},
            {"Tryndamere", {api->FNV1A32("UndyingRage")}},
            {"TwistedFate", {api->FNV1A32("Gate")}},
            {"Velkoz", {api->FNV1A32("VelkozR")}},
            {"Xerath", {api->FNV1A32("XerathLocusOfPower2")}},
            {"Yuumi", {api->FNV1A32("YuumiWAttach")}},
            {"Zac", {api->FNV1A32("ZacE")}}
        };
    }
}
