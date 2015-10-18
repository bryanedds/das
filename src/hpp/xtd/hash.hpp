#ifndef xtd_hash_hpp
#define xtd_hash_hpp

#include <cstddef>
#include <algorithm>
#include <numeric>

#include "prelude.hpp"

namespace xtd
{
    template<typename T>
    std::size_t get_hash(const T& t)
    {
        return std::hash<T>()(t);
    }

    template<typename T, typename It>
    std::size_t get_hash_range(const It& begin, const It& end)
    {
        return std::accumulate(begin, end, 0_z, [](auto acc, const auto& t) { return get_hash<T>(t) ^ acc; });
    }
}

#endif
