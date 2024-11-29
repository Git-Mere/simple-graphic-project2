#include "PermutationHash.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void game_over(const std::string& message)
{
    std::cout << message << std::endl;
    exit(-1);
}

#define ASSERT(expr, msg)                                                                                                                                                                              \
    if (!(expr))                                                                                                                                                                                       \
    game_over((msg))

namespace
{
    constexpr graphics::noise::PeriodDimension TableSizes[] = { graphics::noise::PeriodDimension::_2,    graphics::noise::PeriodDimension::_4,    graphics::noise::PeriodDimension::_8,
                                                                graphics::noise::PeriodDimension::_16,   graphics::noise::PeriodDimension::_32,   graphics::noise::PeriodDimension::_64,
                                                                graphics::noise::PeriodDimension::_128,  graphics::noise::PeriodDimension::_256,  graphics::noise::PeriodDimension::_512,
                                                                graphics::noise::PeriodDimension::_1024, graphics::noise::PeriodDimension::_2048, graphics::noise::PeriodDimension::_4096,
                                                                graphics::noise::PeriodDimension::_8192, graphics::noise::PeriodDimension::_16384 };
}

using namespace std::string_literals;
using namespace std;

void test_hash1d(void)
{
    for (auto table_size : TableSizes)
    {
        graphics::noise::PermutationHash hash(table_size);
        int                              size = int(table_size);
        for (int x = -size; x <= size; ++x)
        {
            const auto result = hash(x);
            ASSERT(result >= 0, "hash("s + to_string(x) + ") returned "s + to_string(result) + ". Must be >= 0"s);
            ASSERT(result < size, "hash("s + to_string(x) + ") returned "s + to_string(result) + ". Must be < "s + to_string(size) + " with period size of "s + to_string(size));
            ASSERT(result == hash(x), "calling hash("s + to_string(x) + ") multiple times should produce the same result every time! "s);
        }

        for (int y = -size; y <= size; ++y)
        {
            const auto result = hash(0, y);
            ASSERT(result >= 0, "hash(0, "s + to_string(y) + ") returned "s + to_string(result) + ". Must be >= 0"s);
            ASSERT(result < size, "hash(0, "s + to_string(y) + ") returned "s + to_string(result) + ". Must be < "s + to_string(size) + " with period size of "s + to_string(size));
            ASSERT(result == hash(0, y), "calling hash(0, "s + to_string(y) + ") multiple times should produce the same result every time! "s);
        }

        for (int z = -size; z <= size; ++z)
        {
            const auto result = hash(0, 0, z);
            ASSERT(result >= 0, "hash(0, 0, "s + to_string(z) + ") returned "s + to_string(result) + ". Must be >= 0"s);
            ASSERT(result < size, "hash(0, 0, "s + to_string(z) + ") returned "s + to_string(result) + ". Must be < "s + to_string(size) + " with period size of "s + to_string(size));
            ASSERT(result == hash(0, 0, z), "calling hash(0, 0, "s + to_string(z) + ") multiple times should produce the same result every time! "s);
        }
    }
}

void test_hash2d(void)
{
    for (auto table_size : TableSizes)
    {
        graphics::noise::PermutationHash hash(table_size);
        int                              size = int(table_size);
        for (int x = -size; x <= size; x += 2)
        {
            for (int y = -size; y <= size; y += 1)
            {
                const auto result = hash(x, y);
                ASSERT(result >= 0, "hash("s + to_string(x) + ", "s + to_string(y) + ") returned "s + to_string(result) + ". Must be >= 0"s);
                ASSERT(
                    result < size,
                    "hash("s + to_string(x) + ", "s + to_string(y) + ") returned "s + to_string(result) + ". Must be < "s + to_string(size) + " with period size of "s + to_string(size));
                ASSERT(result == hash(x, y), "calling hash("s + to_string(x) + ", "s + to_string(y) + ") multiple times should produce the same result every time! "s);
            }
        }
    }
}

void test_hash3d(void)
{
    for (auto table_size : TableSizes)
    {
        graphics::noise::PermutationHash hash(table_size);
        int                              size = int(table_size);
        for (int x = -size, y = 0; x <= size; x += 2, ++y)
        {
            for (int z = -size; z <= size; z += 1)
            {
                const auto result = hash(x, y, z);
                ASSERT(result >= 0, "hash("s + to_string(x) + ", "s + to_string(y) + ", "s + to_string(z) + ") returned "s + to_string(result) + ". Must be >= 0"s);
                ASSERT(
                    result < size, "hash("s + to_string(x) + ", "s + to_string(y) + ", "s + to_string(z) + ") returned "s + to_string(result) + ". Must be < "s + to_string(size) +
                                       " with period size of "s + to_string(size));
                ASSERT(result == hash(x, y, z), "calling hash("s + to_string(x) + ", "s + to_string(y) + ", "s + to_string(z) + ") multiple times should produce the same result every time! "s);
            }
        }
    }
}

void test_uniformity()
{
    std::unordered_map<int, int> hash_counts;
    for (auto table_size : TableSizes)
    {
        graphics::noise::PermutationHash hash(table_size);
        int                              size = int(table_size);
        hash_counts.clear();
        for (int x = -size; x < size; ++x)
        {
            const auto result = hash(x);
            hash_counts[result]++;
        }

        for (int index = 0; index < size; ++index)
        {
            ASSERT(hash_counts[index] == 2, "Number of occurrences for hash index "s + to_string(index) + " is not equal to 2. Actual: "s + to_string(hash_counts[index]) + ", Expected: 2"s);
        }

        hash_counts.clear();
        for (int y = -size; y < size; ++y)
        {
            const auto result = hash(0, y);
            hash_counts[result]++;
        }
        for (int index = 0; index < size; ++index)
        {
            ASSERT(hash_counts[index] == 2, "Number of occurrences for hash index "s + to_string(index) + " is not equal to 2. Actual: "s + to_string(hash_counts[index]) + ", Expected: 2"s);
        }

        hash_counts.clear();
        for (int z = -size; z < size; ++z)
        {
            const auto result = hash(0, 0, z);
            hash_counts[result]++;
        }
        for (int index = 0; index < size; ++index)
        {
            ASSERT(hash_counts[index] == 2, "Number of occurrences for hash index "s + to_string(index) + " is not equal to 2. Actual: "s + to_string(hash_counts[index]) + ", Expected: 2"s);
        }
    }
}

void test_same_value_repeatedly_called()
{
    graphics::noise::PermutationHash hash(graphics::noise::PeriodDimension::_256);
    int                              x = 5;
    int                              y = 10;
    int                              z = -3;

    {
        const auto result1 = hash(x);
        const auto result2 = hash(x);
        const auto result3 = hash(x);
        ASSERT(result1 == result2 && result2 == result3, "Calling hash function with the same input multiple times should produce the same result every time!");
    }
    {
        const auto result1 = hash(x, y);
        const auto result2 = hash(x, y);
        const auto result3 = hash(x, y);
        ASSERT(result1 == result2 && result2 == result3, "Calling hash function with the same input multiple times should produce the same result every time!");
    }

    {
        const auto result1 = hash(x, y, z);
        const auto result2 = hash(x, y, z);
        const auto result3 = hash(x, y, z);
        ASSERT(result1 == result2 && result2 == result3, "Calling hash function with the same input multiple times should produce the same result every time!");
    }
}

void test_non_sequential_output()
{
    graphics::noise::PermutationHash hash(graphics::noise::PeriodDimension::_16);
    std::vector<int>                 results;
    for (int x = 0; x < 16; ++x)
    {
        results.push_back(hash(x));
    }

    std::adjacent_difference(results.begin(), results.end(), results.begin());
    for (size_t i = 1; i < 16; ++i)
    {
        if (results[i] != 1)
            return;
    }

    ASSERT(false, "Output of hash should not be sequential");
}

int main(void)
{
    test_hash1d(); 
    //test_hash2d();
    //test_hash3d();
    //test_uniformity();
    
    //test_same_value_repeatedly_called();

    //using namespace std;
    //int test = 0;
    //cin >> test;
    //switch (test)
    //{
    //    case 0: test_hash1d(); break;
    //    case 1: test_hash2d(); break;
    //    case 2: test_hash3d(); break;
    //    case 3: test_uniformity(); break;
    //    case 4: test_same_value_repeatedly_called(); break;
    //    default: test_non_sequential_output(); break;
    //}
    //cout << "done\n";
    return 0;
}