
#include "Skillshot.h"

namespace Evade
{
    int Skillshot::counter = 0;

    Skillshot::Skillshot(const ActiveData& data) : data(data) {}

    void Skillshot::Initialize(const ActiveData& data)
    {
        this->unique_id = counter++;
        this->FixOrigin();
        this->Update(true);
    }

    void Skillshot::LoadData(const ActiveData& data)
    {
        this->data = data;
    }

    void Skillshot::FixOrigin()
    {
        bool unfixed = !data.FixedRange;
        float dist = data.StartPos.Distance(data.DestPos);
        if (unfixed && dist < data.Range) data.Range = dist;

        data.Direction = (data.DestPos - data.StartPos).Normalize();
        data.Direction = data.Direction.Rotate(data.RotationAngle);
        data.Perpendicular = data.Direction.Perpendicular();

        data.EndPos = data.StartPos + data.Direction * data.Range;
        data.StartPos = data.StartPos + data.Direction * data.Offset;
        data.Range -= data.Offset; data.Offset = data.RotationAngle = 0.0f;

        if (data.Rotate90)
        {
            auto distance = data.Perpendicular * data.SideRange;
            data.StartPos = data.EndPos - distance * 0.5f;
            data.EndPos = data.EndPos + distance * 0.5f;
            data.Direction = data.Perpendicular.Clone();
            data.Perpendicular = data.Direction.Perpendicular();
            data.Range = data.SideRange; data.Rotate90 = false;
        }

        if (!data.Invert) return;
        data.Direction = -(data.Direction);
        data.Perpendicular = -(data.Perpendicular);
        data.StartPos.SwapXY(data.EndPos);
        data.Invert = false;
    }

    bool Skillshot::IsExpired() const
    {
        float elapsed = API::Get()->GetTime() - data.StartTime;
        return elapsed >= this->TotalLifeTime();
    }

    bool Skillshot::IsPathDangerousInternal(Path& path, bool skip)
    {
        // Destination point is within skillshot area
        if (!skip && !this->IsSafe(path.EndPos)) return true;

        // Skillshot is about to expire, any collision results in hit
        float left = this->TimeToHitInternal() - path.Delay;
        if (left <= 0) return this->IsDangerous(path.StartPos) ||
            this->PathIntersection({path.StartPos, path.EndPos}).Any();

        // Collision is impossible for a blink
        if (IsInfinite(path.Speed)) return false;

        // Predict collision in the future
        Vector vel = this->UpdateVelocity(path);
        Vector p1 = path.StartPos + vel * (left + path.Delta);
        Vector p2 = path.StartPos + vel * (left - path.Delta);

        // No error propagation, so only one position is checked
        if (path.Delta == 0.0f) return this->IsDangerous(p1);

        // Check if collision occured within a tiny time frame
        return this->IsDangerous(p1) || this->IsDangerous(p2)
            || this->PathIntersection({ p1, p2 }).Count() > 0;
    }

    float Skillshot::TimeToHitInternal() const
    {
        bool huge = IsInfinite(data.Speed);
        float life_time = huge ? 0.0f : data.Range / data.Speed;
        float elapsed = API::Get()->GetTime() - data.StartTime;
        return MAX(0.0f, data.Delay - elapsed + life_time);
    }

    float Skillshot::TotalLifeTime() const
    {
        bool huge = IsInfinite(data.Speed);
        float life_time = huge ? 0.0f : data.Range / data.Speed;
        return MAX(0.0f, data.Delay + data.ExtraTime + life_time);
    }

    void Skillshot::Translate(const Vector& dir)
    {
        data.EndPos += dir;
        data.DestPos += dir;
        data.StartPos += dir;
        data.Position += dir;
        data.Polygon.Translate(dir);
        data.OffsetPoly.Translate(dir);
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
