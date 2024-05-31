
#include "Polygon.h"

namespace Evade
{
    Polygon::Polygon(const MainData& data)
    {
        Skillshot::LoadData(data);
        this->position = this->end_pos;
        this->skillshot_type = SkillshotType::POLYGON;
    }

    void Polygon::Update()
    {
        Poly polygon = this->hitbox > 0 ?
            this->polygon.Offset(this->hitbox,
            this->arc_step) : this->polygon;
        this->offset_polygon = polygon;
    }

    void Polygon::Draw(uint32_t color)
    {
        const Poly& poly = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, color);
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

    bool Polygon::IsPathDangerous(const Path& path)
    {
        return this->IsPathDangerousInternal(path);
    }

    float Polygon::TimeToHit(const Vector& pos, bool check_safety)
    {
        return (!check_safety || this->IsDangerous(pos))
            ? Skillshot::TimeToHitInternal() : -1.0f;
    }

    Linq<Vector> Polygon::PathIntersection(const Vector& p1, const Vector& p2)
    {
        return this->offset_polygon.PathIntersection(p1, p2);
    }
}
