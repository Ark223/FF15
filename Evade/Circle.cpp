
#include "Circle.h"

namespace Evade
{
    Circle::Circle(const MainData& data)
    {
        Skillshot::LoadData(data);
        this->position = this->end_pos;
        this->skillshot_type = SkillshotType::CIRCLE;
    }

    void Circle::Update()
    {
        // No displacement
    }

    void Circle::Draw(uint32_t color)
    {
        API::Get()->DrawCircle(this->position,
            this->radius, this->height, color);
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

    bool Circle::IsPathDangerous(const Path& path)
    {
        return Skillshot::IsPathDangerousInternal(path);
    }

    float Circle::TimeToHit(const Vector& pos, bool check_safety)
    {
        return (!check_safety || this->IsDangerous(pos))
            ? Skillshot::TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Circle::PathIntersection(const Vector& p1, const Vector& p2)
    {
        return Linq<Vector>(Geometry::CircleSegmentIntersection(
            p1, p2, this->position, this->radius + this->hitbox));
    }
}
