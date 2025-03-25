
#include "Line.h"

namespace Evade
{
    Line::Line(const ActiveData& data) : Skillshot(data)
    {
        this->Initialize(data);
        this->data.Position = this->data.StartPos.Clone();
        this->data.SkillshotType = SkillshotType::LINE;
    }

    void Line::Draw(uint32_t* colors)
    {
        Poly& poly = data.Polygon, offset = data.OffsetPoly;
        API::Get()->DrawPolygon(poly, data.Height, colors[0]);
        API::Get()->DrawPolygon(offset, data.Height, colors[1]);
    }

    void Line::Update(bool force)
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

        // Compute a rectangular hitbox first
        Vector dir = data.Perpendicular * data.Radius;
        data.Polygon.Set(0, data.Position + dir);
        data.Polygon.Set(1, data.Position - dir);
        data.Polygon.Set(2, data.EndPos - dir);
        data.Polygon.Set(3, data.EndPos + dir);

        // Create offset polygon based on hitbox
        data.OffsetPoly = data.Polygon.Offset(
            data.Hitbox, MAX(1.0f, data.ArcStep));

        // Cache position to avoid unnecessary updates
        data.PrevPos = data.Position.Clone();
    }

    Vector Line::Position(float delta)
    {
        if (IsInfinite(data.Speed))
            return data.StartPos.Clone();
        float time = MAX(0.0f, delta - data.Delay +
            API::Get()->GetTime() - data.StartTime);
        float dist = MIN(data.Speed * time, data.Range);
        return data.StartPos + data.Direction * dist;
    }

    bool Line::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Line::IsDangerous(const Vector& pos)
    {
        if (data.Polygon.Size() != 4) return false;
        if (data.Polygon.IsInside(pos)) return true;
        if (data.Hitbox == 0.0f) return false;
        
        float hitbox = data.Hitbox * data.Hitbox;
        for (int i = 0, j = 3; i < 4; j = i++)
        {
            const Vector& p1 = data.Polygon.Get(i);
            const Vector& p2 = data.Polygon.Get(j);
            float dist = pos.DistSqrToSegment(p1, p2);
            if (dist <= hitbox) return true;
        }
        return false;
    }

    bool Line::IsPathDangerous(Path& path, bool skip)
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
        Vector pos = this->Position(path.Delay);
        Vector vel = this->UpdateVelocity(path);
        Vector str_pos = path.StartPos + vel * delay;
        Vector end_pos = path.EndPos + vel * delay;

        // Determine interception time
        float extra_len = path.Speed * path.Delta;
        float col_time = Geometry::DynamicCollision(
            pos, str_pos, data.Direction * data.Speed,
            vel, data.Radius, data.Hitbox + extra_len);

        // No collision detected, path is safe
        if (col_time == FLT_MAX) return false;

        // Collision detected but it will not occur
        // if a skillshot will expire before hit
        float time_left = this->TimeToHitInternal();
        return col_time + delay <= time_left;
    }

    float Line::TimeToHit(const Vector& pos, bool skip)
    {
        // Skip a check if position is unsafe for sure
        if (!skip && this->IsSafe(pos)) return -1.0f;

        // Return expiration time if no displacement
        bool huge = IsInfinite(data.Speed);
        if (huge) return this->TimeToHitInternal();

        // Calculate time with a stationary position
        Vector vel = data.Direction * data.Speed;
        float time = Geometry::DynamicCollision(
            data.Position, pos, vel, Vector(0.0f,
            0.0f), data.Radius, data.Hitbox);

        // Valid time must be a positive number
        return time + MAX(0.0f, data.StartTime -
            API::Get()->GetTime() + data.Delay);
    }

    Linq<Vector> Line::PathIntersection(const Linq<Vector>& path)
    {
        // Ensure polygon is valid including four vertices
        if (data.Polygon.Size() != 4) return Linq<Vector>();

        // Return points directly if no offset
        const Vector& p1 = path.First();
        const Vector& p2 = path.Last();
        if (data.Hitbox == 0.0f) return
            data.Polygon.Intersection({p1, p2});

        // Define the enlarged area by offsetting
        Vector dir = data.Direction * data.Hitbox;
        Vector perp = data.Perpendicular * data.Hitbox;
        std::vector<std::pair<Vector, Vector>> cases =
        {
            { -dir, data.Polygon.Get(0) - dir + perp },
            { -perp, data.Polygon.Get(1) - dir - perp },
            { dir, data.Polygon.Get(2) + dir - perp },
            { perp, data.Polygon.Get(3) + dir + perp }
        };

        Linq<Vector> result;
        for (int j = 0, i = 3; j < 4; i = j++)
        {
            const auto& [dir_c, perp_c] = cases.at(i);
            const Vector& curr = data.Polygon.Get(i);
            const Vector& next = data.Polygon.Get(j);
            Vector exa = curr + dir_c, exb = next + dir_c;
            Vector inter = exa.SegmentIntersection(exb, p1, p2);
            if (inter.IsValid() == true) result.Append(inter);

            // Check intersection with the corners
            auto arc = Geometry::ArcSegmentIntersection(
                p1, p2, curr, perp_c, data.Hitbox, M_PI_2_F);
            for (const Vector& vec : arc) result.Append(vec);
        }

        return result.Distinct();
    }
}
