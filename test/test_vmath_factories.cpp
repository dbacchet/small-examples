#include "vmath.h"

#include <gtest/gtest.h>

#include <cmath>
#include <vector>
#include <numeric>

namespace {
template <typename T>
void print_matrix(const math::Matrix4<T> &m) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            printf("%9.7f ", (double)m(i,j));
        }
        printf("\n");
    }
}
} // namespace

TEST(Factories, matrices) {
    math::Matrix3f m1 = math::factory::matrix3_identity<float>();
    math::Matrix4f m2 = math::factory::matrix4_identity<float>();
    ASSERT_EQ(m1, math::Matrix3f({1,0,0,
                                  0,1,0,
                                  0,0,1}));
    ASSERT_EQ(m2, math::Matrix4f({1,0,0,0,
                                  0,1,0,0,
                                  0,0,1,0,
                                  0,0,0,1}));
    math::Matrix4f m3 = math::factory::create_translation(math::Vector3f(1,2,3));
    ASSERT_EQ(m3, math::Matrix4f({1,0,0,1,
                                  0,1,0,2,
                                  0,0,1,3,
                                  0,0,0,1}));
	math::Quatf r1(0.8775826, 0.1281319, 0.2562637, 0.3843956); // axis {1,2,3}, angle 1rad
    math::Matrix4f m4 = math::factory::create_transformation(math::Vector3f(1,2,3), r1);
    ASSERT_EQ(m4, math::Matrix4f({ 0.5731379, -0.6090066,  0.5482918, 1,
                                   0.7403488,  0.6716445, -0.0278793, 2,
                                  -0.3512785,  0.4219059,  0.8358222, 3,
                                   0,          0,          0,         1}));
}
/*
template <typename T> Matrix3<T> matrix3_identity();
/// create identity matrix
template <typename T> Matrix4<T> matrix4_identity();
/// create a translation matrix
template <typename T> Matrix4<T> create_translation(const Vector3<T> &v);
/// create a transformation matrix
template <typename T> Matrix4<T> create_transformation(const Vector3<T> &v, const Quaternion<T> &q);
/// create look-at matrix
template <typename T>
Matrix4<T> create_lookat(const Vector3<T> &eye, const Vector3<T> &to,
                         const Vector3<T> &up = Vector3<T>(T(0), T(0), T(1)));
/// quaternion from eulers angles.
/// @param x Rotation around x axis (in radians).
/// @param y Rotation around y axis (in radians).
/// @param z Rotation around z axis (in radians).
template <typename T> Quaternion<T> quat_from_euler_321(T x, T y, T z);
/// quaternion given axis and angle
/// @param axis Unit vector expressing axis of rotation.
/// @param angle Angle of rotation around axis (in radians).
template <typename T> Quaternion<T> quat_from_axis_angle(Vector3<T> axis, T angle);
/// quaternion from transformation matrix (only rotation part is kept)
template <typename T> Quaternion<T> quat_from_matrix(const Matrix4<T> &m);
/// quaternion from rotation matrix.
template <typename T> Quaternion<T> quat_from_matrix(const Matrix3<T> &m);

*/
