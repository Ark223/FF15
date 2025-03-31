
#include "API.h"

namespace IPrediction
{
    API* API::m_instance = nullptr;

    API::API(PublicAPI api) : m_api(api) {}

    void API::Destroy()
    {
        m_instance->UnloadEvents();
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

    void API::RegisterEvent(EventType type, void* callback)
    {
        switch (type)
        {
            case EventType::OnDraw:
                this->m_api->set_on_draw(callback);
                break;

            case EventType::OnNewPath:
                this->m_api->set_on_new_path(callback);
                break;

            case EventType::OnProcessSpell:
                this->m_api->set_on_process_spell(callback);
                break;
        }
    }

    void API::UnloadEvents()
    {
        // It is handled automatically by core
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
            hash ^= std::tolower(*str++);
            hash *= 0x01000193;
        }
        return hash;
    }

    Obj_AI_Hero API::GetHero()
    {
        if (this->m_hero != nullptr) return this->m_hero;
        auto manager = this->m_api->get_game_object_manager();
        auto hero = manager->get_local_player()->as_hero();
        this->m_hero = (Obj_AI_Hero)(hero);
        return this->m_hero;
    }

    Vector API::GetCursorPos() const
    {
        auto logic = this->m_api->get_hud_manager()->get_cursor_logic();
        return this->ToVector(*(logic->get_cursor_position()));
    }

    float API::GetLatency() const
    {
        return this->GetPing() / 2.0f;
    }

    float API::GetPing() const
    {
        return this->m_api->get_network_client()->get_ping() * 0.001f;
    }

    float API::GetTime() const
    {
        return this->m_api->get_riot_clock()->get_time();
    }

    // Hero actions

    bool API::CanUseSpell(int slot)
    {
        auto book = this->m_hero->get_spellbook();
        return book->can_use_spell(slot) == SpellState::Ready;
    }

    void API::CastSpell(int slot, const Object& unit)
    {
        this->m_hero->get_spellbook()->cast_spell(slot, unit);
    }

    void API::CastSpell(int slot, const Vector& pos, float height)
    {
        Vector3 vec = this->To3D(pos, height);
        this->m_hero->get_spellbook()->cast_spell(slot, vec);
    }

    // Drawings

    void API::DrawCenteredText(const Vector2& pos, const char* text, uint32_t color)
    {
        auto imgui = this->m_api->get_imgui_helper();
        auto fixed = imgui->calculate_text_size(text);
        uint32_t abgr = this->FixColor(color);
        fixed.x = pos.x - fixed.x / 2.0f;
        fixed.y = pos.y + fixed.y / 2.0f;
        imgui->draw_text(fixed, abgr, text);
    }

    void API::DrawCircle(const Vector& pos, float radius, float height, uint32_t color)
    {
        Vector3 vec = this->To3D(pos, height);
        uint32_t abgr = this->FixColor(color);
        auto imgui = this->m_api->get_imgui_helper();
        imgui->draw_circle_3d(vec, radius, abgr, 2.0f);
    }

    void API::DrawLine(const Vector& p1, const Vector& p2, float height, uint32_t color)
    {
        Vector2 v1 = this->ToScreen(p1, height);
        Vector2 v2 = this->ToScreen(p2, height);
        uint32_t abgr = this->FixColor(color);
        auto imgui = this->m_api->get_imgui_helper();
        imgui->draw_line(v1, v2, abgr, 2.0f);
    }

    void API::DrawPolygon(const std::vector<Vector>& poly, float height, uint32_t color)
    {
        for (size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++)
            this->DrawLine(poly[i], poly[j], height, color);
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
        auto manager = this->m_api->get_navigation_mesh();
        return manager->query_height_for_position({ pos.x, 0, pos.y });
    }

    Vector2 API::ToScreen(const Vector& pos, float height)
    {
        Vector3 vec = this->To3D(pos, height);
        return this->m_api->get_render_helper()->world_to_screen(vec);
    }

    // Spell cast data

    std::string API::GetSpellCastName(const CastInfo& info) const
    {
        return info->get_spell_data()->get_name();
    }

    Vector API::GetSpellCastEndPos(const CastInfo& info) const
    {
        return this->ToVector(info->get_end_position());
    }

    Vector API::GetSpellCastStartPos(const CastInfo& info) const
    {
        return this->ToVector(info->get_start_position());
    }

    float API::GetSpellCastStartTime(const CastInfo& info) const
    {
        return info->get_start_cast_time();
    }

    float API::GetSpellCastWindupTime(const CastInfo& info) const
    {
        return info->get_designer_cast_time();
    }

    // Game object base data

    bool API::Compare(const Object& a, const Object& b) const
    {
        if (a == nullptr || b == nullptr) return false;
        return this->GetObjectId(a) == this->GetObjectId(b);
    }

    uint32_t API::GetObjectId(const Object& object) const
    {
        return object->get_id();
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

    // Object manager

    Linq<Obj_AI_Base> API::GetAllyHeroes(float range, const Vector& pos, bool valid) const
    {
        auto selector = [&](auto& unit) { return this->IsAlly(unit); };
        return this->GetHeroes(range, pos, valid).Where(selector);
    }

    Linq<Obj_AI_Base> API::GetEnemyHeroes(float range, const Vector& pos, bool valid) const
    {
        auto selector = [&](auto& unit) { return this->IsEnemy(unit); };
        return this->GetHeroes(range, pos, valid).Where(selector);
    }

    Linq<Obj_AI_Base> API::GetHeroes(float range, const Vector& pos, bool valid) const
    {
        Linq<Obj_AI_Base> result;
        bool inf = range == 0.0f || range >= 25000.0f;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& hero : manager->get_heroes())
        {
            auto unit = (Obj_AI_Base)hero->as_ai_base();

            if (valid && !this->IsValid(unit)) continue;
            if (valid && !this->IsVisible(unit)) continue;
            if (inf) { result.Append(unit); continue; }

            Vector unit_pos = this->GetPosition(unit);
            float dist = unit_pos.DistanceSquared(pos);
            if (dist <= range * range) result.Append(unit);
        }
        return result;
    }

    Linq<Obj_AI_Base> API::GetAllyMinions(float range, const Vector& pos, bool filter) const
    {
        auto selector = [&](auto& unit) { return this->IsAlly(unit); };
        return this->GetMinions(range, pos, filter).Where(selector);
    }

    Linq<Obj_AI_Base> API::GetEnemyMinions(float range, const Vector& pos, bool filter) const
    {
        auto selector = [&](auto& unit) { return this->IsEnemy(unit); };
        return this->GetMinions(range, pos, filter).Where(selector);
    }

    Linq<Obj_AI_Base> API::GetMinions(float range, const Vector& pos, bool filter) const
    {
        Linq<Obj_AI_Base> result;
        size_t npos = std::string::npos;
        auto manager = this->m_api->get_game_object_manager();
        for (const auto& minion : manager->get_minions())
        {
            auto unit = (Obj_AI_Minion)minion->as_minion();

            if (!this->IsValid(unit)) continue;
            if (!this->IsVisible(unit)) continue;
            if (!this->IsAlive(unit)) continue;

            if (filter && this->IsPlant(unit)) continue;
            if (filter && this->IsWard(unit)) continue;
            
            Vector unit_pos = this->GetPosition(unit);
            float dist = unit_pos.DistanceSquared(pos);
            if (dist <= range * range) result.Append(unit);
        }
        return result;
    }

    // Buffs

    std::string API::GetBuffName(const Buff& buff) const
    {
        return buff->get_name();
    }

    float API::GetBuffTime(const Obj_AI_Base& unit, const BuffTable& hashes) const
    {
        float time = 0.0f;
        for (const auto& buff : unit->get_buff_manager()->get_buffs())
        {
            if (!buff || !buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (hashes.find(buff->get_hash()) != hashes.end())
            {
                time = MAX(time, buff->get_remaining_time());
            }
        }
        return time;
    }

    float API::GetBuffTime(const Obj_AI_Base& unit, const BuffTypes& types) const
    {
        float time = 0.0f;
        for (const auto& buff : unit->get_buff_manager()->get_buffs())
        {
            if (!buff || !buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (types.find(buff->get_type()) != types.end())
            {
                time = MAX(time, buff->get_remaining_time());
            }
        }
        return time;
    }

    bool API::HasBuff(const Obj_AI_Base& unit, const BuffTable& hashes) const
    {
        for (const auto& buff : unit->get_buff_manager()->get_buffs())
        {
            if (!buff || !buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (hashes.find(buff->get_hash()) != hashes.end())
            {
                return true;
            }
        }
        return false;
    }

    bool API::HasBuff(const Obj_AI_Base& unit, const BuffTypes& types) const
    {
        for (const auto& buff : unit->get_buff_manager()->get_buffs())
        {
            if (!buff || !buff->is_valid() || !buff->is_active())
            {
                continue;
            }
            if (types.find(buff->get_type()) != types.end())
            {
                return true;
            }
        }
        return false;
    }

    // Booleans

    bool API::IsAlly(const Obj_AI_Base& unit) const
    {
        return !this->IsEnemy(unit);
    }

    bool API::IsAlive(const Obj_AI_Base& unit) const
    {
        return !this->IsDead(unit);
    }

    bool API::IsDead(const Obj_AI_Base& unit) const
    {
        return unit->is_dead();
    }

    bool API::IsEnemy(const Obj_AI_Base& unit) const
    {
        return unit->get_team() != this->m_hero->get_team();
    }

    bool API::IsHero(const Obj_AI_Base& unit) const
    {
        return unit->get_type() == 0xE260302C;
    }

    bool API::IsMoving(const Obj_AI_Base& unit) const
    {
        return unit->get_navigation_path()->is_moving();
    }

    bool API::IsPlant(const Obj_AI_Minion& unit) const
    {
        return unit->is_plant();
    }

    bool API::IsValid(const Object& unit) const
    {
        return unit && unit->is_valid();
    }

    bool API::IsVisible(const Obj_AI_Base& unit) const
    {
        return unit->is_visible();
    }

    bool API::IsWard(const Obj_AI_Minion& unit) const
    {
        return unit->is_ward();
    }

    // Pathing

    float API::GetDashSpeed(const Obj_AI_Base& unit) const
    {
        return unit->get_navigation_path()->get_dash_speed();
    }

    Vector API::GetDashEndPos(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_navigation_path()->get_end_position());
    }

    Vector API::GetDashStartPos(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_navigation_path()->get_start_position());
    }

    Vector API::GetDirection(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_direction()).Rotate(M_PI_2_F);
    }

    float API::GetHitbox(const Obj_AI_Base& unit) const
    {
        return unit->get_bounding_radius();
    }

    float API::GetMovementSpeed(const Obj_AI_Base& unit) const
    {
        return unit->get_character_intermediate()->get_movement_speed();
    }

    Linq<Vector> API::GetPath(const Obj_AI_Base& unit) const
    {
        auto path = unit->get_navigation_path()->get_paths();
        return Linq(path).Select<Vector>([&](const auto& point)
        {
            return this->ToVector(point);
        });
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

    float API::GetPathSpeed(const Obj_AI_Base& unit) const
    {
        if (this->IsDashing(unit)) return this->GetDashSpeed(unit);
        return unit->get_character_intermediate()->get_movement_speed();
    }

    Vector API::GetPosition(const Obj_AI_Base& unit) const
    {
        return this->ToVector(unit->get_position());
    }

    bool API::IsDashing(const Obj_AI_Base& unit) const
    {
        auto path = unit->get_navigation_path();
        return path->is_moving() && path->get_dash_speed() > 0;
    }

    // Vectors

    Vector2 API::To2D(const Vector& vec) const
    {
        return Vector2(vec.x, vec.y);
    }

    Vector3 API::To3D(const Vector& vec, float y) const
    {
        return Vector3(vec.x, y, vec.y);
    }

    Vector API::ToVector(const Vector2& pos) const
    {
        return Vector(pos.x, pos.y);
    }

    Vector API::ToVector(const Vector3& pos) const
    {
        return Vector(pos.x, pos.z);
    }
}
