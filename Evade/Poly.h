
#ifndef POLY_H
#define POLY_H

#include "Geometry.h"
#include "Linq.cpp"

namespace Evade
{
    class Poly
    {
        private:
            std::vector<Vector> points;

        public:
            Poly();
            explicit Poly(const std::vector<Vector>& points);

            void Add(const Vector& point);
            Vector Get(size_t index) const;
            void Set(int index, const Vector& point);
            void Append(const std::vector<Vector>& points);
            void Clear();
            void Remove(size_t index);
            void Reverse();

            int Size() const;
            float Area() const;
            bool Orientation() const;
            bool IsInside(const Vector& point) const;
            bool IsOutside(const Vector& point) const;
            Poly Offset(float delta, float step);
            void Translate(const Vector& direction) noexcept;
            Linq<Vector> Intersection(const Linq<Vector>& path);
    };
}

#endif // POLY_H
