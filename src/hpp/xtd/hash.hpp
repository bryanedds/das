#ifndef xtd_hash_hpp
#define xtd_hash_hpp

#include <cstddef>
#include <numeric>

#include "prelude.hpp"

namespace xtd
{
    // Get the hash of a T value.
    template<typename T>
    std::size_t get_hash(const T& t)
    {
        return std::hash<T>()(t);
    }

    // Get the hash of a container in terms of its contents.
    template<typename T, typename It>
    std::size_t get_hash_range(const It& begin, const It& end)
    {
        return std::accumulate(begin, end, 0z, [](auto acc, const auto& t) { return get_hash<T>(t) ^ acc; });
    }
}

#endif
