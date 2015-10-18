#ifndef xtd_event_hpp
#define xtd_event_hpp

#include <cstddef>
#include <memory>

#include "../xtd/prelude.hpp"

#include "address.hpp"

namespace evt
{
    // The event type. There is no need to make this an abstract data type for the same reason
    // there's no need to make a Matrix an abstract data type. Sometimes data is just data and
    // life is a breeze!
    template<typename T>
    struct event
    {
        const T data;
        const address address;
        const std::shared_ptr<addressable> subscriber; /// TODO: may be necessary to genericize subscriber
        const std::shared_ptr<addressable> publisher;

        event(
            const T& data,
            const evt::address& address,
            std::shared_ptr<addressable> subscriber,
            std::shared_ptr<addressable> publisher) :
            data(data),
            address(address),
            subscriber(subscriber),
            publisher(publisher) { }

        event(const event& that) = default;
        event(event&& that) = default;
        event& operator=(const event& that) = delete;
        bool operator==(const event& that) = delete;
    };
}

#endif
