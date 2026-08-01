#pragma once
#include <mutex>
#include <map>
#include <string>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>
#include <imgui.h>
#include "SimpleMath.h"
#include "Brand.hpp"
REVERS_STAMP(Globals_hpp)
#include "impact_coord_bypass.hpp"

namespace globals
{
    namespace mutex
    {
        inline std::mutex entity_list_mutex;
    }

    namespace toggles
    {
        inline bool hit_sound = false;
        inline bool kill_sound = false;
        inline bool hit_particles = false;
        inline bool hitlog_enabled = false;
        inline bool death_skeleton = false;
        inline bool aim_warning = false;
        inline bool aim_warning_locked = false;
        inline bool aim_warning_show_aimers = false;
        inline bool aim_warning_show_lookers = false;
        inline bool godmode = false;
        inline bool esp_ignore_peds = false;
        inline bool aim_ignore_peds = false;
    }

    inline int hit_sound_effect = 0;
    inline int kill_sound_effect = 1;
    inline ImVec4 particle_hit_color = ImVec4(0.51f, 0.51f, 1.0f, 1.0f);

    namespace keys
    {
        inline int godmode_key = 0;
        inline int quick_friend_key = VK_F4;
    }

    namespace friends
    {

        struct FriendInfo {
            std::string name;
            Vector3 lastPosition;
            bool isPersistent;
        };

        inline std::map<std::string, FriendInfo> friendList;

        inline std::mutex friendListMutex;

        inline void AddFriend(const std::string& name, const Vector3& position) {
            std::lock_guard<std::mutex> lock(friendListMutex);

            if (friendList.find(name) != friendList.end()) {

                friendList[name].lastPosition = position;
                return;
            }

            FriendInfo info;
            info.name = name;
            info.lastPosition = position;
            info.isPersistent = true;
            friendList[name] = info;
        }

        inline void RemoveFriend(const std::string& name) {
            std::lock_guard<std::mutex> lock(friendListMutex);
            friendList.erase(name);
        }

        inline bool IsFriend(const std::string& name) {
            std::lock_guard<std::mutex> lock(friendListMutex);
            return friendList.find(name) != friendList.end();
        }

        inline bool IsFriend(int id) {

            return false;
        }

        inline void UpdateFriendPosition(const std::string& name, const Vector3& position) {
            std::lock_guard<std::mutex> lock(friendListMutex);
            auto it = friendList.find(name);
            if (it != friendList.end()) {
                it->second.lastPosition = position;
            }
        }
    }

    namespace silent_aim
    {
        inline bool target_closest_to_mouse = true;
        inline int miss_chance = 0;
        inline bool smart_random_bone = true;
    }

    namespace legit_aim
    {
        inline bool enabled = false;
        inline float max_lock_time = 2.5f;
        inline float min_lock_time = 0.8f;
        inline float break_lock_chance = 15.0f;
        inline float target_switch_delay = 0.3f;
        inline float jitter_amount = 2.5f;
        inline float jitter_frequency = 0.1f;
        inline float humanize_factor = 0.7f;
        inline float reaction_delay = 0.05f;
        inline float smooth_curve_power = 2.2f;
        inline float micro_correction_range = 1.5f;
        inline float cooldown_time = 1.0f;
        inline bool random_bone_switch = true;
        inline int bone_switch_interval = 3;

        inline uintptr_t current_target = 0;
        inline float lock_start_time = 0.0f;
        inline float current_lock_duration = 0.0f;
        inline float last_target_switch_time = 0.0f;
        inline float last_jitter_time = 0.0f;
        inline Vector3 jitter_offset = Vector3(0, 0, 0);
        inline int current_bone_index = 0;
        inline int shots_on_target = 0;
        inline std::map<uintptr_t, float> target_cooldowns;
        inline std::mutex cooldown_mutex;

        inline float GetRandomLockTime() {
            return min_lock_time + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_lock_time - min_lock_time)));
        }

        inline bool ShouldBreakLock() {
            return (static_cast<float>(rand() % 100) < break_lock_chance);
        }

        inline Vector3 GetJitterOffset(float time) {
            if (time - last_jitter_time > jitter_frequency) {
                last_jitter_time = time;
                jitter_offset.x = (static_cast<float>(rand() % 200) - 100.0f) / 100.0f * jitter_amount;
                jitter_offset.y = (static_cast<float>(rand() % 200) - 100.0f) / 100.0f * jitter_amount;
                jitter_offset.z = (static_cast<float>(rand() % 100) - 50.0f) / 100.0f * jitter_amount * 0.3f;
            }
            return jitter_offset;
        }

        inline bool IsTargetOnCooldown(uintptr_t target, float currentTime) {
            std::lock_guard<std::mutex> lock(cooldown_mutex);
            auto it = target_cooldowns.find(target);
            if (it != target_cooldowns.end()) {
                if (currentTime - it->second < cooldown_time) {
                    return true;
                }
                target_cooldowns.erase(it);
            }
            return false;
        }

        inline void AddTargetCooldown(uintptr_t target, float currentTime) {
            std::lock_guard<std::mutex> lock(cooldown_mutex);
            target_cooldowns[target] = currentTime;
        }

        inline void CleanupCooldowns(float currentTime) {
            std::lock_guard<std::mutex> lock(cooldown_mutex);
            for (auto it = target_cooldowns.begin(); it != target_cooldowns.end();) {
                if (currentTime - it->second > cooldown_time * 2.0f) {
                    it = target_cooldowns.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        inline float EaseOutQuad(float t) {
            return 1.0f - (1.0f - t) * (1.0f - t);
        }

        inline float EaseInOutCubic(float t) {
            return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
        }

        inline float HumanizedSmooth(float distance, float maxDistance, float smooth) {
            float normalizedDist = distance / maxDistance;
            float curve = powf(normalizedDist, smooth_curve_power);
            float baseFactor = 1.0f / smooth;
            float humanFactor = baseFactor * (1.0f + (1.0f - humanize_factor) * curve);
            float randomVariation = 1.0f + ((static_cast<float>(rand() % 20) - 10.0f) / 100.0f) * (1.0f - humanize_factor);
            return humanFactor * randomVariation;
        }
    }

    namespace death_skeleton
    {
        struct DeathSkeleton {
            Vector3 bonePositions[9];
            float fadeAlpha;
            float timeAlive;
            uintptr_t playerId;
        };

        inline std::vector<DeathSkeleton> deathSkeletons;
        inline float fadeOutDuration = 5.0f;
        inline float skeletonColor[3] = { 1.0f, 0.0f, 0.0f };
    }

    inline bool teleportToPlayer = false;
    inline Vector3 teleportPosition;
}
