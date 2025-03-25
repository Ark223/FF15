
#include "Vector.h"

namespace Evade
{
    bool Vector::operator==(const Vector& v) const
    {
        float dx = this->x - v.x;
        float dy = this->y - v.y;
        return IsZero(dx) && IsZero(dy);
    }

    bool Vector::operator!=(const Vector& v) const
    {
        return !(*this == v);
    }

    Vector Vector::operator+(const Vector& v) const
    {
        return Vector(this->x + v.x, this->y + v.y);
    }

    Vector Vector::operator-(const Vector& v) const
    {
        return Vector(this->x - v.x, this->y - v.y);
    }

    Vector Vector::operator*(const Vector& v) const
    {
        return Vector(this->x * v.x, this->y * v.y);
    }

    Vector Vector::operator*(float s) const
    {
        return Vector(this->x * s, this->y * s);
    }

    Vector Vector::operator/(const Vector& v) const
    {
        return Vector(this->x / v.x, this->y / v.y);
    }

    Vector Vector::operator/(float s) const
    {
        return Vector(this->x / s, this->y / s);
    }

    Vector& Vector::operator+=(const Vector& v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    Vector& Vector::operator-=(const Vector& v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    Vector& Vector::operator*=(const Vector& v)
    {
        this->x *= v.x;
        this->y *= v.y;
        return *this;
    }

    Vector& Vector::operator*=(float s)
    {
        this->x *= s;
        this->y *= s;
        return *this;
    }

    Vector& Vector::operator/=(const Vector& v)
    {
        this->x /= v.x;
        this->y /= v.y;
        return *this;
    }

    Vector& Vector::operator/=(float s)
    {
        this->x /= s;
        this->y /= s;
        return *this;
    }

    Vector Vector::operator-() const
    {
        return Vector(-this->x, -this->y);
    }

    std::string Vector::ToString() const
    {
        auto x = std::to_string(this->x);
        auto y = std::to_string(this->y);
        return "(" + x + ", " + y + ")";
    }

    Vector Vector::Clone() const
    {
        return Vector(this->x, this->y);
    }

    bool Vector::IsValid() const
    {
        return this->x != 0.0f && this->y != 0.0f;
    }

    void Vector::SwapXY(Vector& v) noexcept
    {
        Swap(this->x, v.x);
        Swap(this->y, v.y);
    }

    float Vector::CrossProduct(const Vector& v) const
    {
        return this->x * v.y - this->y * v.x;
    }

    float Vector::DotProduct(const Vector& v) const
    {
        return this->x * v.x + this->y * v.y;
    }

    float Vector::Distance(const Vector& v) const
    {
        return std::sqrtf(this->DistanceSquared(v));
    }

    float Vector::DistanceSquared(const Vector& v) const
    {
        float dx = v.x - this->x;
        float dy = v.y - this->y;
        return dx * dx + dy * dy;
    }

    float Vector::Length() const
    {
        return std::sqrtf(this->LengthSquared());
    }

    float Vector::LengthSquared() const
    {
        return this->x * this->x + this->y * this->y;
    }

    Vector Vector::Normalize() const
    {
        float len = this->Length();
        if (IsZero(len)) return Vector();
        return Vector(this->x / len, this->y / len);
    }

    Vector Vector::Perpendicular() const
    {
        return Vector(-this->y, this->x);
    }

    Vector Vector::Perpendicular2() const
    {
        return Vector(this->y, -this->x);
    }

    Vector Vector::Round() const
    {
        float x = std::floor(this->x + 0.5f);
        float y = std::floor(this->y + 0.5f);
        return Vector(x, y);
    }

    Vector Vector::Append(const Vector& v, float dist) const
    {
        if (IsZero(dist)) return v.Clone();
        return v + (v - *this).Normalize() * dist;
    }

    Vector Vector::Extend(const Vector& v, float dist) const
    {
        if (IsZero(dist)) return this->Clone();
        return *this + (v - *this).Normalize() * dist;
    }

    Vector Vector::Rotate(float phi, const Vector& v) const
    {
        bool zero = IsZero(phi);
        if (zero) return this->Clone();

        float c = std::cosf(phi);
        float s = std::sinf(phi);
        Vector d = *this - v;

        float x = c * d.x - s * d.y;
        float y = s * d.x + c * d.y;
        return Vector(x + v.x, y + v.y);
    }

    Vector Vector::ClosestOnSegment(const Vector& v1, const Vector& v2) const
    {
        Vector ap = *this - v1, ab = v2 - v1;
        float t = ap.DotProduct(ab) / ab.LengthSquared();
        return t <= 0 ? (v1) : t >= 1 ? (v2) : (v1 + ab * t);
    }

    float Vector::DistSqrToSegment(const Vector& v1, const Vector& v2) const
    {
        Vector v = this->ClosestOnSegment(v1, v2);
        return this->DistanceSquared(v);
    }

    float Vector::AngleBetween(const Vector& v1, const Vector& v2, bool normalize) const
    {
        Vector d1 = v1 - *this, d2 = v2 - *this;
        float angle = std::atan2f(d2.y, d2.x);
        angle -= std::atan2f(d1.y, d1.x);

        if (angle < 0) angle += 2.0f * M_PI_F;
        bool fix = normalize && angle - M_PI_F > 0;
        return fix ? (2.0f * M_PI_F - angle) : angle;
    }

    Vector Vector::LineIntersection(const Vector& a2, const Vector& b1, const Vector& b2) const
    {
        Vector a = a2 - *this;
        Vector b = b2 - b1;
        float axb = a.CrossProduct(b);
        if (IsZero(axb)) return Vector();

        Vector bsa = b1 - *this;
        float t = bsa.CrossProduct(b);
        return *this + a * t / axb;
    }

    Vector Vector::SegmentIntersection(const Vector& a2, const Vector& b1, const Vector& b2) const
    {
        Vector a = a2 - *this;
        Vector b = b2 - b1;
        float axb = a.CrossProduct(b);
        if (IsZero(axb)) return Vector();

        Vector bsa = b1 - *this;
        float t1 = bsa.CrossProduct(b) / axb;
        float t2 = bsa.CrossProduct(a) / axb;

        return (t1 >= 0 && t1 <= 1 && t2 >= 0 &&
            t2 <= 1) ? (*this + a * t1) : Vector();
    }
}
