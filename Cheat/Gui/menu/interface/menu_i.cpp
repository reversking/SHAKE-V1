#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <d3d11.h>
#include "../../../Cheat/TextureLoader.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <map>

#include "menu_i.h"
#include "elements_manager.h"
#include "../helpers/anim/anim.h"
#include "../helpers/restore.h"
#include "../helpers/render.h"
#include "../helpers/widgets.h"
#include "../defs/fonts.h"
#include "../defs/textures.h"
#include "../defs/colors.h"
#include "../bytes/bg.h"
#include "../bytes/loadbg.h"
#include "../bytes/medium.h"
#include "../bytes/semibold.h"
#include "../bytes/caviar.h"
#include "../bytes/logo.h"
#include "../bytes/icon.h"
#include "../bytes/shakepriv_logo.h"
#include "../bytes/tap_logo.h"
#include "../../../../Libraries/imgui/include/ImGui/TransparentLogo.hpp"
#include "../../EspPreview.hpp"
#include "../../../../Libraries/silent_esp_texture.h"
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(menu_i_cpp)

namespace Cheats {
    namespace Misc {
        extern bool christmasEnabled;
    }
}

struct Snowflake {
    float x, y;
    float speed;
    float size;
    float wobble;
    float wobbleSpeed;
    float opacity;
    int type;
    float rotation;
    float rotSpeed;
};

class ChristmasEffects {
public:
    static constexpr int MAX_SNOWFLAKES = 200;
    Snowflake snowflakes[MAX_SNOWFLAKES];
    bool initialized = false;
    float lightPhase = 0.f;
    float screenWidth = 1920.f;
    float screenHeight = 1080.f;

    void init() {
        if (initialized) return;
        srand((unsigned int)time(nullptr));

        ImGuiIO& io = ImGui::GetIO();
        screenWidth = io.DisplaySize.x;
        screenHeight = io.DisplaySize.y;

        for (int i = 0; i < MAX_SNOWFLAKES; i++) {
            snowflakes[i].x = (float)(rand() % (int)screenWidth);
            snowflakes[i].y = (float)(rand() % (int)screenHeight) - screenHeight;
            snowflakes[i].speed = 1.2f + (rand() % 100) / 50.f;
            snowflakes[i].size = 4.f + (rand() % 100) / 12.f;
            snowflakes[i].wobble = (float)(rand() % 628) / 100.f;
            snowflakes[i].wobbleSpeed = 0.015f + (rand() % 100) / 3000.f;
            snowflakes[i].opacity = 0.5f + (rand() % 100) / 200.f;
            snowflakes[i].type = rand() % 3;
            snowflakes[i].rotation = (float)(rand() % 628) / 100.f;
            snowflakes[i].rotSpeed = 0.005f + (rand() % 100) / 5000.f;
        }
        initialized = true;
    }

    void drawSnowflake(ImDrawList* draw, ImVec2 pos, float size, float rotation, float opacity) {
        ImU32 col = IM_COL32(255, 255, 255, (int)(opacity * 255));
        ImU32 colInner = IM_COL32(200, 230, 255, (int)(opacity * 200));

        for (int i = 0; i < 6; i++) {
            float angle = rotation + i * 1.0472f;
            float cosA = cosf(angle);
            float sinA = sinf(angle);

            ImVec2 end = ImVec2(pos.x + cosA * size, pos.y + sinA * size);
            draw->AddLine(pos, end, col, 1.5f);

            float branchLen = size * 0.4f;
            float branchPos = size * 0.6f;
            ImVec2 branchStart = ImVec2(pos.x + cosA * branchPos, pos.y + sinA * branchPos);

            float branchAngle1 = angle + 0.5f;
            float branchAngle2 = angle - 0.5f;

            draw->AddLine(branchStart, ImVec2(branchStart.x + cosf(branchAngle1) * branchLen, branchStart.y + sinf(branchAngle1) * branchLen), colInner, 1.f);
            draw->AddLine(branchStart, ImVec2(branchStart.x + cosf(branchAngle2) * branchLen, branchStart.y + sinf(branchAngle2) * branchLen), colInner, 1.f);
        }

        draw->AddCircleFilled(pos, size * 0.15f, col);
    }

    void drawSimpleSnowflake(ImDrawList* draw, ImVec2 pos, float size, float rotation, float opacity) {
        ImU32 col = IM_COL32(255, 255, 255, (int)(opacity * 255));

        for (int i = 0; i < 6; i++) {
            float angle = rotation + i * 1.0472f;
            ImVec2 end = ImVec2(pos.x + cosf(angle) * size, pos.y + sinf(angle) * size);
            draw->AddLine(pos, end, col, 1.2f);
        }
        draw->AddCircleFilled(pos, 2.f, col);
    }

    void drawDotSnowflake(ImDrawList* draw, ImVec2 pos, float size, float opacity) {
        ImU32 col = IM_COL32(255, 255, 255, (int)(opacity * 255));
        ImU32 colGlow = IM_COL32(200, 230, 255, (int)(opacity * 100));
        draw->AddCircleFilled(pos, size * 0.5f, colGlow);
        draw->AddCircleFilled(pos, size * 0.25f, col);
    }

    void updateAndDrawBackground(ImDrawList* draw) {
        if (!Cheats::Misc::christmasEnabled) return;
        if (!initialized) init();

        ImGuiIO& io = ImGui::GetIO();
        screenWidth = io.DisplaySize.x;
        screenHeight = io.DisplaySize.y;

        for (int i = 0; i < MAX_SNOWFLAKES; i++) {
            snowflakes[i].y += snowflakes[i].speed;
            snowflakes[i].wobble += snowflakes[i].wobbleSpeed;
            snowflakes[i].rotation += snowflakes[i].rotSpeed;
            float offsetX = sinf(snowflakes[i].wobble) * 0.8f;
            snowflakes[i].x += offsetX;

            if (snowflakes[i].y > screenHeight + 20.f) {
                snowflakes[i].y = -30.f;
                snowflakes[i].x = (float)(rand() % (int)screenWidth);
            }
            if (snowflakes[i].x < -20) snowflakes[i].x = screenWidth + 20;
            if (snowflakes[i].x > screenWidth + 20) snowflakes[i].x = -20;

            ImVec2 pos = ImVec2(snowflakes[i].x, snowflakes[i].y);

            switch (snowflakes[i].type) {
                case 0:
                    drawSnowflake(draw, pos, snowflakes[i].size, snowflakes[i].rotation, snowflakes[i].opacity);
                    break;
                case 1:
                    drawSimpleSnowflake(draw, pos, snowflakes[i].size * 0.7f, snowflakes[i].rotation, snowflakes[i].opacity);
                    break;
                case 2:
                    drawDotSnowflake(draw, pos, snowflakes[i].size, snowflakes[i].opacity);
                    break;
            }
        }
    }

    void applyChristmasTheme() {
        if (Cheats::Misc::christmasEnabled) {
            c::bg = ImColor(0, 0, 0, 255);
            c::primary = ImColor(80, 180, 80, 255);
            c::primary48 = ImColor(80, 180, 80, 122);
            c::primary24 = ImColor(80, 180, 80, 61);
            c::primary12 = ImColor(80, 180, 80, 30);
            c::primary6 = ImColor(80, 180, 80, 15);
            c::primary0 = ImColor(80, 180, 80, 0);
            c::secondary = ImColor(100, 200, 100, 255);
            c::checkbox_bg = ImColor(0, 0, 0, 255);
            c::checkbox_border = ImColor(80, 110, 150, 150);
            c::checkbox_hover = ImColor(10, 10, 10, 255);
            c::checkbox_active = ImColor(80, 180, 80, 255);
        } else {
            c::bg = c::bg_default;
            c::primary = c::primary_default;
            c::primary48 = ImColor(255, 20, 147, 122);
            c::primary24 = ImColor(255, 20, 147, 61);
            c::primary12 = ImColor(255, 20, 147, 30);
            c::primary6 = ImColor(255, 20, 147, 15);
            c::primary0 = ImColor(255, 20, 147, 0);
            c::secondary = c::secondary_default;
            c::checkbox_bg = ImColor(0, 0, 0, 255);
            c::checkbox_border = ImColor(45, 48, 55, 255);
            c::checkbox_hover = ImColor(10, 10, 10, 255);
            c::checkbox_active = ImColor(255, 20, 147, 255);
        }
    }

    void drawChristmasDecoration(ImDrawList* draw, ImVec2 menuPos, float menuWidth) {
        applyChristmasTheme();

        if (!Cheats::Misc::christmasEnabled) return;

        lightPhase += 0.02f;
        if (lightPhase > 6.28318f) lightPhase -= 6.28318f;

        ImColor lightColors[] = {
            ImColor(220, 40, 40, 255),
            ImColor(40, 180, 40, 255),
            ImColor(255, 200, 0, 255),
            ImColor(40, 120, 220, 255),
            ImColor(220, 40, 220, 255),
            ImColor(255, 120, 0, 255)
        };

        float wireY = menuPos.y - 4.f;
        float spacing = 28.f;
        int lightCount = (int)((menuWidth - 20.f) / spacing);
        float startX = menuPos.x + 15.f;

        ImU32 wireCol = IM_COL32(40, 40, 40, 255);
        draw->AddLine(ImVec2(startX - 5.f, wireY), ImVec2(startX + lightCount * spacing, wireY), wireCol, 2.f);

        for (int i = 0; i < lightCount; i++) {
            float x = startX + i * spacing;
            float hang = 5.f + sinf(i * 0.6f) * 2.f;
            float bulbY = wireY + hang;

            draw->AddLine(ImVec2(x, wireY), ImVec2(x, bulbY - 3.f), wireCol, 1.2f);

            ImColor col = lightColors[i % 6];
            float twinkle = 0.75f + 0.25f * sinf(lightPhase * 2.5f + i * 1.2f);

            ImU32 glow = IM_COL32((int)(col.Value.x * 255), (int)(col.Value.y * 255), (int)(col.Value.z * 255), (int)(70 * twinkle));
            draw->AddCircleFilled(ImVec2(x, bulbY), 8.f, glow);

            ImU32 bulb = IM_COL32((int)(col.Value.x * 255 * twinkle), (int)(col.Value.y * 255 * twinkle), (int)(col.Value.z * 255 * twinkle), 255);
            draw->AddCircleFilled(ImVec2(x, bulbY), 4.f, bulb);

            draw->AddCircleFilled(ImVec2(x - 1.2f, bulbY - 1.2f), 1.3f, IM_COL32(255, 255, 255, (int)(100 * twinkle)));
        }
    }

    void drawDetailedSnowflake(ImDrawList* draw, ImVec2 pos, float size, float rotation, float opacity) {
        ImU32 col = IM_COL32(255, 255, 255, (int)(opacity * 255));
        ImU32 colInner = IM_COL32(220, 240, 255, (int)(opacity * 200));

        for (int i = 0; i < 6; i++) {
            float angle = rotation + i * 1.0472f;
            float cosA = cosf(angle);
            float sinA = sinf(angle);

            ImVec2 end = ImVec2(pos.x + cosA * size, pos.y + sinA * size);
            draw->AddLine(pos, end, col, 1.2f);

            if (size > 4.f) {
                float branchPos = size * 0.6f;
                ImVec2 branchStart = ImVec2(pos.x + cosA * branchPos, pos.y + sinA * branchPos);
                float branchLen = size * 0.35f;

                draw->AddLine(branchStart, ImVec2(branchStart.x + cosf(angle + 0.5f) * branchLen, branchStart.y + sinf(angle + 0.5f) * branchLen), colInner, 0.8f);
                draw->AddLine(branchStart, ImVec2(branchStart.x + cosf(angle - 0.5f) * branchLen, branchStart.y + sinf(angle - 0.5f) * branchLen), colInner, 0.8f);
            }
        }
        draw->AddCircleFilled(pos, size * 0.12f, col);
    }

    void drawBackgroundTree(ImDrawList* draw, ImVec2 pos, float size, float alpha) {
        ImU32 treeCol = IM_COL32(35, 80, 45, (int)(alpha * 255));
        ImU32 treeMid = IM_COL32(25, 60, 35, (int)(alpha * 255));
        ImU32 treeDark = IM_COL32(18, 45, 25, (int)(alpha * 255));

        for (int i = 0; i < 4; i++) {
            float layerY = pos.y - i * size * 0.22f;
            float layerW = size * (0.55f - i * 0.1f);
            float layerH = size * 0.28f;

            ImU32 col = (i == 0) ? treeDark : ((i == 1) ? treeMid : ((i == 2) ? treeCol : treeMid));

            draw->AddTriangleFilled(
                ImVec2(pos.x, layerY - layerH),
                ImVec2(pos.x - layerW, layerY),
                ImVec2(pos.x + layerW, layerY),
                col
            );
        }

        draw->AddRectFilled(ImVec2(pos.x - 4.f, pos.y), ImVec2(pos.x + 4.f, pos.y + size * 0.12f), IM_COL32(70, 45, 30, (int)(alpha * 255)));

        if (alpha > 0.1f) {
            ImU32 starCol = IM_COL32(255, 220, 100, (int)(alpha * 180));
            float starY = pos.y - size * 0.88f - size * 0.28f;
            draw->AddCircleFilled(ImVec2(pos.x, starY), size * 0.06f, starCol);
        }
    }

    void drawSnowman(ImDrawList* draw, ImVec2 pos, float scale, bool facingRight, float armAngle, bool bendingDown) {
        ImU32 snowWhite = IM_COL32(255, 255, 255, 255);
        ImU32 snowShade = IM_COL32(200, 210, 220, 255);
        ImU32 snowOutline = IM_COL32(40, 40, 40, 255);
        ImU32 black = IM_COL32(20, 20, 20, 255);
        ImU32 orange = IM_COL32(255, 120, 40, 255);
        ImU32 red = IM_COL32(200, 50, 50, 255);
        ImU32 hatBlack = IM_COL32(30, 30, 30, 255);
        ImU32 brown = IM_COL32(90, 60, 30, 255);

        float bendOffset = bendingDown ? 5.f * scale : 0.f;

        draw->AddCircleFilled(ImVec2(pos.x, pos.y + 12.f * scale), 16.f * scale, snowOutline);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y + 12.f * scale), 14.f * scale, snowWhite);
        draw->AddCircleFilled(ImVec2(pos.x - 4.f * scale, pos.y + 8.f * scale), 4.f * scale, snowShade);

        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 6.f * scale + bendOffset), 12.f * scale, snowOutline);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 6.f * scale + bendOffset), 10.f * scale, snowWhite);
        draw->AddCircleFilled(ImVec2(pos.x - 3.f * scale, pos.y - 9.f * scale + bendOffset), 3.f * scale, snowShade);

        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 22.f * scale + bendOffset), 9.f * scale, snowOutline);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 22.f * scale + bendOffset), 7.5f * scale, snowWhite);

        float faceDir = facingRight ? 1.f : -1.f;
        float eyeX1 = pos.x + faceDir * 2.f * scale - 2.5f * scale;
        float eyeX2 = pos.x + faceDir * 2.f * scale + 2.5f * scale;
        float eyeY = pos.y - 24.f * scale + bendOffset;
        draw->AddCircleFilled(ImVec2(eyeX1, eyeY), 1.5f * scale, black);
        draw->AddCircleFilled(ImVec2(eyeX2, eyeY), 1.5f * scale, black);

        float smileY = pos.y - 19.f * scale + bendOffset;
        draw->AddCircleFilled(ImVec2(pos.x - 2.f * scale, smileY), 1.f * scale, black);
        draw->AddCircleFilled(ImVec2(pos.x, smileY + 0.5f * scale), 1.f * scale, black);
        draw->AddCircleFilled(ImVec2(pos.x + 2.f * scale, smileY), 1.f * scale, black);

        ImVec2 noseStart = ImVec2(pos.x + faceDir * 3.f * scale, pos.y - 22.f * scale + bendOffset);
        ImVec2 noseEnd = ImVec2(pos.x + faceDir * 10.f * scale, pos.y - 21.f * scale + bendOffset);
        draw->AddLine(noseStart, noseEnd, orange, 3.f * scale);
        draw->AddCircleFilled(noseStart, 1.5f * scale, orange);

        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 2.f * scale + bendOffset), 2.f * scale, black);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y - 8.f * scale + bendOffset), 2.f * scale, black);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y + 5.f * scale), 2.f * scale, black);
        draw->AddCircleFilled(ImVec2(pos.x, pos.y + 12.f * scale), 2.f * scale, black);

        float scarfY = pos.y - 14.f * scale + bendOffset;
        draw->AddRectFilled(ImVec2(pos.x - 8.f * scale, scarfY - 2.f * scale), ImVec2(pos.x + 8.f * scale, scarfY + 2.f * scale), red);
        draw->AddRectFilled(ImVec2(pos.x + 4.f * scale * faceDir, scarfY), ImVec2(pos.x + 10.f * scale * faceDir, scarfY + 12.f * scale), red);
        draw->AddLine(ImVec2(pos.x + 5.f * scale * faceDir, scarfY + 4.f * scale), ImVec2(pos.x + 9.f * scale * faceDir, scarfY + 4.f * scale), IM_COL32(150, 30, 30, 255), 1.f);
        draw->AddLine(ImVec2(pos.x + 5.f * scale * faceDir, scarfY + 8.f * scale), ImVec2(pos.x + 9.f * scale * faceDir, scarfY + 8.f * scale), IM_COL32(150, 30, 30, 255), 1.f);

        float hatY = pos.y - 30.f * scale + bendOffset;
        draw->AddRectFilled(ImVec2(pos.x - 9.f * scale, hatY), ImVec2(pos.x + 9.f * scale, hatY + 3.f * scale), hatBlack);
        draw->AddRectFilled(ImVec2(pos.x - 6.f * scale, hatY - 12.f * scale), ImVec2(pos.x + 6.f * scale, hatY), hatBlack);
        draw->AddRectFilled(ImVec2(pos.x - 6.f * scale, hatY - 3.f * scale), ImVec2(pos.x + 6.f * scale, hatY), red);

        float armDir = facingRight ? 1.f : -1.f;
        float armStartX = pos.x + 10.f * scale * armDir;
        float armStartY = pos.y - 6.f * scale + bendOffset;

        float throwArmEndX = armStartX + cosf(armAngle) * 18.f * scale * armDir;
        float throwArmEndY = armStartY + sinf(armAngle) * 18.f * scale;
        draw->AddLine(ImVec2(armStartX, armStartY), ImVec2(throwArmEndX, throwArmEndY), brown, 3.f * scale);

        float branchLen = 5.f * scale;
        float branchAngle1 = armAngle + 0.5f;
        float branchAngle2 = armAngle - 0.5f;
        draw->AddLine(ImVec2(throwArmEndX, throwArmEndY), ImVec2(throwArmEndX + cosf(branchAngle1) * branchLen * armDir, throwArmEndY + sinf(branchAngle1) * branchLen), brown, 2.f * scale);
        draw->AddLine(ImVec2(throwArmEndX, throwArmEndY), ImVec2(throwArmEndX + cosf(branchAngle2) * branchLen * armDir, throwArmEndY + sinf(branchAngle2) * branchLen), brown, 2.f * scale);

        float otherArmStartX = pos.x - 10.f * scale * armDir;
        float otherArmEndX = otherArmStartX - 15.f * scale * armDir;
        float otherArmEndY = armStartY + 5.f * scale;
        draw->AddLine(ImVec2(otherArmStartX, armStartY), ImVec2(otherArmEndX, otherArmEndY), brown, 3.f * scale);
        draw->AddLine(ImVec2(otherArmEndX, otherArmEndY), ImVec2(otherArmEndX - 4.f * scale * armDir, otherArmEndY - 3.f * scale), brown, 2.f * scale);
        draw->AddLine(ImVec2(otherArmEndX, otherArmEndY), ImVec2(otherArmEndX - 4.f * scale * armDir, otherArmEndY + 3.f * scale), brown, 2.f * scale);
    }

    void drawSnowball(ImDrawList* draw, ImVec2 pos, float size) {
        draw->AddCircleFilled(pos, size + 2.f, IM_COL32(180, 190, 200, 220));
        draw->AddCircleFilled(pos, size, IM_COL32(255, 255, 255, 255));
        draw->AddCircleFilled(ImVec2(pos.x - size * 0.25f, pos.y - size * 0.25f), size * 0.4f, IM_COL32(240, 248, 255, 200));
        draw->AddCircleFilled(ImVec2(pos.x + size * 0.15f, pos.y + size * 0.15f), size * 0.2f, IM_COL32(220, 230, 240, 150));
    }

    void drawChristmasBackground(ImDrawList* draw, ImVec2 menuPos, float menuWidth, float menuHeight) {
        if (!Cheats::Misc::christmasEnabled) return;

        static float snowPhase = 0.f;
        snowPhase += 0.012f;
        if (snowPhase > 100.f) snowPhase = 0.f;

        drawDetailedSnowflake(draw, ImVec2(menuPos.x + menuWidth * 0.15f, menuPos.y + menuHeight * 0.3f), 45.f, snowPhase * 0.08f, 0.06f);
        drawDetailedSnowflake(draw, ImVec2(menuPos.x + menuWidth * 0.75f, menuPos.y + menuHeight * 0.25f), 55.f, snowPhase * 0.05f + 1.f, 0.05f);
        drawDetailedSnowflake(draw, ImVec2(menuPos.x + menuWidth * 0.5f, menuPos.y + menuHeight * 0.6f), 50.f, snowPhase * 0.06f + 2.f, 0.04f);
        drawDetailedSnowflake(draw, ImVec2(menuPos.x + menuWidth * 0.85f, menuPos.y + menuHeight * 0.7f), 40.f, snowPhase * 0.07f + 3.f, 0.05f);
        drawDetailedSnowflake(draw, ImVec2(menuPos.x + menuWidth * 0.25f, menuPos.y + menuHeight * 0.75f), 35.f, snowPhase * 0.09f + 4.f, 0.04f);

        for (int i = 0; i < 50; i++) {
            float baseX = (i * 19.7f);
            float baseY = (i * 13.1f);
            float speed = 0.3f + (i % 5) * 0.15f;

            float x = menuPos.x + fmodf(baseX + snowPhase * 8.f * ((i % 3) - 1) * 0.3f, menuWidth);
            float y = menuPos.y + fmodf(baseY + snowPhase * 25.f * speed, menuHeight - 60.f);

            float size = 2.5f + (i % 5) * 1.5f;
            float rot = snowPhase * 0.4f + i * 0.6f;
            float alpha = 0.35f + (i % 4) * 0.12f;

            if (i % 6 == 0 && size > 5.f) {
                drawDetailedSnowflake(draw, ImVec2(x, y), size, rot, alpha);
            } else {
                ImU32 snowCol = IM_COL32(255, 255, 255, (int)(alpha * 255));
                draw->AddCircleFilled(ImVec2(x, y), size * 0.4f, snowCol);
            }
        }

        float groundY = menuPos.y + menuHeight - 45.f;

        ImU32 snowGroundBase = IM_COL32(255, 255, 255, 200);
        ImU32 snowGroundTop = IM_COL32(240, 248, 255, 180);
        ImU32 snowGroundShade = IM_COL32(220, 230, 240, 150);

        draw->AddRectFilledMultiColor(
            ImVec2(menuPos.x, groundY + 10.f),
            ImVec2(menuPos.x + menuWidth, menuPos.y + menuHeight),
            snowGroundTop, snowGroundTop, snowGroundBase, snowGroundBase
        );

        for (int i = 0; i < 40; i++) {
            float x = menuPos.x + i * (menuWidth / 39.f);
            float bumpHeight = 12.f + sinf(i * 0.4f) * 6.f + cosf(i * 0.7f) * 4.f;
            draw->AddCircleFilled(ImVec2(x, groundY + 10.f), bumpHeight, IM_COL32(255, 255, 255, 220));
        }

        for (int i = 0; i < 25; i++) {
            float x = menuPos.x + 20.f + i * (menuWidth / 26.f);
            float bumpHeight = 8.f + sinf(i * 0.6f + 1.f) * 4.f;
            draw->AddCircleFilled(ImVec2(x, groundY + 5.f), bumpHeight, IM_COL32(250, 252, 255, 200));
        }

        for (int i = 0; i < 60; i++) {
            float x = menuPos.x + fmodf(i * 16.3f, menuWidth);
            float y = groundY + 10.f + fmodf(i * 7.1f, 30.f);
            draw->AddCircleFilled(ImVec2(x, y), 1.5f + (i % 3) * 0.5f, IM_COL32(230, 240, 250, 100));
        }

        static float animTime = 0.f;
        animTime += 0.012f;
        float cycleDuration = 10.f;
        float cycleTime = fmodf(animTime, cycleDuration);

        float leftSnowmanX = menuPos.x + 70.f;
        float rightSnowmanX = menuPos.x + menuWidth - 70.f;
        float snowmanY = groundY + 5.f;
        float snowmanScale = 1.4f;

        float leftArmAngle = 0.3f;
        float rightArmAngle = 0.3f;
        bool leftBending = false;
        bool rightBending = false;
        bool leftThrowing = cycleTime < (cycleDuration / 2.f);

        float halfCycle = cycleDuration / 2.f;

        if (leftThrowing) {
            float phase = cycleTime / halfCycle;

            if (phase < 0.3f) {
                float bendProgress = phase / 0.3f;
                leftArmAngle = 0.3f + bendProgress * 1.2f;
                leftBending = bendProgress > 0.3f;
            } else if (phase < 0.5f) {
                float pickProgress = (phase - 0.3f) / 0.2f;
                leftArmAngle = 1.5f - pickProgress * 0.5f;
                leftBending = true;
            } else if (phase < 0.7f) {
                float raiseProgress = (phase - 0.5f) / 0.2f;
                leftArmAngle = 1.0f - raiseProgress * 2.5f;
                leftBending = 1.f - raiseProgress > 0.3f;
            } else {
                float throwProgress = (phase - 0.7f) / 0.3f;
                leftArmAngle = -1.5f + throwProgress * 1.8f;
            }
        } else {
            float phase = (cycleTime - halfCycle) / halfCycle;

            if (phase < 0.3f) {
                float bendProgress = phase / 0.3f;
                rightArmAngle = 0.3f + bendProgress * 1.2f;
                rightBending = bendProgress > 0.3f;
            } else if (phase < 0.5f) {
                float pickProgress = (phase - 0.3f) / 0.2f;
                rightArmAngle = 1.5f - pickProgress * 0.5f;
                rightBending = true;
            } else if (phase < 0.7f) {
                float raiseProgress = (phase - 0.5f) / 0.2f;
                rightArmAngle = 1.0f - raiseProgress * 2.5f;
                rightBending = 1.f - raiseProgress > 0.3f;
            } else {
                float throwProgress = (phase - 0.7f) / 0.3f;
                rightArmAngle = -1.5f + throwProgress * 1.8f;
            }
        }

        drawSnowman(draw, ImVec2(leftSnowmanX, snowmanY), snowmanScale, true, leftArmAngle, leftBending);
        drawSnowman(draw, ImVec2(rightSnowmanX, snowmanY), snowmanScale, false, rightArmAngle, rightBending);

        float snowballSize = 7.f;

        if (leftThrowing) {
            float phase = cycleTime / halfCycle;

            if (phase >= 0.35f && phase < 0.5f) {
                float armEndX = leftSnowmanX + 10.f * snowmanScale + cosf(leftArmAngle) * 18.f * snowmanScale;
                float armEndY = snowmanY - 6.f * snowmanScale + sinf(leftArmAngle) * 18.f * snowmanScale;
                drawSnowball(draw, ImVec2(armEndX, armEndY), snowballSize);
            } else if (phase >= 0.5f && phase < 0.7f) {
                float armEndX = leftSnowmanX + 10.f * snowmanScale + cosf(leftArmAngle) * 18.f * snowmanScale;
                float armEndY = snowmanY - 6.f * snowmanScale + sinf(leftArmAngle) * 18.f * snowmanScale;
                drawSnowball(draw, ImVec2(armEndX, armEndY), snowballSize);
            } else if (phase >= 0.7f && phase < 0.95f) {
                float flyProgress = (phase - 0.7f) / 0.25f;

                float startX = leftSnowmanX + 25.f * snowmanScale;
                float startY = snowmanY - 20.f * snowmanScale;
                float endX = rightSnowmanX - 15.f * snowmanScale;
                float endY = snowmanY - 10.f * snowmanScale;

                float arcHeight = -100.f;
                float ballX = startX + (endX - startX) * flyProgress;
                float ballY = startY + (endY - startY) * flyProgress + arcHeight * sinf(flyProgress * 3.14159f);

                drawSnowball(draw, ImVec2(ballX, ballY), snowballSize * (1.f - flyProgress * 0.3f));
            }
        } else {
            float phase = (cycleTime - halfCycle) / halfCycle;

            if (phase >= 0.35f && phase < 0.5f) {
                float armEndX = rightSnowmanX - 10.f * snowmanScale + cosf(rightArmAngle) * 18.f * snowmanScale * (-1.f);
                float armEndY = snowmanY - 6.f * snowmanScale + sinf(rightArmAngle) * 18.f * snowmanScale;
                drawSnowball(draw, ImVec2(armEndX, armEndY), snowballSize);
            } else if (phase >= 0.5f && phase < 0.7f) {
                float armEndX = rightSnowmanX - 10.f * snowmanScale + cosf(rightArmAngle) * 18.f * snowmanScale * (-1.f);
                float armEndY = snowmanY - 6.f * snowmanScale + sinf(rightArmAngle) * 18.f * snowmanScale;
                drawSnowball(draw, ImVec2(armEndX, armEndY), snowballSize);
            } else if (phase >= 0.7f && phase < 0.95f) {
                float flyProgress = (phase - 0.7f) / 0.25f;

                float startX = rightSnowmanX - 25.f * snowmanScale;
                float startY = snowmanY - 20.f * snowmanScale;
                float endX = leftSnowmanX + 15.f * snowmanScale;
                float endY = snowmanY - 10.f * snowmanScale;

                float arcHeight = -100.f;
                float ballX = startX + (endX - startX) * flyProgress;
                float ballY = startY + (endY - startY) * flyProgress + arcHeight * sinf(flyProgress * 3.14159f);

                drawSnowball(draw, ImVec2(ballX, ballY), snowballSize * (1.f - flyProgress * 0.3f));
            }
        }

        ImU32 iceBorder = IM_COL32(200, 225, 250, 40);
        draw->AddRect(menuPos + ImVec2(2, 2), menuPos + ImVec2(menuWidth - 2, menuHeight - 2), iceBorder, 15.f, 0, 1.f);

        for (int corner = 0; corner < 4; corner++) {
            float cx = (corner % 2 == 0) ? menuPos.x + 22.f : menuPos.x + menuWidth - 22.f;
            float cy = (corner < 2) ? menuPos.y + 22.f : menuPos.y + menuHeight - 22.f;
            drawDetailedSnowflake(draw, ImVec2(cx, cy), 12.f, snowPhase * 0.2f + corner * 1.57f, 0.45f);
        }
    }
};

static ChristmasEffects g_christmasEffects;

namespace Cheats {
    namespace Esp {
        extern int healthBarPos;
        extern int armorBarPos;
        extern int nameEspPos;
        extern int weaponEspPos;
        extern int idEspPos;
        extern int distanceEspPos;
    }
}
#include "../bytes/esp.h"
#include "elements_manager.h"

namespace Cheats {
    namespace Aim {
        extern int espPreviewSelectedBone;
        extern bool espPreviewSelectedBones[11];
        extern int SlientSlectedType;
        extern bool missChance;
        extern int missChanceValue;
        extern bool silentIgnoreDeath;
    }
    namespace Esp {
        extern bool skeletonEsp;
        extern float skeletonEspColor[3];
        extern int skeletonEspSize;
        extern bool boxEsp;
        extern float boxEspColor[3];
        extern int boxEspSelectedType;
        extern float BoxSizeX;
        extern bool useCustomGradient;
        extern float boxGradientTopColor[4];
        extern float boxGradientBottomColor[4];
        extern bool gradientEnabled;
        extern float gradientIntensity;
        extern bool lineEsp;
        extern float lineEspColor[3];
        extern int lineEspSelectedType;
        extern bool nameEsp;
        extern float nameEspColor[3];
        extern bool idEsp;
        extern float idEspColor[3];
        extern bool weaponEsp;
        extern float weaponEspColor[3];
        extern bool distanceEsp;
        extern float distanceEspColor[3];
        extern bool healthBar;
        extern bool armorBar;
    }
}

using namespace ImGui;


float alpha_text = 1.f;

#define ch_pre(xx, yy, name) \
GetCurrentWindow()->DC.CursorPos = ImVec2(p.x + 991.f + xx, p.y + yy);\
static bool example##name = false;\
skeleton_checkbox(#name, &example##name);\
anim::get_anim_obj(CONST_HASH((std::string(#name) + "##line_pre").c_str()))->active = example##name;

#define line_pre(xx, yy, name, xx2, yy2, name2) \
{\
auto curr = anim::animation((std::string(#name) + "##line_pre").c_str(), anim_t(clamp_out, 0.1f));\
auto curr2 = anim::animation((std::string(#name2) + "##line_pre").c_str(), anim_t(clamp_out, 0.1f));\
ImColor col2 = lerp(lerp(c::white0, c::white24, global_a), lerp(c::white24, c::primary, global_a), curr->val);\
ImColor col3 = lerp(lerp(c::white0, c::white24, global_a), lerp(c::white24, c::primary, global_a), curr2->val);\
\
int vert_start_idx = draw->VtxBuffer.Size;\
draw->AddLine(ImVec2(p.x + 991.f + xx + 9.5f, p.y + yy + 9.5f),\
    ImVec2(p.x + 991.f + xx2 + 9.5f, p.y + yy2 + 9.5f), IM_COL32_WHITE);\
int vert_end_idx = draw->VtxBuffer.Size;\
ShadeVertsLinearGradY(draw, vert_start_idx, vert_end_idx,\
    ImVec2(p.x + 991.f + xx + 9.5f, p.y + yy + 9.5f),\
    ImVec2(p.x + 991.f + xx2 + 9.5f, p.y + yy2 + 9.5f), col2, col3);\
}

class _c_menu : public c_menu
{
public:
    void setup_data(ID3D11Device* device) override
    {
        MGR->g_pd3dDevice = g_pd3dDevice = device;
        if (tex::bg == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, bg, sizeof(bg), &tex::bg);
        if (tex::sidebar_bg == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, sidebar, sizeof(sidebar), &tex::sidebar_bg);
        if (tex::main_logo == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), &tex::main_logo);
        if (tex::esp == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, esp, sizeof(esp), &tex::esp);
        if (tex::transparent_logo == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, TransparentLogo, sizeof(TransparentLogo), &tex::transparent_logo);
        if (tex::tap_logo == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, tap_logo_png, sizeof(tap_logo_png), &tex::tap_logo);
        if (tex::silent_esp_preview == nullptr)
            CreateShaderResourceViewFromMemory(g_pd3dDevice, silent_esp, sizeof(silent_esp), &tex::silent_esp_preview);

        ImGuiIO& io = ImGui::GetIO();

        ImFontConfig fontConfig;
        fontConfig.OversampleH = 3;
        fontConfig.OversampleV = 3;
        fontConfig.PixelSnapH = true;
        fontConfig.RasterizerDensity = 1.8f;
        fontConfig.FontDataOwnedByAtlas = false;


        static const ImWchar turkishRanges[] = {
            0x0020, 0x00FF,
            0x0100, 0x017F,
            0x0400, 0x04FF,
            0,
        };


        f::sbold16 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 18.f, &fontConfig, turkishRanges);


        ImFontConfig thinConfig;
        thinConfig.OversampleH = 2;
        thinConfig.OversampleV = 2;
        thinConfig.PixelSnapH = true;
        thinConfig.RasterizerDensity = 1.0f;
        thinConfig.FontDataOwnedByAtlas = false;

        f::medium14 = io.Fonts->AddFontFromMemoryTTF(caviar_ttf, sizeof caviar_ttf, 16.f, &thinConfig, turkishRanges);
        f::medium12 = io.Fonts->AddFontFromMemoryTTF(caviar_ttf, sizeof caviar_ttf, 14.f, &thinConfig, turkishRanges);
        f::thin20 = io.Fonts->AddFontFromMemoryTTF(caviar_ttf, sizeof caviar_ttf, 16.f, &thinConfig, turkishRanges);
        f::thin16 = io.Fonts->AddFontFromMemoryTTF(caviar_ttf, sizeof caviar_ttf, 14.f, &thinConfig, turkishRanges);
        f::thin14 = io.Fonts->AddFontFromMemoryTTF(caviar_ttf, sizeof caviar_ttf, 12.f, &thinConfig, turkishRanges);


        {
            ImFontConfig v2Cfg = fontConfig;
            v2Cfg.OversampleH = 4;
            v2Cfg.OversampleV = 4;
            v2Cfg.RasterizerDensity = 2.2f;
            v2Cfg.PixelSnapH = false;
            static const ImWchar v2Ranges[] = { (ImWchar)'2', (ImWchar)'2', (ImWchar)'V', (ImWchar)'V', 0 };
            f::v2Display = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 110.f, &v2Cfg, v2Ranges);
        }


        f::icons60 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 60.f, &fontConfig, turkishRanges);
        f::icons30 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 30.f, &fontConfig, turkishRanges);
        f::icons20 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 20.f, &fontConfig, turkishRanges);
        f::icons13 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 13.f, &fontConfig, turkishRanges);
        f::icons12 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 12.f, &fontConfig, turkishRanges);
        f::icons10 = io.Fonts->AddFontFromMemoryTTF(&medium_ttf, sizeof medium_ttf, 10.f, &fontConfig, turkishRanges);
    }
    void setup_imgui() override
    {
        ImGuiStyle* style = &::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 200);
        colors[ImGuiCol_ChildBg] = ImColor(6, 5, 10, 240);
        colors[ImGuiCol_PopupBg] = ImColor(7, 8, 18, 240);
        colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = c::white4;
        colors[ImGuiCol_ScrollbarGrab] = c::white12;
        colors[ImGuiCol_ScrollbarGrabHovered] = c::white12;
        colors[ImGuiCol_ScrollbarGrabActive] = c::white12;
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        colors[ImGuiCol_WindowShadow] = ImVec4(0.08f, 0.08f, 0.08f, 0.35f);

        style->ChildRounding = 0.f;
        style->WindowRounding = 16.f;
        style->WindowPadding = ImVec2(0, 0);
        style->ItemSpacing.y = 4.f;
        style->ScrollbarSize = 5.f;
        style->ScrollbarRounding = 512.f;
    }
    bool text_button(ImVec2 pos, const char* text, ImColor col = c::white24, float f_sz = 10.f, ImFont* font = f::icons10)
    {
        auto draw = ::GetWindowDrawList();
        auto curr = anim::animation((std::string(text) + "##ibutton").c_str(), anim_t(clamp_out, 0.1f));

        rot_start();

        draw->AddText(font, f_sz, pos, lerp(col, c::primary, curr->val), text);

        rot_end(0.0f);

        ::GetCurrentWindow()->DC.CursorPos = pos;
        ImVec2 text_size = font->CalcTextSizeA(f_sz, FLT_MAX, -1.f, text, 0, NULL);
        bool ret = ::InvisibleButton((std::string(text) + "##ibutton").c_str(), ImVec2(text_size.x, f_sz));
        curr->active = ::IsItemHovered();

        return ret;
    }

    void esp_preview()
    {
        auto draw = ::GetBackgroundDrawList();
        if (!draw) return;

        const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
        auto switch_screen_a = anim::get_anim_obj(CONST_HASH("switch_screen"));

        if (!switch_screen_a || switch_screen_a->val < 0.01f) return;

        float gap = 20.f;
        float preview_width = 290.f;
        float preview_height = 530.f;


        float anim_offset_y = (1.f - switch_screen_a->val) * (preview_height + 100.f);
        ImVec2 Clip_pos = ImVec2(p.x + 750.f + gap, p.y + 18.f + anim_offset_y);

        if (Cheats::Misc::christmasEnabled) {
            ImU32 glassCol = IM_COL32(15, 22, 35, (int)(220 * switch_screen_a->val));
            draw->AddRectFilled(Clip_pos, Clip_pos + ImVec2(preview_width, preview_height), glassCol, 16.f);

            ImU32 frostCol = IM_COL32(200, 220, 240, (int)(15 * switch_screen_a->val));
            draw->AddRectFilled(Clip_pos, Clip_pos + ImVec2(preview_width, preview_height), frostCol, 16.f);

            ImU32 borderCol = IM_COL32(200, 220, 240, (int)(80 * switch_screen_a->val));
            draw->AddRect(Clip_pos, Clip_pos + ImVec2(preview_width, preview_height), borderCol, 16.f, 0, 1.5f);

            static float espSnowPhase = 0.f;
            espSnowPhase += 0.01f;
            if (espSnowPhase > 100.f) espSnowPhase = 0.f;

            g_christmasEffects.drawDetailedSnowflake(draw, ImVec2(Clip_pos.x + preview_width * 0.2f, Clip_pos.y + preview_height * 0.15f), 25.f, espSnowPhase * 0.1f, 0.08f * switch_screen_a->val);
            g_christmasEffects.drawDetailedSnowflake(draw, ImVec2(Clip_pos.x + preview_width * 0.8f, Clip_pos.y + preview_height * 0.85f), 22.f, espSnowPhase * 0.08f + 2.f, 0.07f * switch_screen_a->val);
            g_christmasEffects.drawDetailedSnowflake(draw, ImVec2(Clip_pos.x + preview_width * 0.15f, Clip_pos.y + preview_height * 0.75f), 18.f, espSnowPhase * 0.12f + 1.f, 0.06f * switch_screen_a->val);
            g_christmasEffects.drawDetailedSnowflake(draw, ImVec2(Clip_pos.x + preview_width * 0.85f, Clip_pos.y + preview_height * 0.25f), 20.f, espSnowPhase * 0.09f + 3.f, 0.07f * switch_screen_a->val);

            for (int i = 0; i < 15; i++) {
                float x = Clip_pos.x + fmodf(i * 21.3f + espSnowPhase * 5.f, preview_width);
                float y = Clip_pos.y + fmodf(i * 38.7f + espSnowPhase * 12.f, preview_height);
                float alpha = (0.3f + (i % 3) * 0.1f) * switch_screen_a->val;
                float size = 1.5f + (i % 4) * 0.8f;
                draw->AddCircleFilled(ImVec2(x, y), size, IM_COL32(255, 255, 255, (int)(alpha * 255)));
            }

            for (int corner = 0; corner < 4; corner++) {
                float cx = (corner % 2 == 0) ? Clip_pos.x + 15.f : Clip_pos.x + preview_width - 15.f;
                float cy = (corner < 2) ? Clip_pos.y + 15.f : Clip_pos.y + preview_height - 15.f;
                g_christmasEffects.drawDetailedSnowflake(draw, ImVec2(cx, cy), 8.f, espSnowPhase * 0.15f + corner * 1.57f, 0.4f * switch_screen_a->val);
            }
        } else {

            ImU32 bgCol = IM_COL32(10, 8, 14, (int)(245 * switch_screen_a->val));
            draw->AddRectFilled(Clip_pos, Clip_pos + ImVec2(preview_width, preview_height), bgCol, 16.f);


            ImU32 borderCol = IM_COL32(255, 20, 147, (int)(80 * switch_screen_a->val));
            draw->AddRect(Clip_pos, Clip_pos + ImVec2(preview_width, preview_height), borderCol, 16.f, 0, 1.5f);
        }

        global_a = switch_screen_a->val;

        ::SetNextWindowPos(Clip_pos + ImVec2(10.f, 10.f), ImGuiCond_Always);
        ::SetNextWindowSize(ImVec2(preview_width - 20.f, preview_height - 20.f), ImGuiCond_Always);
        ::PushStyleVar(ImGuiStyleVar_Alpha, switch_screen_a->val);

        if (::Begin("##ESPPreviewContent", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground))
        {
            ::SetScrollY(0);
            extern c_esp_drag m_esp_drag;


            static int lastHealthPos = Cheats::Esp::healthBarPos;
            static int lastArmorPos = Cheats::Esp::armorBarPos;
            static int lastNamePos = Cheats::Esp::nameEspPos;
            static int lastWeaponPos = Cheats::Esp::weaponEspPos;
            static int lastIdPos = Cheats::Esp::idEspPos;
            static int lastDistancePos = Cheats::Esp::distanceEspPos;


            if (lastHealthPos != Cheats::Esp::healthBarPos) {
                m_esp_drag.m_items[0].pos = Cheats::Esp::healthBarPos;
                lastHealthPos = Cheats::Esp::healthBarPos;
            }
            if (lastArmorPos != Cheats::Esp::armorBarPos) {
                m_esp_drag.m_items[1].pos = Cheats::Esp::armorBarPos;
                lastArmorPos = Cheats::Esp::armorBarPos;
            }
            if (lastNamePos != Cheats::Esp::nameEspPos) {
                m_esp_drag.m_items[2].pos = Cheats::Esp::nameEspPos;
                lastNamePos = Cheats::Esp::nameEspPos;
            }
            if (lastWeaponPos != Cheats::Esp::weaponEspPos) {
                m_esp_drag.m_items[3].pos = Cheats::Esp::weaponEspPos;
                lastWeaponPos = Cheats::Esp::weaponEspPos;
            }
            if (lastIdPos != Cheats::Esp::idEspPos) {
                m_esp_drag.m_items[4].pos = Cheats::Esp::idEspPos;
                lastIdPos = Cheats::Esp::idEspPos;
            }
            if (lastDistancePos != Cheats::Esp::distanceEspPos) {
                m_esp_drag.m_items[5].pos = Cheats::Esp::distanceEspPos;
                lastDistancePos = Cheats::Esp::distanceEspPos;
            }

            m_esp_drag.m_items[0].enabled = Cheats::Esp::healthBar;
            m_esp_drag.m_items[1].enabled = Cheats::Esp::armorBar;
            m_esp_drag.m_items[2].enabled = Cheats::Esp::nameEsp;
            m_esp_drag.m_items[3].enabled = Cheats::Esp::weaponEsp;
            m_esp_drag.m_items[4].enabled = Cheats::Esp::idEsp;
            m_esp_drag.m_items[5].enabled = Cheats::Esp::distanceEsp;


            m_esp_drag.m_items[2].col = ImColor(
                Cheats::Esp::nameEspColor[0],
                Cheats::Esp::nameEspColor[1],
                Cheats::Esp::nameEspColor[2], 1.0f);
            m_esp_drag.m_items[3].col = ImColor(
                Cheats::Esp::weaponEspColor[0],
                Cheats::Esp::weaponEspColor[1],
                Cheats::Esp::weaponEspColor[2], 1.0f);
            m_esp_drag.m_items[4].col = ImColor(
                Cheats::Esp::idEspColor[0],
                Cheats::Esp::idEspColor[1],
                Cheats::Esp::idEspColor[2], 1.0f);
            m_esp_drag.m_items[5].col = ImColor(
                Cheats::Esp::distanceEspColor[0],
                Cheats::Esp::distanceEspColor[1],
                Cheats::Esp::distanceEspColor[2], 1.0f);


            m_esp_drag.m_items[2].text = "Player";
            m_esp_drag.m_items[3].text = "Pistol";
            m_esp_drag.m_items[4].text = "ID";
            m_esp_drag.m_items[5].text = "25m";

            m_esp_drag.set_positions();
            m_esp_drag.on_draw();


            if (Cheats::Esp::skeletonEsp) {
                ImColor skeletonColor = ImColor(
                    Cheats::Esp::skeletonEspColor[0],
                    Cheats::Esp::skeletonEspColor[1],
                    Cheats::Esp::skeletonEspColor[2],
                    1.0f);
                m_esp_drag.draw_skeleton(skeletonColor, (float)Cheats::Esp::skeletonEspSize);
            }


            Cheats::Esp::healthBarPos = m_esp_drag.m_items[0].pos;
            Cheats::Esp::armorBarPos = m_esp_drag.m_items[1].pos;
            Cheats::Esp::nameEspPos = m_esp_drag.m_items[2].pos;
            Cheats::Esp::weaponEspPos = m_esp_drag.m_items[3].pos;
            Cheats::Esp::idEspPos = m_esp_drag.m_items[4].pos;
            Cheats::Esp::distanceEspPos = m_esp_drag.m_items[5].pos;


            lastHealthPos = Cheats::Esp::healthBarPos;
            lastArmorPos = Cheats::Esp::armorBarPos;
            lastNamePos = Cheats::Esp::nameEspPos;
            lastWeaponPos = Cheats::Esp::weaponEspPos;
            lastIdPos = Cheats::Esp::idEspPos;
            lastDistancePos = Cheats::Esp::distanceEspPos;
        }
        ::End();
        ::PopStyleVar();

        return;
    }
    void draw() override
    {
        anim::update_animation_speed();
        auto switch_screen_a = anim::animation("switch_screen", anim_t(clamp_out, 0.1f));
        switch_screen_a->active = MGR->esp_p;

        auto bgDraw = ::GetBackgroundDrawList();


        begin("General");
        {
            background();
            bar();


            auto& active_tab = MGR->tabs.at(screen);
            float top_bar_height = active_tab.has_subs() ? 92.f : 51.f;

            ImGui::SetCursorPos(ImVec2(10.f, top_bar_height));
            ::BeginChild("##t", ImVec2(730.f, 578.f - top_bar_height - 5.f), 0, ImGuiWindowFlags_NoBackground);
            {
                ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0.f, 4.f));

                if (active_tab.has_subs()) {
                    int sub_idx = active_tab.active_sub;
                    if (sub_idx >= 0 && sub_idx < (int)active_tab.sub_tabs.size()) {
                        active_tab.sub_tabs[sub_idx].callback();
                    }
                } else {
                    active_tab.callback();
                }
            }
            ::EndChild();

            esp_preview();
        }

    }
    bool begin(const char* name) override
    {



        ::SetNextWindowSize(ImVec2(750.f, 578.f));
        ImVec2 displaySize = ::GetIO().DisplaySize;
        ::SetNextWindowPos(ImVec2((displaySize.x - 750.f) * 0.5f, (displaySize.y - 578.f) * 0.5f), ImGuiCond_Once);
        bool ret = ::Begin(name, nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);
        return ret;
    }
    void background() override
    {
        auto draw = ::GetWindowDrawList();
        const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
        const ImVec2& region = ::GetContentRegionMax() - ImVec2(2.f, 2.f);


        ImU32 glass_bg = IM_COL32(0, 0, 0, 180);
        draw->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + region.x, p.y + region.y), glass_bg, 16.f);


        ImU32 glass_border = IM_COL32(255, 105, 180, 60);
        draw->AddRect(ImVec2(p.x, p.y), ImVec2(p.x + region.x, p.y + region.y), glass_border, 16.f, 0, 1.5f);
    }


    void draw_tab_icon(ImDrawList* draw, ImVec2 center, float size, const std::string& label, float alpha, bool is_sub = false)
    {
        ImU32 col = IM_COL32(255, 20, 147, (int)(alpha));
        ImU32 col2 = IM_COL32(255, 255, 255, (int)(alpha));
        float r = size * 0.5f;

        if (label == "Aim Assist") {

            draw->AddCircle(center, r * 0.85f, col, 0, 1.4f);
            draw->AddLine(ImVec2(center.x - r, center.y), ImVec2(center.x + r, center.y), col, 1.2f);
            draw->AddLine(ImVec2(center.x, center.y - r), ImVec2(center.x, center.y + r), col, 1.2f);
            draw->AddCircleFilled(center, 1.5f, col);
        }
        else if (label == "Visuals") {

            float ew = r * 1.0f;
            float eh = r * 0.55f;
            ImVec2 left(center.x - ew, center.y);
            ImVec2 right(center.x + ew, center.y);

            draw->AddBezierQuadratic(left, ImVec2(center.x, center.y - eh * 2.f), right, col, 1.4f, 16);

            draw->AddBezierQuadratic(left, ImVec2(center.x, center.y + eh * 2.f), right, col, 1.4f, 16);

            draw->AddCircleFilled(center, r * 0.35f, col);
            draw->AddCircleFilled(ImVec2(center.x - 1.f, center.y - 1.f), r * 0.15f, col2);
        }
        else if (label == "Misc") {

            float s = r * 0.8f;
            draw->AddLine(ImVec2(center.x - s, center.y + s), ImVec2(center.x + s * 0.3f, center.y - s * 0.3f), col, 1.6f);
            draw->AddCircle(ImVec2(center.x + s * 0.5f, center.y - s * 0.5f), r * 0.4f, col, 0, 1.4f);

            draw->AddLine(ImVec2(center.x + s * 0.3f, center.y - s * 0.7f), ImVec2(center.x + s * 0.7f, center.y - s * 0.3f), col, 1.2f);
        }
        else if (label == "Settings") {

            draw->AddCircle(center, r * 0.4f, col, 0, 1.4f);
            for (int i = 0; i < 6; i++) {
                float angle = i * (3.14159f * 2.f / 6.f) - 3.14159f / 2.f;
                ImVec2 outer(center.x + cosf(angle) * r * 0.85f, center.y + sinf(angle) * r * 0.85f);
                ImVec2 inner(center.x + cosf(angle) * r * 0.5f, center.y + sinf(angle) * r * 0.5f);
                draw->AddLine(inner, outer, col, 2.0f);
            }
        }
        else if (label == "Silent") {

            draw->AddCircle(center, r * 0.7f, col, 0, 1.2f);
            draw->AddLine(ImVec2(center.x - r * 0.4f, center.y), ImVec2(center.x + r * 0.4f, center.y), col, 1.1f);
            draw->AddLine(ImVec2(center.x, center.y - r * 0.4f), ImVec2(center.x, center.y + r * 0.4f), col, 1.1f);

            draw->AddLine(ImVec2(center.x - r * 0.8f, center.y - r * 0.8f), ImVec2(center.x + r * 0.8f, center.y + r * 0.8f), col, 1.3f);
        }
        else if (label == "Aimbot") {

            draw->AddCircle(center, r * 0.85f, col, 0, 1.3f);
            draw->AddCircle(center, r * 0.5f, col, 0, 1.2f);
            draw->AddCircleFilled(center, r * 0.2f, col);
        }
        else if (label == "Triggerbot") {

            ImVec2 pts[4] = {
                ImVec2(center.x + r * 0.1f, center.y - r * 0.9f),
                ImVec2(center.x - r * 0.3f, center.y + r * 0.05f),
                ImVec2(center.x + r * 0.15f, center.y + r * 0.05f),
                ImVec2(center.x - r * 0.1f, center.y + r * 0.9f)
            };
            draw->AddLine(pts[0], pts[1], col, 1.6f);
            draw->AddLine(pts[1], pts[2], col, 1.6f);
            draw->AddLine(pts[2], pts[3], col, 1.6f);
        }
        else if (label == "ESP") {

            draw->AddRect(ImVec2(center.x - r * 0.6f, center.y - r * 0.8f), ImVec2(center.x + r * 0.6f, center.y + r * 0.8f), col, 0.f, 0, 1.3f);

            float c_len = r * 0.3f;
            ImVec2 tl(center.x - r * 0.6f, center.y - r * 0.8f);
            draw->AddLine(tl, ImVec2(tl.x + c_len, tl.y), col2, 1.6f);
            draw->AddLine(tl, ImVec2(tl.x, tl.y + c_len), col2, 1.6f);
        }
        else if (label == "World") {

            draw->AddCircle(center, r * 0.8f, col, 0, 1.3f);
            draw->AddLine(ImVec2(center.x - r * 0.8f, center.y), ImVec2(center.x + r * 0.8f, center.y), col, 1.0f);

            draw->AddEllipse(center, ImVec2(r * 0.35f, r * 0.8f), col, 0.f, 0, 1.0f);
        }
        else if (label == "Vehicle") {

            float bw = r * 0.9f, bh = r * 0.4f;
            draw->AddRectFilled(ImVec2(center.x - bw, center.y - bh * 0.2f), ImVec2(center.x + bw, center.y + bh), col, 3.f);

            draw->AddRectFilled(ImVec2(center.x - bw * 0.55f, center.y - bh * 1.1f), ImVec2(center.x + bw * 0.55f, center.y - bh * 0.1f), col, 2.f);

            draw->AddCircleFilled(ImVec2(center.x - bw * 0.55f, center.y + bh), r * 0.22f, col2);
            draw->AddCircleFilled(ImVec2(center.x + bw * 0.55f, center.y + bh), r * 0.22f, col2);
        }
        else {

            draw->AddQuadFilled(
                ImVec2(center.x, center.y - r * 0.6f),
                ImVec2(center.x + r * 0.6f, center.y),
                ImVec2(center.x, center.y + r * 0.6f),
                ImVec2(center.x - r * 0.6f, center.y),
                col);
        }
    }

    bool tab(ImVec2 pos, std::string label, const char* icon, bool active, float tab_width)
    {
        auto curr = anim::animation((label + "##tab").c_str(), anim_t(clamp_out, 0.25f));
        ImVec2 tab_size = ImVec2(tab_width, 32.f);
        auto draw = ::GetWindowDrawList();

        bool hovered = ImGui::IsMouseHoveringRect(pos, pos + tab_size);
        static std::map<std::string, float> hover_anim;
        if (hover_anim.find(label) == hover_anim.end()) hover_anim[label] = 0.f;

        float hover_speed = ImGui::GetIO().DeltaTime * 8.f;
        if (hovered && !active)
            hover_anim[label] = (std::min)(1.f, hover_anim[label] + hover_speed);
        else if (!active)
            hover_anim[label] = (std::max)(0.f, hover_anim[label] - hover_speed);

        float effect = (std::max)(curr->val, hover_anim[label]);


        float bg_alpha = active ? 18.f : (5.f + 8.f * hover_anim[label]);
        draw->AddRectFilled(pos, pos + tab_size, IM_COL32(255, 255, 255, (int)bg_alpha), 6.f);


        float border_alpha = active ? 40.f : (15.f + 12.f * effect);
        draw->AddRect(pos, pos + tab_size, IM_COL32(255, 255, 255, (int)border_alpha), 6.f, 0, 1.0f);


        float icon_size = 14.f;
        ImVec2 text_sz = f::sbold16->CalcTextSizeA(16.f, FLT_MAX, -1.f, label.c_str(), 0, NULL);
        float content_width = icon_size + 6.f + text_sz.x;
        float content_start_x = pos.x + (tab_size.x - content_width) / 2.f;


        ImVec2 icon_center = ImVec2(content_start_x + icon_size * 0.5f, pos.y + tab_size.y * 0.5f);
        float icon_alpha = 180.f + 75.f * effect;
        draw_tab_icon(draw, icon_center, icon_size, label, icon_alpha);


        ImVec2 text_pos = ImVec2(content_start_x + icon_size + 6.f, pos.y + (tab_size.y - 16.f) / 2.f);
        draw->AddText(f::sbold16, 16.f, text_pos + ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, 180), label.c_str());
        draw->AddText(f::sbold16, 16.f, text_pos, IM_COL32(255, 255, 255, (int)(200 + 55 * effect)), label.c_str());

        ::GetCurrentWindow()->DC.CursorPos = pos;
        bool ret = ::InvisibleButton((label + "##tab").c_str(), tab_size);
        curr->active = active;
        return ret;
    }
    bool sub_tab(ImVec2 pos, std::string label, const char* icon, bool active, float tab_width)
    {
        auto curr = anim::animation((label + "##subtab").c_str(), anim_t(clamp_out, 0.2f));
        ImVec2 tab_size = ImVec2(tab_width, 28.f);
        auto draw = ::GetWindowDrawList();

        bool hovered = ImGui::IsMouseHoveringRect(pos, pos + tab_size);
        static std::map<std::string, float> hover_anim;
        if (hover_anim.find(label) == hover_anim.end()) hover_anim[label] = 0.f;

        float hover_speed = ImGui::GetIO().DeltaTime * 8.f;
        if (hovered && !active)
            hover_anim[label] = (std::min)(1.f, hover_anim[label] + hover_speed);
        else if (!active)
            hover_anim[label] = (std::max)(0.f, hover_anim[label] - hover_speed);

        float effect = (std::max)(curr->val, hover_anim[label]);


        float bg_alpha = active ? 14.f : (4.f + 6.f * hover_anim[label]);
        draw->AddRectFilled(pos, pos + tab_size, IM_COL32(255, 255, 255, (int)bg_alpha), 5.f);


        float border_alpha = active ? 35.f : (12.f + 10.f * effect);
        draw->AddRect(pos, pos + tab_size, IM_COL32(255, 255, 255, (int)border_alpha), 5.f, 0, 1.0f);


        float icon_size = 12.f;
        ImVec2 text_sz = f::sbold16->CalcTextSizeA(15.f, FLT_MAX, -1.f, label.c_str(), 0, NULL);
        float content_width = icon_size + 5.f + text_sz.x;
        float content_start_x = pos.x + (tab_size.x - content_width) / 2.f;


        ImVec2 icon_center = ImVec2(content_start_x + icon_size * 0.5f, pos.y + tab_size.y * 0.5f);
        float icon_alpha = 170.f + 85.f * effect;
        draw_tab_icon(draw, icon_center, icon_size, label, icon_alpha, true);


        ImVec2 text_pos = ImVec2(content_start_x + icon_size + 5.f, pos.y + (tab_size.y - 15.f) / 2.f);
        draw->AddText(f::sbold16, 15.f, text_pos + ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, 170), label.c_str());
        draw->AddText(f::sbold16, 15.f, text_pos, IM_COL32(255, 255, 255, (int)(190 + 65 * effect)), label.c_str());

        ::GetCurrentWindow()->DC.CursorPos = pos;
        bool ret = ::InvisibleButton((label + "##subtab").c_str(), tab_size);
        curr->active = active;
        return ret;
    }
    void bar()
    {
        auto draw = ::GetWindowDrawList();
        const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
        const ImVec2& region = ::GetContentRegionMax() - ImVec2(2.f, 2.f);


        float bar_height = 50.f;


        float logo_height = 40.f;
        float logo_aspect = 1.f;
        float logo_width = logo_height * logo_aspect;
        ImVec2 logo_pos = p + ImVec2(12.f, (bar_height - logo_height) / 2.f);

        if (tex::transparent_logo != nullptr) {
            draw->AddImage(ImTextureID(tex::transparent_logo), logo_pos, logo_pos + ImVec2(logo_width, logo_height));
        }

        int tab_count = (int)MGR->tabs.size();
        float logo_end_x = logo_width + 28.f;
        float available_width = region.x - logo_end_x - 10.f;
        float tab_spacing = 4.f;
        float total_spacing = tab_spacing * (tab_count - 1);
        float tab_width = (available_width - total_spacing) / (float)tab_count;
        tab_width = (std::min)(tab_width, 115.f);


        float total_tabs_width = tab_width * tab_count + total_spacing;
        float tabs_start_x = logo_end_x + (available_width - total_tabs_width) / 2.f;

        ImVec2 tab_pos = p + ImVec2(tabs_start_x, (bar_height - 32.f) / 2.f);

        for (int i = 0; i < tab_count; i++)
        {
            auto& element = MGR->tabs.at(i);
            if (tab(tab_pos, element.label, element.icon.c_str(), screen == i, tab_width)) {
                if (screen != i) {

                    datax.clear();
                    for (auto& [key, val] : anim::item_animations) {
                        val.val = 0.0f;
                    }
                }
                screen = i;
            }
            tab_pos.x += tab_width + tab_spacing;
        }


        draw->AddRectFilled(ImVec2(p.x, p.y + bar_height), ImVec2(p.x + region.x, p.y + bar_height + 1.f), c::white4);


        auto& active_tab = MGR->tabs.at(screen);
        if (active_tab.has_subs()) {
            float sub_bar_y = bar_height + 1.f;
            float sub_bar_height = 40.f;


            draw->AddRectFilled(
                ImVec2(p.x, p.y + sub_bar_y),
                ImVec2(p.x + region.x, p.y + sub_bar_y + sub_bar_height),
                IM_COL32(0, 0, 0, 100));

            int sub_count = (int)active_tab.sub_tabs.size();
            float sub_spacing = 6.f;
            float sub_total_spacing = sub_spacing * (sub_count - 1);
            float sub_tab_width = (region.x - 40.f - sub_total_spacing) / (float)sub_count;
            sub_tab_width = (std::min)(sub_tab_width, 200.f);

            float sub_total_width = sub_tab_width * sub_count + sub_total_spacing;
            float sub_start_x = (region.x - sub_total_width) / 2.f;

            ImVec2 sub_pos = p + ImVec2(sub_start_x, sub_bar_y + (sub_bar_height - 28.f) / 2.f);

            for (int j = 0; j < sub_count; j++) {
                auto& sub = active_tab.sub_tabs[j];
                if (sub_tab(sub_pos, sub.label, sub.icon.c_str(), active_tab.active_sub == j, sub_tab_width)) {
                    if (MGR->tabs.at(screen).active_sub != j) {
                        datax.clear();
                        for (auto& [key, val] : anim::item_animations) {
                            val.val = 0.0f;
                        }
                    }
                    MGR->tabs.at(screen).active_sub = j;
                }
                sub_pos.x += sub_tab_width + sub_spacing;
            }


            draw->AddRectFilled(
                ImVec2(p.x, p.y + sub_bar_y + sub_bar_height),
                ImVec2(p.x + region.x, p.y + sub_bar_y + sub_bar_height + 1.f),
                c::white4);
        }
    }
    void end() override
    {
        auto draw = ::GetWindowDrawList();
        const auto& p = ::GetWindowPos() + ImVec2(1.f, 1.f);
        const ImVec2& region = ::GetContentRegionMax() - ImVec2(2.f, 2.f);


        ImVec2 size = ImVec2(320.f, 180.f);

        ImVec2 center_pos = ImVec2((region.x - size.x) / 2.f, (region.y - size.y) / 2.f);
        ImVec2 pos = ImVec2(center_pos.x, -size.y);
        if (MGR->notify)
            pos = center_pos;

        static elastic_point point(pos.y, 0.3f, 0.8f, 1.2f);
        point.set_target(pos.y); point.update(0.16f);
        if (point.get_pos() > -size.y)
        {
            ImGuiWindow* window = ::GetCurrentWindow();

            ImVec2 render_pos = p + ImVec2(pos.x, point.get_pos());

            draw->PushClipRect(p, p + region, true);

            float mod = 1.f - ((148.f - std::clamp(point.get_pos(), 0.f, 148.f)) / 148.f);


            draw->AddRectFilled(p, p + region, a(ImColor(0, 0, 0, 255), 0.85f * mod), 16.f);

            draw->AddRectFilled(render_pos, render_pos + size, ImColor(25, 27, 33, 255), 16.f);

            draw->AddRectFilled(render_pos + ImVec2(1, 1), render_pos + size - ImVec2(1, 1), ImColor(35, 37, 43, 255), 15.f);


            draw->AddRect(render_pos, render_pos + size, ImColor(255, 255, 255, 100), 16.f, 0, 1.5f);

            std::string icon_text = "✓";
            ImColor prim = c::primary;
            ImColor sec = c::secondary;
            c::highlight_t highlight = c::highlight;
            switch (MGR->notify_status)
            {
            case 0:
                icon_text = "i"; prim = c::info_primary; sec = c::info_secondary; highlight.f = c::info_highlight.f; highlight.s = c::info_highlight.s;
                break;
            case 1:
                icon_text = "✓"; prim = c::primary; sec = c::secondary; highlight.f = c::highlight.f; highlight.s = c::highlight.s;
                break;
            case 2:
                icon_text = "✗"; prim = c::error_primary; sec = c::error_secondary; highlight.f = c::error_highlight.f; highlight.s = c::error_highlight.s;
                break;
            default:
                break;
            }


            ImVec2 icon_center = render_pos + ImVec2(size.x / 2.f, 35.f);
            draw->AddCircleFilled(icon_center, 20.f, prim, 32);
            draw->AddCircle(icon_center, 20.f, ImColor(255, 255, 255, 80), 32, 2.0f);


            ImVec2 brandSize = f::medium12->CalcTextSizeA(12.f, FLT_MAX, -1.f, globals::brand::signature, 0, NULL);
            ImVec2 brandPos = render_pos + ImVec2((size.x - brandSize.x) * 0.5f, 12.f);
            draw->AddText(f::medium12, 12.f, brandPos + ImVec2(1.f, 1.f), ImColor(0, 0, 0, 140), globals::brand::signature);
            draw->AddText(f::medium12, 12.f, brandPos, ImColor(255, 130, 200, 255), globals::brand::signature);


            ImVec2 icon_text_size = ImGui::CalcTextSize(icon_text.c_str());
            draw->AddText(ImVec2(icon_center.x - icon_text_size.x / 2.f, icon_center.y - icon_text_size.y / 2.f),
                         ImColor(255, 255, 255, 255), icon_text.c_str());


            float offset_y = 0.f;
            ImVec2 desc_pos = render_pos + ImVec2(size.x / 2.f, 75.f);
            for (auto& desc : MGR->notify_desc)
            {
                ImVec2 text_size = f::medium12->CalcTextSizeA(14.f, FLT_MAX, -1.f, desc.c_str(), 0, NULL);

                draw->AddText(f::medium12, 14.f, desc_pos + ImVec2(-text_size.x / 2.f + 1.f, offset_y + 1.f),
                    ImColor(0, 0, 0, 180), desc.c_str());
                draw->AddText(f::medium12, 14.f, desc_pos + ImVec2(-text_size.x / 2.f, offset_y),
                    ImColor(255, 255, 255, 255), desc.c_str());
                offset_y += 16.f;
            }


            RESTORE(c::primary);
            RESTORE(c::secondary);
            RESTORE(c::highlight);
            c::primary = prim;
            c::secondary = sec;
            c::highlight.f = highlight.f; c::highlight.s = highlight.s;


            ImVec2 button_size = ImVec2(280.f, 36.f);
            ImVec2 button_pos = render_pos + ImVec2((size.x - button_size.x) / 2.f, 130.f);


            bool button_hovered = IsMouseHoveringRect(button_pos, button_pos + button_size);
            ImColor button_bg = button_hovered ? prim : ImColor(60, 62, 68, 255);
            ImColor button_border = button_hovered ? ImColor(255, 255, 255, 150) : prim;


            draw->AddRectFilled(button_pos, button_pos + button_size, button_bg, 6.f);
            draw->AddRect(button_pos, button_pos + button_size, button_border, 6.f, 0, 1.5f);


            ImVec2 text_size = f::medium14->CalcTextSizeA(16.f, FLT_MAX, -1.f, "Continue", 0, NULL);
            ImVec2 text_pos = button_pos + ImVec2((button_size.x - text_size.x) / 2.f, (button_size.y - 16.f) / 2.f);


            draw->AddText(f::medium14, 16.f, text_pos + ImVec2(1, 1), ImColor(0, 0, 0, 120), "Continue");

            draw->AddText(f::medium14, 16.f, text_pos, ImColor(255, 255, 255, 255), "Continue");


            if (button_hovered && GetIO().MouseClicked[0] && !MGR->close_notify)
                MGR->close_notify = true;

            draw->PopClipRect();
        }


        if (MGR->close_notify) {
            MGR->notify = false;
            MGR->close_notify = false;
            MGR->notify_desc.clear();

            RESTORE(c::primary);
            RESTORE(c::secondary);
            RESTORE(c::highlight);
        }


        draw->AddRect(ImVec2(p.x, p.y), ImVec2(p.x + region.x, p.y + region.y), c::white6, 16.f);

        ::End();
    }
    void destroy() override
    {
        delete this;
    }
private:
    int screen = 0;
    int next_screen = 0;
    ID3D11Device* g_pd3dDevice = NULL;
};

c_menu* create_menu()
{
    return new _c_menu();
}
