#pragma once
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "../Cheat/Brand.hpp"
REVERS_STAMP(Overlay_hpp)
#include "../Cheat/Globals.hpp"
#include "../Cheat/Log.hpp"
#include "../Cheat/TextureLoader.hpp"
#include "../Gui/menu/defs/textures.h"
#include "../Gui/menu/defs/fonts.h"
#include "../Gui/menu/bytes/sounds.h"

void DrawAlways();
void DrawEsp();
void vehicleEsp();
void drawInfoBox();
void drawActiveFeatures();


static bool g_loadingScreenShown = false;

struct OverlayStruct {
    HWND hWnd;
    WNDCLASSEX wndClassEx;
    LPCSTR lpClassName = "Spotify";
    LPCSTR lpWindowName = "Spotify";
    ID3D11Device* pDevice;
    ID3D11DeviceContext* pDeviceContext;
    IDXGISwapChain* pSwapChain;
    ID3D11RenderTargetView* pRenderTargetView;
    POINT WindowSize;
} Overlay;


#include "../../Libraries/imgui/include/ImGui/TransparentLogo.hpp"


void ShowWelcomeScreen() {
    if (Cheats::Misc::streamProof && Overlay.hWnd) {
        SetWindowDisplayAffinity(Overlay.hWnd, 0x00000011);
    }

    ID3D11ShaderResourceView* logoTexture = nullptr;
    int logoWidth = 0, logoHeight = 0;

    UINT loadedWidth = 0;
    UINT loadedHeight = 0;
    HRESULT hr = CreateShaderResourceViewFromMemory(
        Overlay.pDevice,
        TransparentLogo,
        sizeof(TransparentLogo),
        &logoTexture,
        &loadedWidth,
        &loadedHeight
    );

    if (SUCCEEDED(hr) && logoTexture) {
        logoWidth = static_cast<int>(loadedWidth);
        logoHeight = static_cast<int>(loadedHeight);
    }

    const float totalDuration = 4.2f;
    const float progressDuration = 3.4f;
    float animationTime = 0.0f;

    auto startTime = std::chrono::high_resolution_clock::now();
    ImGuiIO& io = ImGui::GetIO();

    auto easeOutCubic = [](float t) { return 1.0f - powf(1.0f - t, 3.0f); };
    auto easeOutBack = [](float t) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);
    };
    auto clampf = [](float v, float a, float b) { return (v < a) ? a : ((v > b) ? b : v); };

    while (animationTime < totalDuration) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        animationTime = std::chrono::duration<float>(currentTime - startTime).count();

        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImVec2 screenSize = io.DisplaySize;
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        ImVec2 center = ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f);
        float time = animationTime;

        float fadeOut = 1.0f;
        if (animationTime > totalDuration - 0.55f)
            fadeOut = clampf((totalDuration - animationTime) / 0.55f, 0.0f, 1.0f);

        float progress = clampf(animationTime / progressDuration, 0.0f, 1.0f);
        float easedProgress = easeOutCubic(progress);

        float logoIn = easeOutBack(clampf(animationTime / 0.95f, 0.0f, 1.0f));
        float logoAlpha = clampf(animationTime / 0.55f, 0.0f, 1.0f) * fadeOut;
        float uiIn = easeOutCubic(clampf((animationTime - 0.35f) / 0.7f, 0.0f, 1.0f)) * fadeOut;


        drawList->AddRectFilled(ImVec2(0, 0), screenSize, IM_COL32(4, 4, 8, 255));


        drawList->AddRectFilledMultiColor(
            ImVec2(0, 0), screenSize,
            IM_COL32(0, 0, 0, 180), IM_COL32(0, 0, 0, 180),
            IM_COL32(0, 0, 0, 210), IM_COL32(0, 0, 0, 210));
        const float vig = 220.0f;
        drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(vig, screenSize.y),
            IM_COL32(0, 0, 0, 160), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 160));
        drawList->AddRectFilledMultiColor(ImVec2(screenSize.x - vig, 0), screenSize,
            IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 160), IM_COL32(0, 0, 0, 160), IM_COL32(0, 0, 0, 0));
        drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(screenSize.x, vig),
            IM_COL32(0, 0, 0, 140), IM_COL32(0, 0, 0, 140), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0));
        drawList->AddRectFilledMultiColor(ImVec2(0, screenSize.y - vig), screenSize,
            IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 160), IM_COL32(0, 0, 0, 160));


        for (int p = 0; p < 28; ++p) {
            float seed = p * 17.313f;
            float px = fmodf(seed * 37.1f + time * (12.0f + fmodf(seed, 9.0f)), screenSize.x);
            float py = fmodf(seed * 53.7f - time * (8.0f + fmodf(seed * 0.3f, 7.0f)) + screenSize.y * 2.0f, screenSize.y);
            float size = 1.2f + fmodf(seed, 2.4f);
            float twinkle = 0.35f + 0.65f * (0.5f + 0.5f * sinf(time * 2.4f + seed));
            int a = (int)(55.0f * twinkle * fadeOut);
            drawList->AddCircleFilled(ImVec2(px, py), size, IM_COL32(255, 140, 200, a), 8);
        }


        if (logoTexture && logoWidth > 0 && logoHeight > 0) {
            float targetSize = 220.0f;
            float aspect = (float)logoWidth / (float)logoHeight;
            float displayHeight = targetSize;
            float displayWidth = targetSize * aspect;

            float breath = 1.0f + 0.025f * sinf(time * 2.2f);
            float scale = (0.72f + 0.28f * logoIn) * breath;
            displayWidth *= scale;
            displayHeight *= scale;


            ImVec2 groupCenter = ImVec2(center.x, center.y - 78.0f);
            ImVec2 logoPos = ImVec2(groupCenter.x - displayWidth * 0.5f, groupCenter.y - displayHeight * 0.5f);
            ImVec2 logoEnd = ImVec2(logoPos.x + displayWidth, logoPos.y + displayHeight);
            ImVec2 logoCenter = ImVec2(logoPos.x + displayWidth * 0.5f, logoPos.y + displayHeight * 0.5f);


            float ringR = displayWidth * 0.58f;
            float ringAlpha = logoAlpha * 0.55f;
            drawList->AddCircle(logoCenter, ringR, IM_COL32(255, 105, 180, (int)(40 * ringAlpha)), 64, 1.5f);
            for (int o = 0; o < 3; ++o) {
                float ang = time * (1.1f + o * 0.35f) + o * 2.1f;
                ImVec2 op = ImVec2(logoCenter.x + cosf(ang) * ringR, logoCenter.y + sinf(ang) * ringR);
                drawList->AddCircleFilled(op, 2.5f + o * 0.4f, IM_COL32(255, 160, 210, (int)(160 * ringAlpha)), 10);
            }

            drawList->AddImage(
                (ImTextureID)logoTexture,
                logoPos, logoEnd,
                ImVec2(0, 0), ImVec2(1, 1),
                IM_COL32(255, 255, 255, (int)(255 * logoAlpha))
            );
        }


        if (uiIn > 0.01f) {
            float barWidth = 360.0f;
            float barHeight = 4.0f;
            ImVec2 barPos = ImVec2(center.x - barWidth * 0.5f, center.y + 88.0f);

            const char* welcomeText = globals::brand::welcome;
            ImVec2 welcomeSize = ImGui::CalcTextSize(welcomeText);
            ImVec2 welcomePos = ImVec2(center.x - welcomeSize.x * 0.5f, barPos.y - 28.0f);
            drawList->AddText(welcomePos, IM_COL32(255, 150, 210, (int)(255 * uiIn)), welcomeText);

            drawList->AddRectFilled(barPos, ImVec2(barPos.x + barWidth, barPos.y + barHeight),
                IM_COL32(255, 255, 255, (int)(18 * uiIn)), 2.0f);

            float fillWidth = barWidth * easedProgress;
            if (fillWidth > 0.5f) {
                ImVec2 fillEnd = ImVec2(barPos.x + fillWidth, barPos.y + barHeight);
                drawList->AddRectFilled(barPos, fillEnd,
                    IM_COL32(255, 70, 160, (int)(255 * uiIn)), 2.0f);


                ImVec2 tip = ImVec2(fillEnd.x, barPos.y + barHeight * 0.5f);
                float tipPulse = 0.5f + 0.5f * sinf(time * 6.0f);
                for (int g = 3; g >= 1; --g) {
                    drawList->AddCircleFilled(tip, 3.0f + g * 3.0f * tipPulse,
                        IM_COL32(255, 140, 200, (int)((28.0f / g) * uiIn * tipPulse)), 12);
                }


                for (int s = 0; s < 8; ++s) {
                    float life = fmodf(time * 1.3f + s * 0.28f, 1.0f);
                    float sx = barPos.x + fillWidth * (0.08f + s * 0.11f);
                    if (sx > fillEnd.x) continue;
                    float sy = barPos.y - life * 28.0f;
                    int a = (int)((1.0f - life) * 140.0f * uiIn);
                    drawList->AddCircleFilled(ImVec2(sx, sy), 1.4f + (1.0f - life),
                        IM_COL32(255, 160, 210, a), 8);
                }
            }


            const char* status = "Initializing";
            if (easedProgress > 0.28f) status = "Loading modules";
            if (easedProgress > 0.58f) status = "Preparing overlay";
            if (easedProgress > 0.85f) status = "Almost ready";
            if (easedProgress >= 0.999f) status = "Ready";

            char progressText[48];
            sprintf_s(progressText, "%s  ·  %.0f%%", status, easedProgress * 100.0f);
            ImVec2 progressSize = ImGui::CalcTextSize(progressText);
            ImVec2 progressPos = ImVec2(center.x - progressSize.x * 0.5f, barPos.y + 18.0f);
            drawList->AddText(progressPos, IM_COL32(210, 210, 220, (int)(200 * uiIn)), progressText);
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        Overlay.pDeviceContext->OMSetRenderTargets(1, &Overlay.pRenderTargetView, NULL);
        Overlay.pDeviceContext->ClearRenderTargetView(Overlay.pRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        Overlay.pSwapChain->Present(1, 0);
    }

    if (logoTexture) {
        logoTexture->Release();
    }
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);






static std::atomic<bool> g_menuKeyPressed{ false };


#define HOTKEY_MENU     1
#define HOTKEY_RSHIFT   2
#define HOTKEY_F4       3
#define HOTKEY_PGUP     4
#define HOTKEY_PGDN     5

static void RegisterBlockedKeys(HWND overlayHwnd) {
    RegisterHotKey(overlayHwnd, HOTKEY_MENU, 0, Cheats::menuKey);
    RegisterHotKey(overlayHwnd, HOTKEY_RSHIFT, 0, VK_RSHIFT);
    RegisterHotKey(overlayHwnd, HOTKEY_F4, 0, VK_F4);
    RegisterHotKey(overlayHwnd, HOTKEY_PGUP, 0, VK_PRIOR);
    RegisterHotKey(overlayHwnd, HOTKEY_PGDN, 0, VK_NEXT);
}

static void UnregisterBlockedKeys(HWND overlayHwnd) {
    UnregisterHotKey(overlayHwnd, HOTKEY_MENU);
    UnregisterHotKey(overlayHwnd, HOTKEY_RSHIFT);
    UnregisterHotKey(overlayHwnd, HOTKEY_F4);
    UnregisterHotKey(overlayHwnd, HOTKEY_PGUP);
    UnregisterHotKey(overlayHwnd, HOTKEY_PGDN);
}

void OverlayMain() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    initializeMenu();
    ImGui_ImplWin32_Init(Overlay.hWnd);
    ImGui_ImplDX11_Init(Overlay.pDevice, Overlay.pDeviceContext);

    c_menu* menu = create_menu();
    menu->setup_data(Overlay.pDevice);
    menu->setup_imgui();

    MGR->esp_p = false;
    MGR->add_tab("Aim Assist", "a", std::vector<sub_tab_t>{
        sub_tab_t("Silent", "a", silent_tab),
        sub_tab_t("Aimbot", "b", aimbot_tab),
        sub_tab_t("Triggerbot", "d", trigger_tab)
    });
    MGR->add_tab("Visuals", "e", std::vector<sub_tab_t>{
        sub_tab_t("ESP", "e", esp_tab),
        sub_tab_t("World", "f", world_tab),
        sub_tab_t("Vehicle", "g", vehicle_tab)
    });
    MGR->add_tab("Misc", "h", misc_tab);
    MGR->add_tab("Settings", "i", settings_tab);


    if (!g_loadingScreenShown) {
        ShowWelcomeScreen();
        g_loadingScreenShown = true;


        PlaySoundA((LPCSTR)checkbox_sound, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
    }


    ImGui::GetIO().MouseDrawCursor = true;


    RegisterBlockedKeys(Overlay.hWnd);

    while (!exitLoop) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {

            if (msg.message == WM_HOTKEY) {
                if (msg.wParam == HOTKEY_MENU) {
                    g_menuKeyPressed.store(true);
                }

                continue;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        if (g_menuKeyPressed.load()) {
            g_menuKeyPressed.store(false);

            isMenuVisible = !isMenuVisible;
            ImGui::GetIO().MouseDrawCursor = isMenuVisible;
            if (isMenuVisible) {

                SetWindowLong(Overlay.hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);

                DWORD foreThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
                DWORD ourThread = GetCurrentThreadId();
                if (foreThread != ourThread) {
                    AttachThreadInput(foreThread, ourThread, TRUE);
                    SetForegroundWindow(Overlay.hWnd);
                    SetFocus(Overlay.hWnd);
                    AttachThreadInput(foreThread, ourThread, FALSE);
                } else {
                    SetForegroundWindow(Overlay.hWnd);
                    SetFocus(Overlay.hWnd);
                }
            } else {

                SetWindowLong(Overlay.hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
                SetForegroundWindow(Game.hWnd);
            }
            UpdateWindow(Overlay.hWnd);
        }


        if (isMenuVisible) {
            static DWORD lastFocusCheck = 0;
            DWORD now = GetTickCount();
            if (now - lastFocusCheck > 500) {
                lastFocusCheck = now;
                HWND fg = GetForegroundWindow();
                if (fg == Game.hWnd) {
                    DWORD foreThread = GetWindowThreadProcessId(fg, NULL);
                    DWORD ourThread = GetCurrentThreadId();
                    if (foreThread != ourThread) {
                        AttachThreadInput(foreThread, ourThread, TRUE);
                        SetForegroundWindow(Overlay.hWnd);
                        SetFocus(Overlay.hWnd);
                        AttachThreadInput(foreThread, ourThread, FALSE);
                    } else {
                        SetForegroundWindow(Overlay.hWnd);
                        SetFocus(Overlay.hWnd);
                    }
                }
            }
        }

        if (GetAsyncKeyState(Cheats::Misc::NoClipKey) & 1) {
            Cheats::Misc::NoClip = !Cheats::Misc::NoClip;
        }

        if (GetAsyncKeyState(Cheats::Aim::dynamicFovKey) & 1) {
            Cheats::Aim::dynamic_fov_on = !Cheats::Aim::dynamic_fov_on;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        DrawAlways();
        DrawEsp();
        vehicleEsp();
        drawInfoBox();
        drawActiveFeatures();

        if (isMenuVisible) {
            menu->draw();
            menu->end();
        }


        Notification::Render();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        Overlay.pDeviceContext->OMSetRenderTargets(1, &Overlay.pRenderTargetView, NULL);
        Overlay.pDeviceContext->ClearRenderTargetView(Overlay.pRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        Overlay.pSwapChain->Present(0, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();


    UnregisterBlockedKeys(Overlay.hWnd);
}
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
void UpdateOverlay() {
    while (!exitLoop) {
        Sleep(3000);
        if (!Process::Check(Game.pID)) {
            break;
        }

        Game.hWnd = FindWindow(Game.lpClassName, NULL);
        if (!Game.hWnd) {
            break;
        }
        DWORD currentAffinity = NULL;
        GetWindowDisplayAffinity(Overlay.hWnd, &currentAffinity);
        if (Cheats::Misc::streamProof && currentAffinity == WDA_NONE) {
            SetWindowDisplayAffinity(Overlay.hWnd, WDA_EXCLUDEFROMCAPTURE);
        }
        else if (!Cheats::Misc::streamProof && currentAffinity == WDA_EXCLUDEFROMCAPTURE) {
            SetWindowDisplayAffinity(Overlay.hWnd, WDA_NONE);
        }
        RECT currentRect{};
        POINT currentPoint{};
        GetClientRect(Game.hWnd, &currentRect);
        ClientToScreen(Game.hWnd, &currentPoint);
        if (currentRect.left != Game.lpRect.left || currentRect.right != Game.lpRect.right || currentRect.top != Game.lpRect.top || currentRect.bottom != Game.lpRect.bottom || currentPoint.x != Game.lpPoint.x || currentPoint.y != Game.lpPoint.y) {
            Game.lpRect = currentRect;
            Game.lpPoint = currentPoint;
            SetWindowPos(Overlay.hWnd, nullptr, currentPoint.x, currentPoint.y, currentRect.right - currentRect.left, currentRect.bottom - currentRect.top, SWP_NOREDRAW);
        }
    }
}

bool CreateOverlay() {
    WNDCLASSEX wndClassEx;
    ZeroMemory(&wndClassEx, sizeof(wndClassEx));
    wndClassEx.cbSize = sizeof(WNDCLASSEX);
    wndClassEx.lpfnWndProc = WndProc;
    wndClassEx.hInstance = GetModuleHandle(NULL);
    wndClassEx.lpszClassName = Overlay.lpClassName;
    wndClassEx.lpszMenuName = Overlay.lpWindowName;
    wndClassEx.hCursor = NULL;
    wndClassEx.hbrBackground = NULL;
    wndClassEx.hIcon = NULL;
    wndClassEx.hIconSm = NULL;
    wndClassEx.cbClsExtra = 0;
    wndClassEx.cbWndExtra = 0;
    wndClassEx.style = 0;


    UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);

    if (!RegisterClassEx(&wndClassEx)) {
        CheatLogger::WriteLog("Window class registration failed!", CheatLogger::LogLevel::LOG_ERROR);
        return false;
    }


    for (int i = 0; i < 30; i++) {
        GetClientRect(Game.hWnd, &Game.lpRect);
        if (Game.lpRect.right > 0 && Game.lpRect.bottom > 0) break;
        Sleep(200);
    }

    GetClientRect(Game.hWnd, &Game.lpRect);
    ClientToScreen(Game.hWnd, &Game.lpPoint);
    Overlay.WindowSize.x = Game.lpRect.right;
    Overlay.WindowSize.y = Game.lpRect.bottom;


    static HWND hOwner = CreateWindowEx(0, Overlay.lpClassName, NULL, WS_POPUP, 0, 0, 0, 0, NULL, NULL, wndClassEx.hInstance, NULL);

    Overlay.hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, Overlay.lpClassName, Overlay.lpWindowName, WS_POPUP | WS_VISIBLE, Game.lpPoint.x, Game.lpPoint.y, Game.lpRect.right - Game.lpRect.left, Game.lpRect.bottom - Game.lpRect.top, hOwner, NULL, wndClassEx.hInstance, NULL);

    if (!Overlay.hWnd) {
        CheatLogger::WriteLog("Window creation failed!", CheatLogger::LogLevel::LOG_ERROR);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }

    if (!SetLayeredWindowAttributes(Overlay.hWnd, RGB(0, 0, 0), 255, LWA_ALPHA)) {
        CheatLogger::WriteLog("Layered window attributes setup failed!", CheatLogger::LogLevel::LOG_ERROR);
        DestroyWindow(Overlay.hWnd);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }

    MARGINS margin = { -1 };
    if (DwmExtendFrameIntoClientArea(Overlay.hWnd, &margin) != S_OK) {
        CheatLogger::WriteLog("Failed to extend frame into client area!", CheatLogger::LogLevel::LOG_ERROR);
        DestroyWindow(Overlay.hWnd);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = Overlay.hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &Overlay.pSwapChain, &Overlay.pDevice, &featureLevel, &Overlay.pDeviceContext);
    if (FAILED(hResult)) {
        CheatLogger::WriteLog("Direct3D device and swap chain creation failed!", CheatLogger::LogLevel::LOG_ERROR);
        DestroyWindow(Overlay.hWnd);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }

    ID3D11Texture2D* pBackBuffer;
    hResult = Overlay.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (FAILED(hResult)) {
        CheatLogger::WriteLog("Failed to retrieve back buffer!", CheatLogger::LogLevel::LOG_ERROR);
        Overlay.pSwapChain->Release();
        DestroyWindow(Overlay.hWnd);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }

    hResult = Overlay.pDevice->CreateRenderTargetView(pBackBuffer, NULL, &Overlay.pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hResult)) {
        CheatLogger::WriteLog("Render target view creation failed!", CheatLogger::LogLevel::LOG_ERROR);
        Overlay.pSwapChain->Release();
        DestroyWindow(Overlay.hWnd);
        UnregisterClass(Overlay.lpClassName, wndClassEx.hInstance);
        return false;
    }


    SetWindowLong(Overlay.hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    ShowWindow(Overlay.hWnd, SW_SHOWDEFAULT);
    UpdateWindow(Overlay.hWnd);


    DWORD foreThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    DWORD ourThread = GetCurrentThreadId();
    if (foreThread != ourThread) {
        AttachThreadInput(foreThread, ourThread, TRUE);
        SetForegroundWindow(Overlay.hWnd);
        SetFocus(Overlay.hWnd);
        AttachThreadInput(foreThread, ourThread, FALSE);
    } else {
        SetForegroundWindow(Overlay.hWnd);
        SetFocus(Overlay.hWnd);
    }

    return true;
}

void DestroyOverlay() {
    if (Overlay.hWnd) {
        DestroyWindow(Overlay.hWnd);
        Overlay.hWnd = NULL;
    }

    if (Overlay.pRenderTargetView) {
        Overlay.pRenderTargetView->Release();
        Overlay.pRenderTargetView = NULL;
    }

    if (Overlay.pDeviceContext) {
        Overlay.pDeviceContext->Release();
        Overlay.pDeviceContext = NULL;
    }

    if (Overlay.pDevice) {
        Overlay.pDevice->Release();
        Overlay.pDevice = NULL;
    }

    if (Overlay.pSwapChain) {
        Overlay.pSwapChain->Release();
        Overlay.pSwapChain = NULL;
    }

    if (Overlay.lpClassName) {
        UnregisterClass(Overlay.lpClassName, GetModuleHandle(NULL));
        Overlay.lpClassName = NULL;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}