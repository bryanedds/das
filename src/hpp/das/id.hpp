#ifndef DAS_ID_HPP
#define DAS_ID_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <limits>

#include "prelude.hpp"

namespace das
{
    // A unique, counting identifier.
    // Intended to be passed by value.
    // Not intended to be serialized.
    class id_t
    {
    public:

        const int64_t x;
        const int64_t y;

        constexpr id_t() : x(zero<int64_t>()), y(zero<int64_t>()) { }
        constexpr id_t(const id_t&) = default;
        constexpr id_t(id_t&&) = default;
        id_t& operator=(const id_t&) = default;
        id_t& operator=(id_t&&) = default;

        constexpr id_t(int64_t x, int64_t y) : x(x), y(y) { }
        constexpr bool operator==(const id_t& that) const { return x == that.x && y == that.y; }
        static constexpr id_t invalid() { return id_t(); }
        explicit operator std::size_t() const { return x ^ y; }
    };

    template<>
    constexpr id_t zero<id_t>()
    {
        return id_t::invalid();
    }

    template<>
    inline id_t succ<id_t>(id_t id)
    {
        if (id.x == std::numeric_limits<int64_t>::max())
        {
            if (id.y == std::numeric_limits<int64_t>::max()) throw std::out_of_range("das::id_t overflowed.");
            return id_t(zero<int64_t>(), succ(id.y));
        }
        return id_t(succ(id.x), id.y);
    }
}

namespace std
{
    template<>
    struct hash<das::id_t>
    {
        std::size_t operator()(const das::id_t& id) const
        {
            return static_cast<std::size_t>(id);
        }
    };

    template<>
    struct equal_to<das::id_t>
    {
        std::size_t operator()(const das::id_t& left, const das::id_t& right) const
        {
            return left == right;
        }
    };
}

#endif
