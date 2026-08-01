#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "Brand.hpp"
REVERS_STAMP(Sounds_hpp)

namespace sounds
{

    inline void play_hit_sound(int sound_id = 0)
    {
        CreateThread(nullptr, 0, [](LPVOID param) -> DWORD {
            int id = (int)(uintptr_t)param;
            switch (id) {
            case 0: Beep(800, 80); break;
            case 1: Beep(1000, 80); break;
            case 2: Beep(900, 80); break;
            case 3: Beep(1100, 80); break;
            default: Beep(800, 80); break;
            }
            return 0;
            }, (LPVOID)(uintptr_t)sound_id, 0, nullptr);
    }

    inline void play_kill_sound(int sound_id = 0)
    {
        CreateThread(nullptr, 0, [](LPVOID param) -> DWORD {
            int id = (int)(uintptr_t)param;
            switch (id) {
            case 0:
                Beep(600, 100);
                Sleep(50);
                Beep(500, 100);
                Sleep(50);
                Beep(400, 150);
                break;
            case 1:
                Beep(1200, 100);
                Sleep(50);
                Beep(1000, 100);
                Sleep(50);
                Beep(800, 150);
                break;
            case 2:
                Beep(700, 80);
                Sleep(40);
                Beep(600, 80);
                Sleep(40);
                Beep(500, 120);
                break;
            default:
                Beep(1200, 200);
                break;
            }
            return 0;
            }, (LPVOID)(uintptr_t)sound_id, 0, nullptr);
    }

    inline LPCSTR get_sound_by_id(int sound_id)
    {
        return nullptr;
    }
}