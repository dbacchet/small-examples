#include "camera.h"

#include "pt_math.h"
#include "vmath.h"

#include <random>

namespace {

std::mt19937 rnd_gen(23456789);
std::uniform_real_distribution<float> rnd_dis(-1.0, 1.0);

math::Vector3f random_in_unit_disc() {
    while (true) {
        math::Vector3f p = math::Vector3f(rnd_dis(rnd_gen), rnd_dis(rnd_gen), 0.0f);
        if (math::length2(p) <= 1.0f) {
            return p;
        }
    }
}

} // namespace


Camera::Camera(const math::Vector3f &from, const math::Vector3f &to, const math::Vector3f &up, float vfov_deg,
               float aspect_ratio, float aperture, float dist_to_focus) {
    float theta = vfov_deg * M_PI / 180.0f;
    float half_height = std::tan(theta / 2.0);
    float half_width = aspect_ratio * half_height;
    auto w = math::normalized(from - to);
    // set values
    u = math::normalized(up.cross(w));
    v = w.cross(u);
    lower_left_corner = from - half_width * dist_to_focus * u - half_height * dist_to_focus * v - dist_to_focus * w;
    horizontal = 2.0f * half_width * dist_to_focus * u;
    vertical = 2.0f * half_height * dist_to_focus * v;
    origin = from;
    lens_radius = aperture / 2.0;
}

Ray Camera::get_ray(float s, float t) const {
    auto rd = lens_radius * random_in_unit_disc();
    auto offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
}

