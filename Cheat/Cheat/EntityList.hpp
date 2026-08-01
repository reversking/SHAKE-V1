#pragma once
#include <vector>
#include <string>
#include "Brand.hpp"
REVERS_STAMP(EntityList_hpp)
#include "Ped.hpp"
#include "GameSDK.hpp"

namespace entity_list
{

    struct ped_struct
    {
        int index;
        float health;
        Vector3 origin;
        Vector3 v_bones[BONE_COUNT];
        std::string player_name;
    };

    inline std::vector<ped_struct> ped_list;

    inline bool force_refresh = false;
    inline int last_ped_count = 0;

    inline void force_update_ped_list()
    {

        force_refresh = true;
        ped_list.clear();
    }

    inline int get_entity_count()
    {
        return ped_list.size();
    }

    inline bool is_refreshing()
    {
        return force_refresh;
    }

    inline void update_ped_list()
    {

    }
}

namespace calc
{
    inline float distance_v3(const Vector3& a, const Vector3& b)
    {
        return getDistance(a, b);
    }

    inline Vector2 world_to_screen(const Vector3& world_pos)
    {
        Vector2 screen_pos;
        Matrix view_matrix = ReadMemory<Matrix>(Offsets.GameBase + Offsets.ViewPort + 0x24C);
        worldToScreen(view_matrix, world_pos, screen_pos);
        return screen_pos;
    }
}