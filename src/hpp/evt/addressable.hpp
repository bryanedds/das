#ifndef xtd_addressable_hpp
#define xtd_addressable_hpp

#include <cstddef>

#include "../xtd/prelude.hpp"
#include "../xtd/castable.hpp"
#include "../xtd/name.hpp"

namespace evt
{
    // A mixin for making a type addressable.
    //
    // The policy hard-wired into this mixin is that an addressable's name cannot change. If an
    // addressable needs a different name, the policy is to copy it with a different name, then
    // discard the original. This actually works best in simulators because trying to implement
    // mutable identities is overly complicating in practice.
    class addressable : public xtd::castable
    {
    private:

        const xtd::name_t name;

    protected:

        ENABLE_CAST(castable, addressable)

        friend xtd::name_t get_name(const addressable& addressable);

    public:

        addressable(const xtd::name_t& name) : name(name) { }
        addressable(const addressable& that) = delete;
        addressable(addressable&& that) = delete;
        addressable& operator=(const addressable& that) = delete;
        bool operator==(const addressable& that) const = delete;
    };

    xtd::name_t get_name(const addressable& addressable)
    {
        return addressable.name;
    }
}

#endif
