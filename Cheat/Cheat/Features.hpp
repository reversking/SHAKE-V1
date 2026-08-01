#pragma once
#include <string>
#include <mutex>
#include <thread>
#include <Windows.h>
#include "Brand.hpp"
REVERS_STAMP(Features_hpp)
#include "GameSDK.hpp"
#include "Structs.hpp"
#include "GameSDK.hpp"
#include "Globals.hpp"
#include "Sounds.hpp"
#include "Particles.hpp"
#include "Print.hpp"
#include "EntityList.hpp"
#include "Ped.hpp"

extern std::atomic<bool> exitLoop;
extern std::vector<Ped> pedList;
extern std::mutex pedListMutex;
extern Ped localPlayer;

namespace GetPlayerNames {
    std::string GetPedName(Ped& ped);
}

namespace features
{

    inline float distance_v2(const Vector2& a, const Vector2& b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    inline std::string bone_names[BONE_COUNT] = {
        "Head", "Left Foot", "Right Foot", "Left Hand", "Right Hand", "Neck", "Hip",
        "Left Arm", "Right Arm", "Left Elbow", "Right Elbow", "Left Knee", "Right Knee",
        "Left Wrist", "Right Wrist", "Left Calf", "Right Calf"
    };

    inline std::string bone_enum_names[BONE_COUNT] = {
        "Head", "Left Foot", "Right Foot", "Left Hand", "Right Hand", "Neck", "Hip",
        "Left Arm", "Right Arm", "Left Elbow", "Right Elbow", "Left Knee", "Right Knee",
        "Left Wrist", "Right Wrist", "Left Calf", "Right Calf"
    };

    inline float distance(const Vector2& a, const Vector2& b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    inline float distance_v3(const Vector3& a, const Vector3& b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
    }

    inline Vector2 world_to_screen(const Vector3& world_pos) {
        Vector2 screen_pos = { 0, 0 };

        uintptr_t viewport = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
        if (viewport) {
            Matrix view_matrix = ReadMemory<Matrix>(viewport + 0x250);
            worldToScreen(view_matrix, world_pos, screen_pos);
        }

        return screen_pos;
    }

    inline std::string get_bone_impact(Vector3 bones[BONE_COUNT], Vector3 last_impact)
    {
        float closest_distance = FLT_MAX;
        int nearest_index = -1;
        for (int bone_index = 0; bone_index < BONE_COUNT; bone_index++)
        {
            float distance = distance_v3(last_impact, bones[bone_index]);
            if (distance < closest_distance)
            {
                closest_distance = distance;
                nearest_index = bone_index;
            }
        }

        return bone_names[nearest_index];
    }

    inline ParticleManager particleManager;

    struct HitInfo {
        int previousHealth;
        float lastUpdateTime;
        bool damageShown;
    };

    struct DamageIndicator {
        Vector3 worldPosition;
        int damage;
        float lifetime;
        float maxLifetime;
        bool isKill;
        ImVec4 color;
    };

    inline std::map<uintptr_t, HitInfo> hitInfoMap;
    inline Vector3 lastImpactPosition = Vector3(0, 0, 0);
    inline std::vector<DamageIndicator> damageIndicators;

    inline std::string GetClosestBoneName(const Vector3 boneList[BONE_COUNT], const Vector3& impactPos) {
        float minDist = 999999.0f;
        std::string boneName = "Body";

        struct BoneInfo { int index; const char* name; };
        BoneInfo bones[] = {
            {Head, "Head"}, {Neck, "Neck"}, {Hip, "Hip"},
            {LeftArm, "Left Arm"}, {RightArm, "Right Arm"},
            {LeftKnee, "Left Leg"}, {RightKnee, "Right Leg"}
        };

        for (int i = 0; i < 7; i++) {
            float dist = getDistance(boneList[bones[i].index], impactPos);
            if (dist < minDist) {
                minDist = dist;
                boneName = bones[i].name;
            }
        }

        return boneName;
    }

    inline void hitlog()
    {
        while (!exitLoop)
        {
            if (!globals::toggles::hitlog_enabled) {
                Sleep(50);
                continue;
            }

            try {
                uintptr_t world = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
                if (!world) {
                    Sleep(100);
                    continue;
                }

                uintptr_t localPlayerPtr = ReadMemory<uintptr_t>(world + Offsets.LocalPlayer);
                if (!localPlayerPtr) {
                    Sleep(100);
                    continue;
                }

                uintptr_t weaponManager = ReadMemory<uintptr_t>(localPlayerPtr + Offsets.WeaponManager);
                Vector3 currentImpact = Vector3(0, 0, 0);
                bool hasValidImpact = false;

                if (weaponManager) {
                    currentImpact = ReadMemory<Vector3>(weaponManager + 0x1B0);
                    if (currentImpact.x != 0 || currentImpact.y != 0 || currentImpact.z != 0) {
                        hasValidImpact = true;
                    }
                }

                Vector3 localPosition = Vector3(0, 0, 0);
                if (::localPlayer.pointer != 0 && ::localPlayer.update()) {
                    localPosition = ::localPlayer.position;
                }

                std::vector<Ped> pedListCopy;
                {
                    std::lock_guard<std::mutex> lock(pedListMutex);
                    pedListCopy = pedList;
                }

                for (auto& ped : pedListCopy) {
                    if (!ped.update()) continue;
                    if (ped.pointer == localPlayerPtr) continue;

                    int currentHealth = (int)ped.health;
                    if (currentHealth < 0) currentHealth = 0;

                    auto it = hitInfoMap.find(ped.pointer);

                    if (it != hitInfoMap.end()) {
                        int prevHealth = it->second.previousHealth;

                        if (currentHealth < prevHealth && prevHealth > 0) {
                            int damage = prevHealth - currentHealth;

                            if (damage > 0 && damage < 500) {
                                if (!hasValidImpact) continue;

                                bool isMyHit = false;

                                for (int boneIdx = 0; boneIdx < BONE_COUNT; boneIdx++) {
                                    if (ped.boneList[boneIdx].x == 0 && ped.boneList[boneIdx].y == 0 && ped.boneList[boneIdx].z == 0) continue;

                                    float boneDist = getDistance(ped.boneList[boneIdx], currentImpact);
                                    if (boneDist < 1.5f) {
                                        isMyHit = true;
                                        break;
                                    }
                                }

                                if (!isMyHit) {
                                    float distToImpact = getDistance(ped.position, currentImpact);
                                    if (distToImpact < 2.0f) {
                                        isMyHit = true;
                                    }
                                }

                                if (!isMyHit) continue;
                                std::string playerName = "Unknown";
                                if (ped.isPlayer()) {
                                    try {
                                        playerName = GetPlayerNames::GetPedName(ped);
                                        if (playerName.empty() || playerName == "Unknown") {
                                            playerName = "Player";
                                        }
                                    }
                                    catch (...) {
                                        playerName = "Player";
                                    }
                                }
                                else {
                                    playerName = "NPC";
                                }

                                Vector3 hitPosition = currentImpact;
                                if (hitPosition.x == 0 && hitPosition.y == 0 && hitPosition.z == 0) {
                                    hitPosition = ped.position;
                                    hitPosition.z += 1.0f;
                                }

                                std::string boneName = GetClosestBoneName(ped.boneList, hitPosition);

                                bool isKill = (currentHealth <= 0);

                                std::string message = "Hit " + playerName + " in " + boneName +
                                    " for " + std::to_string(damage) + " damage";

                                if (isKill) {
                                    message += " [KILLED]";
                                    printa<MessageType::Error>(message);
                                }
                                else {
                                    message += " (" + std::to_string(currentHealth) + " HP)";
                                    printa<MessageType::Hit>(message);
                                }

                                DamageIndicator indicator;
                                indicator.worldPosition = hitPosition;
                                indicator.worldPosition.z += 0.5f;
                                indicator.damage = damage;
                                indicator.lifetime = 2.0f;
                                indicator.maxLifetime = 2.0f;
                                indicator.isKill = isKill;
                                indicator.color = isKill ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                                damageIndicators.push_back(indicator);

                                if (globals::toggles::hit_particles) {
                                    try {
                                        ParticlePixel particle_col = particle_colors::global_to_pixel(globals::particle_hit_color);
                                        particleManager.addSuperParticle(hitPosition,
                                            ImVec4(particle_col.R, particle_col.G, particle_col.B, 1.0f));
                                    }
                                    catch (...) {}
                                }
                            }
                        }
                    }

                    hitInfoMap[ped.pointer] = { currentHealth, (float)ImGui::GetTime(), false };
                }

                float currentTime = (float)ImGui::GetTime();
                for (auto it = hitInfoMap.begin(); it != hitInfoMap.end();) {
                    if (currentTime - it->second.lastUpdateTime > 5.0f) {
                        it = hitInfoMap.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

            }
            catch (...) {
                Sleep(100);
            }

            Sleep(1);
        }
    }

    inline void update_game_variables()
    {
        static uint64_t lastUpdate = 0;
        static Vector3 lastLocalPosition;
        static uintptr_t lastLocalPlayer = 0;

        while (!exitLoop)
        {
            uint64_t currentTime = GetTickCount64();

            if (currentTime - lastUpdate < 16) {
                Sleep(1);
                continue;
            }

            game::base = Offsets.GameBase;
            game::world = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);

            if (game::world)
            {
                game::local_player = ReadMemory<uintptr_t>(game::world + Offsets.LocalPlayer);

                if (game::local_player)
                {
                    Vector3 newPosition = ReadMemory<Vector3>(game::local_player + 0x90);

                    if (newPosition != lastLocalPosition || game::local_player != lastLocalPlayer) {
                        game::local_position = newPosition;
                        lastLocalPosition = newPosition;
                        lastLocalPlayer = game::local_player;
                    }

                    static uint64_t lastViewUpdate = 0;
                    if (currentTime - lastViewUpdate > 50) {
                        game::view_angles = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
                        lastViewUpdate = currentTime;
                    }
                }
            }

            lastUpdate = currentTime;
            Sleep(8);
        }
    }

    inline bool is_bone_visible(const Vector3& bone_pos) {

        Vector3 localPlayerPos;
        uintptr_t localPlayerPtr = ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld) + Offsets.LocalPlayer);
        if (!localPlayerPtr) return false;

        localPlayerPos = ReadMemory<Vector3>(localPlayerPtr + 0x90);
        uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
        if (camera) {
            Vector3 cameraPos = ReadMemory<Vector3>(camera + 0x60);
            if (!vec3Empty(cameraPos)) {
                localPlayerPos = cameraPos;
            }
        }

        float distance = getDistance(localPlayerPos, bone_pos);

        if (distance < 3.0f) return true;

        return true;
    }

    inline int find_visible_bone(const entity_list::ped_struct& ped) {

        int bone_priority[] = { 0, 7, 9, 19 };

        for (int i = 0; i < 4; i++) {
            int bone_idx = bone_priority[i];
            if (bone_idx < BONE_COUNT && is_bone_visible(ped.v_bones[bone_idx])) {
                return bone_idx;
            }
        }

        return 0;
    }
}