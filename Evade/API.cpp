
#include "API.h"

namespace Evade
{
    API* API::m_instance = nullptr;

    API::API(PublicAPI* api) : m_api(api) {}

    void API::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    API* API::Get(PublicAPI* api)
    {
        if (m_instance == nullptr)
        {
            m_instance = new API(api);
        }
        return m_instance;
    }

    uint32_t API::ARGB(int alpha, int red, int green, int blue) const
    {
        uint32_t result;
        result  = (uint32_t)alpha << 0x18;
        result |= (uint32_t)red   << 0x10;
        result |= (uint32_t)green << 0x08;
        result |= (uint32_t)blue;
        return result;
    }

    uint32_t API::FNV1A32(const char* str) const
    {
        uint32_t hash = 0x811C9DC5u;
        while (*str != 0)
        {
            hash ^= *str++;
            hash *= 0x01000193;
        }
        return hash;
    }

    float API::GetTime() const
    {
        return this->m_api->get_riot_clock()->get_time();
    }

    float API::GetLatency() const
    {
        return this->m_api->get_net_client()->get_ping() * 0.001f;
    }

    Obj_AI_Hero* API::GetMyHero()
    {
        if (this->m_hero != nullptr) return this->m_hero;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& hero : manager->get_heros())
        {
            if (hero->is_local_player())
            {
                this->m_hero = (Obj_AI_Hero*)(hero->get_as_hero());
                break;
            }
        }
        return this->m_hero;
    }

    // Object type handlers

    Obj_AI_Hero* API::AsHero(GameObject* unit) const
    {
        return (Obj_AI_Hero*)(unit->get_as_hero());
    }

    Obj_AI_Minion* API::AsMinion(GameObject* unit) const
    {
        return (Obj_AI_Minion*)(unit->get_as_minion());
    }

    MissileClient* API::AsMissile(GameObject* unit) const
    {
        return (MissileClient*)(unit->get_as_missile());
    }

    bool API::IsHero(GameObject* unit) const
    {
        return unit->get_type() == 0xE260302C;
    }

    bool API::IsMinion(GameObject* unit) const
    {
        return unit->get_type() == 0xCAA58CB2;
    }

    bool API::IsMissile(GameObject* unit) const
    {
        return unit->get_type() == 0x9C8ADE94;
    }

    // Hero actions

    void API::CastSpell(int slot, GameObject* unit)
    {
        this->m_hero->get_spellbook()->cast_spell(slot, unit);
    }

    void API::CastSpell(int slot, const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        this->m_hero->get_spellbook()->cast_spell(slot, vec);
    }

    Linq<Vector> API::GetPath(const Vector& destination) const
    {
        return Linq<Vector>(); // Missing!
    }

    bool API::IsReachable(const Vector& destination) const
    {
        return true; // Missing!
    }

    void API::SendMoveCommand(const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        auto manager = this->m_api->get_hud_manager();
        manager->get_cursor_logic()->right_click_move(&vec, false);
    }

    // Drawings

    void API::DrawCircle(const Vector& pos, float radius, float height, uint32_t color)
    {
        VecInt3 vec = this->To3D(pos, height);
        this->m_api->get_imgui_helper()->Circle3D(vec, radius, color, 3.0f);
    }

    void API::DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color)
    {
        VecInt3 v1 = this->ToScreen(p1, height);
        VecInt3 v2 = this->ToScreen(p2, height);
        this->m_api->get_imgui_helper()->Line(v1, v2, color, 3.0f);
    }

    void API::DrawPolygon(const Poly& poly, float height, uint32_t color)
    {
        size_t n = poly.Size();
        std::vector<VecInt3> points(n);
        for (size_t i = 0; i < n; ++i)
        {
            points[i] = this->ToScreen(poly.Get(i), height);
        }
        for (size_t i = 0, j = n - 1; i < n; j = i++)
        {
            VecInt3 v1 = points[i], v2 = points[j];
            this->m_api->get_imgui_helper()->Line(v1, v2, color, 3.0f);
        }
    }

    float API::GetHeight(const Vector& pos) const
    {
        float height = 0.0f;
        auto manager = this->m_api->get_nav_mesh();
        manager->query_height_for_pos(pos.x, pos.y, height);
        return height;
    }

    VecInt3 API::ToScreen(const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        return this->m_api->get_render_helper()->world_to_screen(vec);
    }

    // Missile data

    Vector API::GetMissileDestination(MissileClient* missile) const
    {
        return this->ToVector(missile->get_movement()->get_target_pos());
    }

    Vector API::GetMissileStart(MissileClient* missile) const
    {
        return this->ToVector(missile->get_movement()->get_start_pos());
    }

    float API::GetMissileSpeed(MissileClient* missile) const
    {
        return missile->get_movement()->get_speed();
    }

    float API::GetMissileWidth(MissileClient* missile) const
    {
        return missile->get_width();
    }

    // Spell cast data

    int API::GetSpellCastLevel(SpellCastInfo* info) const
    {
        return MAX(1, info->get_level());
    }

    std::string API::GetSpellCastName(SpellCastInfo* info) const
    {
        return info->get_spell_data()->get_name();
    }

    uint32_t API::GetSpellCastOwnerId(SpellCastInfo* info) const
    {
        return info->get_target_id();
    }

    Vector API::GetSpellCastDestination(SpellCastInfo* info) const
    {
        return this->ToVector(info->get_end());
    }

    Vector API::GetSpellCastStart(SpellCastInfo* info) const
    {
        return this->ToVector(info->get_start());
    }

    float API::GetSpellCastStartTime(SpellCastInfo* info) const
    {
        return info->get_start_cast_time();
    }

    float API::GetSpellCastWindupTime(SpellCastInfo* info) const
    {
        return info->get_cooldown();
    }

    // Hero statistics

    float API::GetArmor(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_armor();
    }

    float API::GetAttackRange(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_attack_range();
    }

    float API::GetAttackSpeedMod(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_attack_speed_mod();
    }

    float API::GetBonusArmor(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_bonus_armor();
    }

    float API::GetBonusAttackDamage(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_flat_physical_damage_mod();
    }

    float API::GetBonusMana(Obj_AI_Hero* unit) const
    {
        return 0.0f; // Missing?
    }

    float API::GetBonusMagicResistance(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_bonus_spell_block();
    }

    float API::GetFlatMagicPenetration(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_flat_magic_penetration();
    }

    float API::GetHealth(Obj_AI_Hero* unit) const
    {
        return unit->get_health_struct()->current.value;
    }

    float API::GetHitbox(Obj_AI_Hero* unit) const
    {
        return unit->get_bounding_radius();
    }

    float API::GetLethality(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_physical_lethality();
    }

    int API::GetLevel(Obj_AI_Hero* unit) const
    {
        uint32_t level = unit->get_experience()->get_level();
        return MIN(18, MAX(1, static_cast<int>(level)));
    }

    float API::GetMaxHealth(Obj_AI_Hero* unit) const
    {
        return unit->get_health_struct()->max.value;
    }

    float API::GetMaxMana(Obj_AI_Hero* unit) const
    {
        return unit->get_ability_struct()->mana.max.value;
    }

    float API::GetMovementSpeed(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_movement_speed();
    }

    float API::GetMagicResistance(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_spell_block();
    }

    float API::GetPercentArmorPenetration(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_percent_armor_penetration();
    }

    float API::GetPercentBonusArmorPenetration(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_percent_bonus_armor_penetration();
    }

    float API::GetPercentMagicPenetration(Obj_AI_Hero* unit) const
    {
        return unit->get_character_intermediate()->get_percent_magic_penetration();
    }

    float API::GetTotalAttackDamage(Obj_AI_Hero* unit) const
    {
        auto data = unit->get_character_intermediate();
        float mod = data->get_flat_physical_damage_mod();
        return data->get_base_attack_damage() + mod;
    }

    std::string API::GetCharacterName(Obj_AI_Hero* unit) const
    {
        return unit->get_char_name();
    }

    // Object manager

    Linq<Obj_AI_Hero*> API::GetAllyHeroes(float range, const Vector& pos, bool valid) const
    {
        Linq<Obj_AI_Hero*> heroes = this->GetHeroes(range, pos, valid);
        return heroes.Where([&](const auto& u) { return this->IsAlly(u); });
    }

    Linq<Obj_AI_Hero*> API::GetEnemyHeroes(float range, const Vector& pos, bool valid) const
    {
        Linq<Obj_AI_Hero*> heroes = this->GetHeroes(range, pos, valid);
        return heroes.Where([&](const auto& u) { return this->IsEnemy(u); });
    }

    Linq<Obj_AI_Hero*> API::GetHeroes(float range, const Vector& pos, bool valid) const
    {
        Linq<Obj_AI_Hero*> result;
        bool inf = range >= 25000.0f;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& hero : manager->get_heros())
        {
            auto unit = (Obj_AI_Hero*)hero->get_as_hero();
            if (valid && !this->IsValid(unit)) continue;
            if (valid && !this->IsVisible(unit)) continue;
            if (inf) { result.Append(unit); continue; }
            Vector unit_pos = this->GetPosition(unit);
            float dist = unit_pos.DistanceSquared(pos);
            if (dist <= range * range) result.Append(unit);
        }
        return result;
    }

    Linq<Obj_AI_Minion*> API::GetAllyMinions(float range, const Vector& pos) const
    {
        Linq<Obj_AI_Minion*> minions = this->GetMinions(range, pos);
        return minions.Where([&](const auto& u) { return this->IsAlly(u); });
    }

    Linq<Obj_AI_Minion*> API::GetEnemyMinions(float range, const Vector& pos) const
    {
        Linq<Obj_AI_Minion*> minions = this->GetMinions(range, pos);
        return minions.Where([&](const auto& u) { return this->IsEnemy(u); });
    }

    Linq<Obj_AI_Minion*> API::GetMinions(float range, const Vector& pos) const
    {
        Linq<Obj_AI_Minion*> result;
        size_t npos = std::string::npos;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& minion : manager->get_minions())
        {
            auto unit = (Obj_AI_Minion*)minion->get_as_minion();
            if (!this->IsValid(unit)) continue;
            if (!this->IsVisible(unit)) continue;
            std::string name(unit->get_char_name());
            if (name.find("Minion") == npos) continue;
            Vector unit_pos = this->GetPosition(unit);
            float dist = unit_pos.DistanceSquared(pos);
            if (dist <= range * range) result.Append(unit);
        }
        return result;
    }

    // Buffs / items

    int API::GetBuffCount(Obj_AI_Hero* unit, const uint32_t hash) const
    {
        auto buffs = unit->get_buffs();
        for (const auto& buff : buffs)
        {
            if (!buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (buff->get_hash() == hash)
            {
                return buff->get_count();
            }
        }
        return 0;
    }

    float API::GetBuffRemainingTime(Obj_AI_Hero* unit, const uint32_t hash) const
    {
        auto buffs = unit->get_buffs();
        for (const auto& buff : buffs)
        {
            if (!buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (buff->get_hash() == hash)
            {
                return buff->get_end_time() - this->GetTime();
            }
        }
        return 0.0f;
    }

    int API::GetItemSlot(Obj_AI_Hero* unit, const int item_id) const
    {
        for (int i = 0; i <= 6; ++i)
        {
            auto inventory = unit->get_inventory();
            auto data = inventory->get_slot(i)->get_item_data();
            if (data->get_id() == item_id) return (i + 6);
        }
        return -1;
    }

    bool API::HasBuff(Obj_AI_Hero* unit, const uint32_t hash) const
    {
        auto buffs = unit->get_buffs();
        for (const auto& buff : buffs)
        {
            if (!buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (buff->get_hash() == hash)
            {
                return true;
            }
        }
        return false;
    }

    bool API::HasBuffType(Obj_AI_Hero* unit, const BuffType type) const
    {
        auto buffs = unit->get_buffs();
        for (const auto& buff : buffs)
        {
            if (!buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (buff->get_type() == type)
            {
                return true;
            }
        }
        return false;
    }

    // Booleans

    bool API::IsAlive(Obj_AI_Base* unit) const
    {
        return !this->IsDead(unit);
    }

    bool API::IsAlly(Obj_AI_Base* unit) const
    {
        return !this->IsEnemy(unit);
    }

    bool API::IsAttacking(Obj_AI_Base* unit) const
    {
        return false; // Missing?
    }

    bool API::IsDead(Obj_AI_Base* unit) const
    {
        return unit->is_dead();
    }

    bool API::IsEnemy(Obj_AI_Base* unit) const
    {
        return unit->get_team() != this->m_hero->get_team();
    }

    bool API::IsImmortal(Obj_AI_Base* unit) const
    {
        return unit->is_invulnerable();
    }

    bool API::IsMoving(Obj_AI_Base* unit) const
    {
        return unit->get_nav_path()->is_moving();
    }

    bool API::IsValid(Obj_AI_Base* unit) const
    {
        return unit->is_valid();
    }

    bool API::IsVisible(Obj_AI_Base* unit) const
    {
        return unit->is_visible();
    }

    bool API::IsVulnerable(Obj_AI_Base* unit) const
    {
        return !this->IsImmortal(unit);
    }

    bool API::IsWindingUp(Obj_AI_Base* unit) const
    {
        return unit->is_winding_up();
    }

    // Pathing

    bool API::IsDashing(Obj_AI_Base* unit) const
    {
        auto path = unit->get_nav_path();
        return path->is_moving() && path->get_dash_speed() > 0;
    }

    float API::GetDashSpeed(Obj_AI_Base* unit) const
    {
        return unit->get_nav_path()->get_dash_speed();
    }

    Vector API::GetDashStart(Obj_AI_Base* unit) const
    {
        return this->ToVector(unit->get_nav_path()->get_start_pos());
    }

    Vector API::GetDashDestination(Obj_AI_Base* unit) const
    {
        return this->ToVector(unit->get_nav_path()->get_end_pos());
    }

    Vector API::GetPathDestination(Obj_AI_Base* unit) const
    {
        auto data = unit->get_nav_path();
        uint32_t count = data->get_path_count();
        size_t size = static_cast<size_t>(count);
        VecInt3 pos = size <= 1 ? unit->get_position()
            : data->get_paths().at(size - 1);
        return this->ToVector(pos);
    }

    Vector API::GetPosition(Obj_AI_Base* unit) const
    {
        return this->ToVector(unit->get_position());
    }

    auto API::GetWaypoints(Obj_AI_Base* unit) const
    {
        auto data = unit->get_nav_path();
        auto paths = data->get_paths();
        std::vector<Vector> waypoints;
        waypoints.reserve(paths.size());
        for (const auto& vec_int : paths)
        {
            Vector vec = this->ToVector(vec_int);
            waypoints.push_back(vec);
        }
        return waypoints;
    }

    // Vectors

    VecInt2 API::To2D(const Vector& vec) const
    {
        return VecInt2(vec.x, vec.y);
    }

    VecInt3 API::To3D(const Vector& vec, float y) const
    {
        return VecInt3(vec.x, y, vec.y);
    }

    Vector API::ToVector(const VecInt2& pos) const
    {
        return Vector(pos.x, pos.y);
    }

    Vector API::ToVector(const VecInt3& pos) const
    {
        return Vector(pos.x, pos.z);
    }
}
