
#include "API.h"

namespace Evade
{
    API* API::m_instance = nullptr;

    API::API(PublicAPI api) : m_api(api) {}

    void API::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    API* API::Get(PublicAPI api)
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
        result  = (uint32_t)alpha << 24;
        result |= (uint32_t)red   << 16;
        result |= (uint32_t)green << 8;
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

    Obj_AI_Hero API::GetHero()
    {
        if (this->m_hero != nullptr) return this->m_hero;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& hero : manager->get_heros())
        {
            if (hero->is_local_player())
            {
                this->m_hero = (Obj_AI_Hero)(hero->get_as_hero());
                break;
            }
        }
        return this->m_hero;
    }

    Vector API::GetCursorPos() const
    {
        auto logic = this->m_api->get_hud_manager()->get_cursor_logic();
        return this->ToVector(*(logic->get_cursor_pos()));
    }

    float API::GetLatency() const
    {
        return this->m_api->get_net_client()->get_ping() * 0.001f;
    }

    float API::GetTime() const
    {
        return this->m_api->get_riot_clock()->get_time();
    }

    // Object type handlers

    Obj_AI_Hero API::AsHero(const Object& object) const
    {
        return (Obj_AI_Hero)(object->get_as_hero());
    }

    Obj_AI_Minion API::AsMinion(const Object& object) const
    {
        return (Obj_AI_Minion)(object->get_as_minion());
    }

    MissileClient API::AsMissile(const Object& object) const
    {
        return (MissileClient)(object->get_as_missile());
    }

    bool API::IsHero(const Object& object) const
    {
        return object->get_type() == 0xE260302C;
    }

    bool API::IsMinion(const Object& object) const
    {
        return object->get_type() == 0xCAA58CB2;
    }

    bool API::IsMissile(const Object& object) const
    {
        return object->get_type() == 0x9C8ADE94;
    }

    bool API::IsParticle(const Object& object) const
    {
        return object->get_type() == 0xDD4DE76F;
    }

    // Hero actions

    void API::BlockCast()
    {
        this->m_api->block_cast();
    }

    void API::BlockOrder()
    {
        this->m_api->block_order();
    }

    bool API::CanUseSpell(int slot)
    {
        auto state = this->m_hero->get_spellbook()->can_use_spell(slot);
        return state == SpellState::Ready;
    }

    void API::CastSpell(int slot, const Object& unit)
    {
        this->m_hero->get_spellbook()->cast_spell(slot, unit);
    }

    void API::CastSpell(int slot, const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        this->m_hero->get_spellbook()->cast_spell(slot, vec);
    }

    void API::SendMoveCommand(const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        auto manager = this->m_api->get_hud_manager();
        manager->get_cursor_logic()->right_click_move(&vec, false);
    }

    void API::SetOverrideOrder(const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        this->m_api->set_override_order(vec);
    }

    // Drawings

    void API::DrawCenteredText(const VecInt2& pos, const char* text, uint32_t color)
    {
        auto imgui = this->m_api->get_imgui_helper();
        VecInt2 fixed = imgui->CalcTextSize(text);
        uint32_t abgr = this->FixColor(color);
        fixed.x = pos.x - fixed.x / 2.0f;
        fixed.y = pos.y + fixed.y / 2.0f;
        imgui->Text(fixed, abgr, text);
    }

    void API::DrawCircle(const Vector& pos, float radius, float height, uint32_t color)
    {
        VecInt3 vec = this->To3D(pos, height);
        uint32_t abgr = this->FixColor(color);
        this->m_api->get_imgui_helper()->Circle3D(vec, radius, abgr, 2.0f);
    }

    void API::DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color)
    {
        VecInt2 v1 = this->ToScreen(p1, height);
        VecInt2 v2 = this->ToScreen(p2, height);
        uint32_t abgr = this->FixColor(color);
        this->m_api->get_imgui_helper()->Line(v1, v2, abgr, 2.0f);
    }

    void API::DrawPolygon(const Poly& poly, float height, uint32_t color)
    {
        size_t size = poly.Size();
        std::vector<VecInt2> points(size);
        uint32_t abgr = this->FixColor(color);

        for (size_t i = 0; i < size; ++i)
        {
            points[i] = this->ToScreen(poly.Get(i), height);
        }

        for (size_t i = 0, j = size - 1; i < size; j = i++)
        {
            VecInt2 v1 = points[i], v2 = points[j];
            this->m_api->get_imgui_helper()->Line(v1, v2, abgr, 2.0f);
        }
    }

    uint32_t API::FixColor(const uint32_t color) const
    {
        uint32_t red = (color & 0x00FF0000) >> 16;
        uint32_t blue = (color & 0x000000FF) << 16;
        return color & 0xFF00FF00 | red | blue;
    }

    float API::GetHeight(const Object& object) const
    {
        return object->get_position().y;
    }

    float API::GetHeight(const Vector& pos) const
    {
        float height = 0.0f;
        auto manager = this->m_api->get_nav_mesh();
        manager->query_height_for_pos(pos.x, pos.y, height);
        return height;
    }

    VecInt2 API::ToScreen(const Vector& pos, float height)
    {
        VecInt3 vec = this->To3D(pos, height);
        return this->m_api->get_render_helper()->world_to_screen(vec);
    }

    // Missile data

    Vector API::GetMissileEndPos(const MissileClient& missile) const
    {
        return this->ToVector(missile->get_movement()->get_target_pos());
    }

    Vector API::GetMissileStartPos(const MissileClient& missile) const
    {
        return this->ToVector(missile->get_movement()->get_start_pos());
    }

    std::string API::GetMissileName(const MissileClient& missile) const
    {
        return missile->get_spellcastinfo()->get_spell_data()->get_name();
    }

    Object API::GetMissileOwner(const MissileClient& missile) const
    {
        auto info = missile->get_spellcastinfo();
        auto manager = this->m_api->get_game_object_manager();
        uint32_t id = info ? info->get_spell_caster_id() : 0;
        return id ? manager->get_obj_by_id(id).release() : nullptr;
    }

    float API::GetMissileSpeed(const MissileClient& missile) const
    {
        return missile->get_movement()->get_speed();
    }

    float API::GetMissileWidth(const MissileClient& missile) const
    {
        return missile->get_width();
    }

    // Spell cast data

    int API::GetSpellCastLevel(const CastInfo& info) const
    {
        return MAX(1, MIN(6, info->get_level()));
    }

    std::string API::GetSpellCastName(const CastInfo& info) const
    {
        return info->get_spell_data()->get_name();
    }

    Vector API::GetSpellCastEndPos(const CastInfo& info) const
    {
        return this->ToVector(info->get_end());
    }

    Vector API::GetSpellCastStartPos(const CastInfo& info) const
    {
        return this->ToVector(info->get_start());
    }

    float API::GetSpellCastStartTime(const CastInfo& info) const
    {
        return info->get_start_cast_time();
    }

    float API::GetSpellCastWindupTime(const CastInfo& info) const
    {
        return info->get_designer_cast_time();
    }

    uint32_t API::GetSpellCastTargetId(const CastInfo& info) const
    {
        return info->get_target_id();
    }

    // Game object statistics

    float API::GetArmor(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_armor();
    }

    float API::GetAttackDelay(const Obj_AI_Base& unit) const
    {
        return (float)unit->get_attack_delay();
    }

    float API::GetAttackRange(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_attack_range();
    }

    float API::GetAttackSpeedMod(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_attack_speed_mod();
    }

    float API::GetAttackWindup(const Obj_AI_Base& unit) const
    {
        return (float)unit->get_attack_cast_delay();
    }

    float API::GetBonusArmor(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_bonus_armor();
    }

    float API::GetBonusAbilityDamage(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_flat_magic_damage_mod();
    }

    float API::GetBonusAttackDamage(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_flat_physical_damage_mod();
    }

    float API::GetBonusHealth(const Obj_AI_Base& unit) const
    {
        return 0.0f; // Missing?
    }

    float API::GetBonusMana(const Obj_AI_Base& unit) const
    {
        return 0.0f; // Missing?
    }

    float API::GetBonusMagicResistance(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_bonus_spell_block();
    }

    float API::GetFlatMagicPenetration(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_flat_magic_penetration();
    }

    float API::GetHealth(const Obj_AI_Base& unit) const
    {
        return unit->get_health_struct()->current.value;
    }

    float API::GetHealthRatio(const Obj_AI_Base& unit) const
    {
        return this->GetHealth(unit) / this->GetMaxHealth(unit);
    }

    float API::GetHitbox(const Obj_AI_Base& unit) const
    {
        return unit->get_bounding_radius();
    }

    float API::GetLethality(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_physical_lethality();
    }

    int API::GetLevel(const Obj_AI_Hero& unit) const
    {
        auto experience = unit->get_experience();
        return MAX(1, MIN(18, (int)experience->get_level()));
    }

    float API::GetMaxHealth(const Obj_AI_Base& unit) const
    {
        return unit->get_health_struct()->max.value;
    }

    float API::GetMaxMana(const Obj_AI_Base& unit) const
    {
        return unit->get_ability_struct()->mana.max.value;
    }

    float API::GetMovementSpeed(const Obj_AI_Base& unit) const
    {
        if (this->IsDashing(unit)) return this->GetDashSpeed(unit);
        return unit->get_character_intermediate()->get_movement_speed();
    }

    float API::GetMagicResistance(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_spell_block();
    }

    float API::GetPercentArmorPenetration(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_percent_armor_penetration();
    }

    float API::GetPercentBonusArmorPenetration(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_percent_bonus_armor_penetration();
    }

    float API::GetPercentMagicPenetration(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_percent_magic_penetration();
    }

    int API::GetSpellLevel(const Obj_AI_Hero& unit, int slot) const
    {
        auto spell = unit->get_spellbook()->get_spell(slot);
        return MAX(1, MIN(6, (int)spell->get_level()));
    }

    float API::GetTotalAbilityDamage(const Obj_AI_Base& unit) const
    {
        auto data = unit->get_character_intermediate();
        float mod = data->get_flat_magic_damage_mod();
        return data->get_base_ability_damage() + mod;
    }

    float API::GetTotalAttackDamage(const Obj_AI_Base& unit) const
    {
        auto data = unit->get_character_intermediate();
        float mod = data->get_flat_physical_damage_mod();
        return data->get_base_attack_damage() + mod;
    }

    // Game object base data

    bool API::Compare(const Object& a, const Object& b) const
    {
        if (!a || !b) return false;
        return this->GetObjectId(a) == this->GetObjectId(b);
    }

    Object API::GetObjectById(const uint32_t object_id) const
    {
        auto manager = this->m_api->get_game_object_manager();
        return manager->get_obj_by_id(object_id).release();
    }

    uint32_t API::GetObjectId(const Object& object) const
    {
        return object->get_id();
    }

    Vector API::GetObjectDirection(const Object& object) const
    {
        auto emitter = (EffectEmitter*)object->get_as_effect_emitter();
        return this->ToVector(emitter->get_orientation());
    }

    Object API::GetObjectOwner(const Object& object) const
    {
        auto manager = this->m_api->get_game_object_manager();
        auto emitter = (EffectEmitter*)object->get_as_effect_emitter();
        auto id = emitter ? emitter->get_attachment_obj_id() : 0;
        return id ? manager->get_obj_by_id(id).release() : nullptr;
    }

    std::string API::GetObjectName(const Object& object) const
    {
        return object->get_name();
    }

    std::string API::GetCharacterName(const Obj_AI_Base& unit) const
    {
        return unit->get_char_name();
    }

    std::string API::GetSpellName(const Obj_AI_Hero& unit, int slot) const
    {
        return unit->get_spellbook()->get_spell(slot)->get_name();
    }

    // Damage calculator

    float API::CalcDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount, int type) const
    {
        switch (type)
        {
            case 0: return this->CalcMagicalDamage(source, target, amount);
            case 1: return this->CalcPhysicalDamage(source, target, amount);
            case 2: return this->CalcMixedDamage(source, target, amount);
            default: return amount;
        }
    }

    float API::CalcPhysicalDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const
    {
        if (amount <= 0) return 0.0f;
        float armor = this->GetArmor(target);
        float armor_pen = this->GetPercentArmorPenetration(source);
        float bonus_pen = this->GetPercentBonusArmorPenetration(source);
        float scaling = 0.6f + 0.4f * this->GetLevel(source) / 18.0f;
        float lethality = this->GetLethality(source) * scaling;
        float bonus = this->GetBonusArmor(target) * (1 - bonus_pen);
        float armor_res = armor * armor_pen - bonus - lethality;
        float value = armor < 0 ? 2.0f - 100.0f / (100.0f - armor)
            : armor_res < 0 ? 1.0f : 100.0f / (100.0f + armor_res);
        return MAX(0.0f, floorf(amount * value));
    }

    float API::CalcMagicalDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const
    {
        if (amount <= 0) return 0.0f;
        float mr_amo = this->GetMagicResistance(target);
        float magic_pen = this->GetPercentMagicPenetration(source);
        float flat_magic_pen = this->GetFlatMagicPenetration(source);
        float magic_res = mr_amo * magic_pen - flat_magic_pen;
        float value = mr_amo < 0 ? 2.0f - 100.0f / (100.0f - mr_amo)
            : magic_res < 0 ? 1.0f : 100.0f / (100.0f + magic_res);
        return MAX(0.0f, floorf(amount * value));
    }

    float API::CalcMixedDamage(Obj_AI_Hero& source, Obj_AI_Hero& target, float amount) const
    {
        return (0.5f * amount) + this->CalcMagicalDamage(source, target, 0.5f * amount);
    }

    // Object manager

    Linq<Obj_AI_Hero> API::GetAllyHeroes(float range, const Vector& pos, bool valid) const
    {
        auto heroes = this->GetHeroes(range, pos, valid);
        return heroes.Where([&](const auto& u) { return this->IsAlly(u); });
    }

    Linq<Obj_AI_Hero> API::GetEnemyHeroes(float range, const Vector& pos, bool valid) const
    {
        auto heroes = this->GetHeroes(range, pos, valid);
        return heroes.Where([&](const auto& u) { return this->IsEnemy(u); });
    }

    Linq<Obj_AI_Hero> API::GetHeroes(float range, const Vector& pos, bool valid) const
    {
        Linq<Obj_AI_Hero> result;
        bool inf = range >= 25000.0f;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& hero : manager->get_heros())
        {
            auto unit = (Obj_AI_Hero)hero->get_as_hero();
            if (valid && !this->IsValid(unit)) continue;
            if (valid && !this->IsVisible(unit)) continue;
            if (inf) { result.Append(unit); continue; }
            Vector unit_pos = this->GetPosition(unit);
            float dist = unit_pos.DistanceSquared(pos);
            if (dist <= range * range) result.Append(unit);
        }
        return result;
    }

    Linq<Obj_AI_Minion> API::GetAllyMinions(float range, const Vector& pos) const
    {
        auto minions = this->GetMinions(range, pos);
        return minions.Where([&](const auto& u) { return this->IsAlly(u); });
    }

    Linq<Obj_AI_Minion> API::GetEnemyMinions(float range, const Vector& pos) const
    {
        auto minions = this->GetMinions(range, pos);
        return minions.Where([&](const auto& u) { return this->IsEnemy(u); });
    }

    Linq<Obj_AI_Minion> API::GetMinions(float range, const Vector& pos) const
    {
        Linq<Obj_AI_Minion> result;
        size_t npos = std::string::npos;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& minion : manager->get_minions())
        {
            auto unit = (Obj_AI_Minion)minion->get_as_minion();
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

    std::string API::GetBuffName(const Buff& buff) const
    {
        return buff->get_name();
    }

    Object API::GetBuffOwner(const Buff& buff) const
    {
        uint32_t id = buff->get_caster_id();
        auto manager = this->m_api->get_game_object_manager();
        return id ? manager->get_obj_by_id(id).release() : nullptr;
    }

    int API::GetBuffCount(const Obj_AI_Hero& unit, uint32_t hash) const
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

    float API::GetBuffTime(const Obj_AI_Hero& unit, uint32_t hash) const
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

    int API::GetItemSlot(const Obj_AI_Hero& unit, int item_id) const
    {
        for (int i = 0; i <= 6; ++i)
        {
            auto inventory = unit->get_inventory();
            auto data = inventory->get_slot(i)->get_item_data();
            if (data->get_id() == item_id) return (i + 6);
        }
        return -1;
    }

    bool API::HasBuff(const Obj_AI_Hero& unit, uint32_t hash) const
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

    bool API::HasBuffType(const Obj_AI_Hero& unit, BuffType type) const
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

    bool API::IsAlive(const Obj_AI_Base& unit) const
    {
        return !this->IsDead(unit);
    }

    bool API::IsAlly(const Obj_AI_Base& unit) const
    {
        if (this->IsEnemy(unit)) return false;
        return !this->Compare(unit, this->m_hero);
    }

    bool API::IsAttacking(const Obj_AI_Hero& unit) const
    {
        auto spell_cast = unit->get_spellbook()->get_spellcasting_obj();
        return spell_cast && spell_cast->is_auto_attack();
    }

    bool API::IsDead(const Obj_AI_Base& unit) const
    {
        return unit->is_dead();
    }

    bool API::IsEnemy(const Obj_AI_Base& unit) const
    {
        return unit->get_team() != this->m_hero->get_team();
    }

    bool API::IsImmortal(const Obj_AI_Base& unit) const
    {
        return unit->is_invulnerable();
    }

    bool API::IsMoving(const Obj_AI_Base& unit) const
    {
        return unit->get_nav_path()->is_moving();
    }

    bool API::IsValid(const Object& unit) const
    {
        return unit && unit->is_valid();
    }

    bool API::IsVisible(const Obj_AI_Base& unit) const
    {
        return unit->is_visible();
    }

    bool API::IsVulnerable(const Obj_AI_Base& unit) const
    {
        return !this->IsImmortal(unit);
    }

    bool API::IsWindingUp(const Obj_AI_Base& unit) const
    {
        return unit->is_winding_up();
    }

    // Pathing

    float API::GetDashSpeed(const Obj_AI_Base& unit) const
    {
        return unit->get_nav_path()->get_dash_speed();
    }

    Vector API::GetDashEndPos(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_nav_path()->get_end_pos());
    }

    Vector API::GetDashStartPos(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_nav_path()->get_start_pos());
    }

    Vector API::GetDirection(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_direction()).Rotate(M_PI_2);
    }

    Linq<Vector> API::GetPath(const Vector& pos) const
    {
        Linq<Vector> result;
        bool smoothing = true;
        auto& hero = this->m_hero;

        float height = this->GetHeight(pos);
        VecInt3 start_pos = hero->get_position();
        VecInt3 end_pos = this->To3D(pos, height);
        auto path = hero->build_navigation_path(
            start_pos, end_pos, smoothing);

        if (path == nullptr) return result;
        for (const auto& point : path->get_paths())
            result.Append(this->ToVector(point));

        return result;
    }

    Vector API::GetPathEndPos(const Obj_AI_Base& unit) const
    {
        auto data = unit->get_nav_path();
        uint32_t count = data->get_path_count();
        size_t size = static_cast<size_t>(count);
        VecInt3 position = unit->get_position();
        VecInt3 last = size <= 1 ? position :
            data->get_paths().at(size - 1);
        return this->ToVector(last);
    }

    float API::GetPathLength(const Linq<Vector>& path) const
    {
        float length = 0.0f;
        for (size_t i = 0; i < path.Count() - 1; ++i)
        {
            const Vector& p1 = path.ElementAt(i);
            const Vector& p2 = path.ElementAt(i + 1);
            length += p1.Distance(p2);
        }
        return length;
    }

    Vector API::GetPosition(const Object& unit) const
    {
        return this->ToVector(unit->get_position());
    }

    bool API::IsDashing(const Obj_AI_Base& unit) const
    {
        auto path = unit->get_nav_path();
        return path->is_moving() && path->get_dash_speed() > 0;
    }

    bool API::IsReachable(const Vector& pos) const
    {
        auto path = this->GetPath(pos);
        if (path.Count() < 2) return false;

        const Vector& last = path.Last();
        float length = last.DistanceSquared(pos);
        if (length > 10.0f) return false;

        const auto& hero = this->m_hero;
        Vector start = this->GetPosition(hero);
        float shortest = start.Distance(pos);
        length = this->GetPathLength(path);
        return length < shortest + 100.0f;
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