
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Data.h"

namespace Prediction
{
    class Circle : public IAoeSpell
    {
        private:
            Vector star_point;
            PredictionInput input;
            Linq<Vector> points;

            int CountCovered(const Solution& circle);
            bool EraseFarthest(const Solution& circle);
            bool IsInside(const Solution& circle, const Vector& point);
            Solution Trivial(const Linq<Vector>& boundary);
            Solution Welzl(Linq<Vector>& boundary);
            Solution FindMEC();

        public:
            AoeSolution GetAoeSolution() override;
            void SetCandidates(Linq<Vector>& points) override;
            void SetStarPoint(const Vector& star_point) override;
    };
}

#endif // CIRCLE_H
