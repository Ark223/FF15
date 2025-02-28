
#ifndef CONFIG_H
#define CONFIG_H

#include "API/public_menu_api.h"

namespace Evade
{
    using MenuItem = Menu::AMenu*;
    using PublicAPI = NS SurrenderAt15*;
    using PublicMenuAPI = Menu::AMenuConstructor*;
    using Items = std::vector<std::string>;

    class Config
    {
        private:
            PublicAPI m_api = nullptr;
            PublicMenuAPI m_menu = nullptr;
            static Config* m_instance;

            Config(PublicAPI api = nullptr);

        public:
            static void Destroy(const std::string& id);
            static Config* Get(PublicAPI api = nullptr);
            bool IsLoaded() const;

            template<typename T>
            T GetValue(MenuItem menu, const std::string& id) const;

            MenuItem AddCheckbox(MenuItem menu, const std::string& id, const std::string& text, bool value);
            MenuItem AddKey(MenuItem menu, const std::string& id, const std::string& text, int key_value);
            MenuItem AddLabel(MenuItem menu, const std::string& id, const std::string& text, bool center = false);
            MenuItem AddList(MenuItem menu, const std::string& id, const std::string& text, Items items, int selected);
            MenuItem AddSlider(MenuItem menu, const std::string& id, const std::string& text, int value, int min, int max, int step);
            MenuItem AddToggle(MenuItem menu, const std::string& id, const std::string& text, int key_value, bool value);
            MenuItem AddSubMenu(MenuItem menu, const std::string& id, const std::string& text);
            MenuItem CreateMenu(const std::string& id, const std::string& text);
            void PrintText(const std::string& text);
            void RemoveMenu(const std::string& id);
            void RegisterAPI();
    };
}

#endif // CONFIG_H
