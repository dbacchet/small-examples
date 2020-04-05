#pragma once

#include "pt_math.h"
#include "vmath.h"

struct Camera {
    math::Vector3f lower_left_corner;
    math::Vector3f horizontal;
    math::Vector3f vertical;
    math::Vector3f origin;
    math::Vector3f u;
    math::Vector3f v;
    float lens_radius = 1.0f;

    Camera(const math::Vector3f &from, const math::Vector3f &to, const math::Vector3f &up, float vfov_deg, float aspect_ratio, float aperture, float dist_to_focus);

    Ray get_ray(float s, float t) const;
};


