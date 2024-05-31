
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
    struct MissileInfo { std::string Caster, SpellName, MissileName; };
    struct ParticleInfo { std::string Caster, SpellName, ParticleName; };

    class Program
    {
        public:
            Program(PublicAPI* api);
            ~Program();
        
        private:
            API* api = nullptr;
            Data* data = nullptr;
            Config* config = nullptr;
            PublicAPI* m_api = nullptr;
            Obj_AI_Hero* my_hero = nullptr;
            static Program* m_instance;

            void RegisterAPI();
            void RegisterData();
            void RegisterEvents();
            void RegisterMenu();
            void RegisterMyHero();
            static Program* Get();

            static void OnTickWrapper();
            static void OnDrawWrapper();
            static void OnProcessSpellWrapper(GameObject* unit, SpellCastInfo* info);
            static void OnCreateObjectWrapper(GameObject* unit, uint32_t id);
            static void OnBuffGainWrapper(BuffInstance* buff);
            static void OnWndProcWrapper(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
            static void OnIssueOrderWrapper(VecInt3 pos, int order, int target_id);

            // Main components
            
            uint32_t hash_code = -1, safe_order = 0;
            bool can_evade = true, dodge_only_danger = false,
                dont_use_spell = false, recalc_path = true,
                undodgeable = false;
            float attack_timer = 0.0f, block_timer = 0.0f,
                click_timer = 0.0f, danger_timer = 0.0f,
                delay_timer = 0.0f, issue_timer = 0.0f,
                recalc_timer = 0.0f, move_speed = 0.0f;
            Linq<Skillshot*> dangerous, dodgeable, skillshots;
            std::unordered_map<std::string, Menu*> settings;
            Vector hero_pos, move_pos, prev_pos, safe_pos;
            std::function<int(char)> char_to_slot;
            std::function<char(int)> slot_to_char;

            void BuildMenu();

            void OnTick();
            void OnDraw();
            void OnProcessSpell(GameObject* unit, const std::string& name,
                int level, const Vector& start_pos, const Vector& end_pos,
                uint32_t target_id, float cast_delay, float cast_time);
            void OnProcessSpellInternal(GameObject* unit, SpellCastInfo* info);
            void OnCreateObject(GameObject* unit, uint32_t id);
            void OnBuffGain(BuffInstance* buff);
            void OnWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
            void OnIssueOrder(VecInt3 pos, int order, int target_id);
    };
}

#endif // PROGRAM_H
