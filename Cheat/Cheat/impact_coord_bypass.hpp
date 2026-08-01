#pragma once
#include <cstdint>
#include <vector>
#include "Brand.hpp"
REVERS_STAMP(impact_coord_bypass_hpp)
#include "Memory.hpp"
#include "Game.hpp"



namespace impact_coord_bypass
{
    inline bool      g_initialized = false;
    inline bool      g_hooked      = false;
    inline uintptr_t g_nativeAddr  = 0;
    inline uintptr_t g_caveAddr    = 0;
    inline uint8_t   g_origBytes[12]{};

    inline constexpr size_t kHookSize = 12;

    inline bool findNative()
    {
        std::vector<uint8_t> pattern = Pattern2Vector("48 89 5C 24 ? 57 48 83 EC ? 33 DB 48 8B FA 48 89 5A");
        g_nativeAddr = MemoryScanner::FindSignature(pattern);
        if (!g_nativeAddr) {
            std::vector<uint8_t> alt = Pattern2Vector("48 89 5C 24 08 57 48 83 EC 20 33 DB 48 8B FA");
            g_nativeAddr = MemoryScanner::FindSignature(alt);
        }
        return g_nativeAddr != 0;
    }

    inline Vector3 normalizeDir(const Vector3& v)
    {
        float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        if (len < 0.001f) return Vector3(0.f, 1.f, 0.f);
        return Vector3(v.x / len, v.y / len, v.z / len);
    }

    inline Vector3 calcImpactPos(const Vector3& camPos, const Vector3& camDir, const Vector3& targetPos)
    {
        float dx = targetPos.x - camPos.x;
        float dy = targetPos.y - camPos.y;
        float dz = targetPos.z - camPos.z;
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if (dist < 0.001f) dist = 50.f;

        Vector3 dir = normalizeDir(camDir);
        return Vector3(
            camPos.x + dir.x * dist,
            camPos.y + dir.y * dist,
            camPos.z + dir.z * dist);
    }

    inline void writeShellcode(const Vector3& impact)
    {
        union { float f; uint32_t i; } posX, posY, posZ;
        posX.f = impact.x;
        posY.f = impact.y;
        posZ.f = impact.z;

        std::vector<uint8_t> shellcode = {
            0xC7, 0x02,
            static_cast<uint8_t>(posX.i & 0xFF),
            static_cast<uint8_t>((posX.i >> 8) & 0xFF),
            static_cast<uint8_t>((posX.i >> 16) & 0xFF),
            static_cast<uint8_t>((posX.i >> 24) & 0xFF),
            0xC7, 0x42, 0x04,
            static_cast<uint8_t>(posY.i & 0xFF),
            static_cast<uint8_t>((posY.i >> 8) & 0xFF),
            static_cast<uint8_t>((posY.i >> 16) & 0xFF),
            static_cast<uint8_t>((posY.i >> 24) & 0xFF),
            0xC7, 0x42, 0x08,
            static_cast<uint8_t>(posZ.i & 0xFF),
            static_cast<uint8_t>((posZ.i >> 8) & 0xFF),
            static_cast<uint8_t>((posZ.i >> 16) & 0xFF),
            static_cast<uint8_t>((posZ.i >> 24) & 0xFF),
            0xC7, 0x42, 0x0C, 0x00, 0x00, 0x00, 0x00,
            0xB0, 0x01,
            0xC3
        };
        WriteBytes(g_caveAddr, shellcode.data(), shellcode.size());
    }

    inline void installHook()
    {
        uint8_t hook[kHookSize] = { 0x48, 0xB8 };
        uint64_t cave = (uint64_t)g_caveAddr;
        for (int i = 0; i < 8; i++)
            hook[2 + i] = (uint8_t)((cave >> (i * 8)) & 0xFF);
        hook[10] = 0xFF;
        hook[11] = 0xE0;
        WriteBytes(g_nativeAddr, hook, kHookSize);
        g_hooked = true;
    }

    inline bool initialize()
    {
        if (g_initialized) return g_nativeAddr && g_caveAddr;

        if (!Offsets.GameBase || !Game.hProcess) {
            g_initialized = true;
            return false;
        }

        if (!findNative()) {
            g_initialized = true;
            return false;
        }

        g_caveAddr = (uintptr_t)VirtualAllocEx(Game.hProcess, NULL, 0x1000,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!g_caveAddr) {
            g_initialized = true;
            return false;
        }

        for (size_t i = 0; i < kHookSize; i++)
            g_origBytes[i] = ReadMemory<uint8_t>(g_nativeAddr + i);

        g_initialized = true;
        return true;
    }

    inline bool apply(const Vector3& camPos, const Vector3& camDir, const Vector3& targetPos)
    {
        if (!initialize()) return false;

        Vector3 impact = calcImpactPos(camPos, camDir, targetPos);

        if (!g_hooked) {
            writeShellcode(impact);
            installHook();
        }
        else {
            union { float f; uint32_t i; } posX, posY, posZ;
            posX.f = impact.x;
            posY.f = impact.y;
            posZ.f = impact.z;

            uint8_t xb[4] = {
                (uint8_t)(posX.i & 0xFF), (uint8_t)((posX.i >> 8) & 0xFF),
                (uint8_t)((posX.i >> 16) & 0xFF), (uint8_t)((posX.i >> 24) & 0xFF)
            };
            uint8_t yb[4] = {
                (uint8_t)(posY.i & 0xFF), (uint8_t)((posY.i >> 8) & 0xFF),
                (uint8_t)((posY.i >> 16) & 0xFF), (uint8_t)((posY.i >> 24) & 0xFF)
            };
            uint8_t zb[4] = {
                (uint8_t)(posZ.i & 0xFF), (uint8_t)((posZ.i >> 8) & 0xFF),
                (uint8_t)((posZ.i >> 16) & 0xFF), (uint8_t)((posZ.i >> 24) & 0xFF)
            };
            WriteBytes(g_caveAddr + 2, xb, 4);
            WriteBytes(g_caveAddr + 9, yb, 4);
            WriteBytes(g_caveAddr + 16, zb, 4);
        }
        return true;
    }

    inline void restore()
    {
        if (!g_hooked || !g_nativeAddr) return;
        WriteBytes(g_nativeAddr, g_origBytes, kHookSize);
        g_hooked = false;
    }

    inline void shutdown()
    {
        restore();
        if (g_caveAddr && Game.hProcess) {
            VirtualFreeEx(Game.hProcess, (LPVOID)g_caveAddr, 0, MEM_RELEASE);
            g_caveAddr = 0;
        }
        g_initialized = false;
        g_nativeAddr = 0;
    }
}
