
#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI / 2.0f)
#endif

#ifndef M_PI_2_F
#define M_PI_2_F (M_PI * 2.0f)
#endif

#ifndef ABS
#define ABS(v) ((v) < 0 ? -(v) : (v))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

namespace Evade
{
    class Vector
    {
        public:
            float x, y;
            explicit Vector(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

            bool operator==(const Vector& v) const;
            bool operator!=(const Vector& v) const;
            Vector operator+(const Vector& v) const;
            Vector operator-(const Vector& v) const;
            Vector operator*(const Vector& v) const;
            Vector operator*(float s) const;
            Vector operator/(const Vector& v) const;
            Vector operator/(float s) const;
            Vector& operator+=(const Vector& v);
            Vector& operator-=(const Vector& v);
            Vector& operator*=(const Vector& v);
            Vector& operator*=(float s);
            Vector& operator/=(const Vector& v);
            Vector& operator/=(float s);
            Vector operator-() const;
            std::string ToString() const;

            Vector Clone() const;
            bool IsValid() const;
            void Swap(Vector& v) noexcept;
            float CrossProduct(const Vector& v) const;
            float DotProduct(const Vector& v) const;
            float Distance(const Vector& v) const;
            float DistanceSquared(const Vector& v) const;
            float Length() const;
            float LengthSquared() const;
            Vector Normalize() const;
            Vector Perpendicular() const;
            Vector Perpendicular2() const;
            Vector Round() const;
            Vector Append(const Vector& v, float dist) const;
            Vector Extend(const Vector& v, float dist) const;
            Vector Rotate(float phi, const Vector& v = Vector()) const;
            Vector ClosestOnSegment(const Vector& v1, const Vector& v2) const;
            float DistSqrToSegment(const Vector& v1, const Vector& v2) const;
            float AngleBetween(const Vector& v1, const Vector& v2, bool normalize = false) const;
            Vector LineIntersection(const Vector& a2, const Vector& b1, const Vector& b2) const;
            Vector SegmentIntersection(const Vector& a2, const Vector& b1, const Vector& b2) const;
    };
}

namespace std
{
    template <>
    struct hash<Evade::Vector>
    {
        size_t operator()(const Evade::Vector& v) const
        {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };
}

#endif // VECTOR_H
