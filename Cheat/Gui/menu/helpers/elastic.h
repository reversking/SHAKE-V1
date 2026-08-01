#pragma once
#include <corecrt_math.h>
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(elastic_h)
class elastic_point {
private:
    float position_x{};
    float target_x{};
    float velocity_x{};
    float stiffness{};
    float damping{};
    float mass{};

public:
    elastic_point(){}

    elastic_point(float start_x, float stiffness = 0.1f, float damping = 0.5f, float mass = 1.0f)
        : position_x(start_x), target_x(start_x), velocity_x(0), stiffness(stiffness), damping(damping), mass(mass) {}

    void set_target(float new_target)
    {
        target_x = new_target;
    }
    void add_v(float new_v)
    {
        velocity_x += new_v;
    }
    void avoid_v()
    {
        velocity_x = -velocity_x * 1.5f;
    }
    void set_pos(float new_pos)
    {
        position_x = new_pos;
    }
    float get_target()
    {
        return target_x;
    }
    float get_v()
    {
        return velocity_x;
    }
    void update(float delta_time)
    {
        float displacement = position_x - target_x;
        float spring_force = -stiffness * displacement;
        float damping_force = -damping * velocity_x;
        float acceleration = (spring_force + damping_force) / mass;

        velocity_x += acceleration * delta_time;
        position_x += velocity_x * delta_time;

        if (is_at_rest())
        {
            velocity_x = 0.f;
            position_x = target_x;
        }
    }

    float get_pos() const
    {
        return position_x;
    }

    bool is_at_rest(float threshold = 0.1f) const
    {
        return fabsf(position_x - target_x) < threshold && fabsf(velocity_x) < threshold;
    }
};

struct elastic_xy
{
    elastic_xy(float xx, float yy)
    {
        x = elastic_point(xx, 0.4f, 0.8f, 1.4f);
        y = elastic_point(yy, 0.4f, 0.8f, 1.4f);
    };
    elastic_point x{};
    elastic_point y{};
};
