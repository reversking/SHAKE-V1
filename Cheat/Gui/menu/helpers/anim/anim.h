#pragma once
#include <corecrt_math.h>
#include <algorithm>
#include <string>
#include <map>
#include "hash.hpp"
#include <menu/helpers/helpers.h>
#include "../../../../Cheat/Brand.hpp"
REVERS_STAMP(anim_h)
enum animation_flags_t
{
    clamp_out = (1 << 0),
    skip_enable = (1 << 1),
    skip_disable = (1 << 2),
    lerp_anim = (1 << 3),
};

enum improve_anim_flags_t
{
    i_bz4 = (1 << 0),
    i_bounce = (1 << 1),
    i_inv_bounce = (1 << 2),
    i_elastic = (1 << 3),
    i_inv_elastic = (1 << 4),
    i_clamp = (1 << 5),
    i_inverse = (1 << 6),
};

struct anim_t
{
    anim_t() {};
    anim_t(unsigned int def_flags, float def_speed, float def_min = 0.f, float def_max = 1.f)
    {
        flags = def_flags; speed = def_speed; min = def_min; max = def_max;
    };

    bool active{ false };
    float val{ 0.f };

    unsigned int flags{ animation_flags_t::clamp_out };
    float speed{ 1.f };
    float min{ 0.f };
    float max{ 1.f };

    INLINE float extra(unsigned int fl = improve_anim_flags_t::i_bz4 | improve_anim_flags_t::i_clamp)
    {
        float mod = fabsf(val);
        if (fl & i_inverse)
        {
            mod = fabsf(inverse());
        }

        mod = std::clamp(mod, 0.f, 1.f);

        if (fl & i_bz4)
        {
            mod = helpers::bz4(mod);
        }
        if (fl & i_bounce)
        {
            mod = helpers::bounce(mod);
        }
        if (fl & i_inv_bounce)
        {
            mod = helpers::inverse_bounce(mod);
        }
        if (fl & i_elastic)
        {
            mod = helpers::elastic(mod);
        }
        if (fl & i_inv_elastic)
        {
            mod = helpers::inverse_elastic(mod);
        }
        if (fl & i_clamp)
            mod = std::clamp(mod, 0.f, 1.f);

        return mod;
    }

    INLINE float inverse() {
        return 1.f - val;
    }

    INLINE void reset()
    {
        active = false;
        val = 0.f;
    }
};

namespace anim
{
    anim_t* animation(std::string id_name, anim_t data = anim_t());
    anim_t* animation(std::uint32_t id, anim_t data = anim_t());
    anim_t* create_animation(std::uint32_t id, anim_t data = anim_t());
    void update_animation(float& mod, bool& cond, float speed_multiplier = 1.f, unsigned int animation_flags = animation_flags_t::clamp_out, float min = 0.f, float max = 1.f);
    anim_t* get_anim_obj(std::uint32_t id);
    float get_animation_speed();
    void update_animation_speed();
    float system_time();
    inline float last_time_updated{ -1.0f };
    inline float animation_speed{ 0.16f };
    inline std::map< std::uint32_t, anim_t > item_animations{};
}
