
#include "Utilities.h"

namespace IPrediction
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

    AoeSolution Utilities::GetAoeSolution(const PredictionInput& input,
        const Linq<Vector>& candidates, const Vector& star_point) const
    {
        IAoeSpell* spell = nullptr;
        switch (input.SpellType)
        {
            case SpellType::None: return AoeSolution();
            case SpellType::Conic: return AoeSolution();
            case SpellType::Linear: return AoeSolution();
            case SpellType::Circular: spell = new Circle(input);
        }
        spell->SetCandidates(candidates);
        spell->SetStarPoint(star_point);
        return spell->GetAoeSolution();
    }

    std::vector<CollisionData> Utilities::GetCollision(const PredictionInput& input,
        const Vector& destination, float buffer, uint32_t exclude_id) const
    {
        Linq<CollisionData> results = Linq<CollisionData>();
        if (!destination.IsValid()) return results.ToArray();
        if (!input.CollisionFlags) return results.ToArray();
        if (input.Speed > 1e+4) return results.ToArray();

        Linq<Obj_AI_Base> units;
        Vector source = input.SourcePosition;
        const float range = input.Range + 500.0f;

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

        // Find all potential candidates based on the set collision flag
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

        // Detect any collisions that occur within windwall boundaries
        if ((input.CollisionFlags & (uint32_t)CollisionFlag::WindWall) != 0)
        {
            float radius = MAX(input.Radius, input.Width / 2.0f);
            if (input.SpellType != SpellType::Linear) radius = 100.0f;
            this->program->GetWindWalls().ForEach([&](const auto& data)
            {
                const float hitbox = radius + buffer;
                const auto& polygon = data.Offset(data, radius).Polygon;
                auto solution = Geometry::Intersection(missile, polygon);

                // Add collision if missile intersects windwall
                if (solution.first && solution.second.IsValid())
                {
                    CollisionData data = CollisionData();
                    data.Position = solution.second.Clone();
                    data.CollisionFlag = CollisionFlag::WindWall;
                    results.Append(data);
                }

                // Add collision if missile starts inside the windwall
                else if (Geometry::IsInside(polygon, missile.StartPos))
                {
                    CollisionData data = CollisionData();
                    data.Position = missile.StartPos.Clone();
                    data.CollisionFlag = CollisionFlag::WindWall;
                    results.Append(data);
                }
            });
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
        // Return if we are unable to hit the target
        const Obj_AI_Base& target = input.TargetObject;
        if (!output.TargetPosition.IsValid()) return;
        if (!this->ShouldCast(target)) return;

        Vector source = input.SourcePosition;
        Vector cast_pos = output.CastPosition;
        const Vector& pred_pos = output.TargetPosition;
        const Segment& last_path = output.Waypoints.back();

        float timer = this->api->GetTime();
        float mia_time = this->program->GetMiaDuration(target);
        float dash_time = this->program->GetDashDuration(target);
        float path_time = this->program->GetPathChangeTime(target);
        float windup_time = this->program->GetLastWindupTime(target);
        float hit_time = output.Intercept - this->GetTotalLatency(2);

        uint32_t target_id = this->api->GetObjectId(target);
        uint32_t exclusion_id = this->api->FNV1A32("PantheonE");
        bool action_taken = path_time < 0.1f || windup_time < 0.1f;
        bool pantheon = this->api->HasBuff(target, {exclusion_id});
        if (pantheon) hit_time -= output.Intercept - output.TimeToHit;

        float intercept = output.Intercept;
        float hitbox = this->api->GetHitbox(target);
        float speed = this->api->GetMovementSpeed(target);
        float max_margin = output.Margin / speed;

        // Use object position if object is valid
        if (this->api->IsValid(input.SourceObject))
        {
            source = this->api->GetPosition(input.SourceObject);
        }

        // Abort if target is immune at the time of impact
        if (this->GetImmunityTime(target) > hit_time)
        {
            output.HitChance = HitChance::Impossible;
            return;
        }

        // Check if target is out of range
        if (output.Distance > input.Range)
        {
            output.HitChance = HitChance::OutOfRange;
            return;
        }

        // Perform a center-range check for non-circular spells
        else if (source.DistanceSquared(pred_pos) > input.Range *
            input.Range && input.SpellType != SpellType::Circular)
        {
            output.HitChance = HitChance::OutOfRange;
            return;
        }

        // Check for possible collisions along missile path
        cast_pos = source.Extend(cast_pos, output.Distance);
        output.Collisions = this->GetCollision(input, cast_pos,
            this->program->GetCollisionBuffer(), target_id);

        // Return if exceeds max amount of allowed collisions
        if (output.Collisions.size() > input.MaxCollisions)
        {
            output.HitChance = HitChance::Collision;
            return;
        }

        // Return if the target remains immobile until impact
        float immobility = this->GetImmobilityTime(target);
        if (output.TimeToHit - immobility <= 0.0f)
        {
            output.CastRate = CastRate::Precise;
            output.HitChance = HitChance::Immobile;
            output.Confidence = 1.0f; return;
        }

        // Hitting the target before dash completion ensures a hit
        if (dash_time > 0 && output.TargetPosition != last_path.EndPos)
        {
            output.CastRate = CastRate::Precise;
            output.HitChance = HitChance::Dashing;
            output.Confidence = 1.0f; return;
        }

        // Hit is guaranteed if interception covers the escape window
        float pause_time = dash_time > 0 ? dash_time : immobility;
        if ((intercept -= pause_time - mia_time) <= max_margin)
        {
            output.CastRate = CastRate::Precise;
            output.HitChance = HitChance::Certain;
            output.Confidence = 1.0f; return;
        }

        // Assume high accuracy if no path history exists
        const auto& data = this->program->GetPathData();
        if (data.find(target_id) == data.end())
        {
            output.CastRate = CastRate::Moderate;
            output.HitChance = HitChance::Extreme;
            output.Confidence = 0.99f; return;
        }

        // Compute metrics and pass to the model
        const auto& lt = data.at(target_id).Last();
        float hit_ratio = (max_margin / intercept);
        float mean_angle = this->program->GetMeanAngleDiff(target);
        float path_count = (float)(data.at(target_id).Count() - 1);
        float path_len = lt.PathLength, react_time = lt.UpdateTime;
        react_time = MAX(0.0f, react_time - timer + 0.25f) / 0.25f;
        float linear = (float)(input.SpellType == SpellType::Linear);

        // Estimate spell hit probability (0.0% – 99.99%)
        output.Confidence = this->network->Predict({ hit_ratio,
            mean_angle / M_PI_F, MIN(1.0f, path_len / 1000.0f),
            MIN(1.0f, path_count / 5.0f), react_time, linear })[0];
        output.HitChance = output.GetHitChance(output.Confidence);

        // Set cast frequency based on path change time or high confidence
        bool freq = action_taken || output.HitChance >= HitChance::High;
        output.CastRate = freq ? CastRate::Moderate : CastRate::Instant;
    }

    PredictionOutput Utilities::PredictOnPath(const PredictionInput& input) const
    {
        const auto& target = input.TargetObject;
        PredictionOutput output = PredictionOutput();
        Path path = this->program->GetWaypoints(target);
        if (!path.back().EndPos.IsValid()) return output;

        output.Waypoints = path;
        Vector source = input.SourcePosition;
        uint32_t hero_flag = (uint32_t)CollisionFlag::Heroes;
        uint32_t minion_flag = (uint32_t)CollisionFlag::Minions;
        uint32_t mixed_flag = (uint32_t)(hero_flag | minion_flag);
        uint32_t target_id = this->api->GetObjectId(target);

        const auto& data = this->program->GetActiveDashes();
        float mia_time = this->program->GetMiaDuration(target);
        bool casting_dash = this->program->IsCastingDash(target);
        bool is_blinking = this->program->IsBlinking(target);
        bool is_dashing = this->program->IsDashing(target);
        bool conic = input.SpellType == SpellType::Conic;

        const float min_cast_len = 50.0f;
        float hitbox = this->api->GetHitbox(target);
        float delay = input.Delay + this->GetTotalLatency(2);
        float angle = conic ? M_RAD(input.Angle / 2.0f) : 0.0f;
        float radius = MAX(input.Radius, input.Width / 2.0f);
        output.Margin = (radius + hitbox * input.AddHitbox);
        float offset = !is_dashing ? output.Margin : 0.0f;
        float speed = path.back().Speed, boundary = 0.0f;

        // Dash waypoints are extended backwards if windup is active
        // Use a starting point in case position does not lie on path
        const auto fix_position = [&](const Vector& pos) -> Vector
        {
            if (casting_dash == false) return pos;
            const DashData& dash = data.at(target_id);
            const Segment& path = dash.TotalPath.front();
            float length = pos.DistanceSquared(path.EndPos);
            Vector start = path.EndPos - path.Direction * dash.Range;
            return length <= dash.Range * dash.Range ? pos : start;
        };

        // Stationary target has one waypoint with zero length
        const auto is_standing = [&](const Path& path) -> bool
        {
            return path.size() == 1 && IsZero(path[0].Length);
        };

        // Dynamic conic spells are not supported
        if (input.Speed < 1e+4 && conic == true)
        {
            return output;
        }

        // Set the spell boundary to calculate time of impact precisely
        if (input.Speed < 1e+4 && (input.CollisionFlags & mixed_flag) != 0)
        {
            boundary = output.Margin;
        }

        // Use object position if object is valid
        if (this->api->IsValid(input.SourceObject))
        {
            source = this->api->GetPosition(input.SourceObject);
        }

        // Adjust path for time spent in fog of war
        if (casting_dash == false && mia_time > 0.0f)
        {
            path = Geometry::CutPath(path, speed * mia_time);
        }

        // Target is either stationary or blinking
        if (is_blinking || is_standing(path))
        {
            const Vector& start = path.front().StartPos;
            const Vector& ending = path.front().EndPos;
            for (const Vector& position : {start, ending})
            {
                Vector direction = (position - source);
                float distance = direction.Length();
                direction = direction / distance;

                float fixed_len = boundary == 0.0f ? distance
                    : MAX(min_cast_len, distance - boundary);
                output.Distance = MAX(0.0f, distance - boundary);
                output.Intercept = distance / input.Speed + delay;
                output.TimeToHit = output.Distance / input.Speed + delay;
                output.CastPosition = source + direction * fixed_len;
                output.TargetPosition = position.Clone();

                if (!is_blinking || position == ending) return output;
                float time = this->program->GetBlinkDuration(target);
                if (output.TimeToHit < time) return output;
            }
        }

        // For static spells, offset position by delay
        if (input.Speed == FLT_MAX || input.Speed > 1e+4)
        {
            delay += 0.03334f; // Some spells need an extra tick
            output.TargetPosition = Geometry::PositionAfter(path, delay);
            output.Margin += angle * output.TargetPosition.Distance(source);
            offset = delay - (offset > 0.0f ? output.Margin : 0.0f) / speed;
            output.CastPosition = Geometry::PositionAfter(path, offset);

            output.Intercept = output.TimeToHit = delay;
            output.Distance = output.CastPosition.Distance(source);
            output.CastPosition = fix_position(output.CastPosition);
            output.TargetPosition = fix_position(output.TargetPosition);
            return output;
        }

        // Trim target's path by offset to improve accuracy
        path = Geometry::CutPath(path, speed * delay - offset);

        // Calculate the interception point for spell to land accurately
        auto solution = Geometry::Interception(path, source, input.Speed);
        Vector intercept = fix_position(solution.second);
        float shift = solution.first + offset / speed;

        if (boundary > 0.0f)
        {
            // Adjust the path to account for delay
            path = Geometry::CutPath(path, offset);
            volatile float margin = output.Margin;

            // Compute the exact collision for a missile skillshot
            margin -= (hitbox * input.AddHitbox - EPSILON * 2.0f);
            auto segment = Segment(source, intercept, input.Speed);
            auto collision = Geometry::DynamicCollision(segment,
                {Obstacle(std::make_pair(hitbox, path))}, margin);

            // This condition should theoretically never be met
            // It may occur due to floating-point precision errors
            if (collision.size() != 1) return output;

            // Extract collision data: impact position and time
            const Solution& result = collision.front().second;
            float intercept_time = segment.Length / segment.Speed;
            Vector pos = Geometry::PositionAfter(path, result.first);
            Vector cast_pos = segment.StartPos + segment.Direction *
                MAX(min_cast_len, segment.Speed * result.first);

            // Set the output based on collision results
            output.Distance = segment.Speed * result.first;
            output.Intercept = intercept_time + delay;
            output.TimeToHit = result.first + delay;
            output.CastPosition = cast_pos;
            output.TargetPosition = pos;
            return output;
        }

        // Handle the remaining cases
        output.CastPosition = intercept;
        output.TargetPosition = Geometry::PositionAfter(path, shift);
        output.TargetPosition = fix_position(output.TargetPosition);
        output.Distance = output.CastPosition.Distance(source);
        output.TimeToHit = output.Distance / input.Speed + delay;
        output.Intercept = output.TimeToHit;
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

        // Windup duration (e.g., spell casting lock duration)
        float windup_time = this->program->GetWindupDuration(unit);
        return MAX(buff_time, MAX(immobile_time, windup_time));
    }

    float Utilities::GetImmunityTime(const Obj_AI_Base& unit) const
    {
        const auto& buffs = this->data->GetImmunityBuffs();
        float time = this->api->GetBuffTime(unit, buffs);
        return time > 0.0f ? time + 0.1f : 0.0f;
    }

    float Utilities::GetTotalLatency(int tick_counter) const
    {
        return this->api->GetPing() + 0.03334f * tick_counter;
    }

    Path Utilities::GetWaypoints(const Obj_AI_Base& unit) const
    {
        Path empty = Path({ Segment() });
        if (!this->api->IsValid(unit)) return empty;
        if (!this->api->IsVisible(unit)) return empty;
        if (!this->api->IsAlive(unit)) return empty;

        Path result = Path();
        float speed = this->api->GetPathSpeed(unit);
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
            !this->api->HasBuff(unit, { 0x3DC195A6 }) ? 6.5f : 0.0f);
    }
}
