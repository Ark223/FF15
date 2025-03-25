
#include "Data.h"
#include "LZ.h"
#include "MapLibrary.h"

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// Utils

inline std::pair<float, float> ClosestOnCell(
    float cx, float cy, float px, float py, int len)
{
    if (ABS(cx - px) <= len)
    {
        int sign = (py >= cy) ? 1 : -1;
        return { px, cy + sign * len };
    }
    if (ABS(cy - py) <= len)
    {
        int sign = (px >= cx) ? 1 : -1;
        return { cx + sign * len, py };
    }
    return
    {
        (px > cx) ? cx + len : cx - len,
        (py > cy) ? cy + len : cy - len
    };
}

inline float DistanceSquared(float ax, float ay, float bx, float by)
{
    float dx = bx - ax, dy = by - ay;
    return dx * dx + dy * dy;
}

// Map library

namespace MapLibraryV2
{
    MapLibrary::MapLibrary()
    {
        this->id = -1;
        this->size = 50;
        this->dirX = { -1, 0, 1, 0 };
        this->dirY = { 0, 1, 0, -1 };
        this->limitX = this->limitY = 0;
        this->offsetX = this->offsetY = 0;
    }

    MapLibrary::~MapLibrary()
    {
        this->heights.clear();
        this->regions.clear();
        this->map.clear();
    }

    // Map ID getter and loaders

    HashTable MapLibrary::LoadMapData(const std::vector<Char>& data)
    {
        int outer_key, inner_key, value; char character;
        std::string json = LZ::DecompressFromBytes(data);
        std::stringstream string_stream(json);
        HashTable result = HashTable();
        string_stream >> character;
        while (string_stream >> outer_key)
        {
            string_stream >> character >> character;
            while (string_stream >> inner_key)
            {
                string_stream >> character >> value;
                result[outer_key][inner_key] = value;
                if (!(string_stream >> character)
                    || character == '}') break;
            }
            if (!(string_stream >> character)
                || character == '}') break;
        }
        return result;
    }

    int MapLibrary::GetLoadedMapId() const
    {
        return this->id;
    }

    bool MapLibrary::LoadHeightMap()
    {
        if (this->id < 1) return false;
        if (this->id == 5) return false;
        if (!this->heights.empty()) return false;
        this->heights = this->LoadMapData(
            SUMMONER_RIFT_HEIGHT_MAP_DATA);
        return true;
    }

    bool MapLibrary::LoadRegionMap()
    {
        if (this->id < 1) return false;
        if (this->id == 5) return false;
        if (!this->regions.empty()) return false;
        this->regions = this->LoadMapData(
            SUMMONER_RIFT_REGION_MAP_DATA);
        return true;
    }

    void MapLibrary::ReloadMap(MapId map_id)
    {
        this->reloading = true;
        int id = (int)map_id;

        std::map<int, const std::vector<unsigned char>> mapper =
        {
            {0, HOWLING_ABYSS},
            {1, SUMMONER_RIFT_STANDARD},
            {2, SUMMONER_RIFT_DRAGON_INFERNAL},
            {3, SUMMONER_RIFT_DRAGON_MOUNTAIN},
            {4, SUMMONER_RIFT_DRAGON_OCEAN},
            {5, ARENA_2v2v2v2_FEATURED_MODE}
        };

        int lx = id == 0 ? 243 : id == 5 ? 273 : 294;
        int ly = id == 0 ? 234 : id == 5 ? 273 : 294;
        int ox = id == 0 ? 504 : id == 5 ? 2150 : 1;
        int oy = id == 0 ? 369 : id == 5 ? 2150 : 83;

        this->id = id;
        this->map.clear();
        this->map = this->LoadMapData(mapper[id]);
        this->offsetX = ox; this->offsetY = oy;
        this->limitX = lx; this->limitY = ly;
        this->reloading = false;
    }

    // Converters

    VecF MapLibrary::CellToWorld(int x, int y)
    {
        int tx = x * this->size + this->offsetX;
        int ty = y * this->size + this->offsetY;
        tx += (this->size / 2);
        ty += (this->size / 2);
        return { (float)tx, (float)ty };
    }

    VecF MapLibrary::WorldToCellInternal(float x, float y)
    {
        float tx = (x - this->offsetX) / this->size;
        float ty = (y - this->offsetY) / this->size;
        return { tx, ty };
    }

    VecI MapLibrary::WorldToCell(float x, float y)
    {
        float tx = (x - this->offsetX) / this->size;
        float ty = (y - this->offsetY) / this->size;
        return { (int)tx, (int)ty };
    }

    // Breadth first search

    MLResults MapLibrary::BFS(float x, float y, float range, Flag flag, bool closest)
    {
        MLResults result;
        std::vector<VecI> queue;
        int size = this->size / 2;
        bool finished = false, inside;
        float best_distance = FLT_INF;
        auto &dx = this->dirX, &dy = this->dirY;
        auto [tx, ty] = this->WorldToCell(x, y);
        auto start = std::make_pair(tx, ty);
        std::unordered_map<VecI, bool, pair_hash> visits;
        queue.push_back(start); visits[start] = true;
        auto add_to_queue = [&](int id, int px, int py)
        {
            if (!inside || finished && closest) return;
            int ax = px + dx[id], ay = py + dy[id];
            VecI point = std::make_pair(ax, ay);
            if (visits.count(point) > 0) return;
            queue.push_back(point);
            visits[point] = true;
        };
        while (!queue.empty())
        {
            auto &[px, py] = queue.front();
            queue.erase(queue.begin());
            auto [cx, cy] = this->CellToWorld(px, py);
            auto [fx, fy] = ClosestOnCell(cx, cy, x, y, size);
            if (px == tx && py == ty) { fx = x; fy = y; }
            float dist = DistanceSquared(fx, fy, x, y);
            inside = range > 0 && dist <= range * range;
            if (inside && (closest && dist <= best_distance ||
                !closest) && this->GetFlag(px, py) == flag)
                { best_distance = dist; finished = true;
                result.push_back({ cx, cy, fx, fy, dist, true }); }
            for (int i = 0; i < 4; i++) add_to_queue(i, px, py);
        }
        return result;
    }

    MLResult MapLibrary::FindClosestBush(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        auto result = this->BFS(x, y, range, Flag::BUSH, true);
        return !result.empty() ? result.front() : MLResult();
    }

    MLResult MapLibrary::FindClosestRiver(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        auto result = this->BFS(x, y, range, Flag::RIVER, true);
        return !result.empty() ? result.front() : MLResult();
    }

    MLResult MapLibrary::FindClosestWall(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        auto result = this->BFS(x, y, range, Flag::WALL, true);
        return !result.empty() ? result.front() : MLResult();
    }

    MLResults MapLibrary::FindBushCells(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        return this->BFS(x, y, range, Flag::BUSH, false);
    }

    MLResults MapLibrary::FindRiverCells(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        return this->BFS(x, y, range, Flag::RIVER, false);
    }

    MLResults MapLibrary::FindWallCells(float x, float y, float range)
    {
        range = MAX(0.0f, MIN(range, 1000.0f));
        return this->BFS(x, y, range, Flag::WALL, false);
    }

    // Custom methods

    float MapLibrary::GetHeight(float x, float y)
    {
        if (this->id == 0) return -178.31f;
        if (this->heights.empty()) return 0.0f;
        int &lx = this->limitX, &ly = this->limitY;
        auto [rx, ry] = this->WorldToCell(x, y);
        if (rx > lx || ry > ly) return 0.0f;
        if (rx < 0 || ry < 0) return 0.0f;
        return (float)this->heights[rx][ry];
    }

    Region MapLibrary::GetRegion(float x, float y)
    {
        Region def = Region::UNKNOWN;
        if (this->regions.empty()) return def;
        int &lx = this->limitX, &ly = this->limitY;
        auto [rx, ry] = this->WorldToCell(x, y);
        if (rx > lx || ry > ly) return def;
        if (rx < 0 || ry < 0) return def;
        int id = this->regions[rx][ry];
        return (Region)(id - 1);
    }

    // Fast voxel traversal

    VecF MapLibrary::VoxelTraversal(float x1, float y1, float x2, float y2, Flag flag)
    {
        auto is_found = [this, &flag](int a, int b)
            { return this->GetFlag(a, b) == flag; };
        auto f0 = [](float n) { return n - std::floor(n); };
        auto f1 = [](float n) { return 1 - n + std::floor(n); };
        auto [xa, ya] = this->WorldToCellInternal(x1, y1);
        auto [xb, yb] = this->WorldToCellInternal(x2, y2);
        int cx = (int)xa, cy = (int)ya;
        int lx = (int)xb, ly = (int)yb;
        float dx = xb - xa, dy = yb - ya;
        int sx = SIGN(dx), sy = SIGN(dy);
        int steps = ABS(lx - cx) + ABS(ly - cy);
        float tx = sx == 0 ? FLT_INF : (sx / dx);
        float ty = sy == 0 ? FLT_INF : (sy / dy);
        float mx = tx * (sx > 0 ? f1(xa) : f0(xa));
        float my = ty * (sy > 0 ? f1(ya) : f0(ya));
        while (steps-- >= 0 && (mx <= 1 || my <= 1))
        {
            if (mx == my && is_found(cx + sx, cy))
                return this->CellToWorld(cx + sx, cy);
            int iter_step_x = mx < my ? sx : 0;
            int iter_step_y = mx >= my ? sy : 0;
            float iter_max_x = mx < my ? tx : 0;
            float iter_max_y = mx >= my ? ty : 0;
            cx += iter_step_x; mx += iter_max_x;
            cy += iter_step_y; my += iter_max_y;
            if (is_found(cx, cy) == true) return
                this->CellToWorld(cx, cy);
        }
        return VecF();
    }

    VecF MapLibrary::IntersectsBush(float x1, float y1, float x2, float y2)
    {
        return this->VoxelTraversal(x1, y1, x2, y2, Flag::BUSH);
    }

    VecF MapLibrary::IntersectsRiver(float x1, float y1, float x2, float y2)
    {
        return this->VoxelTraversal(x1, y1, x2, y2, Flag::RIVER);
    }

    VecF MapLibrary::IntersectsWall(float x1, float y1, float x2, float y2)
    {
        return this->VoxelTraversal(x1, y1, x2, y2, Flag::WALL);
    }

    // Lookup methods

    Flag MapLibrary::GetFlag(int x, int y)
    {
        if (this->reloading) return Flag::UNKNOWN;
        if (this->map.empty()) return Flag::UNKNOWN;
        int& lx = this->limitX, &ly = this->limitY;
        if (x > lx || y > ly) return Flag::WALL;
        if (x < 0 || y < 0) return Flag::WALL;
        return (Flag)this->map[x][y];
    }

    Flag MapLibrary::GetFlag(float x, float y)
    {
        if (this->reloading) return Flag::UNKNOWN;
        if (this->map.empty()) return Flag::UNKNOWN;
        int &lx = this->limitX, &ly = this->limitY;
        auto [rx, ry] = this->WorldToCell(x, y);
        if (rx > lx || ry > ly) return Flag::WALL;
        if (rx < 0 || ry < 0) return Flag::WALL;
        return (Flag)this->map[rx][ry];
    }

    bool MapLibrary::IsBush(float x, float y)
    {
        return this->GetFlag(x, y) == Flag::BUSH;
    }

    bool MapLibrary::IsRiver(float x, float y)
    {
        return this->GetFlag(x, y) == Flag::RIVER;
    }

    bool MapLibrary::IsTerrain(float x, float y)
    {
        return this->GetFlag(x, y) == Flag::TERRAIN;
    }

    bool MapLibrary::IsWall(float x, float y)
    {
        return this->GetFlag(x, y) == Flag::WALL;
    }
}
