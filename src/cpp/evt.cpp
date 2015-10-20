#ifdef EVT_CPP

#include <cstddef>
#include <functional>
#include <string>
#include <iostream>

#include "../hpp/xtd/prelude.hpp"
#include "../hpp/xtd/string.hpp"
#include "../hpp/evt/addressable.hpp"
#include "../hpp/evt/address.hpp"
#include "../hpp/evt/eventable.hpp"

namespace prg
{
    // Forward evt api to prg's api.
    using namespace evt;

    // A trivial program type to demonstrate the eventable program mixin.
    class event_program : public evt::eventable<event_program>
    {
    protected:

        ENABLE_CAST(evt::eventable<event_program>, event_program)
    };

    // Shadow evt::handler with a simplified form.
    template<typename T>
    using handler = evt::handler<T, event_program>;
}

int main(int, char*[])
{
    /// instantiate the event program
    prg::event_program program;

    /// contrive an event for publishing
    auto event_address = prg::address("event");

    /// instantiate an event participant
    auto participant = std::make_shared<prg::addressable>(xtd::name_t("participant"));

    /// subscribe to an event with a printf handler that returns true to allow the event to propagate to any downstream handlers
    auto unsubscribe = prg::subscribe_event<std::string, prg::event_program>(program, event_address, participant, [](const auto& event, auto&)
    {
        printf("%s\r\n", event.data.c_str());
        return true;
    });

    /// publish the event!
    prg::publish_event(program, "Event handled!"s, event_address, participant);

    /// unsubscribe from the event
    unsubscribe(program);

    /// publish the event again, but to no effect
    prg::publish_event(program, "Event unhandled."s, event_address, participant);

    /// great success!
    return 0;
}

#endif
