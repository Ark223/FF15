
#include "Geometry.h"

namespace Evade
{
    float Geometry::AngleBetween(const Vector& v1, const Vector& v2)
    {
        float dot = v1.DotProduct(v2);
        float cross = v1.CrossProduct(v2);
        return std::atan2(ABS(cross), dot);
    }

    std::vector<Vector> Geometry::Arc(const Vector& p1, const Vector& p2, float phi, float step)
    {
        float angle = -phi * 0.5f;
        std::vector<Vector> result;
        float length = p1.Distance(p2) * phi;
        if (length == 0.0f) return { p1 };
        if (step > length) step = length;
        int steps = (int)(length / step);
        for (int i = 0; i <= steps; ++i)
        {
            Vector rotated = p2.Rotate(angle, p1);
            result.push_back(rotated);
            angle += phi / steps;
        }
        return result;
    }

    std::vector<Vector> Geometry::ArcSegmentIntersection(const Vector& p1,
        const Vector& p2, const Vector& a1, const Vector& a2, float range, float angle)
    {
        auto intersections = CircleSegmentIntersection(p1, p2, a1, range);
        std::vector<Vector> result; float half_angle = angle * 0.5f;
        for (const Vector& intersection : intersections)
        {
            float phi = a1.AngleBetween(intersection, a2);
            if (MIN(phi, 2.0f * M_PI_F - phi) <= half_angle)
            {
                result.push_back(intersection);
            }
        }
        return result;
    }

    std::vector<Vector> Geometry::CircleSegmentIntersection(
        const Vector& p1, const Vector& p2, const Vector& circle, float radius)
    {
        std::vector<Vector> result;
        if (radius <= 0) return result;
        Vector dp = p2 - p1;
        Vector dc = p1 - circle;
        float sqr = radius * radius;
        float a = dp.LengthSquared();
        float b = 2.0f * dc.DotProduct(dp);
        float c = dc.LengthSquared() - sqr;
        float delta = b * b - 4.0f * a * c;
        if (delta < 0) return result;
        delta = std::sqrtf(delta);
        float t1 = (-b - delta) / (2.0f * a);
        float t2 = (-b + delta) / (2.0f * a);
        bool u1 = (t1 >= 0.0f && t1 <= 1.0f);
        bool u2 = (t2 >= 0.0f && t2 <= 1.0f);
        if (u1) result.push_back(p1 + dp * t1);
        if (u2) result.push_back(p1 + dp * t2);
        return result;
    }

    float Geometry::DynamicCollision(const Vector& p1, const Vector& p2,
        const Vector& v1, const Vector& v2, float r1, float r2)
    {
        Vector dp = p1 - p2, dv = v1 - v2;
        float hitbox = (r1 + r2) * (r1 + r2);
        float a = dv.LengthSquared();
        float b = 2.0f * dp.DotProduct(dv);
        float c = dp.LengthSquared() - hitbox;
        if (IsZero(a)) return MAX(0.0f, -c / b);
        float delta = b * b - 4.0f * a * c;
        if (delta < 0) return -FLT_MAX;
        delta = std::sqrtf(delta);
        float t1 = (-b - delta) / (2.0f * a);
        float t2 = (-b + delta) / (2.0f * a);
        return MAX(0.0f, MIN(t1, t2));
    }

    float Geometry::Interception(const Vector& p1,
        const Vector& p2, const Vector& vel, float speed, float hitbox)
    {
        Vector dir = p1 - p2;
        float offset = hitbox * speed;
        float a = vel.LengthSquared() - speed * speed;
        float b = 2.0f * (vel.DotProduct(dir) - offset);
        float c = dir.LengthSquared() - hitbox * hitbox;
        if (IsZero(a)) return MAX(0.0f, -c / b);
        float delta = b * b - 4.0f * a * c;
        if (delta < 0) return -FLT_MAX;
        delta = std::sqrtf(delta);
        float t1 = (-b - delta) / (2.0f * a);
        float t2 = (-b + delta) / (2.0f * a);
        float maxv = MAX(t1, t2), minv = MIN(t1, t2);
        return MAX(0.0f, minv < 0 ? maxv : minv);
    }

    bool Geometry::IsInsideCone(const Vector& p1,
        const Vector& p2, const Vector& pos, float range, float angle)
    {
        if (range <= 0) return false;
        float distance = p1.DistanceSquared(pos);
        bool outside = distance > range * range;
        if (outside == true) return false;
        float phi = p1.AngleBetween(pos, p2);
        phi = MIN(phi, 2.0f * M_PI_F - phi);
        return phi <= angle * 0.5f;
    }

    float Geometry::PathLength(const std::vector<Vector>& path)
    {
        float distance = 0.0f;
        size_t size = path.size() - 1;
        for (size_t i = 0; i < size; ++i)
        {
            const Vector& p1 = path[i];
            const Vector& p2 = path[i + 1];
            distance += p1.Distance(p2);
        }
        return distance;
    }
}
