#pragma once

#include "objects.h"
#include "camera.h"

#include "vmath_types.h"

#include <vector>

struct Image {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t samples = 0;
    std::vector<math::Vector3f> data;

    Image(uint32_t w, uint32_t h)
    : width(w), height(h) {
        data.resize(width*height, math::Vector3f(0,0,0));
    }

    math::Vector3<uint8_t> pixel_color(uint32_t i, uint32_t j) const {
        auto v = data[j*width+i]/float(samples);
        return math::Vector3<uint8_t>(uint8_t(v.x*255.99),uint8_t(v.y*255.99),uint8_t(v.z*255.99));
    }
};

HitableList create_test_scene();

bool render_step(const HitableList &world, const Camera &camera, Image &image);

