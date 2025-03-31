
#ifndef UTILITIES_H
#define UTILITIES_H

#include "Circle.h"
#include "Geometry.h"
#include "Program.h"

namespace IPrediction
{
    class Program;

    class Utilities
    {
        private:
            API* api = nullptr;
            Data* data = nullptr;
            Network* network = nullptr;
            Program* program = nullptr;
            Obj_AI_Hero hero = nullptr;
            static Utilities* m_instance;

        public:
            static void Destroy();
            static Utilities* Get();

            void DrawPath(const Path& path, float height, uint32_t* colors);
            AoeSolution GetAoeSolution(const PredictionInput& aoe_spell_input,
                const Linq<Vector>& candidates, const Vector& star_point) const;
            std::vector<CollisionData> GetCollision(const PredictionInput& input,
                const Vector& destination, float buffer, uint32_t exclude_id = 0U) const;
            void GetHitChance(const PredictionInput& input, PredictionOutput& output) const;
            PredictionOutput PredictOnPath(const PredictionInput& input) const;

            float GetImmobilityTime(const Obj_AI_Base& unit) const;
            float GetImmunityTime(const Obj_AI_Base& unit) const;
            float GetTotalLatency(int tick_counter = 1) const;
            Path GetWaypoints(const Obj_AI_Base& unit) const;
            bool ShouldCast(const Obj_AI_Base& unit) const;
    };
}

#endif // UTILITIES_H
