
#ifndef DATA_H
#define DATA_H

#include "API.h"

namespace Evade
{
    struct Connector;
    struct StatData;
    struct SkillshotData;
    struct EvadingSpellData;
    struct ShieldSpellData;

    using DamageFunc = std::function<float(const StatData&)>;
    using Miscellaneous = std::unordered_map<std::string, Connector>;
    using SkillshotTable = std::unordered_map<std::string, SkillshotData>;
    using EvadingSpellTable = std::unordered_map<std::string, EvadingSpellData>;
    using ShieldSpellTable = std::unordered_map<std::string, ShieldSpellData>;

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

    enum class DamageType
    {
        MAGICAL = 0,
        PHYSICAL,
        MIXED,
        TRUE_DAMAGE
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
        uint32_t BuffHash = 0;
        std::string PrimaryName, SecondaryName = "";
        ConnectionType ConnectType = ConnectionType::FOLLOW_ORIGIN;

        Connector(uint32_t hash) : BuffHash(hash) {}
        Connector(std::string name) : PrimaryName(name) {}
        Connector(std::string first, std::string second)
            : PrimaryName(first), SecondaryName(second) {}
        Connector(std::string name, ConnectionType type)
            : PrimaryName(name), ConnectType(type) {}
    };

    struct StatData
    {
        Obj_AI_Hero* UnitData = nullptr;
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
        std::string DisplayName;
        std::string SkillshotName;
        std::string IconName = "";
        std::string MissileName = "";
        std::string ParticleName = "";
        std::string Overrider = "";
        std::vector<CollisionFlag> Collisions;
        std::vector<DetectionType> Detectors;
        SkillshotType SkillshotType;
        DamageType DamageType;
        DamageFunc Damage;
        char SkillshotSlot = 0;
        int DangerLevel = 1;
        int MultiAngle = 0;
        int MultiCount = 0;
        float ConeAngle = 0.0f;
        float Delay = 0.0f;
        float ExtraTime = 0.0f;
        float Offset = 0.0f;
        float Range = 0.0f;
        float SideRange = 0.0f;
        float Radius = 0.0f;
        float InnerRadius = 0.0f;
        float OuterRadius = 0.0f;
        float MissileDelay = 0.0f;
        float ParticleDelay = 0.0f;
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
        bool BackToCaster = false;
        bool FollowCaster = false;
        bool IgnoreAlive = false;
        bool SkipAncestor = false;
        bool SoftCC = false;
        bool HardCC = false;
        bool FixSpeed = false;
        bool Invert = false;
        bool IsTrap = false;
        bool Rotate90 = false;
        bool UseEmitter = false;
        bool UseWindup = false;
    };

    struct EvadingSpellData
    {
        std::string ChampionName;
        std::string DisplayName;
        std::string IconName;
        std::string SpellName;
        EvadingType EvadingType;
        char SpellSlot = ' ';
        int DangerLevel = 1;
        float Delay = 0.0f;
        float Range = 0.0f;
        bool IsTargeted = false;
        std::function<float()> Speed;
        std::function<bool()> Condition;
    };

    struct ShieldSpellData
    {
        std::string ChampionName;
        std::string DisplayName;
        std::string IconName;
        std::string SpellName;
        EvadingType ShieldType;
        char SpellSlot = ' ';
        float Delay = 0.0f;
        float Range = 0.0f;
        float Speed = FLT_INF;
        bool IsTargeted = false;
    };

    // Data class

    class Data
    {
        private:
            SkillshotTable skillshots;
            EvadingSpellTable evaders;
            ShieldSpellTable shields;
            Miscellaneous anti_recalcs;
            Miscellaneous connectors;
            Miscellaneous custom_dashes;
            Miscellaneous patterns;
            Miscellaneous stoppers;
            static Data* m_instance;

            Data();
            void InitSkillshots();
            void InitEvadingSpells();
            void InitShieldSpells();
            void InitAntiRecalcs();
            void InitConnectors();
            void InitCustomDashes();
            void InitPatterns();
            void InitStoppers();

        public:
            static void Destroy();
            static Data* Get();

            const SkillshotTable& GetSkillshots() const { return this->skillshots; }
            const EvadingSpellTable& GetEvadingSpells() const { return this->evaders; }
            const ShieldSpellTable& GetShieldSpells() const { return this->shields; }
            const Miscellaneous& GetAntiRecalcs() const { return this->anti_recalcs; }
            const Miscellaneous& GetConnectors() const { return this->connectors; }
            const Miscellaneous& GetCustomDashes() const { return this->custom_dashes; }
            const Miscellaneous& GetPatterns() const { return this->patterns; }
            const Miscellaneous& GetStoppers() const { return this->stoppers; }
    };
}

#endif // DATA_H
