
#include "Polygon.h"

namespace Evade
{
    Polygon::Polygon(const ActiveData& data) : Skillshot(data)
    {
        this->Initialize(data);
        this->data.Position = this->data.StartPos.Clone();
        this->data.SkillshotType = SkillshotType::POLYGON;
    }

    void Polygon::Draw(uint32_t* colors)
    {
        Poly& poly = data.Polygon, offset = data.OffsetPoly;
        API::Get()->DrawPolygon(poly, data.Height, colors[0]);
        API::Get()->DrawPolygon(offset, data.Height, colors[1]);
    }

    void Polygon::Update(bool force)
    {
        Poly polygon = data.Hitbox > 0 ?
            data.Polygon.Offset(data.Hitbox,
            data.ArcStep) : data.Polygon;
        data.OffsetPoly = polygon;
    }

    Vector Polygon::Position(float delta)
    {
        return data.Position;
    }

    bool Polygon::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Polygon::IsDangerous(const Vector& pos)
    {
        return data.OffsetPoly.IsInside(pos);
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

    Linq<Vector> Polygon::PathIntersection(const Linq<Vector>& path)
    {
        return data.OffsetPoly.Intersection(path);
    }
}
