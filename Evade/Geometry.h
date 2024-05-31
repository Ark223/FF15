
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "Vector.h"

namespace Evade
{
    class Data;
    class Skillshot;

    class Geometry
    {
        public:
            static std::vector<Vector> Arc(const Vector& p1, const Vector& p2, float phi, float step);
            static std::vector<Vector> ArcSegmentIntersection(const Vector& p1, const Vector& p2, const Vector& a1, const Vector& a2, float range, float angle);
            static std::vector<Vector> CircleSegmentIntersection(const Vector& p1, const Vector& p2, const Vector& circle, float radius);
            static float DynamicCollision(const Vector& a1, const Vector& a2, const Vector& b1, const Vector& b2, float sa, float sb, float ra, float rb);
            static float Interception(const Vector& p1, const Vector& p2, const Vector& vel, float speed, float offset);
            static bool IsInsideCone(const Vector& p1, const Vector& p2, const Vector& pos, float range, float angle);
            static float PathLength(const std::vector<Vector>& path);
    };
}

#endif // GEOMETRY_H
