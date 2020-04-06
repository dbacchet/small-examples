#pragma once

#include "pt_math.h"

#include <optional>


struct Scatter {
    Ray ray;
    Eigen::Vector3f color;

    Scatter(const Ray &ray_, const Eigen::Vector3f &color_)
        : ray(ray_), color(color_) {}
};

class Material {
public:
    virtual ~Material() = default;
    // given an input ray, hit point and normal, calculate the scattered output ray and its attenuation
    virtual std::optional<Scatter> scatter(const Ray &ray_in, const Eigen::Vector3f &hit_point, const Eigen::Vector3f &hit_normal) const = 0;
};

// /////////////////// //
// Lambertian material //
// /////////////////// //
class Lambertian: public Material {
public:
    Lambertian(Eigen::Vector3f albedo)
    : albedo_(albedo) {}
    virtual ~Lambertian() = default;

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const Eigen::Vector3f &hit_point, const Eigen::Vector3f &hit_normal) const;

    Eigen::Vector3f albedo_;
};


// ////////////// //
// Metal material //
// ////////////// //
class Metal: public Material {
public:
    Metal(Eigen::Vector3f albedo, float fuzz)
    : albedo_(albedo), fuzz_(fuzz) {}
    virtual ~Metal() = default;

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const Eigen::Vector3f &hit_point, const Eigen::Vector3f &hit_normal) const ;

    Eigen::Vector3f albedo_;
    float fuzz_ = 0.0f;
};


// /////////////////// //
// Dielectric material //
// /////////////////// //
class Dielectric: public Material {
public:
    Dielectric(float refraction_index, float fuzz)
    : refraction_index_(refraction_index), fuzz_(fuzz) {}
    virtual ~Dielectric() = default;

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const Eigen::Vector3f &hit_point, const Eigen::Vector3f &hit_normal) const ;

    float refraction_index_ = 0.0f;
    float fuzz_ = 0.0f;
    Eigen::Vector3f attenuation_ = Eigen::Vector3f(1,1,1);
};


