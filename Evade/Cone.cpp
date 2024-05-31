
#include "Cone.h"

namespace Evade
{
    Cone::Cone(const MainData& data)
    {
        Skillshot::LoadData(data);
        this->position = this->start_pos;
        this->skillshot_type = SkillshotType::CONE;
    }

    void Cone::Update()
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

    void Cone::Draw(uint32_t color)
    {
        const Poly& poly = this->offset_polygon;
        API::Get()->DrawPolygon(poly, this->height, color);
    }

    Vector Cone::Position(float delta)
    {
        if (std::isinf(this->speed))
            return this->start_pos.Clone();
        float time = MAX(0.0f, delta - this->delay +
            API::Get()->GetTime() - this->start_time);
        float dist = MIN(speed * time, this->range);
        return this->start_pos + this->direction * dist;
    }

    bool Cone::IsSafe(const Vector& pos)
    {
        return !this->IsDangerous(pos);
    }

    bool Cone::IsDangerous(const Vector& pos)
    {
        // Initial check with no full offset
        if (Geometry::IsInsideCone(
            this->start_pos, this->end_pos,
            pos, this->range + this->hitbox,
            this->cone_angle)) return true;

        // Pos could be inside offset part
        if (this->hitbox == 0.0f) return false;
        const Vector& front = this->end_arc.front();
        const Vector& back = this->end_arc.back();
        float sqr_hitbox = this->hitbox * this->hitbox;
        float d1 = pos.DistSqrToSegment(this->start_pos, front);
        float d2 = pos.DistSqrToSegment(this->start_pos, back);
        return d1 <= sqr_hitbox || d2 <= sqr_hitbox;
    }

    bool Cone::IsPathDangerous(const Path& path)
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
        Vector dir = path.end_pos - path.start_pos;
        Vector vel = dir.Normalize() * path.speed;
        Vector pos = path.start_pos + vel * delay;

        // Interception test
        float offset = path.speed * path.delta +
            (API::Get()->GetTime() - this->start_time
            + path.delay) * this->speed + this->hitbox;
        float col_time = Geometry::Interception(pos,
            this->start_pos, vel, this->speed, offset);

        // No collision detected, path is safe
        if (col_time == -1.0f) return false;

        // Check if a collision point is unsafe
        return this->IsDangerous(pos + vel * col_time);
    }

    float Cone::TimeToHit(const Vector& pos, bool check_safety)
    {
        // Skip the check if pos is verified to be in area
        if (check_safety && this->IsSafe(pos)) return -1.0f;

        // Return time to expire if no displacement
        bool huge = std::isinf(this->speed);
        if (huge) return this->TimeToHitInternal();

        // Treat cone as moving circle, easy calcs
        float dist = this->start_pos.Distance(pos);
        float time = (dist - this->hitbox) / this->speed;

        // Add skillshot delay, return time >= 0
        return MAX(0.0f, time + this->start_time -
            API::Get()->GetTime() + this->delay);
    }

    Linq<Vector> Cone::PathIntersection(const Vector& p1, const Vector& p2)
    {
        return Linq<Vector>(); // TODO
    }
}
