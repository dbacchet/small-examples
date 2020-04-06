#include "material.h"

#include "pt_math.h"
#include "vmath.h"

#include <random>

// local functions
namespace {

std::mt19937 rnd_gen(12345678);
std::uniform_real_distribution<float> rnd_dis(0.0, 1.0);

math::Vector3f random_in_unit_sphere() {
    while (true) {
        auto p = math::Vector3f(2*rnd_dis(rnd_gen)-1, 2*rnd_dis(rnd_gen)-1, 2*rnd_dis(rnd_gen)-1);

        if (math::length2(p) <= 1.0) {
            return p;
        }
    }
}

math::Vector3f reflect(const math::Vector3f &v, const math::Vector3f &n) {
    return v - 2.0f * v.dot(n) * n;
}

std::optional<math::Vector3f> refract(const math::Vector3f &v, const math::Vector3f &n, float ni_over_nt) {
    math::Vector3f u = math::normalized(v);
    float dt = u.dot(n);
    float discriminant = 1.0- ni_over_nt*ni_over_nt*(1.0-dt*dt);
    if (discriminant>0.0) {
        return ni_over_nt * (u - n*dt) - n*std::sqrt(discriminant);
    }
    return {};
}

// Schlick's approximation for refelctivity function of angle
float schlick(float cosine, float refraction_index) {
    float r0 = (1.0-refraction_index) / (1.0+refraction_index);
    r0 = r0*r0;
    return r0 + (1.0-r0)*std::pow(1.0-cosine,5.0);
}

} // namespace

// /////////////////// //
// Lambertian material //
// /////////////////// //

std::optional<Scatter> Lambertian::scatter(const Ray &ray_in, const math::Vector3f &hit_point,
                                           const math::Vector3f &hit_normal) const {
    auto target = hit_point + hit_normal + random_in_unit_sphere();
    return Scatter(Ray(hit_point, target - hit_point), albedo_);
}

// ////////////// //
// Metal material //
// ////////////// //

std::optional<Scatter> Metal::scatter(const Ray &ray_in, const math::Vector3f &hit_point,
                                      const math::Vector3f &hit_normal) const {
    auto reflected_dir = reflect(math::normalized(ray_in.direction), hit_normal) + fuzz_ * random_in_unit_sphere();
    if (reflected_dir.dot(hit_normal) > 0.0) {
        return Scatter(Ray(hit_point, reflected_dir), albedo_);
    }
    return {};
}

// /////////////////// //
// Dielectric material //
// /////////////////// //

std::optional<Scatter> Dielectric::scatter(const Ray &ray_in, const math::Vector3f &hit_point,
                                      const math::Vector3f &hit_normal) const {
        auto dn_dot = ray_in.direction.dot(hit_normal);
        auto ray_dir_len = math::length(ray_in.direction);
        auto outward_normal = -hit_normal;
        auto ni_over_nt = refraction_index_;
        auto cosine = refraction_index_ * dn_dot / ray_dir_len;
        // swap normal and data if is pointing inside
        if (dn_dot<=0.0) {
            outward_normal = hit_normal;
            ni_over_nt = 1.0/refraction_index_;
            cosine = -dn_dot / ray_dir_len;
        }
        // calc reflected/refracted ray
        auto reflected_dir = reflect(math::normalized(ray_in.direction), hit_normal) + fuzz_*random_in_unit_sphere();
        auto refracted_dir = refract(ray_in.direction, outward_normal, ni_over_nt);
        if (refracted_dir) {
            float reflect_prob = schlick(cosine, refraction_index_);
            if (rnd_dis(rnd_gen) < reflect_prob) {
                return Scatter(Ray(hit_point, reflected_dir), attenuation_);
            } else {
                auto refracted_dir_fuzz = *refracted_dir + fuzz_*random_in_unit_sphere();
                return Scatter(Ray(hit_point, refracted_dir_fuzz), attenuation_);
            }
        }
        return Scatter(Ray(hit_point, reflected_dir), attenuation_);
}


