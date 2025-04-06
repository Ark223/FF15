
#ifndef PROGRAM_H
#define PROGRAM_H

#include <algorithm>
#include <sstream>
#include "Data.h"
#include "Config.h"
#include "Network.h"

namespace IPrediction
{
    class Utilities;

    const std::string PRED_VERSION = "2025.04.06.01";

    class Program
    {
        public:
            Program(PublicAPI api);
            ~Program();

            static Program* Get();

            const bool DependencyCheck() const { return this->libs_loaded; }
            const auto& GetActiveDashes() const { return this->dashes; }
            const auto& GetPathData() const { return this->paths; }
            const auto& GetWindWalls() const { return this->walls; }

            float GetCollisionBuffer() const;

            float GetMiaDuration(const Obj_AI_Base& unit) const;
            float GetImmobilityTime(const Obj_AI_Base& unit) const;
            float GetPathChangeTime(const Obj_AI_Base& unit) const;
            float GetWindupDuration(const Obj_AI_Base& unit) const;

            Path GetWaypoints(const Obj_AI_Base& unit) const;
            bool IsBlinking(const Obj_AI_Base& unit) const;
            bool IsDashing(const Obj_AI_Base& unit) const;

            AoeSolution GetAoeSolution(const Linq<Vector>& candidates,
                const PredictionInput& input, const Vector& star_pt) const;
            AoeSolution GetAoeSolution(const Linq<Obj_AI_Base>& candidates,
                const PredictionInput& input, const Obj_AI_Base& star_obj) const;
            PredictionOutput GetPrediction(const PredictionInput& input) const;

            float GetBlinkDuration(const Obj_AI_Base& unit) const;
            float GetDashDuration(const Obj_AI_Base& unit) const;
            float GetMeanAngleDiff(const Obj_AI_Base& unit) const;
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
            static void OnProcessSpellWrapper(Obj_AI_Base unit, CastInfo cast_info);
            static void OnCreateObjectWrapper(Object object, uint32_t network_id);

            // Main components

            bool libs_loaded = false;
            mutable Linq<WallData> walls;
            mutable std::unordered_map<uint32_t, float> windups;
            mutable std::unordered_map<uint32_t, DashData> dashes;
            mutable std::unordered_map<uint32_t, Linq<PathData>> paths;
            mutable std::unordered_map<std::string, MenuItem> settings;

            void BuildMenu();
            void InitComponents();
            void CleanUpHistory(Linq<PathData>& paths, float timer, float duration);
            void UpdatePaths(const Obj_AI_Base& unit, const Path& path, bool changed);

            void OnDraw();
            void OnNewPath(const Obj_AI_Base& unit, Linq<Vector> path, float speed);
            void OnProcessSpell(const Obj_AI_Base& unit, const std::string& spell_name,
                const Vector& start_pos, const Vector& end_pos, const float cast_delay);
            void OnCreateObject(const Object& game_object, const uint32_t network_id);
            void OnNewPathInternal(const Obj_AI_Base& unit, std::vector<Vector3>& paths);
            void OnProcessSpellInternal(const Obj_AI_Base& unit, const CastInfo& info);
    };
}

#endif // PROGRAM_H
