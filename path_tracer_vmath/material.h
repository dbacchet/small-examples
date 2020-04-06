#pragma once

#include "pt_math.h"

#include <optional>


struct Scatter {
    Ray ray;
    math::Vector3f color;

    Scatter(const Ray &ray_, const math::Vector3f &color_)
        : ray(ray_), color(color_) {}
};

class Material {
public:
    virtual ~Material() = default;
    // given an input ray, hit point and normal, calculate the scattered output ray and its attenuation
    virtual std::optional<Scatter> scatter(const Ray &ray_in, const math::Vector3f &hit_point, const math::Vector3f &hit_normal) const = 0;
};

// /////////////////// //
// Lambertian material //
// /////////////////// //
class Lambertian: public Material {
public:
    Lambertian(math::Vector3f albedo)
    : albedo_(albedo) {}
    virtual ~Lambertian() = default;

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const math::Vector3f &hit_point, const math::Vector3f &hit_normal) const;

    math::Vector3f albedo_;
};


// ////////////// //
// Metal material //
// ////////////// //
class Metal: public Material {
public:
    Metal(math::Vector3f albedo, float fuzz)
    : albedo_(albedo), fuzz_(fuzz) {}
    virtual ~Metal() = default;

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const math::Vector3f &hit_point, const math::Vector3f &hit_normal) const ;

    math::Vector3f albedo_;
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

    virtual std::optional<Scatter> scatter(const Ray &ray_in, const math::Vector3f &hit_point, const math::Vector3f &hit_normal) const ;

    float refraction_index_ = 0.0f;
    float fuzz_ = 0.0f;
    math::Vector3f attenuation_ = math::Vector3f(1,1,1);
};


