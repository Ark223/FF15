
#include "Skillshot.h"

namespace Evade
{
    int Skillshot::unique_id = 0;

    template<typename T>
    void Skillshot::Assign(const Base& data, const std::string& key, T& var)
    {
        auto it = data.find(key);
        if (it == data.end()) return;
        var = std::any_cast<T>(it->second);
    }

    void Skillshot::Initialize(const Base& data)
    {
        unique_id++;
        this->LoadData(data);
        this->FixOrigin();
        this->Update(true);
    }

    void Skillshot::LoadData(const Base& data)
    {
        this->Assign(data, "ArcStep", this->arc_step);
        this->Assign(data, "ConeAngle", this->cone_angle);
        this->Assign(data, "Damage", this->damage);
        this->Assign(data, "DangerLevel", this->danger_level);
        this->Assign(data, "Delay", this->delay);
        this->Assign(data, "ExtraDuration", this->extra_duration);
        this->Assign(data, "Height", this->height);
        this->Assign(data, "Offset", this->offset);
        this->Assign(data, "Range", this->range);
        this->Assign(data, "Radius", this->radius);
        this->Assign(data, "InnerRadius", this->inner_radius);
        this->Assign(data, "OuterRadius", this->outer_radius);
        this->Assign(data, "RotationAngle", this->rot_angle);
        this->Assign(data, "SideRange", this->side_range);
        this->Assign(data, "Speed", this->speed);
        this->Assign(data, "StartTime", this->start_time);
        this->Assign(data, "FixedRange", this->fixed_range);
        this->Assign(data, "FogOfWar", this->fog_of_war);
        this->Assign(data, "SoftCC", this->soft_cc);
        this->Assign(data, "HardCC", this->hard_cc);
        this->Assign(data, "Hitbox", this->hitbox);
        this->Assign(data, "Invert", this->invert);
        this->Assign(data, "Rotate90", this->rotate90);
        this->Assign(data, "DestPos", this->dest_pos);
        this->Assign(data, "OriginPos", this->origin_pos);
        this->Assign(data, "StartPos", this->start_pos);
        this->Assign(data, "Position", this->position);
        this->Assign(data, "Polygon", this->polygon);
        this->Assign(data, "Caster", this->caster);
        this->Assign(data, "CasterName", this->caster_name);
        this->Assign(data, "Collisions", this->collisions);
        this->Assign(data, "SkillshotName", this->name);
        this->Assign(data, "SkillshotSlot", this->slot);
        this->Assign(data, "DetectionType", this->detection_type);
    }

    void Skillshot::FixOrigin()
    {
        bool unfixed = !this->fixed_range;
        float dist = this->start_pos.Distance(this->dest_pos);
        if (unfixed && dist < this->range) this->range = dist;

        this->direction = (this->dest_pos - this->start_pos).Normalize();
        this->direction = this->direction.Rotate(this->rot_angle);
        this->perpendicular = this->direction.Perpendicular();

        this->end_pos = this->start_pos + this->direction * this->range;
        this->start_pos = this->start_pos + this->direction * this->offset;
        this->range -= this->offset; this->offset = this->rot_angle = 0.0f;

        if (this->rotate90)
        {
            auto distance = this->perpendicular * this->side_range;
            this->start_pos = this->end_pos - distance * 0.5f;
            this->end_pos = this->end_pos + distance * 0.5f;
            this->direction = this->perpendicular.Clone();
            this->perpendicular = this->direction.Perpendicular();
            this->range = this->side_range; this->rotate90 = false;
        }

        if (!this->invert) return;
        this->direction = -(this->direction);
        this->perpendicular = -(this->perpendicular);
        this->start_pos.Swap(this->end_pos);
        this->invert = false;
    }

    bool Skillshot::IsExpired() const
    {
        float elapsed = API::Get()->GetTime() - this->start_time;
        return elapsed >= this->TotalLifeTime();
    }

    bool Skillshot::IsPathDangerousInternal(Path& path, bool skip)
    {
        // Destination point is within skillshot area
        if (!skip && !this->IsSafe(path.EndPos)) return true;

        // Skillshot is about to expire, any collision results in hit
        float left = this->TimeToHitInternal() - path.Delay;
        if (left <= 0) return this->IsDangerous(path.StartPos) ||
            this->PathIntersection(path.StartPos, path.EndPos).Any();

        // Unit's movement speed is instant, path is safe
        if (std::isinf(path.Speed)) return false;

        // Predict collision in the future
        Vector vel = this->UpdateVelocity(path);
        Vector p1 = path.StartPos + vel * (left + path.Delta);
        Vector p2 = path.StartPos + vel * (left - path.Delta);

        // No error propagation, so only one position is checked
        if (path.Delta == 0.0f) return this->IsDangerous(p1);

        return this->IsDangerous(p1) || this->IsDangerous(p2)
            || this->PathIntersection(p1, p2).Count() > 0;
    }

    float Skillshot::TimeToHitInternal() const
    {
        float life_time = this->delay + this->range / this->speed;
        return MAX(0.0f, this->start_time - API::Get()->GetTime() + life_time);
    }

    float Skillshot::TotalLifeTime() const
    {
        float life_time = this->delay + this->range / this->speed;
        return life_time + this->extra_duration;
    }

    void Skillshot::Translate(const Vector& dir)
    {
        this->end_pos += dir;
        this->dest_pos += dir;
        this->start_pos += dir;
        this->position += dir;
        this->polygon.Translate(dir);
        this->offset_polygon.Translate(dir);
    }

    Vector Skillshot::UpdateVelocity(Path& path)
    {
        const Vector& vel = path.Velocity;
        if (vel.IsValid()) return vel.Clone();
        Vector dir = path.EndPos - path.StartPos;
        path.Velocity = dir.Normalize() * path.Speed;
        return path.Velocity;
    }
}
