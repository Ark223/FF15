
#include "Circle.h"

namespace Evade
{
    Circle::Circle(const ActiveData& data) : Skillshot(data)
    {
        this->Initialize(data);
        this->data.Position = this->data.EndPos.Clone();
        this->data.SkillshotType = SkillshotType::CIRCLE;
    }

    void Circle::Draw(uint32_t* colors)
    {
        const Vector& pos = data.Position;
        float offset = data.Radius + data.Hitbox;
        float height = data.Height, radius = data.Radius;
        API::Get()->DrawCircle(pos, radius, height, colors[0]);
        API::Get()->DrawCircle(pos, offset, height, colors[1]);
    }

    void Circle::Update(bool force)
    {
        // No displacement
    }

    Vector Circle::Position(float delta)
    {
        return data.Position;
    }

    bool Circle::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Circle::IsDangerous(const Vector& pos)
    {
        float dist = pos.DistanceSquared(data.Position);
        float hitbox = data.Radius + data.Hitbox;
        return dist <= hitbox * hitbox;
    }

    bool Circle::IsPathDangerous(Path& path, bool skip)
    {
        return this->IsPathDangerousInternal(path, skip);
    }

    float Circle::TimeToHit(const Vector& pos, bool skip)
    {
        return (skip || this->IsDangerous(pos))
            ? this->TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Circle::PathIntersection(const Vector& p1, const Vector& p2)
    {
        return Linq<Vector>(Geometry::CircleSegmentIntersection(
            p1, p2, data.Position, data.Radius + data.Hitbox));
    }
}