
#include "Line.h"

namespace Evade
{
    Line::Line(const Base& data)
    {
        this->Initialize(data);
        this->position = this->start_pos.Clone();
        this->skillshot_type = SkillshotType::LINE;
    }

    void Line::Draw(uint32_t* colors)
    {
        Poly& poly = this->polygon, offset = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, colors[0]);
        API::Get()->DrawPolygon(offset, this->height, colors[1]);
    }

    void Line::Update(bool force)
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

        // Build a rectangle (apply offset later)
        Vector dir = this->perpendicular * this->radius;
        this->polygon.Set(0, this->position + dir);
        this->polygon.Set(1, this->position - dir);
        this->polygon.Set(2, this->end_pos - dir);
        this->polygon.Set(3, this->end_pos + dir);

        // Build offset area (hitbox included)
        Poly offset = this->polygon.Offset(
            this->hitbox, this->arc_step);
        this->offset_polygon = offset;

        // Cache position to avoid unnecessary updates
        this->prev_pos = this->position.Clone();
    }

    Vector Line::Position(float delta)
    {
        if (std::isinf(this->speed))
            return this->start_pos.Clone();
        float time = MAX(0.0f, delta - this->delay +
            API::Get()->GetTime() - this->start_time);
        float dist = MIN(this->speed * time, this->range);
        return this->start_pos + this->direction * dist;
    }

    bool Line::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Line::IsDangerous(const Vector& pos)
    {
        if (this->polygon.Size() != 4) return false;
        if (this->polygon.IsInside(pos)) return true;
        if (this->hitbox == 0.0f) return false;
        
        float hitbox = this->hitbox * this->hitbox;
        for (int i = 0, j = 3; i < 4; j = i++)
        {
            const Vector& p1 = this->polygon.Get(i);
            const Vector& p2 = this->polygon.Get(j);
            float dist = pos.DistSqrToSegment(p1, p2);
            if (dist <= hitbox) return true;
        }

        return false;
    }

    bool Line::IsPathDangerous(Path& path, bool skip)
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
        Vector pos = this->Position(path.Delay);
        Vector vel = this->UpdateVelocity(path);
        Vector str_pos = path.StartPos + vel * delay;
        Vector end_pos = path.EndPos + vel * delay;

        // Dynamic collision test
        float extra_len = path.Speed * path.Delta;
        float col_time = Geometry::DynamicCollision(
            pos, str_pos, this->direction * this->speed,
            vel, this->radius, this->hitbox + extra_len);

        // No collision detected, path is safe
        if (col_time == -1.0f) return false;

        // Collision detected but it will not occur
        // if a skillshot will expire before hit
        float time_left = this->TimeToHitInternal();
        return col_time + delay <= time_left;
    }

    float Line::TimeToHit(const Vector& pos, bool skip)
    {
        // If pos is verified to be unsafe, skip the check
        if (!skip && this->IsSafe(pos)) return -1.0f;

        // No displacement = return time to expiration
        bool huge = std::isinf(this->speed);
        if (huge) return this->TimeToHitInternal();

        // Calculate time with a stationary position
        Vector vel = this->direction * this->speed;
        float time = Geometry::DynamicCollision(
            this->position, pos, vel, Vector(),
            this->radius, this->hitbox);

        // Include delay and ensure non-negative time
        return time + MAX(0.0f, this->start_time -
            API::Get()->GetTime() + this->delay);
    }

    Linq<Vector> Line::PathIntersection(const Vector& p1, const Vector& p2)
    {
        // Verify the hitbox is valid - four vertices expected
        if (this->polygon.Size() != 4) return Linq<Vector>();

        // No offset = directly return intersections
        if (this->hitbox == 0.0f) return
            this->polygon.PathIntersection(p1, p2);

        // Define the enlarged area by offsetting
        Vector dir = this->direction * this->hitbox;
        Vector perp = this->perpendicular * this->hitbox;
        std::vector<std::pair<Vector, Vector>> cases =
        {
            { -dir, this->polygon.Get(0) - dir + perp },
            { -perp, this->polygon.Get(1) - dir - perp },
            { dir, this->polygon.Get(2) + dir - perp },
            { perp, this->polygon.Get(3) + dir + perp }
        };

        Linq<Vector> result;
        for (int j = 0, i = 3; j < 4; i = j++)
        {
            const auto& [dir_c, perp_c] = cases.at(i);
            const Vector& curr = this->polygon.Get(i);
            const Vector& next = this->polygon.Get(j);
            Vector exa = curr + dir_c, exb = next + dir_c;
            Vector inter = exa.SegmentIntersection(exb, p1, p2);
            if (inter.IsValid() == true) result.Append(inter);

            // Check intersection with the corners
            auto arc = Geometry::ArcSegmentIntersection(
                p1, p2, curr, perp_c, this->hitbox, M_PI_2);
            for (const Vector& vec : arc) result.Append(vec);
        }

        return result.Distinct();
    }
}
