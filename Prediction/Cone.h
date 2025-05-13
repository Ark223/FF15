
#ifndef CONE_H
#define CONE_H

#include "Data.h"

namespace IPrediction
{
    struct Angle { float Phi; bool Star; };

    class Cone : public IAoeSpell
    {
        using Range = std::pair<size_t, size_t>;

        private:
            float Normalize(float angle);
            Linq<Angle> GetAngles(Linq<Vector>& points);
            AoeSolution FindSolution(Linq<Vector> points);
            void Initialize(Linq<Vector>& points);

        public:
            AoeSolution GetAoeSolution() override;
            void SetCandidates(Linq<Vector> candidates) override;
            void SetStarPoint(const Vector& star_point) override;
            Cone(const PredictionInput& input) : IAoeSpell(input) {}
    };
}

#endif // CONE_H
