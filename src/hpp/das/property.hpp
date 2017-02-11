#ifndef DAS_PROPERTY_HPP
#define DAS_PROPERTY_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"
#include "castable.hpp"

namespace das
{
    template<typename T>
    class property : public castable
    {
    private:

        T data;

    protected:

        ENABLE_CAST(property<T>, castable);

    public:

        CONSTRAINT(property);

        template<typename A>
        using reify = property<A>;

        property() = default;
        property(const property&) = default;
        property(property&&) = default;
        property& operator=(const property&) = default;
        property& operator=(property&&) = default;

        explicit property(const T& value) : data(value) { }

        T& operator=(const T& value) { return data = value; }
        T& operator=(T&& value) { return data = value; }
        const T& operator*() const { return data; }
        T& operator*() { return data; }
    };

    template<typename T>
    const T& get_value(const property<T>& property)
    {
        return *property;
    }

    template<typename T>
    T& get_value(property<T>& property)
    {
        return *property;
    }

    template<typename T>
    T& set_value(property<T>& property, const T& value)
    {
        return property = value;
    }

    // TODO: promote to full type that is inspectable
    using property_map = std::unordered_map<name_t, std::unique_ptr<castable>>;

    template<typename T>
    property<T> get_property(const property_map& properties, const name_t& name)
    {
        const property_map::iterator property_opt = properties.find(name);
        if (property_opt) return *property_opt;
        throw std::logic_error("No such property.");
    }

    template<typename T>
    property<T>& get_property(property_map& properties, const name_t& name)
    {
        const property_map::iterator property_opt = properties.find(name);
        if (property_opt) return *property_opt;
        throw std::logic_error("No such property.");
    }
}

#endif
