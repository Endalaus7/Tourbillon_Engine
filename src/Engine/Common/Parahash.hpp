#pragma once
#include <functional>
#include <cstddef>
#include "log.hpp"

#define Real float

namespace TourBillon
{
    template <typename T>
    inline void hash_combine(std::size_t& seed, const T& value) {
        seed ^= std::hash<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}