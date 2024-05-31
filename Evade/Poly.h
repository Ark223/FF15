
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
            size_t size;

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
            size_t Size() const;
            
            float Area() const;
            bool Orientation() const;
            bool IsInside(const Vector& point) const;
            bool IsOutside(const Vector& point) const;
            Poly Offset(float delta = 0.0f, float step = 0.0f);
            Linq<Vector> PathIntersection(const Vector& p1, const Vector& p2) const;
    };
}

#endif // POLY_H
