#include "Eigen/Dense"
#include "pt_math.h"
#include "material.h"

#include <cmath>
#include <vector>
#include <optional>
#include <memory>



struct HitRecord {
    float t = -1.0f;
    Eigen::Vector3f point;
    Eigen::Vector3f normal;
    Material const *material = nullptr;

    HitRecord() = default;
    HitRecord(float t_, const Eigen::Vector3f &point_, const Eigen::Vector3f &normal_, Material const *mtl)
    : t(t_), point(point_), normal(normal_), material(mtl) {}
};


// hitable trait
class Hitable {
public:
    virtual ~Hitable() = default;
    virtual std::optional<HitRecord> hit(const Ray &ray, float t_min, float t_max) const = 0;
};


// a sphere, defined with center and radisu
class Sphere: public Hitable {
public:

    Sphere(const Eigen::Vector3f &center, float radius, Material *material)
    : center_(center), radius_(radius), material_(material) {}

    virtual ~Sphere() = default;

    std::optional<HitRecord> hit(const Ray &ray, float t_min, float t_max) const {
        const auto oc = ray.origin - center_;
        const auto a = ray.direction.dot(ray.direction);
        const auto b = oc.dot(ray.direction);
        const auto c = oc.dot(oc) - radius_*radius_;
        const auto discriminant = b*b - a*c; // removed the 2.0 factor from b and the 4.0 here, since the result is the same 
        if (discriminant > 0.0) {
            float t = (-b - std::sqrt(discriminant))/a;
            if (t<t_max && t>t_min) {
                auto point = ray.point_at_parameter(t);
                auto normal = (point-center_) / radius_;
                return HitRecord(t, point, normal, material_);
            }
        } else {
            auto t = (-b + std::sqrt(discriminant))/a;
            if (t<t_max && t>t_min) {
                auto point = ray.point_at_parameter(t);
                auto normal = (point-center_) / radius_;
                return HitRecord(t, point, normal, material_);
            }
        }
        return {};
    }

    Eigen::Vector3f center_;
    float radius_ = 1.0f;
    Material *material_ = nullptr;
};


// list of objects that implement the Hitable trait

class HitableList: public Hitable {
public:

    virtual ~HitableList() = default;

    void add(const Sphere &sphere) {
        objects.push_back(sphere);
    }

    std::optional<HitRecord> hit(const Ray &ray, float t_min, float t_max) const {
        std::optional<HitRecord> hit_record;
        float closest = t_max + 0.1;
        for (const Sphere &obj: objects) {
            auto hr = obj.hit(ray, t_min, closest);
            if (hr) {
                hit_record = hr;
                closest = hr->t;
            }
        }
        return hit_record;
    }

    std::vector<Sphere> objects; // TODO: use the base class to make it generic. for now only consider spheres
};

