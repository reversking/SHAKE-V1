#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <imgui.h>
#include "Brand.hpp"
REVERS_STAMP(Print_hpp)

namespace hitlog
{

    enum class MessageType
    {
        Normal,
        Warning,
        Error,
        Hit,
        Info
    };

    struct Message
    {
        std::string text;
        float time;
        ImVec4 color;
        MessageType type;
    };

    inline std::vector<Message> message_list;
    inline std::mutex message_mutex;

    template<MessageType T>
    void printa(const std::string& text)
    {
        std::lock_guard<std::mutex> lock(message_mutex);

        Message msg;
        msg.text = text;
        msg.time = 5.0f;
        msg.type = T;

        switch (T)
        {
        case MessageType::Normal:
            msg.color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case MessageType::Warning:
            msg.color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case MessageType::Error:
            msg.color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case MessageType::Hit:
            msg.color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case MessageType::Info:
            msg.color = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
            break;
        }

        message_list.push_back(msg);

        if (message_list.size() > 10)
        {
            message_list.erase(message_list.begin());
        }
    }

    template void printa<MessageType::Normal>(const std::string& text);
    template void printa<MessageType::Warning>(const std::string& text);
    template void printa<MessageType::Error>(const std::string& text);
    template void printa<MessageType::Hit>(const std::string& text);
    template void printa<MessageType::Info>(const std::string& text);

    constexpr auto normal = MessageType::Normal;
    constexpr auto warning = MessageType::Warning;
    constexpr auto error = MessageType::Error;
    constexpr auto hit = MessageType::Hit;
    constexpr auto info = MessageType::Info;

    void RenderMessages(ImDrawList* drawList, const ImVec2& position)
    {
        std::lock_guard<std::mutex> lock(message_mutex);

        float y_offset = 0.0f;
        float padding = 12.0f;
        float icon_size = 20.0f;
        float min_width = 300.0f;
        float max_width = 500.0f;

        for (auto it = message_list.begin(); it != message_list.end();)
        {
            it->time -= ImGui::GetIO().DeltaTime;

            if (it->time <= 0.0f)
            {
                it = message_list.erase(it);
            }
            else
            {

                float alpha = 1.0f;
                if (it->time < 0.5f) {
                    alpha = it->time * 2.0f;
                }
                else if (it->time > 4.5f) {
                    alpha = (5.0f - it->time) * 2.0f;
                }

                ImVec2 text_size = ImGui::CalcTextSize(it->text.c_str());
                float box_height = text_size.y + padding * 2;

                float text_width = text_size.x + 60.0f;
                float box_width = ImClamp(text_width, min_width, max_width);

                float slide_progress = 1.0f;
                if (it->time > 4.5f) {
                    slide_progress = (5.0f - it->time) * 2.0f;
                }
                float animation_offset = (1.0f - slide_progress) * 100.0f;

                ImVec2 box_pos = ImVec2(position.x - animation_offset, position.y + y_offset);
                ImVec2 box_end = ImVec2(position.x + box_width - animation_offset, position.y + y_offset + box_height);

                ImU32 bg_color_top = IM_COL32(25, 25, 30, (int)(alpha * 240));
                ImU32 bg_color_bottom = IM_COL32(20, 20, 25, (int)(alpha * 240));

                drawList->AddRectFilledMultiColor(
                    box_pos,
                    box_end,
                    bg_color_top,
                    bg_color_top,
                    bg_color_bottom,
                    bg_color_bottom
                );

                drawList->AddRect(box_pos, box_end, IM_COL32(0, 0, 0, 0), 8.0f);

                ImU32 accent_color;
                const char* icon;
                ImU32 glow_color;

                switch (it->type)
                {
                case MessageType::Normal:
                    accent_color = IM_COL32(130, 130, 255, (int)(alpha * 255));
                    icon = "i";
                    glow_color = IM_COL32(130, 130, 255, (int)(alpha * 50));
                    break;
                case MessageType::Warning:
                    accent_color = IM_COL32(255, 200, 0, (int)(alpha * 255));
                    icon = "!";
                    glow_color = IM_COL32(255, 200, 0, (int)(alpha * 50));
                    break;
                case MessageType::Error:
                    accent_color = IM_COL32(255, 60, 60, (int)(alpha * 255));
                    icon = "X";
                    glow_color = IM_COL32(255, 60, 60, (int)(alpha * 50));
                    break;
                case MessageType::Hit:
                    accent_color = IM_COL32(0, 255, 128, (int)(alpha * 255));
                    icon = "✓";
                    glow_color = IM_COL32(0, 255, 128, (int)(alpha * 50));
                    break;
                case MessageType::Info:
                    accent_color = IM_COL32(0, 200, 255, (int)(alpha * 255));
                    icon = "ℹ";
                    glow_color = IM_COL32(0, 200, 255, (int)(alpha * 50));
                    break;
                default:
                    accent_color = IM_COL32(130, 130, 255, (int)(alpha * 255));
                    icon = "i";
                    glow_color = IM_COL32(130, 130, 255, (int)(alpha * 50));
                    break;
                }

                for (int i = 3; i > 0; i--) {
                    drawList->AddRect(
                        ImVec2(box_pos.x - i, box_pos.y - i),
                        ImVec2(box_end.x + i, box_end.y + i),
                        IM_COL32(
                            (glow_color >> 16) & 0xFF,
                            (glow_color >> 8) & 0xFF,
                            glow_color & 0xFF,
                            (int)(alpha * 20 / i)
                        ),
                        8.0f + i
                    );
                }

                drawList->AddRectFilled(
                    box_pos,
                    ImVec2(box_pos.x + 4.0f, box_end.y),
                    accent_color,
                    8.0f,
                    ImDrawFlags_RoundCornersLeft
                );

                ImVec2 icon_center = ImVec2(box_pos.x + 25.0f, box_pos.y + box_height / 2.0f);
                drawList->AddCircleFilled(icon_center, icon_size / 2.0f, IM_COL32(40, 40, 45, (int)(alpha * 255)));
                drawList->AddCircle(icon_center, icon_size / 2.0f, accent_color, 0, 2.0f);

                ImVec2 icon_text_size = ImGui::CalcTextSize(icon);
                drawList->AddText(
                    ImVec2(icon_center.x - icon_text_size.x / 2.0f, icon_center.y - icon_text_size.y / 2.0f),
                    accent_color,
                    icon
                );

                drawList->AddText(
                    ImVec2(box_pos.x + 50.0f, box_pos.y + (box_height - text_size.y) / 2.0f),
                    IM_COL32(255, 255, 255, (int)(alpha * 255)),
                    it->text.c_str()
                );

                drawList->AddRect(
                    box_pos,
                    box_end,
                    IM_COL32(60, 60, 65, (int)(alpha * 255)),
                    8.0f,
                    0,
                    1.0f
                );

                y_offset += box_height + 8.0f;
                ++it;
            }
        }
    }
}

using namespace hitlog;