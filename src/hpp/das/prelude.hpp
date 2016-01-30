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
#define val auto const

// Short-hand for mutable auto.
#define var auto

// Declare a field is intended to be immutable, even tho it's not const to allow assignment.
#define immutable

// Declare a type to be constraint.
#define constraint(constraint_name) \
    using constraint_name##_constraint = void

// Assert a type constraint.
#define constrain(type, constraint_name) \
    typename type::constraint_name##_constraint assert_constraint_fn(); \
    (void)&assert_constraint_fn

// Assert a type is an iterator.
#define constrain_as_iterator(type) \
    typename type::iterator_category assert_iterator_constraint_fn(); \
    (void)&assert_iterator_constraint_fn

// Assert a type is a container.
#define constrain_as_container(type) \
    typename type::size_type assert_container_constraint_fn(); \
    (void)&assert_container_constraint_fn

// Assert a type is a unique ptr.
#define constrain_as_unique_ptr(type) \
    typename type::element_type assert_unique_ptr_constraint_fn(); \
    (void)&assert_unique_ptr_constraint_fn

// Assert a type is a shared ptr.
#define constrain_as_shared_ptr(type) \
    typename type::element_type assert_shared_ptr_constraint_fn(); \
    (void)&assert_shared_ptr_constraint_fn

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
    class unit { public: constraint(unit); };
}

namespace std
{
    // Transform as an expression.
    // NOTE: it is not legal to add to the std namespace, but since this form is so egregiously
    // missing, I did it anyway.
    template<typename Cr, typename It, typename Fn>
    Cr transform(const It& begin, const It& end, const Fn& fn)
    {
        constrain_as_container(Cr);
        constrain_as_iterator(It);
        Cr transformed{};
        for (var it = begin; it != end; ++it)
        {
            val& temp = fn(*it);
            transformed.insert(std::end(transformed), temp);
        }
        return transformed;
    }
}

#endif
