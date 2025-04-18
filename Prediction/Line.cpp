
#include "Line.h"

namespace IPrediction
{
    // Helper methods

    float Line::Normalize(float angle)
    {
        // Normalize an angle to [-π, π) boundary
        while (angle < -M_PI_F) angle += 2.0f * M_PI_F;
        while (angle >= M_PI_F) angle -= 2.0f * M_PI_F;
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
            events.AddRange({{alpha, +1}, {beta, -1}});
        });

        // Shift events by 2π to handle wraparound in sweep
        for (size_t id = 0; id < 2 * points.Count(); ++id)
        {
            const Event& event = events.ElementAt(id);
            events.Append({event.Angle + two_pi, event.Delta});
        }

        // Order is used to efficiently sweep through angles
        return events.OrderBy<float>([](const Event& event)
        {
            return event.Angle;
        });
    }

    AoeSolution Line::FindSolution(Linq<Vector> points)
    {
        this->Initialize(points);
        auto events = this->GetEvents(points);
        if (!events.Any()) return {0, Vector()};

        bool max_region = false;
        int best_count = 0, curr_count = 0;
        const float two_pi = 2.0f * M_PI_F;
        const float range = this->input.Range;
        float best_angle, start_angle, widest_region;
        best_angle = start_angle = widest_region = 0.0f;

        // First pass: determine maximum coverage
        events.ForEach([&](const Event& event)
        {
            curr_count = curr_count + event.Delta;
            best_count = MAX(best_count, curr_count);
        });
        curr_count = 0;

        // Compute the wrapped width between two angles
        auto wrap_width = [&](float start, float ending)
        {
            float width = ending - start;
            return width < 0 ? width + two_pi : width;
        };

        // Update best angle if a new region is wider
        auto update_angle = [&](float ending) -> void
        {
            float width = wrap_width(start_angle, ending);
            if (width > widest_region)
            {
                widest_region = width;
                float phi = start_angle + width * 0.5f;
                best_angle = this->Normalize(phi);
            }
        };

        // Find the widest region with most hits
        events.ForEach([&](const Event& event)
        {
            int prev_count = curr_count;
            curr_count += event.Delta;

            // Entered a region that hits the maximum number of targets
            if (prev_count < best_count && curr_count == best_count)
            {
                start_angle = event.Angle;
                max_region = true;
            }

            // Exiting a region including maximum coverage, update solution
            else if (prev_count == best_count && curr_count < best_count)
            {
                update_angle(event.Angle);
                max_region = false;
            }
        });

        // Handle edge case where max region wraps around to the start
        if (max_region) update_angle(events.First().Angle + two_pi);

        // Convert the final angle to a directional vector and return
        Vector direction(std::cosf(best_angle), std::sinf(best_angle));
        return AoeSolution(best_count, this->source + direction * range);
    }

    // Core methods

    void Line::Initialize(Linq<Vector>& points)
    {
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
