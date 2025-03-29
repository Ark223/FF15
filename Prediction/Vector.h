
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <string>

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38f
#endif

#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38f
#endif

#ifndef M_PI_F
#define M_PI_F 3.14159265358979323846f
#endif

#ifndef M_PI_2_F
#define M_PI_2_F (M_PI_F / 2.0f)
#endif

#ifndef M_DEG
#define M_DEG(v) ((v) / M_PI_F * 180.0f)
#endif

#ifndef M_RAD
#define M_RAD(v) ((v) * M_PI_F / 180.0f)
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

namespace IPrediction
{
    constexpr float EPSILON = 1e-2f;

    inline static bool IsInfinite(float s)
    {
        return s >= FLT_MAX || s <= -FLT_MAX;
    }

    inline static bool IsZero(float s)
    {
        return ABS(s) <= EPSILON;
    }

    template<typename T>
    inline void Swap(T& a, T& b)
    {
        T temp = static_cast<T&&>(a);
        a = static_cast<T&&>(b);
        b = static_cast<T&&>(temp);
    }

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
            void SwapXY(Vector& v) noexcept;
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
    struct hash<IPrediction::Vector>
    {
        size_t operator()(const IPrediction::Vector& v) const
        {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };
}

#endif // VECTOR_H
