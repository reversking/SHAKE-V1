#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <d3d11.h>
#include "../../../Cheat/TextureLoader.hpp"
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(widgets_h)
#include <vector>

#include "render.h"
#include "../helpers/anim/anim.h"
#include "../helpers/restore.h"
#include "../defs/fonts.h"
#include "../defs/textures.h"
#include "../defs/colors.h"
#include "../interface/elements_manager.h"

using namespace ImGui;

inline float global_a = 0.f;
inline float global_slide_x = 0.f;
inline bool global_clip_pushed = false;

inline bool checkbox(const char* label, bool* v, bool header = false, bool* parent_enabled = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    bool should_show = (parent_enabled == nullptr) || (*parent_enabled);
    if (!should_show)
        return false;

    auto curr_b = anim::animation((std::string(label) + "##checkbox").c_str(), anim_t(clamp_out, 0.15f));
    curr_b->active = (*v);

    auto curr_h = anim::animation((std::string(label) + "##checkbox_hover").c_str(), anim_t(clamp_out, 0.12f));

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 total_size = ImVec2(310.f, header ? 28.f : 22.f);

    float x_offset = header ? 6.f : 14.f;
    float y_offset = header ? 2.f : 6.f;
    if (!header) {
        float slide_off = -50.0f * (1.0f - global_slide_x);
        x_offset += slide_off;
    }
    const ImVec2 pos = window->DC.CursorPos + ImVec2(x_offset, y_offset);
    const ImRect nav_bb(pos, pos + ImVec2(total_size.x, total_size.y));
    ItemSize(nav_bb, style.FramePadding.y - 2.f);
    const bool is_visible = ItemAdd(nav_bb, id);

    bool checked = *v;
    bool hovered, held;
    bool pressed = ButtonBehavior(nav_bb, id, &hovered, &held);
    curr_h->active = hovered;

    if (pressed) checked = !checked;
    if (*v != checked) { *v = checked; pressed = true; MarkItemEdited(id); }

    float extra_a = curr_b->val;
    float hover_a = curr_h->val;

    if (is_visible)
    {
        if (header)
        {

            const float sw_w = 28.f, sw_h = 14.f;
            ImVec2 sw_pos(nav_bb.Max.x - sw_w - 2.f, nav_bb.Min.y + (total_size.y - sw_h) * 0.5f);
            float sw_r = sw_h * 0.5f;


            ImU32 track_col = IM_COL32(
                (int)(10 + 245 * extra_a * 0.08f),
                (int)(10 + 20 * extra_a * 0.3f),
                (int)(10 + 147 * extra_a * 0.1f), 255);
            window->DrawList->AddRectFilled(sw_pos, ImVec2(sw_pos.x + sw_w, sw_pos.y + sw_h), track_col, sw_r);


            ImU32 border_col = IM_COL32(255, 60, 160, (int)(40 + 80 * extra_a));
            window->DrawList->AddRect(sw_pos, ImVec2(sw_pos.x + sw_w, sw_pos.y + sw_h), border_col, sw_r, 0, 1.0f);


            if (extra_a > 0.01f) {
                for (int gi = 2; gi > 0; gi--) {
                    ImU32 gc = IM_COL32(255, 40, 150, (int)(15 * extra_a / gi));
                    window->DrawList->AddRect(sw_pos - ImVec2((float)gi,(float)gi),
                        ImVec2(sw_pos.x + sw_w + gi, sw_pos.y + sw_h + gi), gc, sw_r + gi, 0, 1.0f);
                }
            }


            float knob_x = sw_pos.x + sw_r + (sw_w - sw_h) * extra_a;
            float knob_y = sw_pos.y + sw_r;
            float knob_r = sw_r - 2.0f;
            ImU32 knob_col = IM_COL32(255, (int)(255 - 155 * extra_a), (int)(255 - 75 * extra_a), 255);
            window->DrawList->AddCircleFilled(ImVec2(knob_x, knob_y), knob_r + 1.f, IM_COL32(0,0,0,80), 16);
            window->DrawList->AddCircleFilled(ImVec2(knob_x, knob_y), knob_r, knob_col, 16);


            if (tex::transparent_logo != nullptr) {
                float icon_size = 14.f;
                ImVec2 icon_pos = nav_bb.Min + ImVec2(0.f, (total_size.y - icon_size) * 0.5f);
                if (extra_a > 0.f) {
                    for (int gi = 2; gi > 0; gi--) {
                        ImU32 ig = IM_COL32(255, 105, 180, (int)(30.f * extra_a / gi));
                        window->DrawList->AddImage((ImTextureID)tex::transparent_logo,
                            icon_pos - ImVec2((float)gi,(float)gi),
                            icon_pos + ImVec2(icon_size + gi, icon_size + gi),
                            ImVec2(0,0), ImVec2(1,1), ig);
                    }
                }
                ImU32 ia = *v ? IM_COL32(255,255,255,255) : IM_COL32(255,255,255,(int)(120+135*(hover_a+extra_a)*0.5f));
                window->DrawList->AddImage((ImTextureID)tex::transparent_logo, icon_pos,
                    icon_pos + ImVec2(icon_size, icon_size), ImVec2(0,0), ImVec2(1,1), ia);
            }


            float text_x = nav_bb.Min.x + 18.f;
            float text_y = nav_bb.Min.y + 2.f;
            ImVec2 label_pos(text_x, text_y);
            ImColor text_col = *v ? ImColor(255,255,255,255) : (ImColor)c::white72;
            if (*v && extra_a > 0.1f) {
                window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(0.f,1.f),
                    ImColor(255,60,160,(int)(30*extra_a)), label);
            }
            window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f,1.f), ImColor(0,0,0,40), label);
            window->DrawList->AddText(f::medium14, 16.f, label_pos, text_col, label);
        }
        else
        {

            const float sw_w = 30.f, sw_h = 14.f;
            ImVec2 sw_pos(nav_bb.Max.x - sw_w - 2.f, nav_bb.Min.y + (total_size.y - sw_h) * 0.5f);
            float sw_r = sw_h * 0.5f;


            ImU32 track_off = IM_COL32(20, 20, 25, 255);
            ImU32 track_on = IM_COL32(255, 20, 147, (int)(60 + 80 * extra_a));
            ImU32 track_col = IM_COL32(
                (int)(20 + (255-20) * extra_a * 0.25f),
                (int)(20 + (20-20) * extra_a),
                (int)(25 + (147-25) * extra_a * 0.3f),
                255);
            window->DrawList->AddRectFilled(sw_pos, ImVec2(sw_pos.x + sw_w, sw_pos.y + sw_h), track_col, sw_r);


            if (extra_a > 0.01f) {
                ImU32 fill = IM_COL32(255, 20, 147, (int)(50 * extra_a));
                window->DrawList->AddRectFilled(sw_pos, ImVec2(sw_pos.x + sw_w, sw_pos.y + sw_h), fill, sw_r);
            }


            float ba = 30.f + 50.f * (extra_a + hover_a * 0.3f);
            window->DrawList->AddRect(sw_pos, ImVec2(sw_pos.x + sw_w, sw_pos.y + sw_h),
                IM_COL32(255, 60, 160, (int)ba), sw_r, 0, 1.0f);


            if (extra_a > 0.05f) {
                for (int gi = 3; gi > 0; gi--) {
                    ImU32 gc = IM_COL32(255, 30, 150, (int)(12 * extra_a / gi));
                    window->DrawList->AddRect(
                        sw_pos - ImVec2((float)gi,(float)gi),
                        ImVec2(sw_pos.x + sw_w + gi, sw_pos.y + sw_h + gi),
                        gc, sw_r + gi, 0, 1.0f);
                }
            }


            float knob_x = sw_pos.x + sw_r + (sw_w - sw_h) * extra_a;
            float knob_y = sw_pos.y + sw_r;
            float knob_r = sw_r - 2.0f;


            window->DrawList->AddCircleFilled(ImVec2(knob_x + 0.5f, knob_y + 0.5f), knob_r + 1.f, IM_COL32(0,0,0,60), 16);


            int kr = (int)(230 + 25 * extra_a);
            int kg = (int)(230 - 130 * extra_a);
            int kb = (int)(230 - 40 * extra_a);
            window->DrawList->AddCircleFilled(ImVec2(knob_x, knob_y), knob_r, IM_COL32(kr, kg, kb, 255), 16);


            if (extra_a > 0.01f) {
                window->DrawList->AddCircleFilled(ImVec2(knob_x, knob_y - 1.f), knob_r * 0.5f,
                    IM_COL32(255, 255, 255, (int)(40 * extra_a)), 12);
            }


            ImVec2 label_pos(nav_bb.Min.x, nav_bb.Min.y + 2.f);
            ImColor text_col = lerp(c::white72, c::white, (hover_a + extra_a) * 0.5f);
            window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f,1.f), ImColor(0,0,0,40), label);
            window->DrawList->AddText(f::medium14, 16.f, label_pos, text_col, label);
        }
    }

    return pressed;
}

inline bool skeleton_checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    auto curr_b = anim::animation((std::string(label) + "##skeleton").c_str(), anim_t(clamp_out, 0.1f));
    curr_b->active = (*v);

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID((std::string(label) + "##skeleton").c_str());
    const ImVec2 total_size = ImVec2(19.f, 19.f);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect nav_bb(pos, pos + ImVec2(total_size.x, total_size.y));
    ItemSize(nav_bb, style.FramePadding.y - 2.f);
    const bool is_visible = ItemAdd(nav_bb, id);

    bool checked = *v;

    bool hovered, held;
    bool pressed = ButtonBehavior(nav_bb, id, &hovered, &held);

    if (pressed)
        checked = !checked;

    if (*v != checked)
    {
        *v = checked;
        pressed = true;
        MarkItemEdited(id);
    }

    float extra_a = curr_b->val;
    if (is_visible)
    {

        ImColor col2 = lerp(lerp(c::white0, c::gray, global_a), lerp(c::gray, c::secondary, global_a), extra_a);
        ImColor col3 = lerp(lerp(c::white0, c::gray2, global_a), lerp(c::gray2, c::primary, global_a), extra_a);
        ImColor col4 = lerp(lerp(c::white0, c::white36, global_a), lerp(c::white36, c::white, global_a), extra_a);

        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddCircleFilled(nav_bb.Min + ImVec2(total_size.x / 2.f, total_size.y / 2.f), total_size.x / 2.f, IM_COL32_WHITE);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        ShadeVertsLinearGradY(window->DrawList, vert_start_idx, vert_end_idx,
            nav_bb.Min, nav_bb.Max, col2, col3);

        window->DrawList->AddCircle(nav_bb.Min + ImVec2(total_size.x / 2.f, total_size.y / 2.f),
            (total_size.x / 2.f - 4.f * extra_a), col4, 360, 1.f + 3.f * extra_a);


























    }

    return pressed;
}

inline static const char* KeyNames[] = {
    "n/a",
    "LBUTTON",
    "RBUTTON",
    "CANCEL",
    "MBUTTON",
    "XBUTTON1",
    "XBUTTON2",
    "Unknown",
    "BACK",
    "TAB",
    "Unknown",
    "Unknown",
    "CLEAR",
    "RETURN",
    "Unknown",
    "Unknown",
    "SHIFT",
    "CONTROL",
    "MENU",
    "PAUSE",
    "CAPITAL",
    "KANA",
    "Unknown",
    "JUNJA",
    "FINAL",
    "KANJI",
    "Unknown",
    "ESCAPE",
    "CONVERT",
    "NONCONVERT",
    "ACCEPT",
    "MODECHANGE",
    "SPACE",
    "PRIOR",
    "NEXT",
    "END",
    "HOME",
    "LEFT",
    "UP",
    "RIGHT",
    "DOWN",
    "SELECT",
    "PRINT",
    "EXECUTE",
    "SNAPSHOT",
    "INSERT",
    "DELETE",
    "HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "Unknown",
    "SLEEP",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "MULTIPLY",
    "ADD",
    "SEPARATOR",
    "SUBTRACT",
    "DECIMAL",
    "DIVIDE",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "NUMLOCK",
    "SCROLL",
    "OEM_NEC_EQUAL",
    "OEM_FJ_MASSHOU",
    "OEM_FJ_TOUROKU",
    "OEM_FJ_LOYA",
    "OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "LSHIFT",
    "RSHIFT",
    "LCONTROL",
    "RCONTROL",
    "LMENU",
    "RMENU"
};
inline static const int KeyCodes[] = {
    0x0,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1A,
    0x1B,
    0x1C,
    0x1D,
    0x1E,
    0x1F,
    0x20,
    0x21,
    0x22,
    0x23,
    0x24,
    0x25,
    0x26,
    0x27,
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    0x2D,
    0x2E,
    0x2F,
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x3D,
    0x3E,
    0x3F,
    0x40,
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46,
    0x47,
    0x48,
    0x49,
    0x4A,
    0x4B,
    0x4C,
    0x4B,
    0x4E,
    0x4F,
    0x50,
    0x51,
    0x52,
    0x53,
    0x54,
    0x55,
    0x56,
    0x57,
    0x58,
    0x59,
    0x5A,
    0x5B,
    0x5C,
    0x5D,
    0x5E,
    0x5F,
    0x60,
    0x61,
    0x62,
    0x63,
    0x64,
    0x65,
    0x66,
    0x67,
    0x68,
    0x69,
    0x6A,
    0x6B,
    0x6C,
    0x6D,
    0x6E,
    0x6F,
    0x70,
    0x71,
    0x72,
    0x73,
    0x74,
    0x75,
    0x76,
    0x77,
    0x78,
    0x79,
    0x7A,
    0x7B,
    0x7C,
    0x7D,
    0x7E,
    0x7F,
    0x80,
    0x81,
    0x82,
    0x83,
    0x84,
    0x85,
    0x86,
    0x87,
    0x88,
    0x89,
    0x8A,
    0x8B,
    0x8C,
    0x8D,
    0x8E,
    0x8F,
    0x90,
    0x91,
    0x92,
    0x93,
    0x94,
    0x95,
    0x96,
    0x97,
    0x98,
    0x99,
    0x9A,
    0x9B,
    0x9C,
    0x9D,
    0x9E,
    0x9F,
    0xA0,
    0xA1,
    0xA2,
    0xA3,
    0xA4,
    0xA5
};

inline bool keybind(const char* label, int* k, bool* parent_enabled = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;


    bool should_show = (parent_enabled == nullptr) || (*parent_enabled);
    if (!should_show)
        return false;

    auto curr_act = anim::animation((std::string(label) + "##actkeybind").c_str(), anim_t(clamp_out, 0.1f));
    auto curr_b = anim::animation((std::string(label) + "##keybind").c_str(), anim_t(clamp_out, 0.1f));

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 total_size = ImVec2(310.f, 25.f);

    const ImVec2 pos = window->DC.CursorPos + ImVec2(14.f + (-50.0f * (1.0f - global_slide_x)), 6.f);
    const ImRect nav_bb(pos, pos + ImVec2(total_size.x, total_size.y));
    ItemSize(nav_bb, style.FramePadding.y - 2.f);
    const bool is_visible = ItemAdd(nav_bb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(nav_bb, id, &hovered, &held);

    std::string key_sz = KeyNames[*(int*)k];

    curr_b->active = (*(int*)k) != 0;

    if (curr_act->active)
    {
        key_sz = "...";
        curr_b->active = false;
        if (curr_act->val > 0.8f)
        {
            for (auto& Key : KeyCodes)
            {
                if (GetAsyncKeyState(Key) & 0x8000) {
                    if (Key == VK_ESCAPE) {
                        *(int*)k = 0;
                    }
                    else {
                        *(int*)k = Key;
                    }
                    curr_act->active = false;
                }
            }
        }
    }

    ImVec2 text_size = f::medium14->CalcTextSizeA(16.f, FLT_MAX, -1.f, key_sz.c_str(), 0, NULL);

    const ImRect check_bb(nav_bb.Max - ImVec2(12.f + text_size.x, 25.f), nav_bb.Max);

    float extra_a = curr_b->val;
    if (is_visible)
    {

        ImColor col2 = lerp(lerp(c::white2, c::white4, global_a), lerp(c::white48, c::secondary, global_a), extra_a);
        ImColor col3 = lerp(lerp(c::white3, c::white2, global_a), lerp(c::white24, c::primary, global_a), extra_a);

        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, IM_COL32_WHITE, 4.f);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        ShadeVertsLinearGradY(window->DrawList, vert_start_idx, vert_end_idx,
            check_bb.Min, check_bb.Max, col2, col3);

        window->DrawList->AddRect(check_bb.Min, check_bb.Max, c::white4, 4.f);

        ImColor col4 = lerp(lerp(c::white48, c::white72, global_a), lerp(ImColor(20, 20, 20, 255), c::bg, global_a), extra_a);
        ImVec2 t_pos = ImVec2(check_bb.Min.x + 7.f, check_bb.Min.y + 2.f);

        window->DrawList->AddText(f::medium14, 16.f, t_pos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 40), key_sz.c_str());
        window->DrawList->AddText(f::medium14, 16.f, t_pos, col4, key_sz.c_str());























        const ImVec2 label_pos = ImVec2(nav_bb.Min.x, nav_bb.Min.y + 1.f);
        ImColor col = lerp(c::white72, c::white, global_a);


        window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 40), label);
        window->DrawList->AddText(f::medium14, 16.f, label_pos, col, label);
    }

    if (pressed)
        curr_act->active = !curr_act->active;

    return pressed;
}

inline bool slider_scalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, bool* parent_enabled = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;


    bool should_show = (parent_enabled == nullptr) || (*parent_enabled);
    if (!should_show)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 total_size = ImVec2(310.f, 29.f);

    const ImVec2 pos = window->DC.CursorPos + ImVec2(14.f + (-50.0f * (1.0f - global_slide_x)), 6.f);
    const ImRect total_bb(pos, pos + total_size);
    const ImRect extra_nav_bb(pos + ImVec2(-4.f, 0.f), pos + ImVec2(total_size.x + 4.f, total_size.y));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;


    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(total_bb, id, g.LastItemData.ItemFlags);


    const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
    const bool make_active = (clicked || g.NavActivateId == id);
    if (make_active && clicked)
        SetKeyOwner(ImGuiKey_MouseLeft, id);


    if (make_active)
        memcpy(&g.ActiveIdValueOnActivation, p_data, DataTypeGetInfo(data_type)->Size);

    if (make_active)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    window->DrawList->AddRectFilled(total_bb.Min + ImVec2(0.f, 25.f), total_bb.Max, c::white4, 512.f);


    ImRect grab_bb;
    const bool value_changed = SliderBehavior(extra_nav_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);
    float pp = grab_bb.GetCenter().x - window->DC.CursorPos.x;

    static std::map<ImGuiID, elastic_point> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, elastic_point(pp, 0.3f, 0.8f, 1.2f) });
        it_anim = anim.find(id);
    }


    if (grab_bb.Max.x > grab_bb.Min.x)
    {
        it_anim->second.set_target(pp);
        float cur_pos = window->DC.CursorPos.x + it_anim->second.get_pos();

        if (total_bb.Min.x < cur_pos) {

            ImVec2 fill_start = total_bb.Min + ImVec2(0.f, 25.f);
            ImVec2 fill_end = ImVec2(cur_pos, total_bb.Max.y);


            window->DrawList->AddRectFilledMultiColor(
                fill_start, fill_end,
                ImColor(255, 20, 147, 255),
                ImColor(255, 105, 180, 255),
                ImColor(255, 105, 180, 255),
                ImColor(255, 20, 147, 255)
            );


            window->DrawList->AddRectFilledMultiColor(
                fill_start, ImVec2(fill_end.x, fill_start.y + 1.5f),
                ImColor(255, 255, 255, 60),
                ImColor(255, 255, 255, 60),
                ImColor(255, 255, 255, 0),
                ImColor(255, 255, 255, 0)
            );
        }
        else {
            it_anim->second.avoid_v();
        }

        if (total_bb.Max.x < cur_pos)
            it_anim->second.avoid_v();


        float grab_y = total_bb.Max.y - 2.f;


        for (int i = 3; i > 0; i--) {
            float glow_radius = 8.f + (i * 2.f);
            int glow_alpha = 25 - (i * 5);
            window->DrawList->AddCircleFilled(
                ImVec2(cur_pos, grab_y),
                glow_radius,
                ImColor(255, 20, 147, glow_alpha)
            );
        }


        window->DrawList->AddCircleFilled(ImVec2(cur_pos, grab_y), 7.f, ImColor(255, 20, 147, 100));


        window->DrawList->AddCircleFilled(ImVec2(cur_pos, grab_y), 5.f, ImColor(255, 255, 255, 255));
        window->DrawList->AddCircleFilled(ImVec2(cur_pos, grab_y), 3.5f, ImColor(255, 240, 245, 255));


        window->DrawList->AddCircleFilled(ImVec2(cur_pos, grab_y - 1.f), 2.f, ImColor(255, 255, 255, 120));
    }
    it_anim->second.update(0.16f);

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    const char* text_display_end = FindRenderedTextEnd(value_buf, value_buf_end);
    ImVec2 text_size = f::medium14->CalcTextSizeA(16.f, FLT_MAX, -1.f, value_buf, 0, NULL);
    ImVec2 t_pos = ImVec2(total_bb.Max.x - text_size.x, total_bb.Min.y);

    window->DrawList->AddText(f::medium14, 16.f, t_pos + ImVec2(1.f, 1.f),
        ImColor(0, 0, 0, 40), value_buf, text_display_end);
    window->DrawList->AddText(f::medium14, 16.f, t_pos,
        lerp(c::white48, c::white72, global_a), value_buf, text_display_end);

    const ImVec2 label_pos = ImVec2(total_bb.Min.x, total_bb.Min.y);

    window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f, 1.f),
        ImColor(0, 0, 0, 40), label);
    window->DrawList->AddText(f::medium14, 16.f, label_pos,
        lerp(c::white72, c::white, global_a), label);

    return value_changed;
}

inline bool slider(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0, bool* parent_enabled = nullptr)
{
    return slider_scalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags, parent_enabled);
}

inline static void fColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
        return;



    if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
        *H = g.ColorEditSavedHue;


    if (*V == 0.0f)
        *S = g.ColorEditSavedSat;
}

inline bool color_button(const char* desc_id, const ImVec4& col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImVec2 pos = window->DC.CursorPos;
    window->DrawList->AddCircleFilled(pos + ImVec2(10.f, 10.f), 7.f, a(col, 0.12f + 0.12f * global_a));
    window->DrawList->AddCircleFilled(pos + ImVec2(10.f, 10.f), 5.f, a(col, 0.48f + 0.52f * global_a));
    bool ret = ::InvisibleButton((std::string(desc_id)+"##cb").c_str(), ImVec2(21.f, 21.f));
    return ret;
}

inline bool color_edit(const char* label, float col[4], bool* parent_enabled = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;


    bool should_show = (parent_enabled == nullptr) || (*parent_enabled);
    if (!should_show)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = GetFrameHeight();
    const char* label_display_end = FindRenderedTextEnd(label);
    float w_full = CalcItemWidth();
    g.NextItemData.ClearFlags();

    BeginGroup();
    PushID(label);
    const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
    if (set_current_color_edit_id)
        g.ColorEditCurrentID = window->IDStack.back();


    const ImGuiColorEditFlags flags_untouched = 0;
    ImGuiColorEditFlags flags = 0;

    if (!(flags & ImGuiColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
    if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));

    const bool alpha = false;
    const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;
    const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = ImMax(w_full - w_button, 1.0f);
    w_full = w_inputs + w_button;


    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
    {

        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        fColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
    }
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos + ImVec2(14.f + (-50.0f * (1.0f - global_slide_x)), 16.f);
    const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;


    ImGuiWindow* picker_active_window = NULL;
    if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + 296, pos.y);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (color_button("##ColorButton", col_v4))
        {
            if (!(flags & ImGuiColorEditFlags_NoPicker))
            {

                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
            }
        }

        if (BeginPopup("picker", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
        {
            if (g.CurrentWindow->BeginCount == 1)
            {
                auto draw = ::GetWindowDrawList();
                const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
                const ImVec2& region = ::GetContentRegionMax() - ImVec2(2.f, 2.f);
                draw->AddRectFilled(p, p + region, c::white2, 8.f);

                picker_active_window = g.CurrentWindow;
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 10.0f);
                value_changed |= ColorPicker4((std::string(label) + "##picker").c_str(), col, picker_flags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview, &g.ColorPickerRef.x);


                draw->AddRect(p, p + region, c::white6, 8.f);
            }
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {





        const ImVec2 label_pos = pos;
        ImColor col = lerp(c::white72, c::white, global_a);

        window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 40), label, label_display_end);
        window->DrawList->AddText(f::medium14, 16.f, label_pos, col, label, label_display_end);
    }


    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
        {
            g.ColorEditSavedHue = f[0];
            g.ColorEditSavedSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
        }
        if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    if (set_current_color_edit_id)
        g.ColorEditCurrentID = 0;
    PopID();
    EndGroup();



    if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(g.LastItemData.ItemFlags & ImGuiItemFlags_ReadOnly) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3);
            value_changed = accepted_drag_drop = true;
        }
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }


        if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
        EndDragDropTarget();
    }


    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        g.LastItemData.ID = g.ActiveId;

    if (value_changed && g.LastItemData.ID != 0)
        MarkItemEdited(g.LastItemData.ID);

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.f, -14.f));

    return value_changed;
}

struct data_s
{
    float sz_y{};
    float y2{};
    float pulse_time{};
    float arrow_rot{};
    float shimmer_pos{};
};

inline static std::map<ImGuiID, data_s> datax;
inline std::map<ImGuiID, data_s>::iterator it_data;

inline void child(const char* str_id, bool *v, bool empty)
{
    ImGuiID id = GetCurrentWindow()->GetID(str_id);

    it_data = datax.find(id);
    if (it_data == datax.end())
    {
        datax.insert({ id, data_s() });
        it_data = datax.find(id);
    }
    std::string g = str_id;


    auto curr_b = anim::animation((std::string(str_id) + "##child").c_str(), anim_t(clamp_out, 0.10f));
    auto curr_h = anim::animation((std::string(str_id) + "##child_hover").c_str(), anim_t(clamp_out, 0.12f));
    auto curr_fade = anim::animation((std::string(str_id) + "##child_fade").c_str(), anim_t(clamp_out, 0.08f));

    bool show_content = (v == nullptr) || (*v);
    if (v != nullptr) { curr_b->active = (*v); curr_fade->active = (*v); }
    else { curr_b->active = true; curr_fade->active = true; }

    float anim_progress = curr_b->val;

    float eased;
    if (show_content) {

        eased = anim_progress * anim_progress * (3.0f - 2.0f * anim_progress);
    } else {

        float t = anim_progress;
        eased = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    float content_fade = ImClamp(curr_fade->val * 1.2f - 0.2f, 0.0f, 1.0f);


    bool anim_active = anim_progress > 0.01f;

    const float header_h = 36.f;
    float full_height = header_h + it_data->second.sz_y;
    float animated_height = header_h + ((full_height - header_h) * eased);


    if (show_content && it_data->second.sz_y < 1.f) {
        animated_height = header_h + 200.f * eased;
    }

    float dt = ImGui::GetIO().DeltaTime;
    it_data->second.pulse_time += dt;
    it_data->second.shimmer_pos += dt * 130.0f;
    if (it_data->second.shimmer_pos > 600.f) it_data->second.shimmer_pos -= 600.f;

    float target_rot = show_content ? 0.0f : -90.0f;
    it_data->second.arrow_rot += (target_rot - it_data->second.arrow_rot) * (std::min)(dt * 6.0f, 1.0f);

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(8.f, 0.f));
    ::BeginChild(("##child" + g).c_str(), ImVec2(340.f, animated_height), 0,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        auto draw = ::GetWindowDrawList();
        const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
        const ImVec2& region = ::GetContentRegionMax() - ImVec2(2.f, 2.f);


        draw->PushClipRect(
            ImVec2(p.x - 2.f, p.y - 2.f),
            ImVec2(p.x + region.x + 2.f, p.y + animated_height + 8.f),
            true
        );

        float extra_b = ImClamp(eased, 0.f, 1.f);

        ImVec2 header_min = p;
        ImVec2 header_max = p + ImVec2(region.x, header_h);
        bool is_hovered = ImGui::IsMouseHoveringRect(header_min, header_max) && !ImGui::IsAnyItemActive();
        curr_h->active = is_hovered;
        float hover_a = curr_h->val;

        ImVec2 header_region = ImVec2(region.x, header_h);
        float pulse = sinf(it_data->second.pulse_time * 2.5f) * 0.5f + 0.5f;
        bool is_active = (v != nullptr && *v);
        float effect = is_active ? extra_b : hover_a;

        ImVec2 box_min = p;
        ImVec2 box_max = p + ImVec2(region.x, (std::max)(animated_height - 2.f, header_h));
        bool closed_state = (animated_height <= header_h + 1.f);
        ImDrawFlags rnd_flags = closed_state ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersAll;



        ImU32 hdr_tl = is_active ? IM_COL32(18, 8, 28, 245) : IM_COL32(12, 10, 18, 240);
        ImU32 hdr_tr = is_active ? IM_COL32(14, 6, 24, 245) : IM_COL32(10, 8, 16, 240);
        ImU32 hdr_bl = IM_COL32(6, 4, 10, 250);
        ImU32 hdr_br = IM_COL32(4, 3, 8, 250);
        ImDrawFlags hdr_rnd = closed_state ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersTop;
        draw->AddRectFilledMultiColor(p, p + header_region, hdr_tl, hdr_tr, hdr_br, hdr_bl);
        draw->AddRectFilled(p, p + header_region, IM_COL32(0, 0, 0, 0), 10.f, hdr_rnd);


        if (hover_a > 0.01f || is_active) {
            float glass_a = is_active ? (12.0f + 4.0f * pulse) : (8.0f * hover_a);
            draw->AddRectFilled(p, ImVec2(p.x + region.x, p.y + header_h * 0.35f),
                IM_COL32(255, 255, 255, (int)glass_a), 10.f, ImDrawFlags_RoundCornersTop);
        }


        if (effect > 0.01f) {
            float w = region.x;
            float shm = it_data->second.shimmer_pos;
            int step = 2;
            for (int seg = 0; seg < (int)w; seg += step) {
                float sf = (float)seg;
                float t_n = sf / w;
                float r = 255.0f;
                float g_c = 20.0f + 100.0f * t_n;
                float b_c = 140.0f + 80.0f * t_n;
                float dist = fabsf(sf - fmodf(shm, w + 80.f) + 40.f);
                float shm_i = (std::max)(0.0f, 1.0f - dist / 50.0f);
                shm_i *= shm_i;
                float al = (50.0f + 205.0f * shm_i) * effect;
                al = (std::min)(al, 255.0f);
                float br = 1.0f + 0.7f * shm_i;
                ImU32 col = IM_COL32(
                    (int)(std::min)(r * br, 255.0f),
                    (int)(std::min)(g_c * br, 255.0f),
                    (int)(std::min)(b_c * br, 255.0f),
                    (int)al);
                draw->AddRectFilled(ImVec2(p.x + sf, p.y), ImVec2(p.x + sf + (float)step, p.y + 2.f), col);
            }

            for (int gi = 1; gi <= 4; gi++) {
                float ga = (18.0f * effect) / (float)gi;
                draw->AddLine(ImVec2(p.x + 8.f, p.y - (float)gi),
                    ImVec2(p.x + region.x - 8.f, p.y - (float)gi),
                    IM_COL32(255, 50, 150, (int)ga), 1.0f);
            }
        }


        if (!closed_state) {

            ImU32 ct_tl = IM_COL32(8, 6, 14, 248);
            ImU32 ct_tr = IM_COL32(6, 5, 12, 248);
            ImU32 ct_bl = IM_COL32(4, 3, 8, 252);
            ImU32 ct_br = IM_COL32(3, 2, 6, 252);
            draw->AddRectFilledMultiColor(
                ImVec2(p.x, p.y + header_h), ImVec2(p.x + region.x, box_max.y),
                ct_tl, ct_tr, ct_br, ct_bl);
            draw->AddRectFilled(ImVec2(p.x, p.y + header_h), box_max,
                IM_COL32(0, 0, 0, 0), 10.f, ImDrawFlags_RoundCornersBottom);


            float content_h = box_max.y - (p.y + header_h);
            if (content_h > 4.f) {

                for (int gi = 0; gi < 3; gi++) {
                    float ia = (6.0f * extra_b) / ((float)gi + 1.0f);
                    draw->AddLine(
                        ImVec2(p.x + 1.f + (float)gi, p.y + header_h + 4.f),
                        ImVec2(p.x + 1.f + (float)gi, box_max.y - 4.f),
                        IM_COL32(255, 60, 160, (int)ia), 1.0f);
                }

                for (int gi = 0; gi < 2; gi++) {
                    float ia = (4.0f * extra_b) / ((float)gi + 1.0f);
                    draw->AddLine(
                        ImVec2(p.x + region.x - 2.f - (float)gi, p.y + header_h + 4.f),
                        ImVec2(p.x + region.x - 2.f - (float)gi, box_max.y - 4.f),
                        IM_COL32(255, 255, 255, (int)ia), 1.0f);
                }
            }


            if (extra_b > 0.1f) {
                float bot_y = box_max.y - 1.f;
                int bot_step = 4;
                for (int seg = 0; seg < (int)region.x; seg += bot_step) {
                    float tn = (float)seg / region.x;
                    float inten = sinf(tn * 3.14159f);
                    float ia = 15.0f * extra_b * inten;
                    draw->AddRectFilled(
                        ImVec2(p.x + (float)seg, bot_y),
                        ImVec2(p.x + (float)seg + (float)bot_step, bot_y + 1.f),
                        IM_COL32(255, 80, 180, (int)ia));
                }
            }
        }


        if (!closed_state) {
            ImVec2 content_min = ImVec2(box_min.x, box_min.y + header_h);
            ImVec2 content_max = box_max;

            if (effect > 0.02f) {
                for (int gi = 4; gi > 0; gi--) {
                    float ga = (10.0f + 6.0f * pulse) * effect / (float)gi;
                    ImU32 gc = IM_COL32(255, 30, 150, (int)ga);
                    float g = (float)gi;
                    draw->AddLine(ImVec2(content_min.x - g, content_min.y), ImVec2(content_min.x - g, content_max.y), gc, 1.0f);
                    draw->AddLine(ImVec2(content_max.x + g, content_min.y), ImVec2(content_max.x + g, content_max.y), gc, 1.0f);
                    draw->AddLine(ImVec2(content_min.x, content_max.y + g), ImVec2(content_max.x, content_max.y + g), gc, 1.0f);
                }
            }

            float ba = 20.0f + 40.0f * effect;
            ImU32 bc = IM_COL32(255, 50, 160, (int)ba);
            float rnd = 10.f;

            draw->AddLine(ImVec2(content_min.x, content_min.y), ImVec2(content_min.x, content_max.y - rnd), bc, 1.0f);

            draw->AddLine(ImVec2(content_max.x, content_min.y), ImVec2(content_max.x, content_max.y - rnd), bc, 1.0f);

            draw->AddLine(ImVec2(content_min.x + rnd, content_max.y), ImVec2(content_max.x - rnd, content_max.y), bc, 1.0f);

            draw->PathArcTo(ImVec2(content_min.x + rnd, content_max.y - rnd), rnd, 3.14159f * 0.5f, 3.14159f, 8);
            draw->PathStroke(bc, 0, 1.0f);

            draw->PathArcTo(ImVec2(content_max.x - rnd, content_max.y - rnd), rnd, 0.0f, 3.14159f * 0.5f, 8);
            draw->PathStroke(bc, 0, 1.0f);
        }


        if (effect > 0.01f) {
            draw->AddLine(ImVec2(box_min.x + 14.f, box_min.y + 1.f),
                ImVec2(box_max.x - 14.f, box_min.y + 1.f),
                IM_COL32(255, 255, 255, (int)(10.0f * effect)), 1.0f);
        }


        if (is_active && extra_b > 0.05f && !closed_state) {
            float bw = region.x * 0.4f * extra_b;
            float bx = p.x + (region.x - bw) * 0.5f;
            for (int gi = 1; gi <= 3; gi++) {
                float ga = (15.0f * extra_b) / (float)gi;
                draw->AddLine(ImVec2(bx, box_max.y + (float)gi),
                    ImVec2(bx + bw, box_max.y + (float)gi),
                    IM_COL32(255, 50, 160, (int)ga), 1.0f);
            }
            draw->AddLine(ImVec2(bx, box_max.y), ImVec2(bx + bw, box_max.y),
                IM_COL32(255, 80, 180, (int)(80.0f * extra_b)), 1.5f);
        }


        {
            float ax = p.x + region.x - 24.f;
            float ay = p.y + header_h * 0.5f;
            float sz = 4.5f;
            float rad = helpers::deg2rad(it_data->second.arrow_rot);
            float cr = cosf(rad), sr = sinf(rad);
            ImVec2 pts[3] = {
                ImVec2(-sz, -sz * 0.55f), ImVec2(sz, -sz * 0.55f), ImVec2(0.f, sz * 0.65f)
            };
            for (int i = 0; i < 3; i++) {
                float rx = pts[i].x * cr - pts[i].y * sr;
                float ry = pts[i].x * sr + pts[i].y * cr;
                pts[i] = ImVec2(ax + rx, ay + ry);
            }
            if (effect > 0.01f) {
                draw->AddTriangleFilled(pts[0], pts[1], pts[2],
                    IM_COL32(255, 70, 170, (int)(50.0f * effect)));
            }
            float aa = 90.0f + 165.0f * effect;
            draw->AddTriangleFilled(pts[0], pts[1], pts[2],
                IM_COL32(255, 110, 200, (int)aa));
        }

        global_a = content_fade;


        float slide_t = eased;
        float slide_eased = 1.0f - (1.0f - slide_t) * (1.0f - slide_t) * (1.0f - slide_t);
        global_slide_x = slide_eased;

        if (v != nullptr) {
            ImGui::SetCursorPos(ImVec2(14.f, 4.f));
            checkbox(g.c_str(), v, true);
        } else {
            ImGui::SetCursorPos(ImVec2(16.f, 6.f));
            ImVec2 text_pos = ::GetWindowPos() + ImVec2(16.f, 6.f);
            if (is_active && extra_b > 0.1f) {
                draw->AddText(f::medium14, 16.f, text_pos + ImVec2(0.f, 1.f),
                    IM_COL32(255, 80, 180, (int)(25.0f * extra_b)), str_id);
            }
            ImColor text_col = lerp(c::white, c::white, effect);
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)text_col);
            ImGui::Text("%s", str_id);
            ImGui::PopStyleColor();
        }

        it_data->second.y2 = ImGui::GetCursorPos().y;

        if (!empty && anim_active) {


            float slide_offset_x = -60.0f * (1.0f - slide_eased);
            ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(slide_offset_x, 6.f));

            float content_clip_right = p.x + region.x;
            float content_clip_left = p.x;
            draw->PushClipRect(
                ImVec2(content_clip_left, p.y + header_h),
                ImVec2(content_clip_right, p.y + animated_height + 8.f),
                true
            );
            global_clip_pushed = true;
        } else {
            global_clip_pushed = false;
        }
    }
}

inline void end_child(const char* str_id)
{

    if (global_clip_pushed) {
        auto draw = ::GetWindowDrawList();
        draw->PopClipRect();
        global_clip_pushed = false;
    }


    {
        auto draw = ::GetWindowDrawList();
        draw->PopClipRect();
    }

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.f, 12.f));

    if (it_data != datax.end())
        it_data->second.sz_y = ImGui::GetCursorPos().y - it_data->second.y2;

    ::EndChild();
}

inline bool begincombo(const char* label, const char* preview_value, int all_count, int enabled_count, ImGuiComboFlags flags = 0)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.HasFlags;
    g.NextWindowData.ClearFlags();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview));
    if (flags & ImGuiComboFlags_WidthFitPreview)
        IM_ASSERT((flags & (ImGuiComboFlags_NoPreview | (ImGuiComboFlags)ImGuiComboFlags_CustomPreview)) == 0);

    auto curr = anim::animation(label, anim_t(clamp_out, 0.15f));
    const ImVec2 total_size = ImVec2(310.f, 33.f + 21.f + 4.f);
    const ImVec2 pos = window->DC.CursorPos + ImVec2(14.f + (-50.0f * (1.0f - global_slide_x)), 6.f);
    const ImRect total_bb(pos, pos + total_size);
    const ImRect bb(pos + ImVec2(0.f, 21.f + 4.f), pos + total_size);

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;


    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32(8, 6, 14, 240), 6.f);


    window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x, bb.Min.y + (bb.Max.y - bb.Min.y) * 0.4f),
        IM_COL32(255, 255, 255, 6), 6.f, ImDrawFlags_RoundCornersTop);


    float border_a = popup_open ? 80.f : (hovered ? 60.f : 30.f);
    window->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(255, 50, 160, (int)border_a), 6.f, 0, 1.0f);


    if (popup_open) {
        for (int gi = 2; gi > 0; gi--) {
            window->DrawList->AddRect(bb.Min - ImVec2((float)gi,(float)gi), bb.Max + ImVec2((float)gi,(float)gi),
                IM_COL32(255, 30, 150, (int)(12.f / gi)), 6.f + gi, 0, 1.0f);
        }
    }

    ImVec2 arr_pos = ImVec2(bb.Max.x - 11.f - 9.f, bb.Min.y + 11.f);
    ImColor col2 = lerp(lerp(c::white24, c::white48, global_a), lerp(c::white24, c::white72, global_a), curr->extra());
    rot_start();
    window->DrawList->AddText(f::icons10, 10.f, arr_pos, col2, "e");
    rot_end(180.f * curr->extra());
    curr->active = popup_open;

    ImColor col = lerp(c::white72, c::white, global_a);


    if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
    {
        const ImVec2 preview_pos = ImVec2(bb.Min.x + 8.f, bb.Min.y + 6.f);

        window->DrawList->AddText(f::medium14, 16.f, preview_pos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 40), preview_value);
        window->DrawList->AddText(f::medium14, 16.f, preview_pos, col2, preview_value);
    }

    const ImVec2 label_pos = ImVec2(total_bb.Min.x, total_bb.Min.y);

    window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 40), label);
    window->DrawList->AddText(f::medium14, 16.f, label_pos, col, label);

    std::string counter = std::to_string(enabled_count) + "/" + std::to_string(all_count);
    ImVec2 counter_size = f::medium14->CalcTextSizeA(16.f, FLT_MAX, -1.f, counter.c_str(), 0, NULL);

    window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(total_size.x - counter_size.x + 1.f, 1.f), ImColor(0, 0, 0, 40), counter.c_str());
    window->DrawList->AddText(f::medium14, 16.f, label_pos + ImVec2(total_size.x - counter_size.x, 0.f), col, counter.c_str());

    if (!popup_open)
        return false;

    g.NextWindowData.HasFlags = backup_next_window_data_flags;
    return BeginComboPopup(popup_id, bb, flags);
}

inline bool selectable_(const char* owner, const char* label, bool last, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0, 0))
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    auto curr = anim::animation((std::string(label) + owner).c_str(), anim_t(clamp_out, 0.15f));
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;


    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(297.f, 25.f);
    ImVec2 pos = window->DC.CursorPos;

    ItemSize(size, 0.0f);



    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;




    ImRect bb(min_x, pos.y, min_x + size.x, pos.y + size.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
        const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }


    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const ImGuiItemFlags extra_item_flags = disabled_item ? (ImGuiItemFlags)ImGuiItemFlags_Disabled : ImGuiItemFlags_None;
    bool is_visible;
    if (span_all_columns)
    {

        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }
    else
    {
        is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
    }

    const bool is_multi_select = (g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect) != 0;
    if (!is_visible)
        if (!is_multi_select || !g.BoxSelectState.UnclipMode || !g.BoxSelectState.UnclipRect.Overlaps(bb))
            return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global)
        BeginDisabled();



    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePushBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PushColumnsBackground();
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }


    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
    if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }


    const bool was_selected = selected;
    if (is_multi_select)
    {

        MultiSelectItemHeader(id, &selected, &button_flags);
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);


    if (is_multi_select)
    {
        MultiSelectItemFooter(id, &selected, &pressed);
    }
    else
    {







        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)
                selected = pressed = true;
    }


    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavHighlightItemUnderNav && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb));
            if (g.IO.ConfigNavCursorVisibleAuto)
                g.NavCursorVisible = false;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (selected != was_selected)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;
    curr->active = selected;
    float extra_a = curr->val;
    float extra_b = std::clamp(extra_a * 2.f, 0.f, 1.f);
    float extra_c = std::clamp((extra_b - 0.5f) * 2.f, 0.f, 1.f);

    ImColor col2 = lerp(c::white0, lerp(c::white48, c::secondary, global_a), extra_c);
    ImColor col3 = lerp(c::white0, lerp(c::white24, c::primary, global_a), extra_b);
    ImColor col4 = lerp(lerp(c::white24, c::white48, global_a), lerp(ImColor(20, 20, 20, 255), c::bg, global_a), extra_a);


    if (is_visible)
    {

        if (extra_a > 0.01f) {

            ImU32 sel_left = IM_COL32(255, 20, 147, (int)(180 * extra_b));
            ImU32 sel_right = IM_COL32(199, 21, 133, (int)(120 * extra_b));
            int vs = window->DrawList->VtxBuffer.Size;
            window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32_WHITE, last ? 4.f : 0, last ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_None);
            int ve = window->DrawList->VtxBuffer.Size;
            ShadeVertsLinearGradX(window->DrawList, vs, ve, bb.Min, bb.Max, sel_left, sel_right);
        } else if (hovered) {

            window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32(255, 255, 255, 8), last ? 4.f : 0, last ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_None);
        }


        if (extra_c > 0.f) {
            float bar_h = (bb.Max.y - bb.Min.y) * 0.6f * extra_c;
            float bar_y = bb.Min.y + ((bb.Max.y - bb.Min.y) - bar_h) * 0.5f;
            window->DrawList->AddRectFilled(
                ImVec2(bb.Min.x + 2.f, bar_y),
                ImVec2(bb.Min.x + 4.f, bar_y + bar_h),
                IM_COL32(255, 255, 255, (int)(220 * extra_c)), 2.f);
        }


        if (!last) {
            window->DrawList->AddLine(
                ImVec2(bb.Min.x + 8.f, bb.Max.y),
                ImVec2(bb.Max.x - 8.f, bb.Max.y),
                IM_COL32(255, 255, 255, 8), 1.0f);
        }
    }

    if (span_all_columns)
    {
        if (g.CurrentTable)
            TablePopBackgroundChannel();
        else if (window->DC.CurrentColumns)
            PopColumnsBackground();
    }


    if (is_visible)
    {

        ImU32 text_col = selected
            ? IM_COL32(255, 255, 255, 255)
            : (hovered ? IM_COL32(240, 240, 245, 255) : IM_COL32(200, 200, 210, 255));
        const ImVec2 label_pos = ImVec2(bb.Min.x + 10.f + 6.f * extra_c, bb.Min.y + 5.f);
        window->DrawList->AddText(f::medium14, 14.f, label_pos + ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, 60), label);
        window->DrawList->AddText(f::medium14, 14.f, label_pos, text_col, label);
    }

    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_NoAutoClosePopups) && (g.LastItemData.ItemFlags & ImGuiItemFlags_AutoClosePopups))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();




    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

inline bool selectable(const char* owner, const char* label, bool last, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0, 0))
{
    if (selectable_(owner, label, last, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

inline void MultiCombo(const char* label, bool combos[], const char* items[], int items_count, bool* parent_enabled = nullptr)
{

    bool should_show = (parent_enabled == nullptr) || (*parent_enabled);
    if (!should_show)
        return;

    std::vector<std::string> vec;
    static std::string preview;
    int j = 0;
    for (int i = 0; i < items_count; i++)
    {
        if (combos[i])
        {
            vec.push_back(items[i]);
            if (j > 2)
                preview = vec.at(0) + ", " + vec.at(1) + ", " + vec.at(2) + ", ...";
            else if (j)
                preview += ", " + (std::string)items[i];
            else
                preview = items[i];

            j++;
        }
    }
    if (begincombo(label, preview.c_str(), items_count, j))
    {
        ImGuiWindow* window = GetCurrentWindow();
        const auto& p = ::GetWindowPos();
        auto curr = anim::get_anim_obj(CONST_HASH(label));
        ImVec2 region = ::GetContentRegionMax();
        region.y *= curr->val;

        window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), true);
        window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
        window->DrawList->PopClipRect();


        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
        window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
        for (int i = 0; i < items_count; i++) {
            selectable(label, items[i], items_count == (i + 1), &combos[i], ImGuiSelectableFlags_DontClosePopups);
        }
        window->DrawList->PopClipRect();


        window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
        window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
        window->DrawList->PopClipRect();


        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
        EndCombo();
    }

    preview = "None";
}



inline bool neon_button(const char* label, const ImVec2& size_arg = ImVec2(0, 0))
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);


    static std::map<ImGuiID, float> button_anims;
    static std::map<ImGuiID, float> button_press_anims;
    if (button_anims.find(id) == button_anims.end()) button_anims[id] = 0.0f;
    if (button_press_anims.find(id) == button_press_anims.end()) button_press_anims[id] = 0.0f;

    float dt = g.IO.DeltaTime;
    float& anim = button_anims[id];
    float& pressAnim = button_press_anims[id];
    float target = (hovered || held) ? 1.0f : 0.0f;
    anim += (target - anim) * dt * 10.0f;
    float pressTarget = held ? 1.0f : 0.0f;
    pressAnim += (pressTarget - pressAnim) * dt * 16.0f;

    ImDrawList* dl = window->DrawList;
    float rounding = 5.0f;


    if (anim > 0.01f) {
        for (int i = 4; i >= 1; i--) {
            float expand = (float)i * 2.5f * anim;
            int alpha = (int)((22 - i * 4) * anim);
            dl->AddRect(
                bb.Min - ImVec2(expand, expand),
                bb.Max + ImVec2(expand, expand),
                IM_COL32(255, 20, 147, alpha),
                rounding + expand, 0, 1.0f);
        }
    }


    ImU32 bgTL = held ? IM_COL32(80, 10, 50, 240) :
                 IM_COL32((int)(12 + 30 * anim), (int)(8 + 5 * anim), (int)(18 + 20 * anim), 245);
    ImU32 bgBR = held ? IM_COL32(50, 5, 35, 240) :
                 IM_COL32((int)(8 + 18 * anim), (int)(5 + 2 * anim), (int)(14 + 12 * anim), 245);
    dl->AddRectFilledMultiColor(bb.Min, bb.Max, bgTL, bgTL, bgBR, bgBR);


    float borderAlpha = 0.3f + anim * 0.5f + pressAnim * 0.2f;
    dl->AddRect(bb.Min, bb.Max,
        IM_COL32(255, 20, 147, (int)(borderAlpha * 255)), rounding, 0, 1.2f + anim * 0.3f);


    if (anim > 0.01f) {
        ImVec2 hlMin = ImVec2(bb.Min.x + 2, bb.Min.y + 1);
        ImVec2 hlMax = ImVec2(bb.Max.x - 2, bb.Min.y + 3.0f);
        dl->AddRectFilledMultiColor(hlMin, hlMax,
            IM_COL32(255, 180, 220, (int)(50 * anim)),
            IM_COL32(255, 180, 220, (int)(50 * anim)),
            IM_COL32(255, 180, 220, 0),
            IM_COL32(255, 180, 220, 0));
    }


    if (anim > 0.01f) {
        ImVec2 brMin = ImVec2(bb.Min.x + 4, bb.Max.y - 2.5f);
        ImVec2 brMax = ImVec2(bb.Max.x - 4, bb.Max.y - 0.5f);
        dl->AddRectFilledMultiColor(brMin, brMax,
            IM_COL32(255, 20, 147, 0),
            IM_COL32(255, 20, 147, 0),
            IM_COL32(255, 20, 147, (int)(25 * anim)),
            IM_COL32(255, 20, 147, (int)(25 * anim)));
    }


    if (anim > 0.05f) {
        dl->AddRectFilled(
            ImVec2(bb.Min.x + 1, bb.Min.y + 4),
            ImVec2(bb.Min.x + 2.5f, bb.Max.y - 4),
            IM_COL32(255, 20, 147, (int)(120 * anim)), 1.0f);
    }


    ImVec2 text_pos = bb.Min + ImVec2((size.x - label_size.x) * 0.5f, (size.y - label_size.y) * 0.5f);

    if (pressAnim > 0.01f) text_pos.y += 1.0f * pressAnim;
    dl->AddText(text_pos + ImVec2(1, 1), IM_COL32(0, 0, 0, 140), label);

    if (anim > 0.1f) {
        dl->AddText(text_pos, IM_COL32(255, 180, 220, (int)(60 * anim)), label);
    }
    dl->AddText(text_pos, IM_COL32(255, 255, 255, 255), label);

    return pressed;
}
