
#include "Ring.h"

namespace Evade
{
    Ring::Ring(const MainData& data)
    {
        Skillshot::LoadData(data);
        this->position = this->end_pos;
        this->skillshot_type = SkillshotType::RING;
    }

    void Ring::Update()
    {
        // No displacement
    }

    void Ring::Draw(uint32_t color)
    {
        API::Get()->DrawCircle(this->position,
            this->inner_radius, this->height, color);
        API::Get()->DrawCircle(this->position,
            this->outer_radius, this->height, color);
    }

    Vector Ring::Position(float delta)
    {
        return this->position;
    }

    bool Ring::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Ring::IsDangerous(const Vector& pos)
    {
        float dist = pos.DistanceSquared(this->position);
        return dist >= this->inner_radius * this->inner_radius
            && dist <= this->outer_radius * this->outer_radius;
    }

    bool Ring::IsPathDangerous(const Path& path)
    {
        return this->IsPathDangerousInternal(path);
    }

    float Ring::TimeToHit(const Vector& pos, bool check_safety)
    {
        return (!check_safety || this->IsDangerous(pos))
            ? Skillshot::TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Ring::PathIntersection(const Vector& p1, const Vector& p2)
    {
        auto ints_a = Geometry::CircleSegmentIntersection(
            p1, p2, this->position, this->inner_radius);
        auto ints_b = Geometry::CircleSegmentIntersection(
            p1, p2, this->position, this->outer_radius);
        return Linq<Vector>(ints_a).Concat(ints_b);
    }
}
