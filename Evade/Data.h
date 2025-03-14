
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
        ABSORPTION = 0,
        CC_IMMUNITY,
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
        std::string PrimaryName = "";
        ConnectionType ConnectType{};

        Connector(uint32_t hash) : BuffHash(hash) {}
        Connector(std::string name) : PrimaryName(name) {}
        Connector(std::string name, ConnectionType type)
            : PrimaryName(name), ConnectType(type) {}
    };

    struct StatData
    {
        Obj_AI_Hero UnitData = nullptr;
        float Armor, BonusAbilityDamage, BonusArmor,
            BonusAttackDamage, BonusHealth, BonusMana,
            BonusSpellBlock, Health, Lethality, MaxHealth,
            MaxMana, SpellBlock, TargetHealth, TargetMaxHealth,
            TotalAbilityDamage, TotalAttackDamage;
        int SpellLevel, UnitLevel;
    };

    struct ActiveData
    {
        DetectionType DetectionType = DetectionType::UNDEFINED;
        SkillshotType SkillshotType = SkillshotType::NONE;
        Vector Direction, Perpendicular, DestPos, EndPos;
        Vector OriginPos, PrevPos, StartPos, Position;
        std::string CasterName, SkillshotName;
        std::vector<CollisionFlag> Collisions;
        std::vector<Vector> StartArc, EndArc;
        Obj_AI_Hero Caster = nullptr;
        Object ObjectPtr = nullptr;
        Poly OffsetPoly, Polygon;
        DamageFunc DamageFunc{};
        DamageType DamageType{};
        int DangerLevel = 0;
        int SkillshotSlot = 0;
        float ArcStep = 0.0f;
        float ConeAngle = 0.0f;
        float Damage = 0.0f;
        float Delay = 0.0f;
        float ExtraTime = 0.0f;
        float Height = 0.0f;
        float Hitbox = 0.0f;
        float Offset = 0.0f;
        float Range = 0.0f;
        float Radius = 0.0f;
        float InnerRadius = 0.0f;
        float OuterRadius = 0.0f;
        float RotationAngle = 0.0f;
        float SideRange = 0.0f;
        float Speed = 0.0f;
        float StartTime = 0.0f;
        bool FixedRange = false;
        bool FixSpeed = false;
        bool FogOfWar = false;
        bool HardCC = false;
        bool SoftCC = false;
        bool Invert = false;
        bool Processed = false;
        bool Rotate90 = false;
        bool Undodgeable = false;

        ActiveData(const Vector& dest_pos, const Vector& origin_pos,
            const Vector& start_pos, const std::string& skill_name,
            const std::string& caster_name, const Obj_AI_Hero& caster,
            const Object& object, int danger_level, int slot, bool fixed_range,
            bool fog_of_war, bool invert_origin, bool hard_cc, bool soft_cc,
            bool rotate90, float height, float hitbox, float offset,
            float range, float side_range, float radius, float inner_radius,
            float outer_radius, float cone_angle, float delay, float extra_time,
            float speed, const std::vector<CollisionFlag>& collision_table,
            Evade::DetectionType detect_type, Evade::SkillshotType skill_type,
            const Evade::DamageFunc& damage_func, Evade::DamageType damage_type
        ) :
            DestPos(dest_pos), OriginPos(origin_pos), StartPos(start_pos),
            SkillshotName(skill_name), CasterName(caster_name), Caster(caster),
            ObjectPtr(object), DangerLevel(danger_level), SkillshotSlot(slot),
            FixedRange(fixed_range), FogOfWar(fog_of_war), Invert(invert_origin),
            HardCC(hard_cc), SoftCC(soft_cc), Rotate90(rotate90), Height(height),
            Hitbox(hitbox), Offset(offset), Range(range), SideRange(side_range),
            Radius(radius), InnerRadius(inner_radius), OuterRadius(outer_radius),
            ConeAngle(cone_angle), Delay(delay), ExtraTime(extra_time),
            Speed(speed), Collisions(collision_table), DetectionType(detect_type),
            SkillshotType(skill_type), DamageFunc(damage_func), DamageType(damage_type)
        {}
    };

    struct SkillshotData
    {
        std::string ChampionName = "";
        std::string DisplayName = "";
        std::string SkillshotName = "";
        std::string IconName = "";
        std::string MissileName = "";
        std::string ParticleName = "";
        std::string Overrider = "";
        std::vector<std::string> Exclusions;
        std::vector<CollisionFlag> Collisions;
        std::vector<DetectionType> Detectors;
        SkillshotType SkillshotType{};
        DamageType DamageType{};
        DamageFunc Damage{};
        char SkillshotSlot = 0;
        int DangerLevel = 1;
        int MultiAngle = 0;
        int MultiCount = 0;
        float ConeAngle = 0.0f;
        float Delay = 0.0f;
        float ExtraTime = 0.0f;
        float Windup = 0.0f;
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
        float Speed = FLT_MAX;
        bool AddHitbox = false;
        bool Dangerous = false;
        bool Exception = false;
        bool FastEvade = false;
        bool FixedRange = false;
        bool FogSupport = false;
        bool HookObject = false;
        bool BackToCaster = false;
        bool FollowCaster = false;
        bool IgnoreAlive = false;
        bool SkipAncestor = false;
        bool SoftCC = false;
        bool HardCC = false;
        bool FixSpeed = false;
        bool Invert = false;
        bool IsGlobal = false;
        bool IsTrap = false;
        bool Rotate90 = false;
        bool UseDirection = false;
        bool UseEmitter = false;
        bool UseWindup = false;
    };

    struct EvadingSpellData
    {
        std::string ChampionName = "";
        std::string DisplayName = "";
        std::string IconName = "";
        std::string SpellName = "";
        EvadingType EvadingType;
        char SpellSlot = ' ';
        int DangerLevel = 1;
        float Delay = 0.0f;
        float Range = 0.0f;
        bool IsTargeted = false;
        std::function<float(API*)> Speed;
        std::function<bool(API*)> Condition;
    };

    struct ShieldSpellData
    {
        std::string ChampionName = "";
        std::string DisplayName = "";
        std::string IconName = "";
        std::string SpellName = "";
        EvadingType ShieldType;
        char SpellSlot = ' ';
        float Delay = 0.0f;
        float Range = 0.0f;
        float Speed = FLT_MAX;
        bool IsTargeted = false;
    };

    // Data class

    class Data
    {
        private:
            SkillshotTable skillshots;
            EvadingSpellTable evaders;
            ShieldSpellTable shields;
            Miscellaneous connectors;
            Miscellaneous dash_buffs;
            Miscellaneous patterns;
            Miscellaneous stoppers;
            static Data* m_instance;

            Data();
            void InitSkillshots();
            void InitEvadingSpells();
            void InitShieldSpells();
            void InitConnectors();
            void InitDashBuffs();
            void InitPatterns();
            void InitStoppers();

        public:
            static void Destroy();
            static Data* Get();

            const SkillshotTable& GetSkillshots() const { return this->skillshots; }
            const EvadingSpellTable& GetEvadingSpells() const { return this->evaders; }
            const ShieldSpellTable& GetShieldSpells() const { return this->shields; }
            const Miscellaneous& GetConnectors() const { return this->connectors; }
            const Miscellaneous& GetDashBuffs() const { return this->dash_buffs; }
            const Miscellaneous& GetPatterns() const { return this->patterns; }
            const Miscellaneous& GetStoppers() const { return this->stoppers; }
    };
}

#endif // DATA_H
