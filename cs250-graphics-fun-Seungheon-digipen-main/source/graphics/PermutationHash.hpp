/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once


#include <algorithm> // std::copy
#include <numeric>   // std::iota
#include <util/Random.hpp>
#include <utility> // std::swap
#include <vector>

namespace graphics::noise
{
    enum class PeriodDimension : int
    {
        _2     = 1 << 1,  // 2^1 = 2
        _4     = 1 << 2,  // 2^2 = 4
        _8     = 1 << 3,  // 2^3 = 8
        _16    = 1 << 4,  // 2^4 = 16
        _32    = 1 << 5,  // 2^5 = 32
        _64    = 1 << 6,  // 2^6 = 64
        _128   = 1 << 7,  // 2^7 = 128
        _256   = 1 << 8,  // 2^8 = 256
        _512   = 1 << 9,  // 2^9 = 512
        _1024  = 1 << 10, // 2^10 = 1024
        _2048  = 1 << 11, // 2^11 = 2048
        _4096  = 1 << 12, // 2^12 = 4096
        _8192  = 1 << 13, // 2^13 = 8192
        _16384 = 1 << 14, // 2^14 = 16384

    };

    [[nodiscard]] constexpr int period_dimension_mask(PeriodDimension period_dimension) noexcept
    {
        return static_cast<int>(period_dimension) - 1;
    }

    template <class RandomAccessIter>
    void my_random_shuffle(RandomAccessIter first, RandomAccessIter last)
    {
        int target_index = 1;
        for (auto target = first + 1; target != last; ++target_index, ++target)
        {
            const int offset = util::random(target_index + 1);
            if (offset != target_index)
            {
                std::swap(*target, *(first + offset));
            }
        }
    }

class PermutationHash
    {
    public:
        PermutationHash() = default;

        explicit PermutationHash(PeriodDimension table_size)
        {
            mask     = period_dimension_mask(table_size);
            int size = (mask + 1) << 1;

            table.resize(static_cast<unsigned>(size));
            auto middle = table.begin() + (mask + 1);
            iota(table.begin(), middle, 0);
            my_random_shuffle(table.begin(), middle);
            std::copy(table.begin(), middle, middle);
        }

        [[nodiscard]] int operator()(int x) const noexcept
        {
            unsigned mask_x = static_cast<unsigned>(x & mask);
            return table[mask_x];
        }

        [[nodiscard]] int operator()(int x, int y) const noexcept
        {
            unsigned mask_x = static_cast<unsigned>(x & mask);
            unsigned mask_y = static_cast<unsigned>(y & mask);
            mask_y          = static_cast<unsigned>(table[mask_x]) + mask_y;
            return table[mask_y];
        }

        [[nodiscard]] int operator()(int x, int y, int z) const noexcept
        {
            unsigned mask_x = static_cast<unsigned>(x & mask);
            unsigned mask_y = static_cast<unsigned>(y & mask);
            unsigned mask_z = static_cast<unsigned>(z & mask);
            mask_y          = static_cast<unsigned>(table[mask_x]) + mask_y;
            mask_z          = static_cast<unsigned>(table[mask_y]) + mask_z;

            return table[mask_z];
        }

    private:
        std::vector<int> table{};
        int              mask = 0;
    };

}