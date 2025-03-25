
#include "Poly.h"

namespace Evade
{
    Poly::Poly() : points() {}

    Poly::Poly(const std::vector<Vector>& points) : points(points) {}

    void Poly::Add(const Vector& point)
    {
        this->points.push_back(point);
    }

    Vector Poly::Get(size_t index) const
    {
        return this->points.at(index);
    }

    void Poly::Set(int index, const Vector& point)
    {
        if (index >= this->Size()) this->Add(point);
        else this->points[index] = point;
    }

    void Poly::Append(const std::vector<Vector>& points)
    {
        this->points.insert(this->points.end(), points.begin(), points.end());
    }

    void Poly::Clear()
    {
        this->points.clear();
    }

    void Poly::Remove(size_t index)
    {
        if (index >= this->Size()) return;
        this->points.erase(this->points.begin() + index);
    }

    void Poly::Reverse()
    {
        std::reverse(this->points.begin(), this->points.end());
    }

    int Poly::Size() const
    {
        return (int)this->points.size();
    }

    float Poly::Area() const
    {
        float area = 0;
        int size = this->Size();
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            area += (p2.x - p1.x) * (p2.y + p1.y);
        }
        return area * 0.5f;
    }

    bool Poly::Orientation() const
    {
        return this->Area() >= 0;
    }

    bool Poly::IsInside(const Vector& point) const
    {
        bool inside = false;
        int size = this->Size();
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            if ((p1.y >= point.y) != (p2.y >= point.y))
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
        int size = this->Size();
        if (size < 3) return *this;
        if (delta <= 0 || step <= 0) return *this;
        if (!this->Orientation()) this->Reverse();

        // Precompute the normals for each edge
        Poly result; std::vector<Vector> norms(size);
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            Vector dir = (p2 - p1).Normalize();
            norms[i] = dir.Perpendicular() * delta;
        }

        // Compute new offset vertices
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            int k = i == 1 ? size - 1 : j - 1;
            const Vector& p1 = this->points[i];
            const Vector& p2 = this->points[j];
            const Vector& p3 = this->points[k];
            Vector a = p1 + norms[i], b = p2 + norms[i];
            Vector c = p2 + norms[j], d = p3 + norms[j];
            Vector inter = a.LineIntersection(b, c, d);
            if (!inter.IsValid()) continue;

            // Add rounding to convex corners only
            float phi = M_PI_F - p2.AngleBetween(p1, p3);
            if (phi < 0) { result.Add(inter); continue; }
            Vector vertex = p2 + (inter - p2).Normalize() * delta;
            result.Append(Geometry::Arc(p2, vertex, phi, step));
        }

        return result;
    }

    void Poly::Translate(const Vector& direction) noexcept
    {
        for (int i = 0; i < this->Size(); i++)
        {
            this->points[i] += direction;
        }
    }

    Linq<Vector> Poly::Intersection(const Linq<Vector>& path)
    {
        Linq<Vector> result;
        int size = this->Size();
        for (int i = 0, j = size - 1; i < size; j = i++)
        {
            const Vector& pa = this->points[i];
            const Vector& pb = this->points[j];
            for (size_t k = 0; k < path.Count() - 1; ++k)
            {
                const Vector& p1 = path.ElementAt(k);
                const Vector& p2 = path.ElementAt(k + 1);
                Vector inter = p1.SegmentIntersection(p2, pa, pb);
                if (inter.IsValid()) result.Append(inter);
            }
        }
        return result.Distinct();
    }
}
