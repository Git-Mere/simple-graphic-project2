/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once

namespace graphics::noise
{
    struct [[nodiscard]] NoiseCoordinate
    {
        int   base        = 0;
        int   next        = 0;
        float interpolant = 0.0f;
    };

    constexpr NoiseCoordinate make_noise_coord(float input) noexcept
    {
        NoiseCoordinate result;
        if (input >= 0)
        {
            result.base        = static_cast<int>(input);
            result.next        = result.base + 1;
            result.interpolant = input - static_cast<float>(result.base);
        }
        else
        {
            result.base        = static_cast<int>(input) - 1;
            result.next        = result.base + 1;
            result.interpolant = input - static_cast<float>(result.base);
        }
        return result;
    }

}