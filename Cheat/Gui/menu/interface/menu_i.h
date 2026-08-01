#pragma once
#include <string>
#include <d3d11.h>
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(menu_i_h)

class c_menu
{
public:
    virtual ~c_menu() = default;

    virtual void setup_data(ID3D11Device* device) = 0;
    virtual void setup_imgui() = 0;
    virtual void draw() = 0;
    virtual bool begin(const char* name) = 0;
    virtual void background() = 0;
    virtual void end() = 0;

    virtual void destroy() = 0;
};

c_menu* create_menu();
