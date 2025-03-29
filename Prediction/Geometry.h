
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <functional>
#include <vector>
#include "Vector.h"

namespace IPrediction
{
    struct Segment;

    using Path = std::vector<Segment>;
    using Wall = std::vector<Vector>;
    using Obstacle = std::pair<float, Path>;
    using Solution = std::pair<float, Vector>;
    using Collision = std::pair<size_t, Solution>;

    struct Segment
    {
        Vector StartPos = Vector();
        Vector EndPos = Vector();
        Vector Direction = Vector();
        Vector Velocity = Vector();
        float Length = 0.0f;
        float Speed = 0.0f;

        Segment() = default;

        Segment(const Vector& position, float speed)
        {
            this->Speed = speed;
            this->StartPos = position;
            this->EndPos = position;
        }

        Segment(const Vector& start, const Vector& ending, float speed)
        {
            this->Speed = speed;
            this->Direction = ending - start;
            this->Length = this->Direction.Length();
            this->StartPos = start; this->EndPos = ending;
            this->Direction = this->Direction / this->Length;
            this->Velocity = this->Direction * this->Speed;
        }
    };

    class Geometry
    {
        public:
            static float Angle(const Vector& v1, const Vector& v2);
            static Path CutPath(const Path& path, float length);
            static float FindRoot(float a, float b, float c, float max);
            static std::vector<Collision> DynamicCollision(const Segment& missile,
                const std::vector<Obstacle>& obstacles, float hitbox, size_t* hit_index = 0);
            static Solution Interception(const Path& path, const Vector& source, float speed);
            static Solution Intersection(const Segment& missile, const std::vector<Vector>& poly);
            static Vector PositionAfter(const Path& path, float time);

    };
}

#endif // GEOMETRY_H
