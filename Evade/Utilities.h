
#ifndef UTILITIES_H
#define UTILITIES_H

#include "Program.h"

namespace Evade
{
    class Program;

    struct Solution
    {
        Vector Destination;
        SpellInfo SpellInfo;
        Object Target = nullptr;
        float TimeToReach = 0.0f;
        bool IsValid = false;
    };

    class Utilities
    {
        private:
            API* api = nullptr;
            Data* data = nullptr;
            Program* program = nullptr;
            Obj_AI_Hero hero = nullptr;
            static Utilities* m_instance;

        public:
            static void Destroy();
            static Utilities* Get();

            bool CanEvade() const;
            void DrawArrow(const Vector& start, const Vector& ending,
                float height, uint32_t color_a, uint32_t color_b);
            bool EvadeToSpot(const Vector& spot) const;
            Vector Execute(Solution& solution) const;
            Vector ExtendPath(const Vector& destination, float buffer) const;
            Linq<Vector> FindSafeSpots(Linq<Skillshot*>& considered, Path& path,
                float max_range, float dash_range, float delta, bool fixed_range);
            std::pair<Vector, Vector> FindSegment(const Poly& poly, Vector& point);
            Solution FindSolution(Linq<Skillshot*>& skillshots, bool any_dynamic);
            Obj_AI_Hero GetObjectOwner(const std::string& name);
            void ShieldAllies(const SpellInfo& shield_spell);
            bool ShouldEvade(Linq<Skillshot*>& skillshots,
                Solution& solution, bool any_dynamic) const;
            float TimeToRecall(float extra_delay = 0.034f);
            void UpdateItemSlots(Linq<SpellInfo>& spells);
    };
}

#endif // UTILITIES_H
