#pragma once
#include <d3d11.h>
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(textures_h)

namespace tex
{
    inline ID3D11ShaderResourceView* bg = nullptr;
    inline ID3D11ShaderResourceView* sidebar_bg = nullptr;
    inline ID3D11ShaderResourceView* main_logo = nullptr;
    inline ID3D11ShaderResourceView* esp = nullptr;
    inline ID3D11ShaderResourceView* welcome_logo = nullptr;
    inline ID3D11ShaderResourceView* transparent_logo = nullptr;
    inline ID3D11ShaderResourceView* tap_logo = nullptr;
    inline ID3D11ShaderResourceView* silent_esp_preview = nullptr;
}
