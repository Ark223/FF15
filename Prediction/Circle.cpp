
#include "Circle.h"

namespace IPrediction
{
    // Helper methods

    int Circle::Count(const Solution& circle, Linq<Vector>& points)
    {
        return (int)points.Count([&](const Vector& point)
        {
            return this->IsInside(circle, point);
        });
    }

    void Circle::Erase(const Solution& circle, Linq<Vector>& points)
    {
        points.Remove(points.MaxBy<float>([&](const Vector& point)
        {
            return point.DistanceSquared(circle.second);
        }));
    }

    bool Circle::IsInside(const Solution& circle, const Vector& point)
    {
        return point.DistanceSquared(circle.second) <= circle.first + EPSILON;
    }

    Solution Circle::Trivial(const Linq<Vector>& boundary, bool repair)
    {
        if (boundary.Count() == 0)
        {
            return {0.0f, Vector()};
        }
        else if (boundary.Count() == 1)
        {
            return {0.0f, boundary.First()};
        }
        else if (boundary.Count() == 2)
        {
            const Vector& first = boundary.First();
            Vector center = (first + boundary.Last()) / 2.0f;
            return {center.DistanceSquared(first), center};
        }

        // For three points, compute the circumcenter
        const Vector& pa = boundary.ElementAt(0);
        const Vector& pb = boundary.ElementAt(1);
        const Vector& pc = boundary.ElementAt(2);

        // Calculate the area of the triangle
        Vector db = (pb - pa), dc = (pc - pa);
        float area = db.CrossProduct(dc);

        // Zero denominator suggests the points do not form triangle
        // So then, return circle defined by the two farthest points
        if (repair && IsZero(area))
        {
            float dab = pa.DistanceSquared(pb) / 4.0f;
            float dac = pa.DistanceSquared(pc) / 4.0f;
            float dbc = pb.DistanceSquared(pc) / 4.0f;
            return (dab >= dac && dab >= dbc) ?
                Solution({ dab, (pa + pb) / 2.0f }) :
                (dac >= dab && dac >= dbc) ?
                Solution({ dac, (pa + pc) / 2.0f }) :
                Solution({ dbc, (pb + pc) / 2.0f });
        }

        // Finally, compute circumcenter for non-collinear points
        float ldb = db.LengthSquared(), ldc = dc.LengthSquared();
        Vector shift = (db * ldc - dc * ldb).Perpendicular();
        Vector center = (pa + shift / (2.0f * area));
        return {center.DistanceSquared(pa), center};
    }

    Solution Circle::Welzl(Linq<Vector> points, Linq<Vector>& boundary)
    {
        auto& array = points.ToArray();
        size_t size = array.size();
        if (size == 0 || boundary.Count() == 3)
            return this->Trivial(boundary, true);

        // Pick a random point from candidates
        size_t index = std::rand() % size--;
        std::swap(array[index], array[size]);
        Vector point = array[size];
        points.RemoveAt(size);

        // Circle is valid if point lies inside it
        Solution circle = this->Welzl(points, boundary);
        if (this->IsInside(circle, point)) return circle;

        // Point must be on the boundary
        boundary.Append(point);
        circle = this->Welzl(points, boundary);
        boundary.RemoveAt(boundary.Count() - 1);
        return circle;
    }

    AoeSolution Circle::FindMEC(Linq<Vector> points)
    {
        while (points.Count() > 0)
        {
            Linq<Vector> empty;
            auto solution = this->Welzl(points, empty);
            int count = this->Count(solution, points);
            AoeSolution result = {count, solution.second};
            if (count == points.Count()) return result;
            this->Erase(solution, points);
        }
        return AoeSolution();
    }

    // Core methods

    AoeSolution Circle::GetAoeSolution()
    {
        return this->FindMEC(this->points);
    }

    void Circle::SetCandidates(Linq<Vector>& points)
    {
        this->points = points;
    }

    void Circle::SetStarPoint(const Vector& star_point)
    {
        this->star_point = star_point;
    }
}
