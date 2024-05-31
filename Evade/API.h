
#ifndef API_H
#define API_H

#include "API/api_public.h"
#include "API/enums.h"
#include "Poly.h"

namespace Evade
{
    using BuffInstance = BuffInstance;
    using BuffType = sdk::enums::BuffType;
    using GameObject = GameObject;
    using MissileClient = Missile;
    using Obj_AI_Base = AIBaseCommon;
    using Obj_AI_Hero = AIHeroPlayer;
    using Obj_AI_Minion = AIMinionCommon;
    using PublicAPI = NS SurrenderAt15;
    using SpellCastInfo = SpellCastInfo;
    using VecInt2 = NSSDK structs::math::vector2;
    using VecInt3 = NSSDK structs::math::vector3;

    class API
    {
        private:
            PublicAPI* m_api = nullptr;
            Obj_AI_Hero* m_hero = nullptr;
            static API* m_instance;

            API(PublicAPI* api = nullptr);

        public:
            static void Destroy();
            static API* Get(PublicAPI* api = nullptr);

            uint32_t ARGB(int alpha, int red, int green, int blue) const;
            uint32_t FNV1A32(const char* str) const;

            float GetTime() const;
            float GetLatency() const;
            Obj_AI_Hero* GetMyHero();

            Obj_AI_Hero* AsHero(GameObject* unit) const;
            Obj_AI_Minion* AsMinion(GameObject* unit) const;
            MissileClient* AsMissile(GameObject* unit) const;
            bool IsHero(GameObject* unit) const;
            bool IsMinion(GameObject* unit) const;
            bool IsMissile(GameObject* unit) const;

            void CastSpell(int slot, GameObject* unit);
            void CastSpell(int slot, const Vector& pos, float height);
            Linq<Vector> GetPath(const Vector& destination) const;
            bool IsReachable(const Vector& destination) const;
            void SendMoveCommand(const Vector& pos, float height);

            void DrawCircle(const Vector& pos, float radius, float height, uint32_t color);
            void DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color);
            void DrawPolygon(const Poly& poly, float height, uint32_t color);
            float GetHeight(const Vector& pos) const;
            VecInt3 ToScreen(const Vector& pos, float height);

            Vector GetMissileDestination(MissileClient* missile) const;
            Vector GetMissileStart(MissileClient* missile) const;
            float GetMissileSpeed(MissileClient* missile) const;
            float GetMissileWidth(MissileClient* missile) const;

            int GetSpellCastLevel(SpellCastInfo* info) const;
            std::string GetSpellCastName(SpellCastInfo* info) const;
            uint32_t GetSpellCastOwnerId(SpellCastInfo* info) const;
            Vector GetSpellCastDestination(SpellCastInfo* info) const;
            Vector GetSpellCastStart(SpellCastInfo* info) const;
            float GetSpellCastStartTime(SpellCastInfo* info) const;
            float GetSpellCastWindupTime(SpellCastInfo* info) const;

            float GetArmor(Obj_AI_Hero* unit) const;
            float GetAttackRange(Obj_AI_Hero* unit) const;
            float GetAttackSpeedMod(Obj_AI_Hero* unit) const;
            float GetBonusArmor(Obj_AI_Hero* unit) const;
            float GetBonusAttackDamage(Obj_AI_Hero* unit) const;
            float GetBonusMana(Obj_AI_Hero* unit) const;
            float GetBonusMagicResistance(Obj_AI_Hero* unit) const;
            float GetFlatMagicPenetration(Obj_AI_Hero* unit) const;
            float GetHealth(Obj_AI_Hero* unit) const;
            float GetHitbox(Obj_AI_Hero* unit) const;
            float GetLethality(Obj_AI_Hero* unit) const;
            int GetLevel(Obj_AI_Hero* unit) const;
            float GetMaxHealth(Obj_AI_Hero* unit) const;
            float GetMaxMana(Obj_AI_Hero* unit) const;
            float GetMovementSpeed(Obj_AI_Hero* unit) const;
            float GetMagicResistance(Obj_AI_Hero* unit) const;
            float GetPercentArmorPenetration(Obj_AI_Hero* unit) const;
            float GetPercentBonusArmorPenetration(Obj_AI_Hero* unit) const;
            float GetPercentMagicPenetration(Obj_AI_Hero* unit) const;
            float GetTotalAttackDamage(Obj_AI_Hero* unit) const;
            std::string GetCharacterName(Obj_AI_Hero* unit) const;

            Linq<Obj_AI_Hero*> GetAllyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero*> GetEnemyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero*> GetHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Minion*> GetAllyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion*> GetEnemyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion*> GetMinions(float range, const Vector& pos) const;

            int GetBuffCount(Obj_AI_Hero* unit, const uint32_t hash) const;
            float GetBuffRemainingTime(Obj_AI_Hero* unit, const uint32_t hash) const;
            int GetItemSlot(Obj_AI_Hero* unit, const int item_id) const;
            bool HasBuff(Obj_AI_Hero* unit, const uint32_t hash) const;
            bool HasBuffType(Obj_AI_Hero* unit, const BuffType type) const;

            bool IsAlive(Obj_AI_Base* unit) const;
            bool IsAlly(Obj_AI_Base* unit) const;
            bool IsAttacking(Obj_AI_Base* unit) const;
            bool IsDead(Obj_AI_Base* unit) const;
            bool IsEnemy(Obj_AI_Base* unit) const;
            bool IsImmortal(Obj_AI_Base* unit) const;
            bool IsMoving(Obj_AI_Base* unit) const;
            bool IsValid(Obj_AI_Base* unit) const;
            bool IsVisible(Obj_AI_Base* unit) const;
            bool IsVulnerable(Obj_AI_Base* unit) const;
            bool IsWindingUp(Obj_AI_Base* unit) const;

            bool IsDashing(Obj_AI_Base* unit) const;
            float GetDashSpeed(Obj_AI_Base* unit) const;
            Vector GetDashStart(Obj_AI_Base* unit) const;
            Vector GetDashDestination(Obj_AI_Base* unit) const;
            Vector GetPathDestination(Obj_AI_Base* unit) const;
            Vector GetPosition(Obj_AI_Base* unit) const;
            auto GetWaypoints(Obj_AI_Base* unit) const;

            VecInt2 To2D(const Vector& vec) const;
            VecInt3 To3D(const Vector& vec, float y = 0.0f) const;
            Vector ToVector(const VecInt2& pos) const;
            Vector ToVector(const VecInt3& pos) const;
    };
}

#endif // API_H
