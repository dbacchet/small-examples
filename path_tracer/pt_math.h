#pragma once 

#include "vmath_types.h"

struct Ray {
    math::Vector3f origin;
    math::Vector3f direction;

    Ray(const math::Vector3f &origin_, const math::Vector3f &dir_)
    : origin(origin_), direction(dir_) {}

    math::Vector3f point_at_parameter(float t) const {
        return origin + t*direction;
    }
};

