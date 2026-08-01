#include "elements_manager.h"
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(elements_manager_cpp)

void e_manager::add_tab(const char* label, const char* icon, std::function<void()> callback)
{
    tabs.push_back(tab_t(label, icon, callback));
}

void e_manager::add_tab(const char* label, const char* icon, std::vector<sub_tab_t> sub_tabs)
{
    tabs.push_back(tab_t(label, icon, sub_tabs));
}
