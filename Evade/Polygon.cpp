
#include "Polygon.h"

namespace Evade
{
    Polygon::Polygon(const Base& data)
    {
        this->Initialize(data);
        this->position = this->start_pos.Clone();
        this->skillshot_type = SkillshotType::POLYGON;
    }

    void Polygon::Draw(uint32_t* colors)
    {
        Poly& poly = this->polygon, offset = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, colors[0]);
        API::Get()->DrawPolygon(offset, this->height, colors[1]);
    }

    void Polygon::Update(bool force)
    {
        Poly polygon = this->hitbox > 0 ?
            this->polygon.Offset(this->hitbox,
            this->arc_step) : this->polygon;
        this->offset_polygon = polygon;
    }

    Vector Polygon::Position(float delta)
    {
        return this->position;
    }

    bool Polygon::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Polygon::IsDangerous(const Vector& pos)
    {
        return this->offset_polygon.IsInside(pos);
    }

    bool Polygon::IsPathDangerous(Path& path, bool skip)
    {
        return this->IsPathDangerousInternal(path, skip);
    }

    float Polygon::TimeToHit(const Vector& pos, bool skip)
    {
        return (skip || this->IsDangerous(pos))
            ? this->TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Polygon::PathIntersection(const Vector& p1, const Vector& p2)
    {
        return this->offset_polygon.PathIntersection(p1, p2);
    }
}
