
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Data.h"

namespace Prediction
{
    class Circle : public IAoeSpell
    {
        private:
            int Count(const Solution& circle, Linq<Vector>& points);
            void Erase(const Solution& circle, Linq<Vector>& points);
            bool IsInside(const Solution& circle, const Vector& point);
            Solution Trivial(const Linq<Vector>& boundary);
            Solution Welzl(Linq<Vector>& boundary);
            AoeSolution FindMEC(Linq<Vector> points);

        public:
            AoeSolution GetAoeSolution() override;
            void SetCandidates(Linq<Vector>& points) override;
            void SetStarPoint(const Vector& star_point) override;
            Circle(const PredictionInput& input) : IAoeSpell(input) {}
    };
}

#endif // CIRCLE_H
