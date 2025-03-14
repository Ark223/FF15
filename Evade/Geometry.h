
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>
#include <vector>
#include "Vector.h"

namespace Evade
{
    class Geometry
    {
        public:
            static float AngleBetween(const Vector& v1, const Vector& v2);
            static std::array<float, 3> FindRoots(float a, float b, float c, float max, bool simplify);
            static std::vector<Vector> Arc(const Vector& p1, const Vector& p2, float phi, float step);
            static std::vector<Vector> ArcSegmentIntersection(const Vector& p1, const Vector& p2, const Vector& a1, const Vector& a2, float range, float angle);
            static std::vector<Vector> CircleSegmentIntersection(const Vector& p1, const Vector& p2, const Vector& circle, float radius);
            static float DynamicCollision(const Vector& p1, const Vector& p2, const Vector& v1, const Vector& v2, float r1, float r2);
            static float Interception(const Vector& p1, const Vector& p2, const Vector& vel, float speed, float hitbox);
            static bool IsInsideCone(const Vector& p1, const Vector& p2, const Vector& pos, float range, float angle);
            static float PathLength(const std::vector<Vector>& path);
    };
}

#endif // GEOMETRY_H
