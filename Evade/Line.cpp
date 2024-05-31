
#include "Line.h"

namespace Evade
{
    Line::Line(const MainData& data)
    {
        Skillshot::LoadData(data);
        this->position = this->start_pos;
        this->skillshot_type = SkillshotType::LINE;
    }

    void Line::Update()
    {
        // Update skillshot position
        this->position = this->Position();
        if (this->position == this->end_pos)
        {
            this->offset_polygon.Clear();
            this->polygon.Clear(); return;
        }
        if (this->position == this->prev_pos)
        {
            return;
        }

        // Build a rectangle (apply offset later)
        Vector dir = this->perpendicular * this->radius;
        this->polygon.Set(0, this->position + dir);
        this->polygon.Set(1, this->end_pos + dir);
        this->polygon.Set(2, this->end_pos - dir);
        this->polygon.Set(3, this->position - dir);

        // Build offset area (hitbox included)
        Poly offset = this->polygon.Offset(
            this->hitbox, this->arc_step);
        this->offset_polygon = offset;

        // Cache position to avoid unnecessary updates
        this->prev_pos = this->position.Clone();
    }

    void Line::Draw(uint32_t color)
    {
        const Poly& poly = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, color);
    }

    Vector Line::Position(float delta)
    {
        if (std::isinf(this->speed))
            return this->start_pos.Clone();
        float time = MAX(0.0f, delta - this->delay +
            API::Get()->GetTime() - this->start_time);
        float dist = MIN(speed * time, this->range);
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

    bool Line::IsPathDangerous(const Path& path)
    {
        // Destination point is inside skillshot area
        if (this->IsDangerous(path.end_pos)) return true;

        // Check static collision if no displacement
        if (std::isinf(this->speed)) return
            Skillshot::IsPathDangerousInternal(path);

        float delta = path.delay + path.delta;
        float time = this->TimeToHit(path.start_pos);
        if (time >= 0 && time <= delta) return true;

        // Unit's movement speed is instant, path is safe
        if (std::isinf(path.speed)) return false;

        // Predict positions after a delay
        float delay = MAX(0.0f, this->start_time -
            API::Get()->GetTime() + this->delay);
        Vector pos = this->Position(path.delay);
        Vector dir = path.end_pos - path.start_pos;
        Vector vel = dir.Normalize() * path.speed;
        Vector start_pos = path.start_pos + vel * delay;
        Vector end_pos = path.end_pos + vel * delay;

        // Dynamic collision test
        float col_time = Geometry::DynamicCollision(
            pos, this->end_pos, start_pos, end_pos,
            this->speed, path.speed, this->radius,
            this->hitbox + path.speed * path.delta);

        // No collision detected, path is safe
        if (col_time == -1.0f) return false;

        // Collision detected but it will not occur
        // if a skillshot will expire before hit
        float time_left = this->TimeToHitInternal();
        return col_time + delay <= time_left;
    }

    float Line::TimeToHit(const Vector& pos, bool check_safety)
    {
        // Skip the check if pos is verified to be in area
        if (check_safety && this->IsSafe(pos)) return -1.0f;

        // Return time to expire if no displacement
        bool huge = std::isinf(this->speed);
        if (huge) return this->TimeToHitInternal();

        // Given pos is constant, its velocity is zero
        float time = Geometry::DynamicCollision(
            this->position, this->end_pos, pos, pos,
            this->speed, 0.0f, this->radius, this->hitbox);

        // Add skillshot delay, return time >= 0
        return time + MAX(0.0f, this->start_time -
            API::Get()->GetTime() + this->delay);
    }

    Linq<Vector> Line::PathIntersection(const Vector& p1, const Vector& p2)
    {
        if (this->polygon.Size() != 4) return Linq<Vector>();

        // Hitbox is rectangle cause there is no offset
        if (this->hitbox == 0.0f) return
            this->polygon.PathIntersection(p1, p2);

        // Handle borders and corners of enlarged area
        Vector dir = direction * this->hitbox;
        Vector perp = perpendicular * this->hitbox;
        std::vector<std::pair<Vector, Vector>> cases =
        {
            { perp, this->polygon.Get(0) - dir + perp },
            { dir, this->polygon.Get(1) + dir + perp },
            { -perp, this->polygon.Get(2) + dir - perp },
            { -dir, this->polygon.Get(3) - dir - perp }
        };

        Linq<Vector> result;
        for (const auto& [c_dir, c_perp] : cases)
        {
            for (int i = 0, j = 3; i < 4; j = i++)
            {
                // Add intersections of enlarged box
                const Vector& curr = this->polygon.Get(i) + c_dir;
                const Vector& next = this->polygon.Get(j) + c_dir;
                Vector inter = curr.SegmentIntersection(next, p1, p2);
                if (inter.IsValid() == true) result.Append(inter);

                // Add intersections with the corners
                auto arc = Geometry::ArcSegmentIntersection(
                    p1, p2, curr, c_perp, this->hitbox, M_PI_2);
                for (const Vector &vec : arc) result.Append(vec);
            }
        }

        return result.Distinct();
    }
}
