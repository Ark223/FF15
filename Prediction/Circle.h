
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Data.h"

namespace IPrediction
{
    class Circle : public IAoeSpell
    {
        private:
            Vector Mean(Linq<Vector>& points);
            int Count(const Solution& circle, Linq<Vector>& points);
            void Erase(const Vector& center, Linq<Vector>& points);
            bool IsInside(const Vector& circle, const Vector& point);
            bool IsInside(const Solution& circle, const Vector& point);
            Solution Trivial(const Linq<Vector>& boundary, bool repair);
            Solution Welzl(Linq<Vector>& p, Linq<Vector>& r, size_t n);
            AoeSolution FindSolution(Linq<Vector> points);
            void Initialize(Linq<Vector>& points);

        public:
            AoeSolution GetAoeSolution() override;
            void SetCandidates(Linq<Vector> candidates) override;
            void SetStarPoint(const Vector& star_point) override;
            Circle(const PredictionInput& input) : IAoeSpell(input) {}
    };
}

#endif // CIRCLE_H
