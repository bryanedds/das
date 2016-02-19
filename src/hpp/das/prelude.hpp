#ifndef DAS_PRELUDE_HPP
#define DAS_PRELUDE_HPP

// General Note: This library target C++14-conformant (or near-conformant) compilers.
//
// The newer C++ features from which this library would greatly benefit are that of Concepts,
// Uniform Call Syntax, and Filesystem. Modules would also be beneficiary, but to a limited extent
// due to the small size of this library and its dependencies.

// TODO: enable -wall, and selectively disable noise warnings.
#include <cstddef>

// Variable shadowing is a good thing when doing functional-style programming.
#pragma warning(disable: 4456)

// We need to allow operator""z in the global namespace...
#pragma warning(disable: 4455)

// Use of unconditional branches is a common technique in C++ metaprogramming.
#pragma warning(disable: 4127)

// Short-hand for immutable auto.
#define VAL auto const

// Short-hand for mutable auto.
#define VAR auto

// Declare a type to be constraint.
#define CONSTRAINT(constraint_name) \
    using constraint_name##_constraint = void

// Assert a type constraint.
#define CONSTRAIN(type, constraint_name) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::constraint_name##_constraint, unique_type>::value, #type " needs to satisfy " #constraint_name "."); \
    } while (false)

// Assert a type is an iterator.
#define CONSTRAIN_AS_ITERATOR(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::iterator_category, unique_type>::value, #type " needs to be an iterator."); \
    } while (false)

// Assert a type is a container.
#define CONSTRAIN_AS_CONTAINER(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::size_type, unique_type>::value, #type " needs to be a container."); \
    } while (false)

// Assert a type is a shared ptr.
#define CONSTRAIN_AS_SHARED_PTR(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::element_type, unique_type>::value, #type " needs to be a shared_ptr."); \
    } while (false)

// Assert a type is a unique ptr.
#define CONSTRAIN_AS_UNIQUE_PTR(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::element_type, unique_type>::value, #type " needs to be a unique_ptr."); \
    } while (false)

// Enable std::size_t literals.
// Hopefully can be replaced with a built-in operator""z soon.
constexpr std::size_t operator""z(unsigned long long n)
{
    return static_cast<std::size_t>(n);
}

// Restore our warning, of course.
#pragma warning(default: 4455)

namespace das
{
    // Convert from std::size_t to int.
    constexpr int ztoi(std::size_t z) { return static_cast<int>(z); }

    // Convert from int to std::size_t.
    constexpr std::size_t itoz(int i) { return static_cast<std::size_t>(i); }

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

    // The unit type.
    class unit { public: CONSTRAINT(unit); };
}

namespace std
{
    // Transform as an expression.
    // NOTE: it is not legal to add to the std namespace, but since this form is so egregiously
    // missing, I did it anyway.
    template<typename Cr, typename It, typename Fn>
    Cr transform(const It& begin, const It& end, const Fn& fn)
    {
        CONSTRAIN_AS_CONTAINER(Cr);
        CONSTRAIN_AS_ITERATOR(It);
        Cr transformed{};
        for (VAR it = begin; it != end; ++it)
        {
            VAL& temp = fn(*it);
            transformed.insert(std::end(transformed), temp);
        }
        return transformed;
    }
}

#endif
