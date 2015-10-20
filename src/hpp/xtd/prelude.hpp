#ifndef xtd_prelude_hpp
#define xtd_prelude_hpp

#include <cstddef>

// We need to allow operator""z in the global namespace...
#pragma warning(disable: 4455)

// Enable std::size_t literals.
// Hopefully can be replaced with a built-in operator""z soon.
constexpr std::size_t operator""z(unsigned long long n)
{
    return static_cast<std::size_t>(n);
}

// Restore our warning, of course.
#pragma warning(default: 4455)

namespace xtd
{
    // Convert from std::size_t to int.
    constexpr int ztoi(std::size_t z) { return static_cast<int>(z); }

    // Convert from int to std::size_t.
    constexpr int itoz(int i) { return static_cast<std::size_t>(i); }

    // Zero value for a given T.
    template<typename T>
    constexpr T zero() { return static_cast<T>(0); }

    // One value for a given T.
    template<typename T>
    constexpr T one() { return static_cast<T>(1); }

    // Predecessor of a T value.
    template<typename T>
    T pred(T t) { return t - one<T>(); }

    // Successor of a T value.
    template<typename T>
    T succ(T t) { return t + one<T>(); }
}

namespace std
{
    // Transform as an expression.
    // NOTE: it is not legal to add to the std namespace, but since this form is so egregiously
    // missing, I did it anyway.
    template<typename Cr, typename It, typename Fn>
    Cr transform(const It& begin, const It& end, const Fn& fn)
    {
        Cr result;
        for (auto it = begin; it != end; ++it) result.push_back(fn(*it));
        return result;
    }
}

#endif
