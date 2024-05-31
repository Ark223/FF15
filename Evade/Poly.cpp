
#include "Poly.h"

namespace Evade
{
    Poly::Poly() : points(), size(0) {}

    Poly::Poly(const std::vector<Vector>& points) :
        points(points), size(points.size()) {}

    void Poly::Add(const Vector& point)
    {
        this->points.push_back(point);
        this->size++;
    }

    Vector Poly::Get(size_t index) const
    {
        return this->points.at(index);
    }

    void Poly::Set(int index, const Vector& point)
    {
        if (index >= this->size) this->Add(point);
        else this->points[index] = point;
    }

    void Poly::Append(const std::vector<Vector>& points)
    {
        this->points.insert(this->points.end(), points.begin(), points.end());
        this->size += points.size();
    }

    void Poly::Clear()
    {
        this->points.clear();
        this->size = 0;
    }

    void Poly::Remove(size_t index)
    {
        if (index >= this->size) return;
        this->points.erase(this->points.begin() + index);
        this->size--;
    }

    void Poly::Reverse()
    {
        std::reverse(this->points.begin(), this->points.end());
    }

    size_t Poly::Size() const
    {
        return this->size;
    }

    float Poly::Area() const
    {
        float area = 0;
        size_t n = this->size;
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            area += (p2.x - p1.x) * (p2.y + p1.y);
        }
        return -area * 0.5f;
    }

    bool Poly::Orientation() const
    {
        return this->Area() >= 0;
    }

    bool Poly::IsInside(const Vector& point) const
    {
        bool inside = false;
        size_t n = this->size;
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            if ((p1.y <= point.y && p2.y >= point.y) ||
                (p1.y >= point.y && p2.y <= point.y))
            {
                float crs = p1.x + (p2.x - p1.x) *
                    (point.y - p1.y) / (p2.y - p1.y);
                if (crs > point.x) inside = !inside;
                if (crs == point.x) return true;
            }
        }
        return inside;
    }

    bool Poly::IsOutside(const Vector& point) const
    {
        return !this->IsInside(point);
    }

    Poly Poly::Offset(float delta, float step)
    {
        size_t n = this->size;
        if (n < 3) return *this;
        if (delta <= 0 || step <= 0) return *this;
        if (!this->Orientation()) this->Reverse();

        // Precompute the normals for each edge
        Poly result;
        std::vector<Vector> norms(this->size);
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            Vector dir = (p2 - p1).Normalize();
            norms[i] = dir.Perpendicular() * delta;
        }

        // Compute new offset vertices
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            size_t k = i == 1 ? n - 1 : j - 1;
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            const Vector& p3 = this->points[k];
            Vector a = p1 + norms[i], b = p2 + norms[i];
            Vector c = p2 + norms[j], d = p3 + norms[j];
            Vector inter = a.LineIntersection(b, c, d);
            if (!inter.IsValid()) continue;

            // Add rounding to convex corners only
            float phi = M_PI - p2.AngleBetween(p1, p3);
            if (phi < 0) { result.Add(inter); continue; }
            Vector vertex = p2 + (inter - p2).Normalize() * delta;
            result.Append(Geometry::Arc(p2, vertex, phi, step));
        }

        return result;
    }

    Linq<Vector> Poly::PathIntersection(const Vector& p1, const Vector& p2) const
    {
        Linq<Vector> result;
        size_t n = this->size;
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            const Vector& pa = this->points[i];
            const Vector& pb = this->points[j];
            Vector inter = p1.SegmentIntersection(p2, pa, pb);
            if (inter.IsValid()) result.Append(inter);
        }
        return result.Distinct();
    }
}
