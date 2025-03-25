
#ifndef MAP_LIBRARY_H
#define MAP_LIBRARY_H

#include <cmath>
#include <limits>
#include <map>
#include <sstream>
#include <unordered_map>

#ifndef ABS
#define ABS(v) ((v) < 0 ? -(v) : (v))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef SIGN
#define SIGN(v) ((v) < 0 ? -1 : ((v) > 0 ? 1 : 0))
#endif

enum class MapId : int
{
    UNKNOWN = -1,
    HOWLING_ABYSS,
    SUMMONER_RIFT_STANDARD,
    SUMMONER_RIFT_DRAGON_INFERNAL,
    SUMMONER_RIFT_DRAGON_MOUNTAIN,
    SUMMONER_RIFT_DRAGON_OCEAN,
    ARENA_2v2v2v2_FEATURED_MODE
};

enum class Flag : int
{
    UNKNOWN = -1,
    TERRAIN,
    BUSH,
    RIVER,
    WALL
};

enum class Region : int
{
    UNKNOWN = -1,
    BLUE_BASE,
    BOT_LANE,
    BOT_BLUE_JUNGLE,
    BOT_RED_JUNGLE,
    BOT_RIVER,
    MID_LANE,
    TOP_RIVER,
    TOP_RED_JUNGLE,
    TOP_BLUE_JUNGLE,
    TOP_LANE,
    RED_BASE
};

struct MLResult
{
    float x1, y1, x2, y2, dist;
    bool valid;
};

namespace MapLibraryV2
{
    using VecI = std::pair<int, int>;
    using VecF = std::pair<float, float>;
    using MLResults = std::vector<MLResult>;
    using HashTable = std::unordered_map<int, std::unordered_map<int, int>>;

    static constexpr float FLT_INF = std::numeric_limits<float>::infinity();

    class MapLibrary
    {
        private:
            bool reloading;
            std::vector<int> dirX, dirY;
            HashTable heights, regions, map;
            int id, limitX, limitY, offsetX, offsetY, size;

            VecF WorldToCellInternal(float x, float y);
            HashTable LoadMapData(const std::vector<unsigned char>& data);
            MLResults BFS(float x, float y, float range, Flag flag, bool closest);
            VecF VoxelTraversal(float x1, float y1, float x2, float y2, Flag flag);
            Flag GetFlag(int x, int y);
        
        public:
            MapLibrary();
            ~MapLibrary();

            int GetLoadedMapId() const;
            bool LoadHeightMap();
            bool LoadRegionMap();
            void ReloadMap(MapId map_id = MapId::HOWLING_ABYSS);
            VecF CellToWorld(int x, int y);
            VecI WorldToCell(float x, float y);
            MLResult FindClosestBush(float x, float y, float range);
            MLResult FindClosestRiver(float x, float y, float range);
            MLResult FindClosestWall(float x, float y, float range);
            MLResults FindBushCells(float x, float y, float range);
            MLResults FindRiverCells(float x, float y, float range);
            MLResults FindWallCells(float x, float y, float range);
            VecF IntersectsBush(float x1, float y1, float x2, float y2);
            VecF IntersectsRiver(float x1, float y1, float x2, float y2);
            VecF IntersectsWall(float x1, float y1, float x2, float y2);
            float GetHeight(float x, float y);
            Region GetRegion(float x, float y);
            Flag GetFlag(float x, float y);
            bool IsBush(float x, float y);
            bool IsRiver(float x, float y);
            bool IsTerrain(float x, float y);
            bool IsWall(float x, float y);
    };
}

#endif // MAP_LIBRARY_H
