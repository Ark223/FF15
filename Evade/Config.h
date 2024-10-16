
#ifndef CONFIG_H
#define CONFIG_H

#include "API/public_menu_api.h"

namespace Evade
{
    using Items = std::vector<std::string>;
    using Menu = menu::AMenu;
    using PublicAPI = NS SurrenderAt15;
    using PublicMenuAPI = menu::AMenuConstructor;

    class Config
    {
        private:
            PublicAPI* m_api = nullptr;
            PublicMenuAPI* m_menu = nullptr;
            static Config* m_instance;

            Config(PublicAPI* api = nullptr);

        public:
            static void Destroy(const std::string& id);
            static Config* Get(PublicAPI* api = nullptr);
            bool IsLoaded() const;

            template<typename T>
            T GetValue(Menu* menu, const std::string& id) const;

            Menu* AddCheckbox(Menu* menu, const std::string& id, const std::string& text, bool value);
            Menu* AddKey(Menu* menu, const std::string& id, const std::string& text, int key_value);
            Menu* AddLabel(Menu* menu, const std::string& id, const std::string& text, bool center = false);
            Menu* AddList(Menu* menu, const std::string& id, const std::string& text, Items items, int selected);
            Menu* AddSlider(Menu* menu, const std::string& id, const std::string& text, int value, int min, int max, int step);
            Menu* AddSubMenu(Menu* menu, const std::string& id, const std::string& text);
            Menu* CreateMenu(const std::string& id, const std::string& text);
            void PrintText(const std::string& text);
            void RemoveMenu(const std::string& id);
            void RegisterAPI();
    };
}

#endif // CONFIG_H
