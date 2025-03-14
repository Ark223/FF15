
#ifndef DATA_H
#define DATA_H

#include "API.h"
#include "Geometry.h"

namespace Prediction
{
    struct CollisionData;
    struct DashData;
    struct PathData;

    using AoeSolution = std::pair<int, Vector>;
    using CollisionOutput = std::vector<CollisionData>;
    using DashTable = std::unordered_map<std::string, DashData>;

    // Enum types

    enum class CollisionFlag
    {
        None = 0,
        Heroes = 1 << 0,
        Minions = 1 << 1,
        Terrain = 1 << 2,
        WindWall = 1 << 3
    };

    enum class HitChance
    {
        Impossible = 0,
        Collision,
        OutOfRange,
        Low,
        Medium,
        High,
        VeryHigh,
        Guaranteed
    };

    enum class SpellType
    {
        None = 0,
        Linear,
        Circular
    };

    // Structures

    struct CollisionData
    {
        Obj_AI_Base Object = nullptr;
        Vector Position = Vector();
        CollisionFlag CollisionFlag{};
    };

    struct DashData
    {
        std::string SpellName;
        Path TotalPath = Path();
        Path Waypoints = Path();
        float Delay = 0.0f;
        float Range = 0.0f;
        float Offset = 0.0f;
        float Speed = 0.0f;
        float StartTime = 0.0f;
        bool FixedRange = false;
        bool IsBlink = false;
    };

    struct PathData
    {
        Path Waypoints = Path();
        Vector Direction = Vector();
        Vector StartPos = Vector();
        Vector EndPos = Vector();
        float MiaTime = -1.0f;
        float UpdateTime = 0.0f;
        float PathLength = 0.0f;

        PathData(const Path& waypoints)
        {
            size_t last = waypoints.size() - 1;
            this->EndPos = waypoints[last].EndPos;
            this->StartPos = waypoints[0].StartPos;
            this->Direction = this->EndPos - this->StartPos;
            this->Direction = this->Direction.Normalize();
            this->UpdateTime = API::Get()->GetTime();
            this->PathLength = GetLength(waypoints);
            this->Waypoints = waypoints;
        }

        float GetLength(const Path& path);
    };

    struct PredictionInput
    {
        Vector SourcePosition = Vector();
        Obj_AI_Base SourceObject = nullptr;
        Obj_AI_Base TargetObject = nullptr;
        SpellType SpellType = SpellType::None;
        uint32_t CollisionFlags = 0U;
        uint32_t MaxCollisions = 0U;
        float Angle = 180.0f;
        float Delay = 0.0f;
        float Radius = 1.0f;
        float Width = 1.0f;
        float Range = FLT_MAX;
        float Speed = FLT_MAX;
        bool AddHitbox = false;
        bool AoeSupport = false;
        bool EdgeCast = false;
    };

    struct PredictionOutput
    {
        Vector CastPosition = Vector();
        Vector TargetPosition = Vector();
        HitChance HitChance = HitChance::Impossible;
        std::vector<CollisionData> Collisions{};
        uint32_t AoeHitCount = 0U;
        float Confidence = 0.0f;
        float Distance = 0.0f;
        float TimeToHit = 0.0f;
    };

    // Classes

    class IAoeSpell
    {
        public:
            virtual ~IAoeSpell() = default;
            virtual AoeSolution GetAoeSolution() = 0;
            virtual void SetCandidates(Linq<Vector>& points) = 0;
            virtual void SetStarPoint(const Vector& star_point) = 0;
    };

    class Data
    {
        private:
            API* api = nullptr;
            DashTable dashes;
            BuffTable exclusions;
            BuffTable immunities;
            BuffTable immobile_buffs;
            BuffTypes immobile_types;
            static Data* m_instance;

            Data();
            void InitDashSpells();
            void InitExclusionBuffs();
            void InitImmunityBuffs();
            void InitImmobilityBuffs();
            void InitImmobilityTypes();

        public:
            static void Destroy();
            static Data* Get();

            const DashTable& GetDashSpells();
            const BuffTable& GetExclusionBuffs();
            const BuffTable& GetImmunityBuffs();
            const BuffTable& GetImmobilityBuffs();
            const BuffTypes& GetImmobilityTypes();
    };
}

#endif // DATA_H
