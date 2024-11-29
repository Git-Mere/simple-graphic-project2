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

    template <typename T>
    struct LinearValues
    {
        T Left{};
        T Right{};
    };

    template <typename T, typename U>
    constexpr auto linear_mix(const LinearValues<T>& values, U s) noexcept
    {
        return values.Left * (1 - s) + values.Right * s;
    }

    template <typename T>
    struct BiLinearValues
    {
        LinearValues<T> Bottom{};
        LinearValues<T> Top{};
    };

    template <typename T, typename U>
    constexpr auto bilinear_mix(const BiLinearValues<T>& values, U s, U t) noexcept
    {
        return linear_mix(values.Bottom, s) * (1 - t) + linear_mix(values.Top, s) * t;
    }

    template <typename T>
    struct TriLinearValues
    {
        BiLinearValues<T> Near{};
        BiLinearValues<T> Far{};
    };

    template <typename T, typename U>
    constexpr auto trilinear_mix(const TriLinearValues<T>& values, U s, U t, U p) noexcept
    {
        return bilinear_mix(values.Near, s, t) * (1 - p) + bilinear_mix(values.Far, s, t) * p;
    }
}
