
#include "Ring.h"

namespace Evade
{
    Ring::Ring(const ActiveData& data) : Skillshot(data)
    {
        this->Initialize(data);
        this->data.Position = this->data.EndPos.Clone();
        this->data.SkillshotType = SkillshotType::RING;
    }

    void Ring::Draw(uint32_t* colors)
    {
        const Vector& pos = data.Position;
        float inner = data.InnerRadius, height =
            data.Height, outer = data.OuterRadius;
        API::Get()->DrawCircle(pos, inner, height, colors[0]);
        API::Get()->DrawCircle(pos, outer, height, colors[0]);
    }

    void Ring::Update(bool force)
    {
        if (force || data.Position != data.PrevPos)
            data.Position = data.EndPos.Clone();
        data.PrevPos = data.Position.Clone();
    }

    Vector Ring::Position(float delta)
    {
        return data.Position;
    }

    bool Ring::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Ring::IsDangerous(const Vector& pos)
    {
        float dist = pos.DistanceSquared(data.Position);
        return dist >= data.InnerRadius * data.InnerRadius
            && dist <= data.OuterRadius * data.OuterRadius;
    }

    bool Ring::IsPathDangerous(Path& path, bool skip)
    {
        return this->IsPathDangerousInternal(path, skip);
    }

    float Ring::TimeToHit(const Vector& pos, bool skip)
    {
        return (skip || this->IsDangerous(pos))
            ? this->TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Ring::PathIntersection(const Linq<Vector>& path)
    {
        const Vector& p1 = path.First(), p2 = path.Last();
        auto r1 = Geometry::CircleSegmentIntersection(
            p1, p2, data.Position, data.InnerRadius);
        auto r2 = Geometry::CircleSegmentIntersection(
            p1, p2, data.Position, data.OuterRadius);
        return Linq<Vector>(r1).Concat(r2);
    }
}
