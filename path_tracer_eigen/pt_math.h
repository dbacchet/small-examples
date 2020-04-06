#pragma once 

#include "Eigen/Dense"

struct Ray {
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;

    Ray(const Eigen::Vector3f &origin_, const Eigen::Vector3f &dir_)
    : origin(origin_), direction(dir_) {}

    Eigen::Vector3f point_at_parameter(float t) const {
        return origin + t*direction;
    }
};

