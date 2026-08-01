#pragma once
#include <windows.h>
#include <cstdio>
#include "Brand.hpp"
REVERS_STAMP(ConsoleBranding_hpp)
#include "ConsoleIcon.hpp"

namespace ConsoleBranding
{
    inline void Enable()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &mode))
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

        SetConsoleTitleA(globals::brand::consoleTitle);
        ConsoleIcon::ApplyToConsole();
    }

    inline void Line(const char* text)
    {
        printf("\033[38;5;213m%s\033[0m\n", text);
    }

    inline void Status(const char* label, const char* value)
    {
        printf("\033[38;5;245m  [%s] \033[38;5;218m%s\033[0m\n", label, value);
    }

    inline void Banner()
    {
        Enable();
        printf("\n");
        printf("\033[38;5;205m  ============================================\033[0m\n");
        printf("\033[38;5;213m            %s\033[0m\n", globals::brand::signature);
        printf("\033[38;5;218m              %s\033[0m\n", globals::brand::createdBy);
        printf("\033[38;5;205m  ============================================\033[0m\n");
        printf("\033[38;5;245m              %s\033[0m\n", globals::brand::bannerLine);
        printf("\n");
    }

    inline void WaitingForTarget()
    {
        Line("  Waiting for FiveM...");
    }

    inline void FoundTarget(DWORD pid)
    {
        printf("\033[38;5;205m  [+] FiveM detected\033[38;5;245m  PID: \033[38;5;218m%lu\033[0m\n", pid);
    }

    inline void Attaching()
    {
        Line("  Attaching to FiveM process...");
    }

    inline void Attached(const char* version)
    {
        printf("\033[38;5;205m  [+] Successfully attached\033[0m\n");
        Status("Build", version);
        Status("Author", globals::brand::author);
        Status("Product", globals::brand::signature);
        Line("  Overlay starting...");
        printf("\n");
    }

    inline void Failed(const char* reason)
    {
        printf("\033[38;5;203m  [!] %s\033[0m\n", reason);
    }
}
