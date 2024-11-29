/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once


#include <tuple>


namespace graphics::noise
{
    inline float cosine_step(const float& t)
    {
        return (1 - cos(t * graphics::PI)) * 0.5f;
    }

    inline float smoothstep(const float& t)
    {
        return t * t * (3 - 2 * t);
    }

    inline float quin(const float& t)
    {
        return t * t * t * (6 * t * t - 15 * t + 10);
    }

    enum class SmoothMethod
    {
        Linear,
        Cosine,
        Smoothstep,
        Quintic
    };

    //constexpr auto fade(glm::vec4 value, SmoothMethod smoothing) noexcept
    //{
    //    glm::vec4 result{};
    //    if (smoothing == graphics::noise::SmoothMethod::Linear)
    //    {
    //        result = value;
    //    }
    //    else if (smoothing == graphics::noise::SmoothMethod::Cosine)
    //    {
    //        result.x = cosine_step(value.x);
    //        result.y = cosine_step(value.y);
    //        result.z = cosine_step(value.z);
    //        result.w = cosine_step(value.w);
    //    }
    //    else if (smoothing == graphics::noise::SmoothMethod::Smoothstep)
    //    {
    //        result.x = smoothstep(value.x);
    //        result.y = smoothstep(value.y);
    //        result.z = smoothstep(value.z);
    //        result.w = smoothstep(value.w);
    //    }
    //    else if (smoothing == graphics::noise::SmoothMethod::Quintic)
    //    {
    //        result.x = quin(value.x);
    //        result.y = quin(value.y);
    //        result.z = quin(value.z);
    //        result.w = quin(value.w);
    //    }
    //    return result;
    //}

    constexpr auto fade(float x,SmoothMethod smoothing) noexcept
    {
        glm::vec4 result{};
        if (smoothing == graphics::noise::SmoothMethod::Linear)
        {
            result.x = x;
        }
        else if (smoothing == graphics::noise::SmoothMethod::Cosine)
        {
            result.x = cosine_step(x);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Smoothstep)
        {
            result.x = smoothstep(x);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Quintic)
        {
            result.x = quin(x);
        }
        return result;
    }

    constexpr auto fade(float x, float y, SmoothMethod smoothing) noexcept
    {
        glm::vec4 result{};

        if (smoothing == graphics::noise::SmoothMethod::Linear)
        {
            result.x  = x;
            result.y = y;
        }
        else if (smoothing == graphics::noise::SmoothMethod::Cosine)
        {
            result.x  = cosine_step(x);
            result.y = cosine_step(y);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Smoothstep)
        {
            result.x  = smoothstep(x);
            result.y = smoothstep(y);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Quintic)
        {
            result.x  = quin(x);
            result.y = quin(y);
        }
        return result;
    }

    constexpr auto fade(float x, float y, float z, SmoothMethod smoothing) noexcept
    {
        glm::vec4 result{};
        if (smoothing == graphics::noise::SmoothMethod::Linear)
        {
            result.x = x;
            result.y = y;
            result.z = z;
        }
        else if (smoothing == graphics::noise::SmoothMethod::Cosine)
        {
            result.x = cosine_step(x);
            result.y = cosine_step(y);
            result.z = cosine_step(z);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Smoothstep)
        {
            result.x = smoothstep(x);
            result.y = smoothstep(y);
            result.z = smoothstep(z);
        }
        else if (smoothing == graphics::noise::SmoothMethod::Quintic)
        {
            result.x = quin(x);
            result.y = quin(y);
            result.z = quin(z);
        }
        return result;
    }
}