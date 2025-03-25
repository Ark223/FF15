
#ifndef SKILLSHOT_H
#define SKILLSHOT_H

#include "Data.h"

namespace Evade
{
    struct Path
    {
        float Speed, Delay, Delta;
        Vector StartPos, EndPos, Velocity;
        Path(float Speed = FLT_MAX, float Delay = 0, float Delta = 0,
            Vector StartPos = Vector(), Vector EndPos = Vector()) :
            StartPos(StartPos), EndPos(EndPos), Speed(Speed),
            Delay(Delay), Delta(Delta) {}
    };

    class Skillshot
    {
        private:
            static int counter;
            int unique_id = 0;

        public:
            explicit Skillshot(const ActiveData& data);

            int GetId() const { return this->unique_id; }
            const ActiveData& Get() const { return this->data; }
            ActiveData& Set() { return this->data; }

            void LoadData(const ActiveData& data);
            void FixOrigin();
            bool IsExpired() const;
            float TotalLifeTime() const;
            void Translate(const Vector& dir);

            virtual void Draw(uint32_t* colors) = 0;
            virtual void Update(bool force = false) = 0;
            virtual Vector Position(float delta = 0.0f) = 0;
            virtual bool IsSafe(const Vector& pos) = 0;
            virtual bool IsDangerous(const Vector& pos) = 0;
            virtual bool IsPathDangerous(Path& path, bool skip = false) = 0;
            virtual float TimeToHit(const Vector& pos, bool skip = false) = 0;
            virtual Linq<Vector> PathIntersection(const Linq<Vector>& path) = 0;

        protected:
            ActiveData data;

            void Initialize(const ActiveData& data);
            bool IsPathDangerousInternal(Path& path, bool skip);
            float TimeToHitInternal() const;
            Vector UpdateVelocity(Path& path);
    };
}

#endif // SKILLSHOT_H
