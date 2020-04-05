#include "material.h"

#include "pt_math.h"
#include "vmath.h"

#include <random>

// local functions
namespace {

std::mt19937 rnd_gen(12345678);
std::uniform_real_distribution<float> rnd_dis(-1.0, 1.0);

math::Vector3f random_in_unit_sphere() {
    while (true) {
        auto p = math::Vector3f(rnd_dis(rnd_gen), rnd_dis(rnd_gen), rnd_dis(rnd_gen));
        if (math::length2(p) <= 1.0) {
            return p;
        }
    }
}

math::Vector3f reflect(const math::Vector3f &v, const math::Vector3f &n) {
    return v - 2.0f * v.dot(n) * n;
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

/*
// /////////////////// //
// Dielectric material //
// /////////////////// //
pub struct Dielectric {
    refraction_index: f32,
    fuzz: f32,
    attenuation: Vec3,
}

impl Dielectric {
    pub fn new(refraction_index: f32, fuzz: f32) -> Dielectric {
        Dielectric { refraction_index, fuzz, attenuation: Vec3::new(1.0,1.0,1.0) }
    }
}

impl Material for Dielectric {
    fn scatter(&self, ray_in: Ray, hit_point: Vec3, hit_normal: Vec3) -> Option<Scatter> {
        let dn_dot = dot(ray_in.direction, hit_normal);
        let ray_dir_len = ray_in.direction.length();
        let mut outward_normal = -hit_normal;
        let mut ni_over_nt = self.refraction_index;
        let mut cosine = self.refraction_index * dn_dot / ray_dir_len;
        // swap normal and data if is pointing inside
        if dn_dot<=0.0 {
            outward_normal = hit_normal;
            ni_over_nt = 1.0/self.refraction_index;
            cosine = -dn_dot / ray_dir_len;
        }
        // calc reflected/refracted ray
        let reflected_dir = reflect(unit_vector(ray_in.direction), hit_normal) + self.fuzz*random_in_unit_sphere();
        if let Some(refracted_dir) = refract(ray_in.direction, outward_normal, ni_over_nt) {
            let reflect_prob = schlick(cosine, self.refraction_index);
            let mut rng = rand::thread_rng();
            if rng.gen::<f32>() < reflect_prob {
                return Some( Scatter::new( Ray::new(hit_point, reflected_dir), self.attenuation) );
            } else {
                let refracted_dir = refracted_dir + self.fuzz*random_in_unit_sphere();
                return Some( Scatter::new( Ray::new(hit_point, refracted_dir), self.attenuation) );
            }
        }
        return Some( Scatter::new( Ray::new(hit_point, reflected_dir), self.attenuation) );
    }
}



fn reflect(v: Vec3, n: Vec3) -> Vec3 {
    v - 2.0*dot(v, n)*n
}

fn refract(v: Vec3, n: Vec3, ni_over_nt: f32) -> Option<Vec3> {
    let u = unit_vector(v);
    let dt = dot(u, n);
    let discriminant = 1.0- ni_over_nt*ni_over_nt*(1.0-dt*dt);
    if discriminant>0.0 {
        let refracted = ni_over_nt * (u - n*dt) - n*discriminant.sqrt();
        return Some(refracted);
    }
    return None;
}

// Schlick's approximation for refelctivity function of angle
fn schlick(cosine: f32, refraction_index: f32) -> f32 {
    let r0 = (1.0-refraction_index) / (1.0+refraction_index);
    let r0 = r0*r0;
    r0 + (1.0-r0)*(1.0-cosine).powf(5.0)
}
*/
