
#ifndef LINE_H
#define LINE_H

#include "Data.h"

namespace IPrediction
{
    struct Event { float Angle; int Delta; };
    struct Interval { float Alpha; float Beta; };

    class Line : public IAoeSpell
    {
        private:
            float Normalize(float angle);
            Interval GetInterval(const Vector& point);
            Linq<Event> GetEvents(Linq<Vector>& points);
            AoeSolution FindSolution(Linq<Vector> points);
            void Initialize(Linq<Vector>& points);

        public:
            AoeSolution GetAoeSolution() override;
            void SetCandidates(Linq<Vector> candidates) override;
            void SetStarPoint(const Vector& star_point) override;
            Line(const PredictionInput& input) : IAoeSpell(input) {}
    };
}

#endif // LINE_H
