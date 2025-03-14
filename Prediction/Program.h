
#ifndef PROGRAM_H
#define PROGRAM_H

#include <algorithm>
#include "Data.h"
#include "Config.h"
#include "Network.h"

namespace Prediction
{
    class Utilities;

    const std::string PRED_VERSION = "2025.03.12.01";

    class Program
    {
        public:
            Program(PublicAPI api);
            ~Program();

            static Program* Get();

            const bool DependencyCheck() const { return this->libs_loaded; }
            const auto& GetActiveDashes() const { return this->dashes; }
            const auto& GetPathData() const { return this->paths; }

            Path GetWaypoints(const Obj_AI_Base& unit) const;
            float GetBlinkDuration(const Obj_AI_Base& unit) const;
            float GetInvisibilityTime(const Obj_AI_Base& unit) const;
            float GetMeanAngleDiff(const Obj_AI_Base& unit) const;
            float GetPathChangeTime(const Obj_AI_Base& unit) const;
            float GetWindupTime(const Obj_AI_Base& unit) const;
            bool IsBlinking(const Obj_AI_Base& unit) const;
            bool IsCastingDash(const Obj_AI_Base& unit) const;

            template<typename T>
            T GetValue(const std::string& id) const;

        private:
            API* api = nullptr;
            Data* data = nullptr;
            Config* config = nullptr;
            Network* network = nullptr;
            Utilities* utils = nullptr;
            PublicAPI m_api = nullptr;
            Obj_AI_Hero my_hero = nullptr;
            static Program* m_instance;

            void RegisterCore();
            void RegisterEvents();
            void RegisterMenu();

            static void OnDrawWrapper();
            static void OnNewPathWrapper(Obj_AI_Base unit, std::vector<Vector3> paths);
            static void OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo info);

            // Main components

            bool libs_loaded = false;
            mutable std::unordered_map<uint32_t, float> windups;
            mutable std::unordered_map<uint32_t, DashData> dashes;
            mutable std::unordered_map<uint32_t, Linq<PathData>> paths;
            mutable std::unordered_map<std::string, MenuItem> settings;

            void BuildMenu();
            void InitComponents();
            void CleanUpHistory(Linq<PathData>& paths, float timer, float duration);
            void UpdatePaths(const Obj_AI_Base& unit, const Path& path, bool changed);

            void OnDraw();
            void OnNewPath(const Obj_AI_Base& unit, Linq<Vector> path);
            void OnProcessSpell(const Obj_AI_Base& unit, std::string& spell_name,
                const Vector& start_pos, const Vector& end_pos, const float cast_delay);
            void OnNewPathInternal(const Obj_AI_Base& unit, std::vector<Vector3>& paths);
            void OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info);
    };
}

#endif // PROGRAM_H
