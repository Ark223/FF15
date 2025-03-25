
#ifndef LINE_H
#define LINE_H

#include "Skillshot.h"

namespace Evade
{
    class Line : public Skillshot
    {
        public:
            Line(const ActiveData& data);

            void Draw(uint32_t* colors) override;
            void Update(bool force = false) override;
            Vector Position(float delta = 0.0f) override;
            bool IsSafe(const Vector& pos) override;
            bool IsDangerous(const Vector& pos) override;
            bool IsPathDangerous(Path& path, bool skip = false) override;
            float TimeToHit(const Vector& pos, bool skip = false) override;
            Linq<Vector> PathIntersection(const Linq<Vector>& path) override;
    };
}

#endif // LINE_H
