
#include "Cone.h"

namespace Evade
{
    Cone::Cone(const ActiveData& data) : Skillshot(data)
    {
        this->Initialize(data);
        this->data.Position = this->data.StartPos.Clone();
        this->data.SkillshotType = SkillshotType::CONE;
    }

    void Cone::Draw(uint32_t* colors)
    {
        Poly& poly = data.Polygon, offset = data.OffsetPoly;
        API::Get()->DrawPolygon(poly, data.Height, colors[0]);
        API::Get()->DrawPolygon(offset, data.Height, colors[1]);
    }

    void Cone::Update(bool force)
    {
        // Update the skillshot position
        data.Position = this->Position();
        if (data.Position == data.EndPos)
        {
            data.OffsetPoly.Clear();
            return data.Polygon.Clear();
        }
        if (!force && data.Position == data.PrevPos)
        {
            return;
        }

        // Calculate arcs to define the cone shape
        data.StartArc = Geometry::Arc(data.StartPos,
            data.Position, data.ConeAngle, data.ArcStep);

        data.EndArc = Geometry::Arc(data.StartPos,
            data.EndPos, data.ConeAngle, data.ArcStep);

        // Build polygon for drawings
        data.Polygon.Clear();
        data.Polygon.Append(data.StartArc);
        data.Polygon.Reverse();
        data.Polygon.Append(data.EndArc);

        // Create offset polygon based on hitbox
        data.OffsetPoly = data.Polygon.Offset(
            data.Hitbox, MAX(1.0f, data.ArcStep));

        // Cache position to avoid unnecessary updates
        data.PrevPos = data.Position.Clone();
    }

    Vector Cone::Position(float delta)
    {
        if (IsInfinite(data.Speed))
            return data.StartPos.Clone();
        float time = MAX(0.0f, delta - data.Delay +
            API::Get()->GetTime() - data.StartTime);
        float dist = MIN(data.Speed * time, data.Range);
        return data.StartPos + data.Direction * dist;
    }

    bool Cone::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Cone::IsDangerous(const Vector& pos)
    {
        float hitbox = data.Hitbox;
        float angle = data.ConeAngle;
        const Vector& start = data.StartPos;
        const Vector& ending = data.EndPos;

        // Initial quick check without considering offset
        if (Geometry::IsInsideCone(start, ending, pos,
            data.Range + hitbox, angle)) return true;

        // Check if position is within offset area
        if (hitbox == 0.0f) return false;
        float sqr_hitbox = hitbox * hitbox;
        const Vector& front = data.EndArc.front();
        const Vector& back = data.EndArc.back();

        float d1 = pos.DistSqrToSegment(start, front);
        float d2 = pos.DistSqrToSegment(start, back);
        return d1 <= sqr_hitbox || d2 <= sqr_hitbox;
    }

    bool Cone::IsPathDangerous(Path& path, bool skip)
    {
        // Skip initial safety check if indicated
        if (!skip && !this->IsSafe(path.EndPos)) return true;

        // Check static collision if no displacement
        if (IsInfinite(data.Speed) == true) return
            this->IsPathDangerousInternal(path, true);

        float delta = path.Delay + path.Delta;
        float time = this->TimeToHit(path.StartPos);
        if (time >= 0 && time <= delta) return true;
        if (IsInfinite(path.Speed)) return false;

        // Predict positions after a delay
        float delay = MAX(0.0f, data.StartTime -
            API::Get()->GetTime() + data.Delay);
        Vector vel = this->UpdateVelocity(path);
        Vector pos = path.StartPos + vel * delay;

        // Determine interception time
        float offset = path.Speed * path.Delta +
            (API::Get()->GetTime() - data.StartTime
            + path.Delay) * data.Speed + data.Hitbox;
        float col_time = Geometry::Interception(pos,
            data.StartPos, vel, data.Speed, offset);

        // No collision detected, path is safe
        if (col_time == FLT_MAX) return false;

        // Check if a collision point is unsafe
        return this->IsDangerous(pos + vel * col_time);
    }

    float Cone::TimeToHit(const Vector& pos, bool skip)
    {
        // Skip a check if position is unsafe for sure
        if (!skip && this->IsSafe(pos)) return -1.0f;

        // Return expiration time if no displacement
        bool huge = IsInfinite(data.Speed);
        if (huge) return this->TimeToHitInternal();

        // Calculate time with a stationary position
        float dist = data.StartPos.Distance(pos);
        float time = (dist - data.Hitbox) / data.Speed;

        // Valid time must be a positive number
        return MAX(0.0f, time + data.StartTime -
            API::Get()->GetTime() + data.Delay);
    }

    Linq<Vector> Cone::PathIntersection(const Linq<Vector>& path)
    {
        Linq<Vector> result;
        float range = data.Range;
        float hitbox = data.Hitbox;
        float angle = data.ConeAngle;
        const Vector& start = data.StartPos;
        const Vector& ending = data.EndPos;

        // Define boundary edges
        std::pair<Vector, Vector> dir,
            edge_a, edge_b, left, right;
        left.first = data.StartArc.back();
        left.second = data.EndArc.back();
        right.first = data.StartArc.front();
        right.second = data.EndArc.front();
        edge_a = left; edge_b = right;

        if (hitbox > 0)
        {
            dir.first = (left.second - left.first);
            dir.second = (right.second - right.first);
            dir.first = dir.first.Normalize() * hitbox;
            dir.second = dir.second.Normalize() * hitbox;
            Vector pa = dir.first.Perpendicular();
            Vector pb = dir.second.Perpendicular2();
            edge_a = { left.first + pa, left.second + pa };
            edge_b = { right.first + pb, right.second + pb };
        }

        // Add segment intersections with adjusted edges
        const Vector& p1 = path.First(), p2 = path.Last();
        const Vector& eaf = edge_a.first, eas = edge_a.second;
        const Vector& ebf = edge_b.first, ebs = edge_b.second;
        Vector seg_a = eaf.SegmentIntersection(eas, p1, p2);
        Vector seg_b = ebf.SegmentIntersection(ebs, p1, p2);
        if (seg_a.IsValid()) result.Append(seg_a);
        if (seg_b.IsValid()) result.Append(seg_b);

        // Find intersections with cone sectors
        float dist = data.Position.Distance(start);
        result.AddRange(Geometry::ArcSegmentIntersection(
            p1, p2, start, ending, range + hitbox, angle));
        result.AddRange(Geometry::ArcSegmentIntersection(
            p1, p2, start, ending, dist - hitbox, angle));
        if (hitbox == 0.0f) return result.Distinct();

        // Handle corner cases considering hitbox
        std::vector<std::vector<Vector>> corners
        {
            { left.first, eaf, left.first - dir.first },
            { left.second, eas, left.second + dir.first },
            { right.first, ebf, right.first - dir.second },
            { right.second, ebs, right.second + dir.second }
        };

        // Add remaining relevant points
        for (const auto& corner : corners)
        {
            Vector middle = (corner[1] + corner[2]) * 0.5f;
            result.AddRange(Geometry::ArcSegmentIntersection(
                p1, p2, corner[0], middle, hitbox, M_PI_2_F));
        }

        return result.Distinct();
    }
}
