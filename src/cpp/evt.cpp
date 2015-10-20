#ifdef EVT_CPP

#include <cstddef>
#include <functional>
#include <string>
#include <iostream>

#include "../hpp/xtd/prelude.hpp"
#include "../hpp/xtd/castable.hpp"
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

    /// instantiate an event participant
    const auto& participant(std::make_shared<prg::addressable>(xtd::name_t("participant")));

    /// construct our event handler to just printf
    const auto& handler([](auto&, const auto& event) { printf("%s\r\n", event.data.c_str()); return true; });

    /// subscribe to an event with our handler
    const auto& unsubscribe(prg::subscribe_event<std::string, prg::event_program>(program, handler, prg::address("event"), participant));

    /// publish the event!
    prg::publish_event(program, std::string("Event handled!"), prg::address("event"), participant);

    /// unsubscribe from the event
    unsubscribe(program);

    /// publish the event again, but to no effect
    prg::publish_event(program, std::string("Event handled!"), prg::address("event"), participant);

    /// great success!
    return 0;
}

#endif
