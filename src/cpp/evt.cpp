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
    prg::event_program event_program;

    /// instantiate an event participant
    std::shared_ptr<prg::addressable> participant(std::make_shared<prg::addressable>(xtd::name_t("participant")));

    /// construct our event handler to just printf
    prg::handler<std::string> handler([](auto&, const auto& event) { printf(event.data.c_str()); return true; });

    /// subscribe to the "signal" event with our handler
    prg::subscribe_event(event_program, handler, prg::address("event"), participant);

    /// publish the event!
    prg::publish_event(event_program, std::string("Event handled!"), prg::address("event"), participant);

    /// great success!
    return 0;
}

#endif
