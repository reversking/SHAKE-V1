#include <mjLib/mjLib.hpp>
#include <mutex>
using namespace mjLib;

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static std::atomic<bool> exitLoop = false;
static std::atomic<bool> isMenuVisible = true;
std::vector<std::thread> activeThreads;
std::mutex threadMutex;

#include "Gui/menu/interface/menu_i.h"
#include "Gui/menu/interface/elements_manager.h"
#include "Gui/menu/helpers/widgets.h"
#include "Cheat/SimpleMath.h"
#include "Cheat/Game.hpp"
#include "Cheat/Memory.hpp"
#include "Cheat/Globals.hpp"
#include "Cheat/Sounds.hpp"
#include "Cheat/Particles.hpp"
#include "Cheat/Print.hpp"
#include "Cheat/EntityList.hpp"
#include "Cheat/Features.hpp"
#include "Menu.hpp"
#include "Overlay/Overlay.hpp"
#include "Cheat/Log.hpp"
#include "Cheat/Cheat.hpp"
#include "Cheat/ConsoleBranding.hpp"
#include "Cheat/Brand.hpp"
REVERS_STAMP(Source_cpp)

GameStruct Game;
OffsetsStruct Offsets;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void UnloadCheat() {
    ConsoleBranding::Line("  Shutting down...");
    CheatLogger::WriteLog(std::string(globals::brand::signature) + " | " + globals::brand::createdBy + " shutting down", CheatLogger::LogLevel::LOG_INFO);
    exitLoop = true;
    DestroyOverlay();
    {
        std::lock_guard<std::mutex> lock(threadMutex);
        for (auto& thread : activeThreads) {
            if (thread.joinable()) {
                thread.detach();
            }
        }
        activeThreads.clear();
    }
    TerminateProcess(GetCurrentProcess(), 0);
}

template<typename Func>
void CreateManagedThread(Func&& func) {
    std::lock_guard<std::mutex> lock(threadMutex);
    activeThreads.emplace_back(std::forward<Func>(func));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)globals::brand::codeSignature;
    ConsoleBranding::Banner();
    CheatLogger::WriteLog(std::string(globals::brand::signature) + " | " + globals::brand::createdBy + " initialized", CheatLogger::LogLevel::LOG_SUCCESS);
    ConsoleBranding::Status("Signature", globals::brand::fullTitle);

    ConsoleBranding::WaitingForTarget();
    while (!Game.pID) {
        Game.pID = FindGame();
        if (!Game.pID) Sleep(500);
    }
    ConsoleBranding::FoundTarget(Game.pID);

    Sleep(500);
    ConsoleBranding::Attaching();
    Game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Game.pID);
    if (!Game.hProcess) {
        ConsoleBranding::Failed("Could not open FiveM process. Run as Administrator.");
        Sleep(3000);
        return 0;
    }

    Game.Path = GetGamePath();
    if (Game.Path.empty()) {
        ConsoleBranding::Failed("Could not read FiveM path.");
        Sleep(3000);
        return 0;
    }

    CreateManagedThread([]() {
        while (!exitLoop) {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(Game.hProcess, &exitCode) && exitCode != STILL_ACTIVE) {
                ConsoleBranding::Failed("FiveM closed.");
                UnloadCheat();
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    Offsets.GameBase = GetBaseAddress();

    const char* versions[] = { "3751", "3570", "3407", "3323", "3258", "3095", "2944", "2802", "2699" };
    bool found = false;

    for (const char* version : versions) {
        Game.Version = version;
        ReadOffsets();
        uintptr_t GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
        uintptr_t GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
        uintptr_t GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
        if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
            found = true;
            break;
        }
    }

    if (found) {
        std::vector<uint8_t> vehicleDoorsLockPattern = { 0x74, 0x00, 0x83, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x00, 0x45, 0x84, 0xf6 };
        uintptr_t VehicleDoorsLockAddr = MemoryScanner::FindSignature(vehicleDoorsLockPattern);
        if (VehicleDoorsLockAddr != 0) {
            Offsets.VehicleDoorsLockState = ReadMemory<int>(VehicleDoorsLockAddr + 4);
        }
    }
    else {
        ConsoleBranding::Failed("Unsupported FiveM build.");
        Sleep(3000);
        return 0;
    }

    ConsoleBranding::Attached(Game.Version.c_str());

    while (!Game.hWnd) {
        Game.hWnd = FindWindow(Game.lpClassName, NULL);
        Sleep(100);
    }

    Offsets.GameBase = GetBaseAddress();
    game::hwnd = Game.hWnd;
    game::base = Offsets.GameBase;
    game::world = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
    if (game::world) {
        game::local_player = ReadMemory<uintptr_t>(game::world + Offsets.LocalPlayer);
        if (game::local_player) {
            game::local_position = ReadMemory<Vector3>(game::local_player + 0x90);
            game::view_angles = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
        }
    }

    CreateManagedThread([=]() { while (!exitLoop) { UpdateOverlay(); std::this_thread::sleep_for(std::chrono::milliseconds(16)); } });
    CreateManagedThread([=]() { UpdatePeds(); });
    CreateManagedThread([=]() { while (!exitLoop) { SetAim(); std::this_thread::sleep_for(std::chrono::milliseconds(16)); } });
    CreateManagedThread([=]() { while (!exitLoop) { TriggerBot(); std::this_thread::sleep_for(std::chrono::milliseconds(16)); } });
    CreateManagedThread([=]() { while (!exitLoop) { MiscFeatures(); std::this_thread::sleep_for(std::chrono::milliseconds(50)); } });
    CreateManagedThread([=]() { AddPlayerList(); });
    CreateManagedThread([=]() { AddVehicleList(); });
    CreateManagedThread([=]() { GetPlayerNames::UpdatePlayerNames(); });
    CreateManagedThread([=]() { features::update_game_variables(); });
    CreateManagedThread([=]() { features::hitlog(); });

    bool overlayCreated = false;
    for (int attempt = 0; attempt < 5 && !overlayCreated; attempt++) {
        if (attempt > 0) Sleep(1000);
        overlayCreated = CreateOverlay();
    }

    if (overlayCreated) {
        ConsoleBranding::Line("  [+] Menu ready. Press INSERT in-game.");
        ConsoleBranding::Status("Revers", globals::brand::createdBy);
        OverlayMain();
    }
    else {
        ConsoleBranding::Failed("Overlay could not be created.");
    }

    DestroyOverlay();

    CreateManagedThread([=]() {
        while (!exitLoop) {
            if (GetAsyncKeyState(VK_F10) & 1) UnloadCheat();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    {
        std::lock_guard<std::mutex> lock(threadMutex);
        for (auto& thread : activeThreads) {
            if (thread.joinable()) thread.join();
        }
    }

    while (true);
}

int main() {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    ConsoleIcon::ApplyToConsole();
    return WinMain(NULL, NULL, NULL, NULL);
}
