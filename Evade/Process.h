
#ifndef PROCESS_H
#define PROCESS_H

#include "Program.h"

namespace Evade
{
    class Program;

    struct OnProcSpellArgs
    {
        Obj_AI_Hero Caster;
        std::string SpellName;
        Vector StartPos, EndPos;
        float ExtraDelay = 0.0f;
        bool ProcessNext = false;
    };

    class Process
    {
        private:
            API* api = nullptr;
            Data* data = nullptr;
            Program* program = nullptr;
            Obj_AI_Hero hero = nullptr;
            static Process* m_instance;

        public:
            static void Destroy();
            static Process* Get();

            Linq<OnProcSpellArgs> AddSkillshots(Skillshot* data);
            Skillshot* CreateSkillshot(ActiveData& data, bool test);
            void PredictCollision(Skillshot* skillshot, bool fix);
            void UpdateAcceleration(Skillshot* skillshot);
            bool UpdateData(Skillshot* skillshot, int level);
            Linq<Skillshot*> UpdateConsidered();
            void UpdateSkillshots();
    };
}

#endif // PROCESS_H
