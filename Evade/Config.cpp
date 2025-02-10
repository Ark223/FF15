
#include "Config.h"

namespace Evade
{
    Config* Config::m_instance = nullptr;

    Config::Config(PublicAPI api) : m_api(api)
    {
        this->RegisterAPI();
    }

    void Config::Destroy(const std::string& id)
    {
        m_instance->RemoveMenu(id);
        delete m_instance;
        m_instance = nullptr;
    }

    Config* Config::Get(PublicAPI api)
    {
        if (m_instance == nullptr)
        {
            m_instance = new Config(api);
        }
        return m_instance;
    }

    bool Config::IsLoaded() const
    {
        return this->m_menu != nullptr;
    }

    template<typename T>
    T Config::GetValue(MenuItem menu, const std::string& id) const
    {
        return menu->get<T>(id);
    }

    template bool Config::GetValue<bool>(MenuItem, const std::string&) const;

    template int Config::GetValue<int>(MenuItem, const std::string&) const;

    MenuItem Config::AddCheckbox(MenuItem menu, const std::string& id, const std::string& text, bool value)
    {
        return menu->add_checkbox(id, text, value);
    }

    MenuItem Config::AddKey(MenuItem menu, const std::string& id, const std::string& text, int key_value)
    {
        return menu->add_key(id, text, key_value);
    }

    MenuItem Config::AddLabel(MenuItem menu, const std::string& id, const std::string& text, bool center)
    {
        menu->add_separator();
        return menu->add_label(id, text, center, true);
    }

    MenuItem Config::AddList(MenuItem menu, const std::string& id, const std::string& text, Items items, int selected)
    {
        return menu->add_list_box(id, text, items, selected);
    }

    MenuItem Config::AddSlider(MenuItem menu, const std::string& id, const std::string& text, int value, int min, int max, int step)
    {
        return menu->add_slider_int(id, text, value, min, max, step);
    }

    MenuItem Config::AddToggle(MenuItem menu, const std::string& id, const std::string& text, int key_value, bool value)
    {
        return menu->add_key_toggle(id, text, key_value, value);
    }

    MenuItem Config::AddSubMenu(MenuItem menu, const std::string& id, const std::string& text)
    {
        return menu->add_sub_menu(id, text);
    }

    MenuItem Config::CreateMenu(const std::string& id, const std::string& text)
    {
        return this->m_menu->create_menu(id, text);
    }

    void Config::PrintText(const std::string& text)
    {
        this->m_menu->log_info(text);
    }

    void Config::RemoveMenu(const std::string& id)
    {
        this->m_menu->remove_menu(id);
    }

    void Config::RegisterAPI()
    {
        for (const auto plugin : this->m_api->get_apis())
        {
            if (plugin->plugin_name.compare("Menu") == 0 && plugin->version.compare("1") == 0)
            {
                this->m_menu = (PublicMenuAPI)plugin->get_api_vtable();
                return;
            }
        }
    }
}
