
#include "Line.h"

namespace IPrediction
{
    // Helper methods

    float Line::Normalize(float angle)
    {
        const float two_pi = 2.0f * M_PI_F;
        while (angle < -M_PI_F) angle += two_pi;
        while (angle >= M_PI_F) angle -= two_pi;
        return angle;
    }

    Interval Line::GetInterval(const Vector& point)
    {
        Vector diff = point - this->source;
        const float radius = this->input.Radius;
        float length = diff.Length(), half = M_PI_2_F;

        // Candidate is at source: allow full half-circle
        if (IsZero(length)) return Interval({-half, half});

        // Compute maximum deviation: |r*sin(delta)| <= radius
        float delta = std::asinf(MIN(1.0f, radius / length));
        float phi = std::atan2f(diff.y, diff.x);

        // Symmetric interval around base angle
        float alpha = this->Normalize(phi - delta);
        float beta = this->Normalize(phi + delta);
        return Interval({alpha, beta});
    }

    Linq<Event> Line::GetEvents(Linq<Vector>& points)
    {
        const float two_pi = 2.0f * M_PI_F;
        Linq<Event> events = Linq<Event>();

        // Build allowed intervals for each point
        points.ForEach([&](const Vector& point)
        {
            Interval iv = this->GetInterval(point);
            float alpha = iv.Alpha, beta = iv.Beta;
            beta += (alpha > beta) ? two_pi : 0.0f;
            events.Append({ alpha, +1, point });
            events.Append({ beta, -1, point });
        });

        // Shift events by 2π to handle wraparound in sweep
        for (size_t id = 0; id < 2 * points.Count(); ++id)
        {
            const auto& event = events.ElementAt(id);
            const float angle = event.Angle + two_pi;
            events.Append({ angle, event.Delta, event.Point });
        }

        // Order is used to efficiently sweep through angles
        return events.OrderBy<float>([](const Event& event)
        {
            return event.Angle;
        });
    }

    Range Line::GetSweepRange(const Linq<Event>& events)
    {
        const auto& begin = events.begin();
        const auto& ending = events.end();
        const Vector& star = this->star_point;

        // If star exists, restrict to its enter-exit range
        if (star.IsValid() && this->source.DistanceSquared(
            star) <= this->input.Range * this->input.Range)
        {
            // Find the angle where the star first becomes counted
            auto enter = std::find_if(begin, ending, [&](auto& event)
            {
                return star == event.Point && event.Delta == +1;
            });

            // After that, find the corresponding exit event and return
            auto exit = std::find_if(enter + 1, ending, [&](auto& event)
            {
                return star == event.Point && event.Delta == -1;
            });
            return Range(enter - begin, exit - begin);
        }

        // Otherwise, sweep the full list
        return Range(0, events.Count());
    }

    Linq<Vector> Line::FindCovered(Linq<Vector>& points,
        Vector& direction, Linq<float>& laterals)
    {
        if (laterals.Any()) laterals.Clear();
        const float range = this->input.Range;
        const float radius = this->input.Radius;
        Vector perpend = direction.Perpendicular();

        // Return points that lie within oriented slab
        return points.Where([&](const Vector& point)
        {
            // Lateral check (distance to side edge)
            Vector diff = (point - this->source);
            float lateral = ABS(diff.DotProduct(perpend));
            if (lateral > radius + EPSILON) return false;

            // Forward check (distance along direction)
            float forward = diff.DotProduct(direction);
            if (forward > range + EPSILON) return false;
            if (forward < 0.0f) return false;

            laterals.Append(lateral);
            return true;
        });
    }

    AoeSolution Line::FindSolution(Linq<Vector> points)
    {
        this->Initialize(points);
        size_t size = points.Count();
        if (size == 0) return {0, Vector()};
        if (size == 1) return {1, points.First()};

        Linq<float> laterals;
        float best_angle = 0.0f;
        int best_count = 0, curr_count = 0;
        Vector edge_point = points.First();

        auto events = this->GetEvents(points);
        auto range = this->GetSweepRange(events);
        const size_t begin_idx = range.first;
        const size_t end_idx = range.second;

        // Sweep to find angle covering the most points
        for (size_t id = begin_idx; id < end_idx; ++id)
        {
            const Event& event = events.ElementAt(id);
            if ((curr_count += event.Delta) > best_count)
            {
                best_count = curr_count;
                best_angle = event.Angle;
                edge_point = event.Point;
            }
        }

        // Find all points initially covered by the best direction
        Vector dir(std::cosf(best_angle), std::sinf(best_angle));
        auto inside = this->FindCovered(points, dir, laterals);
        Result result = Result({1, -1.0f, points.First()});

        // Avoid bad solutions if initial point lies at the start
        if ((edge_point - this->source).DotProduct(dir) < EPSILON)
        {
            Vector spot = this->source + dir * this->input.Range;
            result = Result({(int)inside.Count(), -1.0f, spot});
        }

        // Build convex hull around covered points for scan
        auto hull = Geometry::FindHull(inside.ToArray());

        // Evaluate pairs to optimize centering
        for (size_t i = 0; i < hull.size(); ++i)
        {
            for (size_t j = i + 1; j < hull.size(); ++j)
            {
                // Compute bisector direction between points
                Vector middle = (hull[i] + hull[j]) * 0.5f;
                Vector dir = (middle - this->source).Normalize();
                Vector spot = this->source + dir * this->input.Range;
                auto check = this->FindCovered(inside, dir, laterals);
                int count = (int)check.Count(), covered = result.Count;

                // Find the smallest gap from any point to the rectangle's side wall
                float margin = laterals.Aggregate(FLT_MAX, [&](float acc, float lat)
                {
                    return MIN(acc, this->input.Radius - lat);
                });

                // Update result if there is better coverage or spacing
                bool wider = count == covered && margin > result.Margin;
                if (count > covered || wider) result = {count, margin, spot};
            }
        }

        // Return solution using best refined direction
        return AoeSolution(result.Count, result.Spot);
    }

    // Core methods

    void Line::Initialize(Linq<Vector>& points)
    {
        using type = std::pair<float, float>;
        this->source = this->input.SourcePosition;
        const auto& object = this->input.SourceObject;

        // Use object position if object is valid
        if (object && this->api->IsValid(object))
        {
            this->source = this->api->GetPosition(object);
        }

        // Remove candidates beyond spell range
        points.RemoveAll([&](const Vector& point)
        {
            return this->source.DistanceSquared(point) >
                this->input.Range * this->input.Range;
        });

        // Sort points lexicographically for hull computations
        points = points.OrderBy<type>([&](const Vector& point)
        {
            return std::make_pair(point.x, point.y);
        });
    }

    AoeSolution Line::GetAoeSolution()
    {
        return this->FindSolution(this->candidates);
    }

    void Line::SetCandidates(Linq<Vector> candidates)
    {
        this->candidates = candidates;
    }

    void Line::SetStarPoint(const Vector& star_point)
    {
        this->star_point = star_point;
    }
}
