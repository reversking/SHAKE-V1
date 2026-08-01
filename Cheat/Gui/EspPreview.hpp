#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include <array>
#include <string>
#include "../Cheat/Brand.hpp"
REVERS_STAMP(EspPreview_hpp)
#include "menu/defs/textures.h"


namespace Cheats {
    namespace Esp {
        extern bool skeletonEsp;
        extern float skeletonEspColor[3];
        extern int skeletonEspSize;
        extern bool nameEsp;
        extern bool idEsp;
        extern bool weaponEsp;
        extern bool distanceEsp;
    }
}

static int esp_scale = 19;

class c_esp_drag {
public:
    class Box_t {
    public:
        int x, y, w, h;
    };

    struct Position {
        ImVec2 pos;
    };

    class c_drag_item {
    public:
        int pos;
        int type;
        ImColor col;
        std::string text;
        std::string name;
        bool small_text = false;
        ImVec2 pos_;
        ImVec2 size;
        bool hovered = false;
        int helding = 0;
        float move_animation = 1.0f;
        float animations[6] = {0, 0, 1, 0, 0, 0};
        bool enabled = true;
        int think_pos = -1;
        bool enable_popup = false;
        int font = 0;
        int last_valid_pos = -1;
    };

    std::array<c_drag_item, 6> m_items = {
        c_drag_item{1, 1, ImColor(0, 255, 12), "Health bar", "Health bar"},
        c_drag_item{0, 1, ImColor(180, 100, 255), "Armor bar" , "Armor bar"},
        c_drag_item{2, 0, ImColor(255,255,255), "Player", "Nickname"},
        c_drag_item{3, 0, ImColor(255,255,255), "Pistol", "Weapon"},
        c_drag_item{2, 0, ImColor(255,255,255), "ID", "ID"},
        c_drag_item{3, 0, ImColor(255,255,255), "25m", "Distance"}};
    int m_offsets[8] = {0};
    Box_t box = { 25, 20, 180, 380 };


    ImVec2 m_preview_silhouette_min{};
    ImVec2 m_preview_silhouette_max{};
    bool m_preview_silhouette_valid = false;

    int find_closest_position(ImVec2 curr, Position positions[], int count) {
        float closest = FLT_MAX;
        int best = -1;
        for (int i = 0; i < count; i++) {
            auto pos = positions[i].pos;
            ImVec2 diff = ImVec2(pos.x - curr.x, pos.y - curr.y);
            float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);

            if (closest > dist) {
                closest = dist;
                best = i;
            }
        }

        return best;
    }

    void set_positions() {
        float barGap = 3.f;
        Position PositionsBar[] = {
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap - 4.f, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
        };

        Position PositionsText[] = {
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w / 2, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w / 2, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
        };

        auto mouse_in_region = [&](ImVec2 pos, ImVec2 size) -> bool {
            auto m_pos = ImGui::GetMousePos();
            if (m_pos.x >= pos.x && m_pos.y >= pos.y &&
                m_pos.x <= pos.x + size.x && m_pos.y <= pos.y + size.y)
                return true;
            return false;
        };

        for (int i = 0; i < m_items.size(); i++) {
            auto& item = m_items[i];
            if (!item.enabled)
                continue;
            ImGui::PushID(i);
            ImGui::SetCursorScreenPos(item.pos_);
            ImGui::PushStyleVar(0, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
            ImGui::Button(("##drag_esp" + item.text).c_str(), item.size);
            bool hovered = ImGui::IsItemHovered();
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();

            int pos;
            if (i == 0 || i == 1) {
                pos = find_closest_position(ImGui::GetMousePos(), PositionsBar, 4);
            } else {
                pos = find_closest_position(ImGui::GetMousePos(), PositionsText, 8);
            }
            item.hovered = false;

            if (item.pos >= 0 && item.pos < 4 && (i == 0 || i == 1)) {
                if (item.last_valid_pos == -1) {
                    item.last_valid_pos = item.pos;
                }
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
                ImGui::SetDragDropPayload("##drag_esp", &i, sizeof(int), 0);
                for (int t = 0; t < m_items.size(); t++)
                    m_items[t].move_animation = ImGui::GetIO().DeltaTime * 34.f;


                if (item.think_pos == -1) {
                    item.last_valid_pos = item.pos;
                }

                item.think_pos = pos;
                item.helding = pos > 1;
                item.hovered = true;
                ImGui::EndDragDropSource();
            }
            else {

                if (item.think_pos >= 0) {

                    if (item.type == 1) {
                        if (item.think_pos >= 0 && item.think_pos < 4) {
                            item.pos = item.think_pos;
                            item.move_animation = 0.f;
                        } else if (item.last_valid_pos >= 0 && item.last_valid_pos < 4) {
                            item.pos = item.last_valid_pos;
                        }
                    } else {
                        if (item.think_pos >= 0 && item.think_pos < 8) {
                            item.pos = item.think_pos;
                            item.move_animation = 0.f;
                        } else if (item.last_valid_pos >= 0 && item.last_valid_pos < 8) {
                            item.pos = item.last_valid_pos;
                        }
                    }
                }

                item.think_pos = -1;
                item.last_valid_pos = -1;
                item.hovered = false;
            }
            item.animations[0] = ImLerp(item.animations[0], item.hovered || hovered ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 34.f);
            ImGui::GetWindowDrawList()->AddRect(item.pos_ - ImVec2(1, 1), item.pos_ + item.size + ImVec2(1, 1), ImColor(255, 255, 255, int(255 * item.animations[0])));
            ImGui::PopID();
        }
    }

    void on_draw() {




        ImVec2 wp = ImGui::GetWindowPos();
        ImVec2 ws = ImGui::GetWindowSize();
        ImGui::GetWindowDrawList()->PushClipRect(
            ImVec2(wp.x - 5.f, wp.y - 5.f),
            ImVec2(wp.x + ws.x + 5.f, wp.y + ws.y + 5.f),
            false);

        float barThick = 4.f;
        float barGap = 3.f;

        Position Positions[] = {
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap - barThick, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w / 2, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w / 2, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x + box.w + barGap, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap, ImGui::GetWindowPos().y + box.y - barGap)},
            {ImVec2(ImGui::GetWindowPos().x + box.x - barGap, ImGui::GetWindowPos().y + box.y + box.h + barGap)},
        };

        ImVec2 Sizes[] = {
            ImVec2(barThick, (float)box.h),
            ImVec2(barThick, (float)box.h),
            ImVec2((float)box.w, barThick),
            ImVec2((float)box.w, barThick),
            ImVec2(barThick, barThick),
            ImVec2(barThick, barThick),
            ImVec2(barThick, barThick),
            ImVec2(barThick, barThick)
        };

        {

            ImVec2 wp = ImGui::GetWindowPos();
            float bodyW = 180.f;
            float bodyH = 380.f;
            float startX = (ImGui::GetContentRegionAvail().x - bodyW) * 0.5f;
            ImGui::SetCursorPos(ImVec2(startX, 55.f));
            ImGui::InvisibleButton("##esp_preview_sil", ImVec2(bodyW, bodyH));
            ImVec2 boxMin = ImGui::GetItemRectMin();
            ImVec2 boxMax = ImGui::GetItemRectMax();
            box.x = (int)(boxMin.x - wp.x);
            box.y = (int)(boxMin.y - wp.y);
            box.w = (int)(boxMax.x - boxMin.x);
            box.h = (int)(boxMax.y - boxMin.y);
            m_preview_silhouette_min = boxMin;
            m_preview_silhouette_max = boxMax;
            m_preview_silhouette_valid = (box.w > 0 && box.h > 0);


            if (tex::silent_esp_preview && m_preview_silhouette_valid) {
                float alpha = ImGui::GetStyle().Alpha;
                ImGui::GetWindowDrawList()->AddImage(
                    (ImTextureID)tex::silent_esp_preview, boxMin, boxMax,
                    ImVec2(0, 0), ImVec2(1, 1),
                    IM_COL32(255, 255, 255, (int)(255 * alpha)));
            }
        }

        float offsetY = 0.f;

        for (auto& item : m_items) {
            item.animations[2] = ImLerp(item.animations[2], item.enabled ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 34.f);
            if (item.animations[2] < 0.1f) {
                continue;
            }

            item.move_animation += ImGui::GetIO().DeltaTime * 34.f;
            item.move_animation = ImClamp(item.move_animation, 0.f, 1.f);


            if (item.hovered || item.think_pos >= 0) {
                auto size = ImGui::CalcTextSize(item.text.c_str());
                static bool s = true;
                if (s) {
                    item.size = size;
                    s = false;
                }
                if (item.small_text)
                    size.y = 12;

                if (item.think_pos >= 0) {
                    if (item.type == 0) {
                        if (item.think_pos >= 0 && item.think_pos < 8) {
                            m_offsets[item.think_pos] += 2.f + size.y + offsetY;
                        }
                    } else {
                        if (item.think_pos >= 0 && item.think_pos < 4) {
                            m_offsets[item.think_pos] += 5.f;
                        }
                    }
                }

                offsetY += size.y;
            }

            if (item.type == 0) {
                auto size = ImGui::CalcTextSize(item.text.c_str());
                item.size = size;
                if (item.small_text)
                    size.y = 12;


                if (item.think_pos >= 0) {
                    item.pos_ = ImLerp(item.pos_, ImGui::GetMousePos() + ImVec2(-size.x / 2.f, -size.y / 2.f), 0.5f);
                    float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                    ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                    ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    continue;
                }

                switch (item.pos) {
                case 0:
                    item.pos_ = ImLerp(item.pos_, Positions[0].pos + ImVec2(-size.x, m_offsets[0]), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[0] += 2.f + size.y;
                    break;
                case 1:
                    item.pos_ = ImLerp(item.pos_, Positions[1].pos + ImVec2(0, m_offsets[1]), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[1] += 2.f + size.y;
                    break;
                case 2:
                    item.pos_ = ImLerp(item.pos_, Positions[2].pos + ImVec2(-size.x / 2.f, -m_offsets[2] - size.y), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[2] += 2.f + size.y;
                    break;
                case 3:
                    item.pos_ = ImLerp(item.pos_, Positions[3].pos + ImVec2(-size.x / 2.f, m_offsets[3]), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[3] += 2.f + size.y;
                    break;
                case 4:
                    item.pos_ = ImLerp(item.pos_, Positions[4].pos + ImVec2(0, -m_offsets[4] - size.y), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[4] += 2.f + size.y;
                    break;
                case 5:
                    item.pos_ = ImLerp(item.pos_, Positions[5].pos + ImVec2(0, m_offsets[5]), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[5] += 2.f + size.y;
                    break;
                case 6:
                    item.pos_ = ImLerp(item.pos_, Positions[6].pos + ImVec2(-size.x, -m_offsets[6] - size.y), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[6] += 2.f + size.y;
                    break;
                case 7:
                    item.pos_ = ImLerp(item.pos_, Positions[7].pos + ImVec2(-size.x, m_offsets[7]), item.move_animation);
                    {
                        float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                        ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(255 * alpha));
                        ImU32 textCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ - ImVec2(0, 1), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_ + ImVec2(1, 0), shadowCol, item.text.c_str());
                        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), item.pos_, textCol, item.text.c_str());
                    }
                    m_offsets[7] += 2.f + size.y;
                    break;
                }

                continue;
            }


            if (item.type == 1) {

                if (item.think_pos >= 0) {
                    ImVec2 barSize = (item.think_pos == 2 || item.think_pos == 3) ? Sizes[2] : Sizes[0];
                    item.pos_ = ImLerp(item.pos_, ImGui::GetMousePos() + ImVec2(-barSize.x / 2.f, -barSize.y / 2.f), 0.5f);
                    float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                    ImU32 barCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));
                    ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + barSize, barCol);
                } else if (item.pos >= 0 && item.pos < 4) {
                    item.size = Sizes[item.pos];
                    float alpha = item.animations[2] * ImGui::GetStyle().Alpha;
                    ImU32 barCol = IM_COL32((int)(item.col.Value.x * 255), (int)(item.col.Value.y * 255), (int)(item.col.Value.z * 255), (int)(255 * alpha));

                    switch (item.pos) {
                    case 0:
                        item.pos_ = ImLerp(item.pos_, Positions[0].pos + ImVec2(-m_offsets[0], 0.f), item.move_animation);
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[0], barCol);
                        m_offsets[0] += barThick + 2.f;
                        break;
                    case 1:
                        item.pos_ = ImLerp(item.pos_, Positions[1].pos + ImVec2(m_offsets[1], 0.f), item.move_animation);
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[1], barCol);
                        m_offsets[1] += barThick + 2.f;
                        break;
                    case 2:
                        item.pos_ = ImLerp(item.pos_, Positions[2].pos + ImVec2(-box.w / 2.f, -m_offsets[2] - barThick), item.move_animation);
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[2], barCol);
                        m_offsets[2] += barThick + 2.f;
                        break;
                    case 3:
                        item.pos_ = ImLerp(item.pos_, Positions[3].pos + ImVec2(-box.w / 2.f, m_offsets[3]), item.move_animation);
                        ImGui::GetWindowDrawList()->AddRectFilled(item.pos_, item.pos_ + Sizes[3], barCol);
                        m_offsets[3] += barThick + 2.f;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < 8; i++)
            m_offsets[i] = 0.f;


        ImGui::GetWindowDrawList()->PopClipRect();
    }

    int getHealthBarPos() {
        return m_items[0].pos;
    }
    int getArmorBarPos() {
        return m_items[1].pos;
    }
    int getNameEspPos() { return m_items[2].pos; }
    int getWeaponEspPos() { return m_items[3].pos; }
    int getIdEspPos() { return m_items[4].pos; }
    int getDistanceEspPos() { return m_items[5].pos; }


    void draw_skeleton(ImColor color, float thickness = 2.0f) {
        if (!m_preview_silhouette_valid)
            return;

        ImVec2 boxTopLeft = m_preview_silhouette_min;
        const float bw = m_preview_silhouette_max.x - m_preview_silhouette_min.x;
        const float bh = m_preview_silhouette_max.y - m_preview_silhouette_min.y;
        if (bw < 2.f || bh < 2.f)
            return;

        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        const float clipPad = ImMax(8.f, thickness * 3.f);
        drawList->PushClipRect(
            ImVec2(boxTopLeft.x - clipPad, boxTopLeft.y - clipPad),
            ImVec2(boxTopLeft.x + bw + clipPad, boxTopLeft.y + bh + clipPad),
            true);

        float alpha = ImGui::GetStyle().Alpha;

        auto bonePos = [&](int boneId) -> ImVec2 {
            switch (boneId) {
            case 0:  return ImVec2(boxTopLeft.x + 0.48f * bw, boxTopLeft.y + 0.055f * bh);
            case 2:  return ImVec2(boxTopLeft.x + 0.48f * bw, boxTopLeft.y + 0.16f * bh);
            case 7:  return ImVec2(boxTopLeft.x + 0.26f * bw, boxTopLeft.y + 0.22f * bh);
            case 8:  return ImVec2(boxTopLeft.x + 0.72f * bw, boxTopLeft.y + 0.22f * bh);
            case 1:  return ImVec2(boxTopLeft.x + 0.48f * bw, boxTopLeft.y + 0.38f * bh);
            case 3:  return ImVec2(boxTopLeft.x + 0.10f * bw, boxTopLeft.y + 0.49f * bh);
            case 4:  return ImVec2(boxTopLeft.x + 0.87f * bw, boxTopLeft.y + 0.49f * bh);
            case 9:  return ImVec2(boxTopLeft.x + 0.38f * bw, boxTopLeft.y + 0.68f * bh);
            case 10: return ImVec2(boxTopLeft.x + 0.60f * bw, boxTopLeft.y + 0.68f * bh);
            case 5:  return ImVec2(boxTopLeft.x + 0.38f * bw, boxTopLeft.y + 0.92f * bh);
            case 6:  return ImVec2(boxTopLeft.x + 0.60f * bw, boxTopLeft.y + 0.92f * bh);
            default: return ImVec2(boxTopLeft.x + 0.48f * bw, boxTopLeft.y + 0.38f * bh);
            }
        };

        ImU32 skeletonColor = IM_COL32(
            (int)(color.Value.x * 255),
            (int)(color.Value.y * 255),
            (int)(color.Value.z * 255),
            (int)(255 * alpha));

        drawList->AddLine(bonePos(0), bonePos(2), skeletonColor, thickness);
        drawList->AddLine(bonePos(2), bonePos(7), skeletonColor, thickness);
        drawList->AddLine(bonePos(2), bonePos(8), skeletonColor, thickness);
        drawList->AddLine(bonePos(7), bonePos(3), skeletonColor, thickness);
        drawList->AddLine(bonePos(8), bonePos(4), skeletonColor, thickness);
        drawList->AddLine(bonePos(2), bonePos(1), skeletonColor, thickness);
        drawList->AddLine(bonePos(1), bonePos(9), skeletonColor, thickness);
        drawList->AddLine(bonePos(9), bonePos(5), skeletonColor, thickness);
        drawList->AddLine(bonePos(1), bonePos(10), skeletonColor, thickness);
        drawList->AddLine(bonePos(10), bonePos(6), skeletonColor, thickness);


        int boneIds[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        float dotR = 6.0f;
        for (int b : boneIds) {
            ImVec2 bp = bonePos(b);

            drawList->AddCircleFilled(bp, dotR + 8.0f, IM_COL32(255, 20, 147, (int)(20 * alpha)), 24);
            drawList->AddCircleFilled(bp, dotR + 5.0f, IM_COL32(255, 20, 147, (int)(45 * alpha)), 24);

            drawList->AddCircleFilled(bp, dotR + 2.0f, IM_COL32(255, 40, 160, (int)(180 * alpha)), 20);
            drawList->AddCircleFilled(bp, dotR,         IM_COL32(255, 80, 180, (int)(220 * alpha)), 20);

            drawList->AddCircleFilled(bp, dotR * 0.55f, IM_COL32(15, 10, 20, (int)(230 * alpha)), 16);

            drawList->AddCircleFilled(bp, dotR * 0.2f,  IM_COL32(255, 200, 230, (int)(255 * alpha)), 10);

            drawList->AddCircle(bp, dotR + 1.0f, IM_COL32(255, 100, 180, (int)(100 * alpha)), 20, 1.2f);
        }

        drawList->PopClipRect();
    }
};

inline c_esp_drag m_esp_drag;

