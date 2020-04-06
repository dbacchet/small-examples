#pragma once

#include "pt_math.h"
#include "Eigen/Dense"

// simple camera model
struct Camera {
    Eigen::Vector3f lower_left_corner;
    Eigen::Vector3f horizontal;
    Eigen::Vector3f vertical;
    Eigen::Vector3f origin;
    Eigen::Vector3f u;
    Eigen::Vector3f v;
    float lens_radius = 1.0f;

    Camera(const Eigen::Vector3f &from, const Eigen::Vector3f &to, const Eigen::Vector3f &up, float vfov_deg, float aspect_ratio, float aperture, float dist_to_focus);

    Ray get_ray(float s, float t) const;
};


