
local version = 0.07
_G.MapLibrary = require "MapLibraryV2"

local mapId = MissionInfo.mapId
local abyssId = _G.HOWLING_ABYSS_MAP_ID
local riftId = _G.SUMMONER_RIFT_STANDARD_MAP_ID
local arenaId = _G.ARENA_2v2v2v2_FEATURED_MODE_MAP_ID
if mapId == 11 then _G.MapLibrary:ReloadMap(riftId) end
if mapId == 12 then _G.MapLibrary:ReloadMap(abyssId) end
if mapId == 30 then _G.MapLibrary:ReloadMap(arenaId) end

GetInternalWebResultAsync('MapAdapter.version', function(result)
    if result and result ~= "" and tonumber(result) > version then
        DownloadInternalFileAsync('MapLibraryV2.lua', COMMON_PATH, function() end)
        DownloadInternalFileAsync('MapAdapter.lua', SCRIPT_PATH, function() end)
    end
end)

AddEvent(Events.OnSoulChange, function(name)
    if _G.MapLibrary == nil then return end
    local id = name == "DragonFireMinimap" and 2
        or name == "DragonEarthMinimap" and 3
        or name == "DragonWaterMinimap" and 4
    if id then _G.MapLibrary:ReloadMap(id) end
end)

