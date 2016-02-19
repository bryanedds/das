#ifndef das_hash_hpp
#define das_hash_hpp

#include <cstddef>
#include <numeric>
#include <functional>

#include "prelude.hpp"

namespace das
{
    // Get the hash of a T value.
    template<typename T>
    std::size_t get_hash(const T& t)
    {
        return std::hash<T>()(t);
    }

    // Get the hash of a container in terms of its content.
    template<typename T, typename It>
    std::size_t get_hash_range(const It& begin, const It& end)
    {
        return std::accumulate(begin, end, 0z, [](VAR acc, VAL& t) { return get_hash<T>(t) ^ acc; });
    }
}

#endif
