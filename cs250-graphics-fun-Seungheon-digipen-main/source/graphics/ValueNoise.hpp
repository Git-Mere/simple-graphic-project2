/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once
#include <numeric>
#include "../graphics/Interporate.hpp"
#include "../graphics/smoothing.hpp"
#include "../graphics/Make_noise_coordinate.hpp"
#include "../graphics/PermutationHash.hpp"

namespace graphics::noise
{
    template <typename T>
    class [[nodiscard]] ValueNoise
    {
    public:
        explicit ValueNoise(PeriodDimension period = PeriodDimension::_256, SmoothMethod smooth_method = SmoothMethod::Quintic);

        [[nodiscard]] T Evaluate(float x) const noexcept;
        [[nodiscard]] T Evaluate(float x, float y) const noexcept;
        [[nodiscard]] T Evaluate(float x, float y, float z) const noexcept;

        [[nodiscard]] constexpr PeriodDimension GetPeriodDimension() const noexcept;
        void                                    SetPeriod(PeriodDimension period);

        [[nodiscard]] constexpr SmoothMethod GetSmoothing() const noexcept;
        constexpr void                       SetSmoothing(SmoothMethod smooth_method);

        [[nodiscard]] constexpr const std::vector<T>& GetValues() const noexcept;
        constexpr void                                SetValues(std::vector<T>&& new_values);

    private:
        PeriodDimension period_;
        SmoothMethod    smoothmethod;
        PermutationHash hash_table;
        std::vector<T>  data;
    };


template <typename T>
    inline ValueNoise<T>::ValueNoise(PeriodDimension period, SmoothMethod smooth_method) 
        : period_(period), smoothmethod(smooth_method), hash_table(PermutationHash(period))
    {
        int size = static_cast<int>(period);
    for (int i = 0; i < size; i++)
    {
        //data.push_back(glm::vec4(util::random()));
        data.push_back(glm::vec4(glm::vec3(util::random()), 1.f));
    }
}

template <typename T>
inline T ValueNoise<T>::Evaluate(float x) const noexcept
{
    NoiseCoordinate point_x = make_noise_coord(x);

    const unsigned c00 = static_cast<unsigned>(hash_table(point_x.base));
    const unsigned c01 = static_cast<unsigned>(hash_table(point_x.next));

    LinearValues<T> one{ data[c00], data[c01] };

    glm::vec4 smooth = fade(point_x.interpolant, smoothmethod);

    return linear_mix(one, smooth.x);
}

template <typename T>
inline T ValueNoise<T>::Evaluate(float x, float y) const noexcept
{
    NoiseCoordinate point_x = make_noise_coord(x);
    NoiseCoordinate point_y = make_noise_coord(y);

    const unsigned c00 = static_cast<unsigned>(hash_table(point_x.base, point_y.base));
    const unsigned c10 = static_cast<unsigned>(hash_table(point_x.next, point_y.base));
    const unsigned c01 = static_cast<unsigned>(hash_table(point_x.base, point_y.next));
    const unsigned c11 = static_cast<unsigned>(hash_table(point_x.next, point_y.next));

    glm::vec4 smooth = fade(point_x.interpolant, point_y.interpolant, smoothmethod);

    LinearValues<T>   one{ data[c00], data[c10] };
    LinearValues<T>   two{ data[c01], data[c11] };
    BiLinearValues<T> three{ one, two };

    return bilinear_mix(three, smooth.x, smooth.y);
}

template <typename T>
inline T ValueNoise<T>::Evaluate([[maybe_unused]] float x, [[maybe_unused]] float y, [[maybe_unused]] float z) const noexcept
{
    NoiseCoordinate point_x = make_noise_coord(x);
    NoiseCoordinate point_y = make_noise_coord(y);
    NoiseCoordinate point_z = make_noise_coord(z);

    const unsigned c000 = static_cast<unsigned>(hash_table(point_x.base, point_y.base, point_z.base));
    const unsigned c100 = static_cast<unsigned>(hash_table(point_x.next, point_y.base, point_z.base));
    const unsigned c010 = static_cast<unsigned>(hash_table(point_x.base, point_y.next, point_z.base));
    const unsigned c110 = static_cast<unsigned>(hash_table(point_x.next, point_y.next, point_z.base));
    const unsigned c001 = static_cast<unsigned>(hash_table(point_x.base, point_y.base, point_z.next));
    const unsigned c101 = static_cast<unsigned>(hash_table(point_x.next, point_y.base, point_z.next));
    const unsigned c011 = static_cast<unsigned>(hash_table(point_x.base, point_y.next, point_z.next));
    const unsigned c111 = static_cast<unsigned>(hash_table(point_x.next, point_y.next, point_z.next));

    glm::vec4 smooth = fade(point_x.interpolant, point_y.interpolant, point_z.interpolant, smoothmethod);

    LinearValues<T>   one{ data[c000], data[c100] };
    LinearValues<T>   two{ data[c010], data[c110] };
    LinearValues<T>   three{ data[c001], data[c101] };
    LinearValues<T>   four{ data[c011], data[c111] };
    BiLinearValues<T> bi_one{ one, two };
    BiLinearValues<T> bi_two{ three, four };
    TriLinearValues<T> tri_one{ bi_one, bi_two };

    return trilinear_mix(tri_one, smooth.x, smooth.y, smooth.z);
}

template <typename T>
inline constexpr PeriodDimension ValueNoise<T>::GetPeriodDimension() const noexcept
{
    return period_;
}

template <typename T>
inline void ValueNoise<T>::SetPeriod(PeriodDimension period)
{
    period_ = period;
    hash_table = PermutationHash(period);

    unsigned size = static_cast<unsigned>(period);
    data.clear();
    data.resize(size);
    for (unsigned i = 0; i < size; i++)
    {
        //data[i] = (glm::vec4(util::random()));
        data[i] = (glm::vec4(glm::vec3(util::random()), 1.f));
    }
}

template <typename T>
inline constexpr SmoothMethod ValueNoise<T>::GetSmoothing() const noexcept
{
    return smoothmethod;
}

template <typename T>
inline constexpr void ValueNoise<T>::SetSmoothing(SmoothMethod smooth_method)
{
    smoothmethod = smooth_method;
}

template <typename T>
inline constexpr const std::vector<T>& ValueNoise<T>::GetValues() const noexcept
{
    return data;
}

template <typename T>
inline constexpr void ValueNoise<T>::SetValues([[maybe_unused]] std::vector<T>&& new_values)
{
    data = new_values;
}

}
