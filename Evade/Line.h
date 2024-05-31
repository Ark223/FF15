
#ifndef LINE_H
#define LINE_H

#include "Skillshot.h"

namespace Evade
{
    class Line : public Skillshot
    {
        public:
            Line(const MainData& data);

            void Update() override;
            void Draw(uint32_t color) override;
            Vector Position(float delta = 0.0f) override;
            bool IsSafe(const Vector& pos) override;
            bool IsDangerous(const Vector& pos) override;
            bool IsPathDangerous(const Path& path) override;
            float TimeToHit(const Vector& pos, bool check_safety = true) override;
            Linq<Vector> PathIntersection(const Vector& p1, const Vector& p2) override;
    };
}

#endif // LINE_H
