
#ifndef API_H
#define API_H

#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_set>
#include "API/api_public.h"
#include "API/enums.h"
#include "Linq.cpp"
#include "Vector.h"

namespace IPrediction
{
    using Buff = BuffInstance*;
    using BuffType = SDK::Enums::BuffType;
    using BuffTable = std::unordered_set<uint32_t>;
    using BuffTypes = std::unordered_set<BuffType>;
    using CastInfo = SpellCastInfo*;
    using Object = GameObject*;
    using Obj_AI_Base = AIBaseCommon*;
    using Obj_AI_Hero = AIHeroPlayer*;
    using Obj_AI_Minion = AIMinionCommon*;
    using PublicAPI = NS SurrenderAt15*;
    using SpellState = SDK::Enums::SpellState;
    using Vector2 = SDK::Structs::Math::Vector2;
    using Vector3 = SDK::Structs::Math::Vector3;

    enum class EventType
    {
        OnDraw = 0,
        OnNewPath,
        OnProcessSpell
    };

    class API
    {
        private:
            PublicAPI m_api = nullptr;
            Obj_AI_Hero m_hero = nullptr;
            static API* m_instance;

            API(PublicAPI api = nullptr);

        public:
            static void Destroy();
            static API* Get(PublicAPI api = nullptr);

            void RegisterEvent(EventType type, void* callback);
            void UnloadEvents();

            uint32_t ARGB(int alpha, int red, int green, int blue) const;
            uint32_t FNV1A32(const char* str) const;

            Obj_AI_Hero GetHero();
            Vector GetCursorPos() const;
            float GetLatency() const;
            float GetPing() const;
            float GetTime() const;

            bool CanUseSpell(int slot);
            void CastSpell(int slot, const Object& unit);
            void CastSpell(int slot, const Vector& pos, float height);

            void DrawCenteredText(const Vector2& pos, const char* text, uint32_t color);
            void DrawCircle(const Vector& pos, float radius, float height, uint32_t color);
            void DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color);
            void DrawPolygon(const std::vector<Vector>& poly, float height, uint32_t color);
            uint32_t FixColor(const uint32_t color) const;
            float GetHeight(const Object& object) const;
            float GetHeight(const Vector& pos) const;
            Vector2 ToScreen(const Vector& pos, float height);

            std::string GetSpellCastName(const CastInfo& info) const;
            Vector GetSpellCastEndPos(const CastInfo& info) const;
            Vector GetSpellCastStartPos(const CastInfo& info) const;
            float GetSpellCastStartTime(const CastInfo& info) const;
            float GetSpellCastWindupTime(const CastInfo& info) const;

            bool Compare(const Object& a, const Object& b) const;
            uint32_t GetObjectId(const Object& object) const;
            std::string GetObjectName(const Object& object) const;
            std::string GetCharacterName(const Obj_AI_Base& unit) const;
            std::string GetSpellName(const Obj_AI_Hero& unit, int slot) const;

            Linq<Obj_AI_Base> GetAllyHeroes(float range, const Vector& pos, bool valid = true) const;
            Linq<Obj_AI_Base> GetEnemyHeroes(float range, const Vector& pos, bool valid = true) const;
            Linq<Obj_AI_Base> GetHeroes(float range, const Vector& pos, bool valid = true) const;
            Linq<Obj_AI_Base> GetAllyMinions(float range, const Vector& pos, bool filter = true) const;
            Linq<Obj_AI_Base> GetEnemyMinions(float range, const Vector& pos, bool filter = true) const;
            Linq<Obj_AI_Base> GetMinions(float range, const Vector& pos, bool filter = true) const;

            std::string GetBuffName(const Buff& buff) const;
            float GetBuffTime(const Obj_AI_Base& unit, const BuffTable& hashes) const;
            float GetBuffTime(const Obj_AI_Base& unit, const BuffTypes& types) const;
            bool HasBuff(const Obj_AI_Base& unit, const BuffTable& hashes) const;
            bool HasBuff(const Obj_AI_Base& unit, const BuffTypes& types) const;

            bool IsAlly(const Obj_AI_Base& unit) const;
            bool IsAlive(const Obj_AI_Base& unit) const;
            bool IsDead(const Obj_AI_Base& unit) const;
            bool IsEnemy(const Obj_AI_Base& unit) const;
            bool IsHero(const Obj_AI_Base& unit) const;
            bool IsMoving(const Obj_AI_Base& unit) const;
            bool IsPlant(const Obj_AI_Minion& unit) const;
            bool IsValid(const Object& unit) const;
            bool IsVisible(const Obj_AI_Base& unit) const;
            bool IsWard(const Obj_AI_Minion& unit) const;

            float GetDashSpeed(const Obj_AI_Base& unit) const;
            Vector GetDashEndPos(const Obj_AI_Base& unit) const;
            Vector GetDashStartPos(const Obj_AI_Base& unit) const;
            Vector GetDirection(const Obj_AI_Base& unit) const;
            float GetHitbox(const Obj_AI_Base& unit) const;
            float GetMovementSpeed(const Obj_AI_Base& unit) const;
            Linq<Vector> GetPath(const Obj_AI_Base& unit) const;
            float GetPathLength(const Linq<Vector>& path) const;
            float GetPathSpeed(const Obj_AI_Base& unit) const;
            Vector GetPosition(const Obj_AI_Base& unit) const;
            bool IsDashing(const Obj_AI_Base& unit) const;

            Vector2 To2D(const Vector& vec) const;
            Vector3 To3D(const Vector& vec, float y = 0.0f) const;
            Vector ToVector(const Vector2& pos) const;
            Vector ToVector(const Vector3& pos) const;
    };
}

#endif // API_H
