#include "anim.h"
#include <time.h>
#include "../../../../Cheat/Brand.hpp"
REVERS_STAMP(anim_cpp)

anim_t* anim::animation(std::string id_name, anim_t data)
{
    return animation(CONST_HASH(id_name.c_str()), data);
}

anim_t* anim::animation(std::uint32_t id, anim_t data)
{
    anim_t* anim = create_animation(id, data);
    update_animation(anim->val, anim->active, anim->speed, anim->flags, anim->min, anim->max);
    return anim;
}

anim_t* anim::create_animation(std::uint32_t id, anim_t data)
{
    auto item_data = item_animations.find(id);

    if (item_data == item_animations.end())
    {
        item_animations.insert({ id, data });
        item_data = item_animations.find(id);
    }
    return &item_data->second;
}

anim_t* anim::get_anim_obj(std::uint32_t id)
{
    return &item_animations[id];
}

void anim::update_animation(float& mod, bool& cond, float speed_multiplier, unsigned int animation_flags, float min, float max)
{
    float time = (get_animation_speed() * 5.f) * speed_multiplier;

    if ((animation_flags & skip_enable) && cond)
        mod = max;
    else if ((animation_flags & skip_disable) && !cond)
        mod = min;

    if (animation_flags & lerp_anim)
        mod = helpers::lerp(mod, cond ? max : min, time);
    else
    {
        if (cond && mod <= max)
            mod += time;
        else if (!cond && mod >= min)
            mod -= time;
    }

    if (animation_flags & clamp_out)
        mod = std::clamp(mod, min, max);
}

float anim::get_animation_speed()
{
    return animation_speed;
}

void anim::update_animation_speed()
{
    if (last_time_updated == -1.f)
        last_time_updated = system_time();

    animation_speed = fabsf(last_time_updated - system_time()) * 5.f;
    last_time_updated = system_time();
}

float anim::system_time()
{
    return (float)(clock() / (float)1000.f);
}
