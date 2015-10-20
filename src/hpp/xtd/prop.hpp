#ifndef xtd_prop_hpp
#define xtd_prop_hpp

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"
#include "castable.hpp"

namespace xtd
{
    // TODO: rename prop to prop_generic and insert a reflective type called prop between it and
    // castable. Props will be serializable to Nu-style algebraic expressions. Also make prop_map
    // hold the new prop type instead of castable.
    template<typename T>
    class prop : castable
    {
    private:

        T data;

    protected:

        ENABLE_CAST(castable, prop<T>)

    public:

        prop() = default
        prop(const T& value) : data(value) { }
        prop(const prop& that) : data(that.data) { }
        prop(prop&& that) : data(std::move(that.data)) { }
        prop& operator=(const prop& that) = default;
        bool operator==(const prop& that) const = default;

        const T& operator()() const { return data; }
        T& operator()() { return data; }
        T& operator=(const T& value) { return data = value; }
    };

    template<typename T>
    T get_const(const prop<T>& prop)
    {
        return prop();
    }

    template<typename T>
    T& get(prop<T>& prop)
    {
        return prop();
    }

    template<typename T>
    T& set(prop<T>& prop, const T& value)
    {
        return prop = value;
    }
}

namespace xtd
{
    using prop_map = std::unordered_map<name_t, std::unique_ptr<castable>>;

    template<typename T>
    prop<T> get_prop_const(const prop_map& properties)
    {
        const prop_map::iterator prop_opt = properties.find(name);
        if (prop_opt) return *prop_opt;
        throw std::logic_error("No such property.");
    }

    template<typename T>
    prop<T>& get_prop(prop_map& properties)
    {
        const prop_map::iterator prop_opt = properties.find(name);
        if (prop_opt) return *prop_opt;
        throw std::logic_error("No such property.");
    }
}

#endif
