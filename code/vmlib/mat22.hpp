#ifndef MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
#define MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

#include <cmath>

#include "vec2.hpp"

/** Mat22f : 2x2 matrix with floats
 *
 * See comments for Vec2f for some discussion.
 *
 * The matrix is stored in row-major order.
 *
 * Example:
 *   Mat22f identity{ 
 *     1.f, 0.f,
 *     0.f, 1.f
 *   };
 */
struct Mat22f
{
    float _00, _01;
    float _10, _11;
};

// Common operators for Mat22f.
// Note that you will need to implement these yourself.

constexpr
Mat22f operator*(Mat22f const& aLeft, Mat22f const& aRight) noexcept
{
    //Executing a 2x2 matrix x matrix multiplication of the spaceship direction
    return Mat22f{
        aLeft._00 * aRight._00 + aLeft._01 * aRight._10,
        aLeft._00 * aRight._01 + aLeft._01 * aRight._11,
        aLeft._10 * aRight._00 + aLeft._11 * aRight._10,
        aLeft._10 * aRight._01 + aLeft._11 * aRight._11
    };
}

constexpr
Vec2f operator*(Mat22f const& aLeft, Vec2f const& aRight) noexcept
{
    //Executing the matrix x vector multiplication of Mat22f x Vec2f
    return Vec2f{
        aLeft._00 * aRight.x + aLeft._01 * aRight.y,
        aLeft._10 * aRight.x + aLeft._11 * aRight.y
    };
}

inline
Mat22f make_rotation_2d(float aAngle) noexcept
{
    //Calculating the cos and sin with the cursor angle to use in the rotation
    // at first I made it positive but the back of the spaceship was following 
    //my mouse cursor so I reversed it, making the cos/sin negative
    float c = -cos(aAngle);
    float s = -sin(aAngle);

    //we use the standard rotation matrix format
    // to rotate a vector around the origin
    return Mat22f{
        c, -s,
        s, c
    };
}

#endif // MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
