#ifndef xtd_cid_hpp
#define xtd_cid_hpp

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <limits>

#include "prelude.hpp"

namespace xtd
{
    // A id that is unique by counting.
    // Small enough to be passed by value.
    class id_t
    {
    public:

        const uint64_t x;
        const uint64_t y;

        constexpr id_t() : x(zero<uint64_t>()), y(zero<uint64_t>()) { }
        constexpr id_t(uint64_t x, uint64_t y) : x(x), y(y) { }
        constexpr id_t(const id_t& that) = default;
        constexpr id_t(id_t&& that) = default;
        id_t& operator=(const id_t& that) = delete;
        bool operator==(const id_t& that) const { return x == that.x && y == that.y; }

        static constexpr id_t invalid() { return id_t(); }
    };

    template<>
    constexpr id_t zero<id_t>()
    {
        return id_t::invalid();
    }

    template<>
    id_t succ<id_t>(id_t id)
    {
        if (id.x == std::numeric_limits<uint64_t>::max())
        {
            if (id.y == std::numeric_limits<uint64_t>::max()) throw std::out_of_range("xtd::id_t overflowed.");
            return id_t(zero<uint64_t>(), succ(id.y));
        }
        return id_t(succ(id.x), id.y);
    }
}

#endif
