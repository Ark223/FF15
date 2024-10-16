
#include "Cone.h"

namespace Evade
{
    Cone::Cone(const Base& data)
    {
        this->Initialize(data);
        this->position = this->start_pos.Clone();
        this->skillshot_type = SkillshotType::CONE;
    }

    void Cone::Draw(uint32_t* colors)
    {
        Poly& poly = this->polygon, offset = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, colors[0]);
        API::Get()->DrawPolygon(offset, this->height, colors[1]);
    }

    void Cone::Update(bool force)
    {
        // Update skillshot position
        this->position = this->Position();
        if (this->position == this->end_pos)
        {
            this->offset_polygon.Clear();
            this->polygon.Clear(); return;
        }
        if (!force && this->position == this->prev_pos)
        {
            return;
        }

        // Build an updated arc / sector structure
        this->start_arc = Geometry::Arc(this->start_pos,
            this->position, this->cone_angle, this->arc_step);

        this->end_arc = Geometry::Arc(this->start_pos,
            this->end_pos, this->cone_angle, this->arc_step);

        // Build approximated area for drawings
        this->polygon.Clear();
        this->polygon.Append(this->start_arc);
        this->polygon.Reverse();
        this->polygon.Append(this->end_arc);

        // Build offset area (hitbox included)
        Poly offset = this->polygon.Offset(
            this->hitbox, this->arc_step);
        this->offset_polygon = offset;

        // Cache position to avoid unnecessary updates
        this->prev_pos = this->position.Clone();
    }

    Vector Cone::Position(float delta)
    {
        if (std::isinf(this->speed))
            return this->start_pos.Clone();
        float time = MAX(0.0f, delta - this->delay +
            API::Get()->GetTime() - this->start_time);
        float dist = MIN(this->speed * time, this->range);
        return this->start_pos + this->direction * dist;
    }

    bool Cone::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Cone::IsDangerous(const Vector& pos)
    {
        float hitbox = this->hitbox;
        float angle = this->cone_angle;
        const Vector& start = this->start_pos;
        const Vector& ending = this->end_pos;

        // Initial check without considering full offset
        if (Geometry::IsInsideCone(start, ending, pos,
            this->range + hitbox, angle)) return true;

        // Position might be within the offset area
        if (hitbox == 0.0f) return false;
        float sqr_hitbox = hitbox * hitbox;
        const Vector& front = this->end_arc.front();
        const Vector& back = this->end_arc.back();
        float d1 = pos.DistSqrToSegment(start, front);
        float d2 = pos.DistSqrToSegment(start, back);
        return d1 <= sqr_hitbox || d2 <= sqr_hitbox;
    }

    bool Cone::IsPathDangerous(Path& path, bool skip)
    {
        // Skip initial safety check if indicated
        if (!skip && !this->IsSafe(path.EndPos)) return true;

        // Check static collision if there is no displacement
        if (std::isinf(this->speed) == true) return
            this->IsPathDangerousInternal(path, true);

        float delta = path.Delay + path.Delta;
        float time = this->TimeToHit(path.StartPos);
        if (time >= 0 && time <= delta) return true;
        if (std::isinf(path.Speed)) return false;

        // Predict positions after a delay
        float delay = MAX(0.0f, this->start_time -
            API::Get()->GetTime() + this->delay);
        Vector vel = this->UpdateVelocity(path);
        Vector pos = path.StartPos + vel * delay;

        // Interception test
        float offset = path.Speed * path.Delta +
            (API::Get()->GetTime() - this->start_time
            + path.Delay) * this->speed + this->hitbox;
        float col_time = Geometry::Interception(pos,
            this->start_pos, vel, this->speed, offset);

        // No collision detected, path is safe
        if (col_time == -1.0f) return false;

        // Check if a collision point is unsafe
        return this->IsDangerous(pos + vel * col_time);
    }

    float Cone::TimeToHit(const Vector& pos, bool skip)
    {
        // If pos is verified to be unsafe, skip the check
        if (!skip && this->IsSafe(pos)) return -1.0f;

        // No displacement = return time to expiration
        bool huge = std::isinf(this->speed);
        if (huge) return this->TimeToHitInternal();

        // Treat cone as a moving circle, easy calcs
        float dist = this->start_pos.Distance(pos);
        float time = (dist - this->hitbox) / this->speed;

        // Include delay and ensure non-negative time
        return MAX(0.0f, time + this->start_time -
            API::Get()->GetTime() + this->delay);
    }

    Linq<Vector> Cone::PathIntersection(const Vector& p1, const Vector& p2)
    {
        Linq<Vector> result;
        float range = this->range;
        float angle = this->cone_angle;
        const Vector& start = this->start_pos;
        const Vector& ending = this->end_pos;

        // Store directions and edges
        std::pair<Vector, Vector> dir,
            edge_a, edge_b, left, right;
        left.first = this->start_arc.back();
        left.second = this->end_arc.back();
        right.first = this->start_arc.front();
        right.second = this->end_arc.front();
        edge_a = left; edge_b = right;

        // Edge adjustment for hitbox > 0
        float hitbox = this->hitbox;
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

        // Add segment intersection points with adjusted edges
        const Vector& eaf = edge_a.first, eas = edge_a.second;
        const Vector& ebf = edge_b.first, ebs = edge_b.second;
        Vector seg_a = eaf.SegmentIntersection(eas, p1, p2);
        Vector seg_b = ebf.SegmentIntersection(ebs, p1, p2);
        if (seg_a.IsValid()) result.Append(seg_a);
        if (seg_b.IsValid()) result.Append(seg_b);

        // Calculate intersections at the cone's points
        float dist = this->position.Distance(start);
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

        // For each corner, add relevant intersection points
        for (const auto& corner : corners)
        {
            Vector middle = (corner[1] + corner[2]) * 0.5f;
            result.AddRange(Geometry::ArcSegmentIntersection(
                p1, p2, corner[0], middle, hitbox, M_PI_2));
        }

        return result.Distinct();
    }
}
