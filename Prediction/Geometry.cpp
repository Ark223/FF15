
#include "Geometry.h"

namespace Prediction
{
    float Geometry::Angle(const Vector& v1, const Vector& v2)
    {
        float dot = v1.DotProduct(v2);
        float cross = v1.CrossProduct(v2);
        return std::atan2(ABS(cross), dot);
    }

    Path Geometry::CutPath(const Path& path, float length)
    {
        Path result = Path();
        if (path.empty()) return result;
        if (IsZero(length)) return path;

        // Helper function to update segment with a new point
        auto update = [](const Segment& segment, float remaining)
        {
            Segment updated = segment;
            Vector direction = segment.Direction * remaining;
            updated.StartPos = segment.StartPos + direction;
            updated.Length = segment.Length - remaining;
            return updated;
        };

        if (length < 0)
        {
            // Extrapolate backwards if length is negative
            result.push_back(update(path.front(), length));

            // Push all remaining points from final path
            for (size_t id = 1; id < path.size(); ++id)
            {
                result.push_back(path[id]);
            }
            return result;
        }

        // Traverse segments until cut lies within one
        float remaining = length; size_t index;
        for (index = 0; index < path.size(); ++index)
        {
            if (remaining <= path[index].Length + EPSILON)
            {
                // Normalize if cut occurs at the boundary
                if (IsZero(remaining - path[index].Length))
                {
                    remaining = 0.0f; ++index;
                }
                break;
            }
            remaining -= path[index].Length;
        }

        // Return last point if cut exceeds path length
        if (index == path.size() || remaining < 0.0f)
        {
            const Segment& last = path.back();
            return { Segment(last.EndPos, last.Speed) };
        }

        // Interpolate new starting point on current segment
        result.push_back(update(path[index], remaining));

        // Append the remainder of the original path
        for (size_t id = index + 1; id < path.size(); ++id)
        {
            result.push_back(path[id]);
        }
        return result;
    }

    float Geometry::FindRoot(float a, float b, float c, float max)
    {
        // Solution range validation helper
        auto root = [&max](float t) -> float
        {
            return (t >= 0 && t <= max) ? t : FLT_MAX;
        };

        // Simplification to our problem
        if (c <= EPSILON) return 0.0f;

        // Solve linear equation if a = 0 & b != 0
        bool a_zero = IsZero(a), b_zero = IsZero(b);
        if (a_zero && !b_zero) return root(-c / b);
        if (a_zero && b_zero) return FLT_MAX;

        // Solve quadratic equation if a != 0
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) return FLT_MAX;
        float sqrt = std::sqrtf(discriminant);

        // Choose minimum out of two solutions
        float t1 = (-b - sqrt) / (2.0f * a);
        float t2 = (-b + sqrt) / (2.0f * a);
        return MIN(root(t1), root(t2));
    }

    std::vector<Collision> Geometry::DynamicCollision(const Segment& missile,
        const std::vector<Obstacle>& obstacles, float hitbox, size_t* hit_index)
    {
        std::vector<Collision> results;
        if (obstacles.empty()) return results;

        const Vector& start = missile.StartPos;
        const Vector& velocity = missile.Velocity;
        float total = missile.Length / missile.Speed;
        float travel_time = total;

        // Update collision list and index of the unit hit first by missile
        auto update_results = [&](float time, Vector& point, size_t index)
        {
            bool update = time < travel_time;
            if (update && hit_index) hit_index = &index;
            results.push_back({ index, {time, point} });
            travel_time = MIN(travel_time, time);
        };

        for (size_t id = 0; id < obstacles.size(); ++id)
        {
            bool collision_found = false;
            const Obstacle& obstacle = obstacles[id];
            const float target_hitbox = obstacle.first;
            const Path& target_path = obstacle.second;
            const auto& last_path = target_path.back();

            float elapsed = 0.0f;
            float total_hitbox = hitbox + target_hitbox;
            float sqr_hitbox = total_hitbox * total_hitbox;

            for (const Segment& segment : target_path)
            {
                // Ensure that obstacle continues along its path
                float duration = segment.Length / segment.Speed;
                duration = MIN(duration, total - elapsed);
                if (duration <= 0.0f) continue;

                Vector shifted = start + velocity * elapsed;
                Vector d_position = shifted - segment.StartPos;
                Vector d_velocity = velocity - segment.Velocity;

                // Setup quadratic coefficients based on the formula
                // Collision condition: |M(t) - O(t)| = total_hitbox
                float a = d_velocity.LengthSquared();
                float b = 2.0f * d_position.DotProduct(d_velocity);
                float c = d_position.LengthSquared() - sqr_hitbox;

                // Solve quadratic for local collision time along segment
                float intercept = Geometry::FindRoot(a, b, c, duration);
                if (intercept >= 0.0f && intercept < FLT_MAX)
                {
                    Vector point = shifted + velocity * intercept;
                    update_results(elapsed + intercept, point, id);
                    collision_found = true; break;
                }

                // Check next segment or stop if completed
                if ((elapsed += duration) >= total) break;
            }

            // Stationary position remained
            float remain = total - elapsed;
            if (!collision_found && remain >= 0.0f)
            {
                // Obstacle velocity is just zero in this case
                Vector shifted = start + velocity * elapsed;
                Vector d_position = shifted - last_path.EndPos;
                Vector d_velocity = velocity - Vector();

                // Use the same algorithm to calculate time
                float a = d_velocity.LengthSquared();
                float b = 2.0f * d_position.DotProduct(d_velocity);
                float c = d_position.LengthSquared() - sqr_hitbox;

                // Collision occurs only if intercept time is valid
                float intercept = Geometry::FindRoot(a, b, c, remain);
                if (intercept >= 0.0f && intercept < FLT_MAX)
                {
                    Vector point = shifted + velocity * intercept;
                    update_results(elapsed + intercept, point, id);
                }
            }
        }

        return results;
    }

    Solution Geometry::Interception(const Path& path, const Vector& source, float speed)
    {
        float elapsed = 0.0f;
        float sqr_speed = speed * speed;
        for (const Segment& segment : path)
        {
            const Vector& start = segment.StartPos;
            const Vector& velocity = segment.Velocity;
            float duration = segment.Length / segment.Speed;
            Vector diff = start - source;

            // Setup quadratic coefficients based on the formula:
            // |diff + velocity * t| = (speed * (elapsed + t))
            float a = velocity.LengthSquared() - sqr_speed;
            float b = 2.0f * (diff.DotProduct(velocity) - sqr_speed * elapsed);
            float c = diff.LengthSquared() - sqr_speed * elapsed * elapsed;

            // Solve quadratic for local intercept time along segment
            float intercept = Geometry::FindRoot(a, b, c, duration);
            if (intercept >= 0.0f && intercept < FLT_MAX)
            {
                Vector point = start + velocity * intercept;
                return { elapsed + intercept, point };
            }

            // Check next segment
            elapsed += duration;
        }

        // Fallback: target has finished path, return last point
        float distance = source.Distance(path.back().EndPos);
        return { distance / speed, path.back().EndPos };
    }

    Solution Geometry::Intersection(const Segment& missile, const std::vector<Vector>& poly)
    {
        float dist = FLT_MAX;
        size_t size = poly.size();
        if (size < 2) return Solution();

        const Vector& start = missile.StartPos;
        const Vector& ending = missile.EndPos;
        Solution solution = { dist, Vector() };

        // Iterate over polygon (rectangle box) edges
        for (size_t i = 0, j = size - 1; i < size; j = i++)
        {
            // Compute intersection of missile with edge and process further
            Vector pos = start.SegmentIntersection(ending, poly[i], poly[j]);

            // Update solution if a closer valid intersection was found
            dist = pos.IsValid() ? pos.DistanceSquared(start) : FLT_MAX;
            if (dist < solution.first) solution = { dist, pos };
        }

        return solution;
    }

    Vector Geometry::PositionAfter(const Path& path, float time)
    {
        // Return a null vector if path is empty
        if (path.empty()) return Vector();

        // If target is stationary, return its end point
        if (path.size() == 1 && path[0].Length == 0.0f)
        {
            return path[0].EndPos;
        }

        // Extrapolate backwards for negative time
        if (time < 0)
        {
            const Segment& segment = path[0];
            float offset = segment.Speed * time;
            const Vector& direction = segment.Direction;
            return segment.StartPos + direction * offset;
        }

        // Traverse segments to find position
        float remaining = time;
        for (const Segment& segment : path)
        {
            const Vector& direction = segment.Direction;
            float duration = segment.Length / segment.Speed;
            if (remaining < duration)
            {
                float offset = segment.Speed * remaining;
                return segment.StartPos + direction * offset;
            }
            remaining -= duration;
        }

        // Fallback: duration exceeded
        return path.back().EndPos;
    }
}
