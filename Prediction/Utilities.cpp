
#include "Utilities.h"

namespace Prediction
{
    Utilities* Utilities::m_instance = nullptr;

    void Utilities::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Utilities* Utilities::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Utilities();
            m_instance->api = API::Get();
            m_instance->data = Data::Get();
            m_instance->network = Network::Get();
            m_instance->program = Program::Get();
            m_instance->hero = API::Get()->GetHero();
        }
        return m_instance;
    }

    // Utility methods

    void Utilities::DrawPath(const Path& path, float height, uint32_t* colors)
    {
        // Render arrows along the entire path
        float bar_size = 25.0f, head_size = 20.0f;
        for (const Segment& segment : path)
        {
            size_t iter = 0;
            float length = segment.Length;
            if (IsZero(length)) continue;

            const Vector& start = segment.StartPos;
            const Vector& ending = segment.EndPos;
            Vector direction = segment.Direction;

            // Draw the arrow body
            while (length > 0)
            {
                float seg_size = bar_size;
                if (!iter) seg_size *= 2.0f;
                float size = MIN(seg_size, length);
                Vector bar_one = start + direction * length;
                Vector bar_two = bar_one - direction * size;

                uint32_t argb = iter % 2 == 0 ? colors[0] : colors[1];
                this->api->DrawLine(bar_one, bar_two, height, argb);
                length -= (seg_size + bar_size / 2.0f); iter++;
            }

            direction = direction * head_size;
            Vector perpendicular = direction.Perpendicular();
            Vector head_one = ending - direction + perpendicular;
            Vector head_two = ending - direction - perpendicular;

            // Draw the arrowhead lines at the end of arrow
            this->api->DrawLine(ending, head_one, height, colors[0]);
            this->api->DrawLine(ending, head_two, height, colors[0]);
        }
    }

    std::vector<CollisionData> Utilities::GetCollision(const PredictionInput& input,
        const Vector& destination, float buffer, uint32_t exclude_id) const
    {
        Linq<CollisionData> results = Linq<CollisionData>();
        if (!destination.IsValid()) return results.ToArray();

        Vector source = input.SourcePosition;
        const float range = input.Range + 500.0f;
        Linq<Obj_AI_Base> units = Linq<Obj_AI_Base>();

        // Function to exclude unit from processing
        auto should_include = [&](const auto& unit)
        {
            return exclude_id != this->api->GetObjectId(unit);
        };

        // Use object position if object is valid
        if (this->api->IsValid(input.SourceObject))
        {
            source = this->api->GetPosition(input.SourceObject);
        }

        // Define the missile's trajectory from source to destination
        Segment missile = Segment(source, destination, input.Speed);
        Vector middle = (missile.StartPos + missile.EndPos) / 2.0f;

        // Find all potential candidates based on the set collision flags
        if ((input.CollisionFlags & (uint32_t)CollisionFlag::Heroes) != 0)
        {
            auto enemies = this->api->GetEnemyHeroes(range, middle);
            units.AddRange(enemies.Where(should_include));
        }
        if ((input.CollisionFlags & (uint32_t)CollisionFlag::Minions) != 0)
        {
            auto minions = this->api->GetEnemyMinions(range, middle);
            units.AddRange(minions.Where(should_include));
        }

        // Convert each unit into an "obstacle" for collision detection
        auto obstacles = units.Select<Obstacle>([&](const auto& unit)
        {
            float hitbox = this->api->GetHitbox(unit);
            Path waypoints = this->program->GetWaypoints(unit);
            float cut_length = waypoints[0].Speed * input.Delay;
            waypoints = Geometry::CutPath(waypoints, cut_length);
            return (Obstacle)(std::make_pair(hitbox, waypoints));
        });

        // Find dynamic unit collisions and add them to the results
        for (const auto& collision : Geometry::DynamicCollision(
            missile, obstacles.ToArray(), input.Radius + buffer))
        {
            CollisionData data = CollisionData();
            data.Position = collision.second.second;
            data.Object = units.ElementAt(collision.first);
            data.CollisionFlag = this->api->IsHero(data.Object) ?
                CollisionFlag::Heroes : CollisionFlag::Minions;
            results.Append(data);
        }

        // Sort results by distance to collision (closest first)
        results = results.OrderBy<float>([&](const auto& data)
        {
            return data.Position.DistanceSquared(source);
        });
        return results.ToArray();
    }

    void Utilities::GetHitChance(const PredictionInput& input, PredictionOutput& output) const
    {
        // 
        if (!output.TargetPosition.IsValid()) return;

        // 
        if (!this->ShouldCast(input.TargetObject)) return;

    }

    PredictionOutput Utilities::PredictOnPath(const PredictionInput& input) const
    {
        const auto& target = input.TargetObject;
        PredictionOutput output = PredictionOutput();
        Path path = this->program->GetWaypoints(target);
        if (!path.back().EndPos.IsValid()) return output;

        Vector source = input.SourcePosition;
        uint32_t hero_flag = (uint32_t)CollisionFlag::Heroes;
        uint32_t minion_flag = (uint32_t)CollisionFlag::Minions;
        bool casting_dash = this->program->IsCastingDash(target);
        bool dashing = casting_dash || this->api->IsDashing(target);
        bool blinking = this->program->IsBlinking(target);
        bool apply_offset = input.EdgeCast && !dashing;

        float epsilon = EPSILON * 2.0f; // 0.02
        float hitbox = this->api->GetHitbox(target);
        float delay = input.Delay + this->GetTotalLatency();
        float mia = this->program->GetInvisibilityTime(target);
        float radius = MAX(input.Radius, input.Width / 2.0f);
        float speed = path.back().Speed, boundary = 0.0f;
        radius += (input.AddHitbox ? hitbox : 0.0f);
        float offset = apply_offset ? radius : 0.0f;

        // Dash waypoints are extended backwards if windup is active
        // Use a starting point in case position does not lie on path
        const auto fix_position = [&](const Vector& pos) -> Vector
        {
            if (casting_dash == false) return pos;
            const Vector& start = path.front().StartPos;
            float length = start.DistanceSquared(path.back().EndPos);
            return length > start.DistanceSquared(pos) ? pos : start;
        };

        // Set the skillshot boundary to calculate hit time precisely
        if ((input.CollisionFlags & (hero_flag | minion_flag)) != 0)
        {
            boundary = radius;
        }

        // Use object position if object is valid
        if (this->api->IsValid(input.SourceObject))
        {
            source = this->api->GetPosition(input.SourceObject);
        }

        // Enable fog-of-war support
        if (!casting_dash && mia > 0)
        {
            path = Geometry::CutPath(path, speed * mia);
        }

        // Target is either stationary or blinking
        if (blinking || path.size() == 1)
        {
            const Vector& start = path.front().StartPos;
            const Vector& ending = path.front().EndPos;
            for (const Vector& position : { start, ending })
            {
                Vector direction = (position - source);
                float distance = direction.Length();
                direction = direction / distance;
    
                output.Distance = MAX(0.0f, distance - boundary);
                output.TimeToHit = output.Distance / input.Speed + delay;
                output.CastPosition = source + direction * output.Distance;
                output.TargetPosition = position.Clone();

                if (!blinking || position == ending) return output;
                float time = this->program->GetBlinkDuration(target);
                if (output.TimeToHit < time) return output;
            }
        }

        // For static spells, offset position by delay
        if (input.Speed == FLT_MAX || input.Speed > 1e+4)
        {
            float shift = delay - offset / speed;
            output.CastPosition = Geometry::PositionAfter(path, shift);
            output.TargetPosition = Geometry::PositionAfter(path, delay);

            output.TimeToHit = delay; // No edge collision here
            output.Distance = output.CastPosition.Distance(source);
            output.CastPosition = fix_position(output.CastPosition);
            output.TargetPosition = fix_position(output.TargetPosition);
            return output;
        }

        // Trim target's path by offset to boost hit chance
        path = Geometry::CutPath(path, speed * delay - offset);

        // Calculate interception point for a skillshot to land accurately
        auto solution = Geometry::Interception(path, source, input.Speed);
        Vector intercept = fix_position(solution.second);
        float shift = solution.first + offset / speed;

        if (boundary > 0.0f)
        {
            // Adjust the path to account for delay
            path = Geometry::CutPath(path, offset);

            // Compute the exact collision for a missile skillshot
            radius -= (input.AddHitbox ? hitbox : 0.0f) - epsilon;
            auto segment = Segment(source, intercept, input.Speed);
            auto collision = Geometry::DynamicCollision(segment,
                {Obstacle(std::make_pair(hitbox, path))}, radius);

            // This condition should theoretically never be met
            // It may occur due to floating-point precision errors
            if (collision.size() != 1) return output;

            // Extract collision data: impact position and time
            const Solution& result = collision.front().second;
            Vector pos = Geometry::PositionAfter(path, result.first);

            // Set the output based on collision results
            output.Distance = segment.Speed * result.first;
            output.TimeToHit = result.first + delay;
            output.CastPosition = result.second;
            output.TargetPosition = pos;
            return output;
        }

        // Handle the remaining cases
        output.CastPosition = intercept;
        output.TargetPosition = Geometry::PositionAfter(path, shift);
        output.TargetPosition = fix_position(output.TargetPosition);
        output.Distance = output.CastPosition.Distance(source);
        output.TimeToHit = output.Distance / input.Speed + delay;
        return output;
    }

    float Utilities::GetImmobilityTime(const Obj_AI_Base& unit) const
    {
        // Immobilizing buffs (e.g., abilities that root the unit)
        const auto& buffs = this->data->GetImmobilityBuffs();
        float buff_time = this->api->GetBuffTime(unit, buffs);

        // Immobilizing types (e.g., stuns, roots, suppressions)
        const auto& types = this->data->GetImmobilityTypes();
        float immobile_time = this->api->GetBuffTime(unit, types);

        // Windup time (e.g., spell casting lock duration)
        float windup_time = this->program->GetWindupTime(unit);
        return MAX(buff_time, MAX(immobile_time, windup_time));
    }

    float Utilities::GetImmunityTime(const Obj_AI_Base& unit) const
    {
        const auto& buffs = this->data->GetImmunityBuffs();
        return this->api->GetBuffTime(unit, buffs);
    }

    float Utilities::GetTotalLatency(int tick_counter) const
    {
        return this->api->GetPing() + 0.034f * tick_counter;
    }

    Path Utilities::GetWaypoints(const Obj_AI_Base& unit) const
    {
        Path empty = Path({ Segment() });
        if (!this->api->IsValid(unit)) return empty;
        if (!this->api->IsVisible(unit)) return empty;

        Path result = Path();
        float speed = this->api->GetMoveSpeed(unit);
        Linq<Vector> path = this->api->GetPath(unit);
        Vector position = this->api->GetPosition(unit);
        Segment static_pos = Segment(position, speed);

        // Less than 2 points means that unit is standing still
        if (path.Count() < 2) return Path({ static_pos });

        // Find which segment is closest to unit's position
        std::tuple<float, size_t, Vector> best_cut_result;
        best_cut_result = { FLT_MAX, path.Count(), position };
        for (size_t id = 0; id < path.Count() - 1; ++id)
        {
            const Vector& p1 = path.ElementAt(id);
            const Vector& p2 = path.ElementAt(id + 1);
            Vector closest = position.ClosestOnSegment(p1, p2);
            float distance = position.DistanceSquared(closest);

            // Update the best cut if a closer point is found
            if (distance <= std::get<0>(best_cut_result))
                best_cut_result = { distance, id, closest };
        }

        // Use the closest point as a new starting position
        size_t best_index = std::get<1>(best_cut_result);
        position = std::get<2>(best_cut_result);

        // Build segments for the remaining path starting from index
        for (size_t id = best_index; id < path.Count() - 1; ++id)
        {
            const Vector& start = (best_index != id)
                ? path.ElementAt(id) : (Vector)position;
            const Vector& ending = path.ElementAt(id + 1);
            Segment segment = Segment(start, ending, speed);
            if (IsZero(segment.Length)) continue;
            result.push_back(segment);
        }

        // Ensure there is at least one point in the output
        return !result.empty() ? result : Path({ static_pos });
    }

    bool Utilities::ShouldCast(const Obj_AI_Base& unit) const
    {
        // Always allow casting on non-hero units
        if (!this->api->IsHero(unit)) return true;

        // Consider active buffs that make the target immune
        const auto& buffs = this->data->GetExclusionBuffs();
        float time = this->api->GetBuffTime(unit, buffs);

        // Special case: Zed's buff remains active until R2 is used
        return time <= (this->api->GetCharacterName(unit) == "Zed" &&
            !this->api->HasBuff(unit, { 0x48E4DD46 }) ? 6.5f : 0.0f);
    }
}
