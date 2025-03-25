
#ifndef API_H
#define API_H

#include "API/api_public.h"
#include "API/enums.h"
#include "Poly.h"

namespace Evade
{
    using Buff = BuffInstance*;
    using BuffType = SDK::Enums::BuffType;
    using CastInfo = SpellCastInfo*;
    using MissileClient = Missile*;
    using Object = GameObject*;
    using Obj_AI_Base = AIBaseCommon*;
    using Obj_AI_Hero = AIHeroPlayer*;
    using Obj_AI_Minion = AIMinionCommon*;
    using ParticleEmitter = EffectEmitter*;
    using PublicAPI = NS SurrenderAt15*;
    using SpellState = SDK::Enums::SpellState;
    using Vector2 = SDK::Structs::Math::Vector2;
    using Vector3 = SDK::Structs::Math::Vector3;

    enum class EventType
    {
        ON_TICK = 0,
        ON_DRAW,
        ON_PROCESS_SPELL,
        ON_CREATE_OBJECT,
        ON_DELETE_OBJECT,
        ON_BUFF_GAIN,
        ON_WND_PROC,
        ON_ISSUE_ORDER
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
            uint32_t GetMapId() const;
            float GetLatency() const;
            float GetTime() const;

            Obj_AI_Hero AsHero(const Object& object) const;
            Obj_AI_Minion AsMinion(const Object& object) const;
            MissileClient AsMissile(const Object& object) const;
            ParticleEmitter AsParticle(const Object& object) const;
            bool IsHero(const Object& object) const;
            bool IsMinion(const Object& object) const;
            bool IsMissile(const Object& object) const;
            bool IsParticle(const Object& object) const;

            void BlockCast();
            void BlockOrder();
            bool CanUseSpell(int slot);
            void CastSpell(int slot, const Object& unit);
            void CastSpell(int slot, const Vector& pos, float height);
            void SendMoveCommand(const Vector& pos, float height);
            void SetOverrideOrder(const Vector& pos, float height);

            void DrawCenteredText(const Vector2& pos, const char* text, uint32_t color);
            void DrawCircle(const Vector& pos, float radius, float height, uint32_t color);
            void DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color);
            void DrawPolygon(const Poly& poly, float height, uint32_t color);
            uint32_t FixColor(const uint32_t color) const;
            float GetHeight(const Object& object) const;
            float GetHeight(const Vector& pos) const;
            Vector2 ToScreen(const Vector& pos, float height);

            Vector GetDirection(const ParticleEmitter& particle) const;
            Vector GetMissileEndPos(const MissileClient& missile) const;
            Vector GetMissileStartPos(const MissileClient& missile) const;
            std::string GetMissileName(const MissileClient& missile) const;
            Object GetMissileOwner(const MissileClient& missile) const;
            float GetMissileSpeed(const MissileClient& missile) const;
            float GetMissileWidth(const MissileClient& missile) const;

            int GetSpellCastLevel(const CastInfo& info) const;
            std::string GetSpellCastName(const CastInfo& info) const;
            Vector GetSpellCastEndPos(const CastInfo& info) const;
            Vector GetSpellCastStartPos(const CastInfo& info) const;
            float GetSpellCastStartTime(const CastInfo& info) const;
            float GetSpellCastWindupTime(const CastInfo& info) const;
            uint32_t GetSpellCastTargetId(const CastInfo& info) const;

            float GetArmor(const Obj_AI_Base& unit) const;
            float GetAttackDelay(const Obj_AI_Base& unit) const;
            float GetAttackRange(const Obj_AI_Base& unit) const;
            float GetAttackSpeedMod(const Obj_AI_Base& unit) const;
            float GetAttackWindup(const Obj_AI_Base& unit) const;
            float GetBonusArmor(const Obj_AI_Base& unit) const;
            float GetBonusAbilityDamage(const Obj_AI_Base& unit) const;
            float GetBonusAttackDamage(const Obj_AI_Base& unit) const;
            float GetBonusHealth(const Obj_AI_Base& unit) const;
            float GetBonusMana(const Obj_AI_Base& unit) const;
            float GetBonusMagicResistance(const Obj_AI_Base& unit) const;
            float GetFlatMagicPenetration(const Obj_AI_Base& unit) const;
            float GetHealth(const Obj_AI_Base& unit) const;
            float GetHealthRatio(const Obj_AI_Base& unit) const;
            float GetHitbox(const Obj_AI_Base& unit) const;
            float GetLethality(const Obj_AI_Base& unit) const;
            int GetLevel(const Obj_AI_Hero& unit) const;
            float GetMaxHealth(const Obj_AI_Base& unit) const;
            float GetMaxMana(const Obj_AI_Base& unit) const;
            float GetMovementSpeed(const Obj_AI_Base& unit) const;
            float GetMagicResistance(const Obj_AI_Base& unit) const;
            float GetPercentArmorPenetration(const Obj_AI_Base& unit) const;
            float GetPercentBonusArmorPenetration(const Obj_AI_Base& unit) const;
            float GetPercentMagicPenetration(const Obj_AI_Base& unit) const;
            int GetSpellLevel(const Obj_AI_Hero& unit, int slot) const;
            float GetTotalAbilityDamage(const Obj_AI_Base& unit) const;
            float GetTotalAttackDamage(const Obj_AI_Base& unit) const;

            bool Compare(const Object& a, const Object& b) const;
            uint32_t GetObjectId(const Object& object) const;
            std::string GetObjectName(const Object& object) const;
            std::string GetCharacterName(const Obj_AI_Base& unit) const;
            std::string GetSpellName(const Obj_AI_Hero& unit, int slot) const;

            float CalcDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount, int type) const;
            float CalcPhysicalDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const;
            float CalcMagicalDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const;
            float CalcMixedDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const;

            Linq<Obj_AI_Hero> GetAllyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero> GetEnemyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero> GetHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Minion> GetAllyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion> GetEnemyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion> GetMinions(float range, const Vector& pos) const;

            std::string GetBuffName(const Buff& buff) const;
            Object GetBuffOwner(const Buff& buff) const;
            int GetBuffCount(const Obj_AI_Hero& unit, uint32_t hash) const;
            float GetBuffTime(const Obj_AI_Hero& unit, uint32_t hash) const;
            int GetItemSlot(const Obj_AI_Hero& unit, int item_id) const;
            bool HasBuff(const Obj_AI_Hero& unit, uint32_t hash) const;
            bool HasBuffType(const Obj_AI_Hero& unit, BuffType type) const;

            bool IsAlive(const Obj_AI_Base& unit) const;
            bool IsAlly(const Obj_AI_Base& unit) const;
            bool IsAttacking(const Obj_AI_Hero& unit) const;
            bool IsDead(const Obj_AI_Base& unit) const;
            bool IsEnemy(const Obj_AI_Base& unit) const;
            bool IsImmortal(const Obj_AI_Base& unit) const;
            bool IsMoving(const Obj_AI_Base& unit) const;
            bool IsValid(const Object& unit) const;
            bool IsVisible(const Obj_AI_Base& unit) const;
            bool IsVulnerable(const Obj_AI_Base& unit) const;
            bool IsWindingUp(const Obj_AI_Base& unit) const;

            float GetDashSpeed(const Obj_AI_Base& unit) const;
            Vector GetDashEndPos(const Obj_AI_Base& unit) const;
            Vector GetDashStartPos(const Obj_AI_Base& unit) const;
            Vector GetDirection(const Obj_AI_Base& unit) const;
            Linq<Vector> GetPath(const Vector& pos) const;
            Vector GetPathEndPos(const Obj_AI_Base& unit) const;
            float GetPathLength(const Linq<Vector>& path) const;
            Vector GetPosition(const Object& unit) const;
            bool IsDashing(const Obj_AI_Base& unit) const;
            bool IsReachable(const Vector& pos) const;

            Vector2 To2D(const Vector& vec) const;
            Vector3 To3D(const Vector& vec, float y = 0.0f) const;
            Vector ToVector(const Vector2& pos) const;
            Vector ToVector(const Vector3& pos) const;
    };
}

#endif // API_H
