#pragma once

#include "objects.h"
#include "camera.h"

#include "Eigen/Dense"

#include <vector>

struct Image {
    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        Color(uint8_t r_=0, uint8_t g_=0, uint8_t b_=0)
        : r(r_), g(g_), b(b_) {}
    };
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t samples = 0;
    std::vector<Eigen::Vector3f> data;

    Image(uint32_t w, uint32_t h)
    : width(w), height(h) {
        data.resize(width*height, Eigen::Vector3f(0,0,0));
    }

    Color pixel_color(uint32_t i, uint32_t j) const {
        Eigen::Vector3f v = data[j*width+i]/float(samples);
        // gamma correction ("gamma 2")
        v = Eigen::Vector3f(std::sqrt(v.x()),std::sqrt(v.y()),std::sqrt(v.z()));
        return Color(uint8_t(v.x()*255.99),uint8_t(v.y()*255.99),uint8_t(v.z()*255.99));
    }
};

HitableList create_test_scene();
HitableList create_book_scene();

bool render_step(const HitableList &world, const Camera &camera, Image &image);

