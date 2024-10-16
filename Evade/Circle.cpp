
#include "Circle.h"

namespace Evade
{
    Circle::Circle(const Base& data)
    {
        this->Initialize(data);
        this->position = this->end_pos.Clone();
        this->skillshot_type = SkillshotType::CIRCLE;
    }

    void Circle::Draw(uint32_t* colors)
    {
        const Vector& pos = this->position;
        float offset = this->radius + this->hitbox,
            height = this->height, radius = this->radius;
        API::Get()->DrawCircle(pos, radius, height, colors[0]);
        API::Get()->DrawCircle(pos, offset, height, colors[1]);
    }

    void Circle::Update(bool force)
    {
        // No displacement
    }

    Vector Circle::Position(float delta)
    {
        return this->position;
    }

    bool Circle::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Circle::IsDangerous(const Vector& pos)
    {
        float dist = pos.DistanceSquared(this->position);
        float hitbox = this->radius + this->hitbox;
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
            p1, p2, this->position, this->radius + this->hitbox));
    }
}
