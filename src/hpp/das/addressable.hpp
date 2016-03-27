#ifndef DAS_ADDRESSABLE_HPP
#define DAS_ADDRESSABLE_HPP

#include <cstddef>

#include "prelude.hpp"
#include "castable.hpp"
#include "name.hpp"

namespace das
{
    // A mixin for making a type addressable.
    //
    // The policy hard-wired into this mixin is that an addressable's name cannot change. If an
    // addressable needs a different name, the policy is to copy it with a different name, then
    // discard the original. This actually works best in simulators because trying to implement
    // mutable identities is overly complicating in practice, and in theory actually makes no
    // sense anyways.
    class addressable : public castable
    {
    private:

        name_t name;

    protected:

        ENABLE_CAST(addressable, castable);
        friend name_t get_name(const addressable& addressable);

    public:

        CONSTRAINT(addressable);
        explicit addressable(const name_t& name) : name(name) { }
    };

    inline name_t get_name(const addressable& addressable)
    {
        return addressable.name;
    }
}

#endif
