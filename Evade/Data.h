
#ifndef DATA_H
#define DATA_H

#include "API.h"

namespace Evade
{
    struct Connector;
    struct SkillshotData;

    using Miscellaneous = std::unordered_map<std::string, Connector>;
    using SkillshotTable = std::unordered_map<std::string, SkillshotData>;

    static constexpr float FLT_INF = std::numeric_limits<float>::infinity();

    // Enum types

    enum class CollisionFlag
    {
        CHAMPION = 0,
        MINION,
        TERRAIN_WALL,
        WIND_WALL
    };

    enum class ConnectionType
    {
        FOLLOW_ORIGIN = 0,
        EXTEND_LENGTH
    };

    enum class DetectionType
    {
        UNDEFINED = 0,
        ON_ACTIVE_SPELL,
        ON_NEW_PATH,
        ON_OBJECT_CREATED,
        ON_PROCESS_SPELL,
        ON_WND_PROC
    };

    enum class EvadingType
    {
        AUTO_WALK = 0,
        CC_IMMUNITY,
        DAMAGE_ABSORPTION,
        DASH_FIXED,
        DASH_TARGETED,
        INVULNERABILITY,
        MOVEMENT_BUFF,
        WIND_WALL
    };

    enum class SkillshotType
    {
        NONE = 0,
        CIRCLE,
        CONE,
        LINE,
        POLYGON,
        RING
    };

    // Structures

    struct Connector
    {
        std::string PrimaryName, SecondName;
        ConnectionType Type = ConnectionType::FOLLOW_ORIGIN;

        Connector(std::string name)
            : PrimaryName(name), SecondName("") {}
        Connector(std::string name, ConnectionType type)
            : PrimaryName(name), SecondName(""), Type(type) {}
        Connector(std::string spell_name, std::string buff_name)
            : PrimaryName(spell_name), SecondName(buff_name) {}
    };

    struct StatData
    {
        float Armor, BonusAbilityDamage, BonusArmor,
            BonusAttackDamage, BonusHealth, BonusMana,
            BonusSpellBlock, Health, Lethality, MaxHealth,
            MaxMana, SpellBlock, TargetHealth, TargetMaxHealth,
            TotalAbilityDamage, TotalAttackDamage;
        int SpellLevel, UnitLevel;
    };

    struct SkillshotData
    {
        std::string ChampionName;
        std::string SkillshotName;
        std::string DisplayName;
        std::string IconName = "";
        std::string MissileName = "";
        std::string ParticleName = "";
        std::string Overrider = "";
        std::function<float(StatData)> Damage;
        std::vector<CollisionFlag> Collisions;
        std::vector<DetectionType> Detectors;
        SkillshotType SkillshotType;
        char SkillshotSlot = ' ';
        int DangerLevel = 1;
        int Multiplicity = 0;
        float ConeAngle = 0.0f;
        float Delay = -1.0f;
        float ExtraTime = 0.0f;
        float Offset = 0.0f;
        float Range = 0.0f;
        float Radius = 0.0f;
        float InnerRadius = 0.0f;
        float OuterRadius = 0.0f;
        float RotationAngle = 0.0f;
        float Acceleration = 0.0f;
        float InitialSpeed = 0.0f;
        float MinSpeed = 0.0f;
        float MaxSpeed = 0.0f;
        float Speed = FLT_INF;
        bool AddHitbox = false;
        bool Dangerous = false;
        bool Exception = false;
        bool FixedRange = false;
        bool FogSupport = false;
        bool IgnoreAlive = false;
        bool SoftCC = false;
        bool HardCC = false;
        bool Invert = false;
        bool Rotate90 = false;
    };

    struct EvadingSpellData
    {
        std::string ChampionName;
        std::string DisplayName;
        std::string IconName;
        EvadingType EvadingType;
        char SpellSlot = ' ';
        int DangerLevel = 1;
        float Delay = 0.0f;
        float Range = 0.0f;
        std::function<float(API*)> Speed;
        std::function<bool(API*)> Condition;
    };

    struct ShieldSpellData
    {
        std::string ChampionName;
        std::string DisplayName;
        std::string IconName;
        EvadingType EvadingType;
        char SpellSlot = ' ';
        float Delay = 0.0f;
        float Range = 0.0f;
    };

    // Data class

    class Data
    {
        private:
            static Data* m_instance;
            SkillshotTable skillshots;
            Linq<EvadingSpellData> evaders;
            Linq<ShieldSpellData> shields;
            Miscellaneous anti_recalcs;
            Miscellaneous connectors;
            Miscellaneous custom_dashes;
            Miscellaneous stoppers;

            Data();
            void InitSkillshots();
            void InitEvadingSpells();
            void InitShieldSpells();
            void InitAntiRecalcs();
            void InitConnectors();
            void InitCustomDashes();
            void InitStoppers();

        public:
            static void Destroy();
            static Data* Get();

            SkillshotTable GetSkillshots() const;
            Linq<EvadingSpellData> GetEvadingSpells() const;
            Linq<ShieldSpellData> GetShieldSpells() const;
            Miscellaneous GetAntiRecalcs() const;
            Miscellaneous GetConnectors() const;
            Miscellaneous GetCustomDashes() const;
            Miscellaneous GetStoppers() const;
    };
}

#endif // DATA_H
