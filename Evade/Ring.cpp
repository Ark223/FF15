
#include "Ring.h"

namespace Evade
{
    Ring::Ring(const Base& data)
    {
        this->Initialize(data);
        this->position = this->end_pos.Clone();
        this->skillshot_type = SkillshotType::RING;
    }

    void Ring::Draw(uint32_t* colors)
    {
        const Vector& pos = this->position;
        float inner = this->inner_radius, height =
            this->height, outer = this->outer_radius;
        API::Get()->DrawCircle(pos, inner, height, colors[0]);
        API::Get()->DrawCircle(pos, outer, height, colors[0]);
    }

    void Ring::Update(bool force)
    {
        // No displacement
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

    bool Ring::IsPathDangerous(Path& path, bool skip)
    {
        return this->IsPathDangerousInternal(path, skip);
    }

    float Ring::TimeToHit(const Vector& pos, bool skip)
    {
        return (skip || this->IsDangerous(pos))
            ? this->TimeToHitInternal() : -1.0f;
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
