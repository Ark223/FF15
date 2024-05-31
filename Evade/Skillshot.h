
#ifndef SKILLSHOT_H
#define SKILLSHOT_H

#include <any>
#include "Data.h"

namespace Evade
{
    using MainData = std::unordered_map<std::string, std::any>;

    struct Path
    {
        Vector start_pos, end_pos;
        float speed, delay, delta;
        Path(float speed = FLT_INF, float delay = 0, float delta = 0,
            Vector start_pos = Vector(), Vector end_pos = Vector()) :
            start_pos(start_pos), end_pos(end_pos), speed(speed),
            delay(delay), delta(delta) {}
    };

    class Skillshot
    {
        private:
            static int unique_id;

            template<typename T>
            void Assign(const MainData& data, const std::string& key, T& var);

        public:
            void Initialize(const MainData& data);
            void LoadData(const MainData& data);
            void FixOrigin();
            bool IsExpired();
            float TotalLifeTime();

            virtual void Update() = 0;
            virtual void Draw(uint32_t color) = 0;
            virtual Vector Position(float delta = 0.0f) = 0;
            virtual bool IsSafe(const Vector& pos) = 0;
            virtual bool IsDangerous(const Vector& pos) = 0;
            virtual bool IsPathDangerous(const Path& path) = 0;
            virtual float TimeToHit(const Vector& pos, bool check_safety = true) = 0;
            virtual Linq<Vector> PathIntersection(const Vector& p1, const Vector& p2) = 0;

        protected:
            int danger_level = 0;
            bool soft_cc = false, hard_cc = false,
                fixed_range = false, fog_of_war = false,
                invert = false, processed = false, rotate90 = false;
            float arc_step = 0.0f, cone_angle = 0.0f,
                extra_duration = 0.0f, delay = 0.0f, height = 0.0f,
                hitbox = 0.0f, offset = 0.0f, range = 0.0f, radius = 0.0f,
                inner_radius = 0.0f, outer_radius = 0.0f, rot_angle = 0.0f,
                side_range = 0.0f, speed = 0.0f, start_time = 0.0f;
            DetectionType detection_type = DetectionType::UNDEFINED;
            SkillshotType skillshot_type = SkillshotType::NONE;
            Vector dest_pos, end_pos, prev_pos, start_pos,
                position, direction, perpendicular;
            std::vector<CollisionFlag> collisions;
            std::string caster_name, name, slot;
            Poly offset_polygon, polygon;
            GameObject* caster = nullptr;

            bool IsPathDangerousInternal(const Path& path);
            float TimeToHitInternal();
    };
}

#endif // SKILLSHOT_H
