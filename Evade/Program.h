
#ifndef PROGRAM_H
#define PROGRAM_H

#include "Circle.h"
#include "Cone.h"
#include "Config.h"
#include "Line.h"
#include "Polygon.h"
#include "Ring.h"

namespace Evade
{
    class Process;
    class Utilities;

    constexpr float SAFETY_BUFFER = 30.0f;
    const std::string EVADE_VERSION = "2025.03.31.02";

    enum class CollectionType { ACTIVE, CONSIDERED, DANGEROUS };

    struct SpellInfo { std::string SpellName; int SpellSlot = -1; };
    struct MissileInfo { std::string CharName, SpellName, MissileName; };
    struct ParticleInfo { std::string CharName, SpellName, ParticleName; };

    using Collisions = std::vector<CollisionFlag>;
    using Objects = Linq<Obj_AI_Minion>;

    class Program
    {
        public:
            Program(PublicAPI api);
            ~Program();

            static Program* Get();

            const bool CanEvade() const { return this->can_evade; }
            const bool CanUseSpell() const { return !this->dont_use_spell; }
            const bool DependencyCheck() const { return this->libs_loaded; }
            const bool DodgeDangerous() const { return this->dodge_dangerous; }

            const int GetOrderCount() const { return this->safe_order_count; }
            const float GetAttackTimer() const { return this->attack_timer; }
            const float GetEvadeTimer() const { return this->evade_timer; }
            const float GetMoveSpeed() const { return this->move_speed; }

            const Vector& GetEvadePos() const { return this->evade_pos; }
            const Vector& GetHeroPos() const { return this->hero_pos; }
            auto& GetEvadingSpells() { return this->evading_spells; }
            auto& GetObjects() { return this->objects; }

            auto& GetSkillshots(CollectionType type = CollectionType::ACTIVE)
            {
                return type == CollectionType::ACTIVE ? this->skillshots : type ==
                    CollectionType::CONSIDERED ? this->considered : this->dangerous;
            }

            bool IsEnabled() const;
            bool IsEvading() const;
            bool IsInsideHardCC() const;
            bool IsInsideSoftCC() const;

            bool IsSafe(float x, float y) const;
            bool IsDangerous(float x, float y) const;
            float TimeToHit(float x, float y) const;

            float TimeToHit() const;
            float CalculateDamage() const;
            int GetDangerLevel() const;

            bool RecalculatePath(float delay = 0.0f);
            void RequestUpdateOnce(int id, float delay = 0.0f);

            template<typename T>
            T GetValue(const std::string& id) const;

        private:
            API* api = nullptr;
            Data* data = nullptr;
            Config* config = nullptr;
            Process* process = nullptr;
            Utilities* utils = nullptr;
            PublicAPI m_api = nullptr;
            Obj_AI_Hero my_hero = nullptr;
            static Program* m_instance;

            void RegisterCore();
            void RegisterEvents();
            void RegisterMenu();

            static void OnTickWrapper();
            static void OnDrawWrapper();
            static void OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo info);
            static void OnCreateObjectWrapper(Object object, uint32_t object_id);
            static void OnDeleteObjectWrapper(Object object);
            static void OnBuffGainWrapper(Obj_AI_Base unit, Buff buff);
            static void OnWndProcWrapper(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
            static void OnIssueOrderWrapper(Vector3 pos, int order, int id, const char* plugin);

            // Main components

            int32_t flash_slot = -1, ghost_slot = -1,
                hash_code = -1, safe_order_count = 0,
                total_retry_count = 0, update_id = -1;
            bool can_evade = true, dodge_dangerous = false,
                dont_use_spell = false, key_pushed = false,
                libs_loaded = false, panic_mode = false,
                recalc_path = true, undodgeable = false;
            float attack_timer = 0.0f, block_timer = 0.0f,
                evade_timer = 0.0f, order_timer = 0.0f,
                recalc_timer = 0.0f, safety_timer = 0.0f,
                summon_timer = 0.0f, move_speed = 0.0f,
                sylas_timer = 0.0f, viego_timer = 0.0f;
            Vector evade_pos, hero_pos, prev_pos, safe_pos;
            Linq<Skillshot*> considered, dangerous, skillshots;
            std::unordered_map<std::string, Objects> objects;
            std::unordered_map<std::string, MenuItem> settings;
            std::function<int(char)> char_to_slot;
            Linq<SpellInfo> evading_spells;
            Linq<ParticleInfo> particles;
            Linq<MissileInfo> missiles;
            SpellInfo shield_spell;

            void BuildMenu();
            void InitComponents();
            void ResetSkillshots();
            void ResetState(float delay);
            void SwitchOff(bool instant);

            void OnTick();
            void OnDraw();
            void OnProcessSpell(const Obj_AI_Base& unit, std::string& spell_name,
                const int spell_level, const Vector& start_pos, const Vector& end_pos,
                uint32_t target_id, float cast_delay, float travel_time, bool proc_next);
            void OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info);
            void OnCreateObject(const Object& object, uint32_t object_id);
            void OnDeleteObject(const Object& object, uint32_t object_id);
            void OnDeleteObjectInternal(const Object& object);
            void OnBuffGain(const Obj_AI_Base& unit, const Buff& buff);
            void OnWndProc(const uint64_t msg, const uint64_t wparam);
            void OnIssueOrder(const Vector& pos, int order, const std::string& plugin);
            void OnIssueOrderInternal(const Vector3& pos, int order, const char* plugin);
    };
}

namespace std
{
    template <>
    struct hash<Evade::SpellInfo>
    {
        size_t operator()(const Evade::SpellInfo& info) const
        {
            return std::hash<std::string>()(info.SpellName);
        }
    };
}

#endif // PROGRAM_H
