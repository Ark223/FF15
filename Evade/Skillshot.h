
#ifndef SKILLSHOT_H
#define SKILLSHOT_H

#include <any>
#include "Data.h"

namespace Evade
{
    using Base = std::unordered_map<std::string, std::any>;

    struct Path
    {
        float Speed, Delay, Delta;
        Vector StartPos, EndPos, Velocity;
        Path(float Speed = FLT_INF, float Delay = 0, float Delta = 0,
            Vector StartPos = Vector(), Vector EndPos = Vector()) :
            StartPos(StartPos), EndPos(EndPos), Speed(Speed),
            Delay(Delay), Delta(Delta) {}
    };

    class Skillshot
    {
        private:
            static int unique_id;

            template<typename T>
            void Assign(const Base& data, const std::string& key, T& var);

        public:
            template<typename T>
            T Get(const std::string& key) const;

            template<typename T>
            void Set(const std::string& key, const T& value);

            void Initialize(const Base& data);
            void LoadData(const Base& data);
            void FixOrigin();
            bool IsExpired() const;
            float TotalLifeTime() const;
            void Translate(const Vector& dir);

            virtual void Draw(uint32_t* colors) = 0;
            virtual void Update(bool force = false) = 0;
            virtual Vector Position(float delta = 0.0f) = 0;
            virtual bool IsSafe(const Vector& pos) = 0;
            virtual bool IsDangerous(const Vector& pos) = 0;
            virtual bool IsPathDangerous(Path& path, bool skip = false) = 0;
            virtual float TimeToHit(const Vector& pos, bool skip = false) = 0;
            virtual Linq<Vector> PathIntersection(const Vector& p1, const Vector& p2) = 0;

        protected:
            int danger_level = 0, slot = 0;
            bool soft_cc = false, hard_cc = false,
                fixed_range = false, fog_of_war = false, invert = false,
                processed = false, rotate90 = false, undodgeable = false;
            float arc_step = 0.0f, cone_angle = 0.0f, damage = 0.0f,
                extra_duration = 0.0f, delay = 0.0f, height = 0.0f,
                hitbox = 0.0f, offset = 0.0f, range = 0.0f, radius = 0.0f,
                inner_radius = 0.0f, outer_radius = 0.0f, rot_angle = 0.0f,
                side_range = 0.0f, speed = 0.0f, start_time = 0.0f;
            DetectionType detection_type = DetectionType::UNDEFINED;
            SkillshotType skillshot_type = SkillshotType::NONE;
            Vector dest_pos, end_pos, origin_pos, start_pos,
                prev_pos, position, direction, perpendicular;
            std::vector<CollisionFlag> collisions;
            std::string caster_name, name;
            Poly offset_polygon, polygon;
            Obj_AI_Hero* caster = nullptr;

            bool IsPathDangerousInternal(Path& path, bool skip);
            float TimeToHitInternal() const;
            Vector UpdateVelocity(Path& path);
    };

    template<typename T>
    T Skillshot::Get(const std::string& key) const
    {
        if constexpr(std::is_same_v<T, int>)
        {
            if (key == "DangerLevel") return this->danger_level;
            if (key == "UniqueId") return this->unique_id;
        }
        if constexpr(std::is_same_v<T, float>)
        {
            if (key == "ArcStep") return this->arc_step;
            if (key == "Damage") return this->damage;
            if (key == "Delay") return this->delay;
            if (key == "Offset") return this->offset;
            if (key == "Range") return this->range;
            if (key == "Radius") return this->radius;
            if (key == "InnerRadius") return this->inner_radius;
            if (key == "OuterRadius") return this->outer_radius;
            if (key == "Speed") return this->speed;
            if (key == "StartTime") return this->start_time;
            if (key == "Hitbox") return this->hitbox;
        }
        if constexpr(std::is_same_v<T, bool>)
        {
            if (key == "FixedRange") return this->fixed_range;
            if (key == "FogOfWar") return this->fog_of_war;
            if (key == "SoftCC") return this->soft_cc;
            if (key == "HardCC") return this->hard_cc;
            if (key == "Processed") return this->processed;
            if (key == "Undodgeable") return this->undodgeable;
        }
        if constexpr(std::is_same_v<T, Vector>)
        {
            if (key == "Direction") return this->direction;
            if (key == "Perpendicular") return this->perpendicular;
            if (key == "DestPos") return this->dest_pos;
            if (key == "EndPos") return this->end_pos;
            if (key == "OriginPos") return this->origin_pos;
            if (key == "StartPos") return this->start_pos;
            if (key == "Position") return this->position;
        }
        if constexpr(std::is_same_v<T, Poly>)
        {
            if (key == "Polygon") return this->polygon;
            if (key == "OffsetPoly") return this->offset_polygon;
        }
        if constexpr(std::is_same_v<T, std::string>)
        {
            if (key == "CasterName") return this->caster_name;
            if (key == "SkillshotName") return this->name;
        }
        if constexpr(std::is_same_v<T, std::vector<CollisionFlag>>)
        {
            return this->collisions;
        }
        if constexpr(std::is_same_v<T, Obj_AI_Hero*>)
        {
            return this->caster;
        }
        if constexpr(std::is_same_v<T, SkillshotType>)
        {
            return this->skillshot_type;
        }
        if constexpr(std::is_same_v<T, DetectionType>)
        {
            return this->detection_type;
        }
        return T();
    }

    template<typename T>
    void Skillshot::Set(const std::string& key, const T& value)
    {
        if constexpr(std::is_same_v<T, int>)
        {
            this->danger_level = value;
        }
        if constexpr(std::is_same_v<T, float>)
        {
            if (key == "ArcStep") this->arc_step = value;
            if (key == "Damage") this->damage = value;
            if (key == "Delay") this->delay = value;
            if (key == "Offset") this->offset = value;
            if (key == "Range") this->range = value;
            if (key == "Radius") this->radius = value;
            if (key == "InnerRadius") this->inner_radius = value;
            if (key == "OuterRadius") this->outer_radius = value;
            if (key == "Speed") this->speed = value;
            if (key == "StartTime") this->start_time = value;
            if (key == "Hitbox") this->hitbox = value;
        }
        if constexpr(std::is_same_v<T, bool>)
        {
            if (key == "FixedRange") this->fixed_range = value;
            if (key == "FogOfWar") this->fog_of_war = value;
            if (key == "SoftCC") this->soft_cc = value;
            if (key == "HardCC") this->hard_cc = value;
            if (key == "Processed") this->processed = value;
            if (key == "Undodgeable") this->undodgeable = value;
        }
        if constexpr(std::is_same_v<T, Vector>)
        {
            if (key == "Direction") this->direction = value;
            if (key == "Perpendicular") this->perpendicular = value;
            if (key == "DestPos") this->dest_pos = value;
            if (key == "EndPos") this->end_pos = value;
            if (key == "OriginPos") this->origin_pos = value;
            if (key == "StartPos") this->start_pos = value;
            if (key == "Position") this->position = value;
        }
        if constexpr(std::is_same_v<T, Poly>)
        {
            if (key == "Polygon") this->polygon = value;
            if (key == "OffsetPoly") this->offset_polygon = value;
        }
        if constexpr(std::is_same_v<T, std::string>)
        {
            if (key == "CasterName") this->caster_name = value;
            if (key == "SkillshotName") this->name = value;
        }
        if constexpr(std::is_same_v<T, std::vector<CollisionFlag>>)
        {
            this->collisions = value;
        }
        if constexpr(std::is_same_v<T, Obj_AI_Hero*>)
        {
            this->caster = value;
        }
        if constexpr(std::is_same_v<T, SkillshotType>)
        {
            this->skillshot_type = value;
        }
        if constexpr(std::is_same_v<T, DetectionType>)
        {
            this->detection_type = value;
        }
    }
}

#endif // SKILLSHOT_H
