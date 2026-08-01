#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <D3D11.h>
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(elements_manager_h)

struct sub_tab_t
{
    sub_tab_t(std::string l, std::string i, std::function<void()> c)
    {
        label = l; icon = i; callback = c;
    };
    std::string label{};
    std::string icon{};
    std::function<void()> callback;
};

struct tab_t
{
    tab_t(std::string l, std::string i, std::function<void()> c)
    {
        label = l; icon = i; callback = c;
    };
    tab_t(std::string l, std::string i, std::vector<sub_tab_t> subs)
    {
        label = l; icon = i; callback = nullptr; sub_tabs = subs;
    };
    std::string label{};
    std::string icon{};
    std::function<void()> callback;
    std::vector<sub_tab_t> sub_tabs{};
    int active_sub = 0;
    bool has_subs() const { return !sub_tabs.empty(); }
};

class e_manager
{
public:

    void add_tab(const char* label, const char* icon, std::function<void()> callback);
    void add_tab(const char* label, const char* icon, std::vector<sub_tab_t> sub_tabs);
    std::vector<tab_t> tabs{};
    bool esp_p = false;


    bool notify = false;
    bool close_notify = false;
    int notify_status = 1;
    std::vector<std::string> notify_desc{};


    void show_notification(int status, const std::vector<std::string>& descriptions) {
        notify_status = status;
        notify_desc = descriptions;
        notify = true;
        close_notify = false;
    }


    void show_key_bind_notification(const std::string& feature_name, const std::string& key_name) {
        std::vector<std::string> desc = {
            globals::brand::signature,
            "Key Binding Set!",
            feature_name + " is now bound to " + key_name,
            "Press the key to activate the feature"
        };
        show_notification(1, desc);
    }


    ID3D11Device* g_pd3dDevice = nullptr;
};

inline e_manager* MGR = new e_manager();
