
#include "Cone.h"

namespace IPrediction
{
    // Helper methods

    float Cone::Normalize(float angle)
    {
        const float two_pi = 2.0f * M_PI_F;
        while (angle < -M_PI_F) angle += two_pi;
        while (angle >= M_PI_F) angle -= two_pi;
        return angle;
    }

    Linq<Angle> Cone::GetAngles(Linq<Vector>& points)
    {
        const float two_pi = 2.0f * M_PI_F;

        // Convert each point to its angle relative to source
        auto angles = points.Select<Angle>([&](auto& point)
        {
            Vector diff = point - this->source;
            float angle = std::atan2f(diff.y, diff.x);
            bool star = point == this->star_point;
            return Angle({ angle, star });
        });

        // Shift all by 2π to handle circular wraparound
        for (size_t id = 0; id < points.Count(); ++id)
        {
            const Angle& data = angles.ElementAt(id);
            const float angle = data.Phi + two_pi;
            angles.Append({ angle, data.Star });
        }

        // Sort by angle to prepare for sliding window sweep
        return angles.OrderBy<float>([](const Angle& data)
        {
            return data.Phi;
        });
    }

    AoeSolution Cone::FindSolution(Linq<Vector> points)
    {
        this->Initialize(points);
        size_t size = points.Count();
        if (size == 0) return {0, Vector()};
        if (size == 1) return {1, points.First()};

        int best_count = 0, right = 0;
        auto angles = this->GetAngles(points);
        float angle = M_RAD(this->input.Angle);
        float best_mid = angles.First().Phi;
        const auto& array = angles.ToArray();

        // Find the star target required for solution
        int star = angles.IndexOf([&](const Angle& data)
        {
            return data.Star;
        });

        // Slide window from each starting angle
        for (int left = 0; left < size; ++left)
        {
            // Expand right side of window while angle fits
            while (right < left + size && angle + EPSILON >=
                array[right].Phi - array[left].Phi) right++;

            // Skip this window if star target is outside of it
            if (star > 0 && !(star >= left && star < right || star
                + size >= left && star + size < right)) continue;

            // Update match if window is larger
            if (right - left > best_count)
            {
                float last = array[right - 1].Phi;
                best_mid = array[left].Phi + last;
                best_count = right - left;
            }
        }

        // Return solution using best direction
        best_mid = this->Normalize(best_mid * 0.5f);
        Vector dir(std::cosf(best_mid), std::sinf(best_mid));
        Vector spot = this->source + dir * this->input.Range;
        return AoeSolution(best_count, spot);
    }

    // Core methods

    void Cone::Initialize(Linq<Vector>& points)
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
    }

    AoeSolution Cone::GetAoeSolution()
    {
        return this->FindSolution(this->candidates);
    }

    void Cone::SetCandidates(Linq<Vector> candidates)
    {
        this->candidates = candidates;
    }

    void Cone::SetStarPoint(const Vector& star_point)
    {
        this->star_point = star_point;
    }
}
