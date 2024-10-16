
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
    using SpellState = sdk::enums::SpellState;
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

            Obj_AI_Hero* GetHero();
            Vector GetCursorPos() const;
            float GetLatency() const;
            float GetTime() const;

            Obj_AI_Hero* AsHero(GameObject* object) const;
            Obj_AI_Minion* AsMinion(GameObject* object) const;
            MissileClient* AsMissile(GameObject* object) const;
            bool IsHero(GameObject* object) const;
            bool IsMinion(GameObject* object) const;
            bool IsMissile(GameObject* object) const;
            bool IsParticle(GameObject* object) const;

            void BlockCast();
            void BlockOrder();
            bool CanUseSpell(int slot);
            void CastSpell(int slot, GameObject* unit);
            void CastSpell(int slot, const Vector& pos, float height);
            void SendMoveCommand(const Vector& pos, float height);
            void SetOverrideOrder(const Vector& pos, float height);

            void DrawCenteredText(const VecInt2& pos, const char* text, uint32_t color);
            void DrawCircle(const Vector& pos, float radius, float height, uint32_t color);
            void DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color);
            void DrawPolygon(const Poly& poly, float height, uint32_t color);
            uint32_t FixColor(uint32_t color) const;
            float GetHeight(GameObject* object) const;
            float GetHeight(const Vector& pos) const;
            VecInt2 ToScreen(const Vector& pos, float height);

            Vector GetMissileEndPos(MissileClient* missile) const;
            Vector GetMissileStartPos(MissileClient* missile) const;
            GameObject* GetMissileOwner(MissileClient* missile) const;
            std::string GetMissileName(MissileClient* missile) const;
            float GetMissileSpeed(MissileClient* missile) const;
            float GetMissileWidth(MissileClient* missile) const;

            int GetSpellCastLevel(SpellCastInfo* info) const;
            std::string GetSpellCastName(SpellCastInfo* info) const;
            Vector GetSpellCastEndPos(SpellCastInfo* info) const;
            Vector GetSpellCastStartPos(SpellCastInfo* info) const;
            float GetSpellCastStartTime(SpellCastInfo* info) const;
            float GetSpellCastWindupTime(SpellCastInfo* info) const;
            uint32_t GetSpellCastTargetId(SpellCastInfo* info) const;

            float GetArmor(Obj_AI_Base* unit) const;
            float GetAttackDelay(Obj_AI_Base* unit) const;
            float GetAttackRange(Obj_AI_Base* unit) const;
            float GetAttackSpeedMod(Obj_AI_Base* unit) const;
            float GetAttackWindup(Obj_AI_Base* unit) const;
            float GetBonusArmor(Obj_AI_Base* unit) const;
            float GetBonusAbilityDamage(Obj_AI_Base* unit) const;
            float GetBonusAttackDamage(Obj_AI_Base* unit) const;
            float GetBonusHealth(Obj_AI_Base* unit) const;
            float GetBonusMana(Obj_AI_Base* unit) const;
            float GetBonusMagicResistance(Obj_AI_Base* unit) const;
            float GetFlatMagicPenetration(Obj_AI_Base* unit) const;
            float GetHealth(Obj_AI_Base* unit) const;
            float GetHealthRatio(Obj_AI_Base* unit) const;
            float GetHitbox(Obj_AI_Base* unit) const;
            float GetLethality(Obj_AI_Base* unit) const;
            int GetLevel(Obj_AI_Hero* unit) const;
            float GetMaxHealth(Obj_AI_Base* unit) const;
            float GetMaxMana(Obj_AI_Base* unit) const;
            float GetMovementSpeed(Obj_AI_Base* unit) const;
            float GetMagicResistance(Obj_AI_Base* unit) const;
            float GetPercentArmorPenetration(Obj_AI_Base* unit) const;
            float GetPercentBonusArmorPenetration(Obj_AI_Base* unit) const;
            float GetPercentMagicPenetration(Obj_AI_Base* unit) const;
            int GetSpellLevel(Obj_AI_Hero* unit, int slot) const;
            float GetTotalAbilityDamage(Obj_AI_Base* unit) const;
            float GetTotalAttackDamage(Obj_AI_Base* unit) const;

            bool Compare(GameObject* first, GameObject* second) const;
            Vector GetObjectDirection(GameObject* object) const;
            uint32_t GetObjectNetworkId(GameObject* object) const;
            GameObject* GetObjectOwner(GameObject* object) const;
            std::string GetObjectName(GameObject* object) const;
            std::string GetCharacterName(Obj_AI_Base* unit) const;
            std::string GetSpellName(Obj_AI_Hero* unit, int slot) const;

            float CalcDamage(Obj_AI_Hero* source, Obj_AI_Hero* target, float amount, int type) const;
            float CalcPhysicalDamage(Obj_AI_Hero* source, Obj_AI_Hero* target, float amount) const;
            float CalcMagicalDamage(Obj_AI_Hero* source, Obj_AI_Hero* target, float amount) const;
            float CalcMixedDamage(Obj_AI_Hero* source, Obj_AI_Hero* target, float amount) const;

            Linq<Obj_AI_Hero*> GetAllyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero*> GetEnemyHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Hero*> GetHeroes(float range, const Vector& pos, bool valid) const;
            Linq<Obj_AI_Minion*> GetAllyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion*> GetEnemyMinions(float range, const Vector& pos) const;
            Linq<Obj_AI_Minion*> GetMinions(float range, const Vector& pos) const;

            std::string GetBuffName(BuffInstance* buff) const;
            GameObject* GetBuffOwner(BuffInstance* buff) const;
            int GetBuffCount(Obj_AI_Hero* unit, uint32_t hash) const;
            float GetBuffTime(Obj_AI_Hero* unit, uint32_t hash) const;
            int GetItemSlot(Obj_AI_Hero* unit, const int item_id) const;
            bool HasBuff(Obj_AI_Hero* unit, uint32_t hash) const;
            bool HasBuffType(Obj_AI_Hero* unit, const BuffType type) const;

            bool IsAlive(Obj_AI_Base* unit) const;
            bool IsAlly(Obj_AI_Base* unit) const;
            bool IsAttacking(Obj_AI_Hero* unit) const;
            bool IsDead(Obj_AI_Base* unit) const;
            bool IsEnemy(Obj_AI_Base* unit) const;
            bool IsImmortal(Obj_AI_Base* unit) const;
            bool IsMoving(Obj_AI_Base* unit) const;
            bool IsValid(GameObject* unit) const;
            bool IsVisible(Obj_AI_Base* unit) const;
            bool IsVulnerable(Obj_AI_Base* unit) const;
            bool IsWindingUp(Obj_AI_Base* unit) const;

            float GetDashSpeed(Obj_AI_Base* unit) const;
            Vector GetDashEndPos(Obj_AI_Base* unit) const;
            Vector GetDashStartPos(Obj_AI_Base* unit) const;
            Linq<Vector> GetPath(const Vector& pos) const;
            Vector GetPathEndPos(Obj_AI_Base* unit) const;
            Vector GetPosition(GameObject* unit) const;
            Linq<Vector> GetWaypoints(Obj_AI_Base* unit) const;
            bool IsDashing(Obj_AI_Base* unit) const;
            bool IsReachable(const Vector& pos) const;

            VecInt2 To2D(const Vector& vec) const;
            VecInt3 To3D(const Vector& vec, float y = 0.0f) const;
            Vector ToVector(const VecInt2& pos) const;
            Vector ToVector(const VecInt3& pos) const;
    };
}

#endif // API_H
