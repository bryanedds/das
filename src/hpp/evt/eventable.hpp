#ifndef xtd_event_system_hpp
#define xtd_event_system_hpp

#include <cstddef>
#include <unordered_map>
#include <memory>

#include "../xtd/prelude.hpp"
#include "../xtd/id.hpp"
#include "../xtd/castable.hpp"

#include "addressable.hpp"
#include "address.hpp"
#include "subscription.hpp"

namespace evt
{
    // A program mixin for enabling publisher-neutral events in a program. What is a program mixin?
    // Well, it's like any other C++ mixin, except it's intended for use on the type that end-user
    // will represent his program with. Program mixins are the good alternative to OOP Singletons.
    template<typename P>
    class eventable : public xtd::castable
    {
    private:

        std::unique_ptr<xtd::id_t> pred_id;
        subscriptions_map subscriptions_map;

    protected:

        ENABLE_CAST(xtd::castable, eventable<P>)

        template<typename P>
        friend xtd::id_t provide_subscription_id(P& program);

        template<typename T, typename P>
        friend void subscribe_event5(P& program, xtd::id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const handler<T, P>& handler);

        template<typename T, typename P>
        friend void publish_event(P& program, const T& event_data, const address& address, const std::shared_ptr<addressable>& publisher);

    public:

        eventable() : pred_id(std::make_unique<xtd::id_t>()), subscriptions_map() { }
        eventable(const eventable& that) = delete;
        eventable(eventable&& that) = delete;
        eventable& operator=(const eventable& that) = delete;
        bool operator==(const eventable& that) const = delete;
    };

    template<typename P>
    xtd::id_t provide_subscription_id(P& program)
    {
        const auto pred_id(*program.pred_id);
        const auto succ_id(xtd::succ(pred_id));
        program.pred_id = std::make_unique<xtd::id_t>(succ_id);
        return succ_id;
    }

    template<typename T, typename P>
    void subscribe_event5(P& program, xtd::id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const handler<T, P>& handler)
    {
        auto subscription_detail(xtd::cast_unique<xtd::castable>(std::make_unique<subscription_detail<T, P>>(handler)));
        const auto subscription(std::make_shared<subscription>(subscription_id, subscriber, std::move(subscription_detail)));
        const auto subscriptions_opt(program.subscriptions_map.find(address));
        if (subscriptions_opt != program.subscriptions_map.end())
        {
            subscriptions_opt->second->push_back(subscription);
        }
        else
        {
            auto subscriptions(std::make_unique<subscription_list>());
            subscriptions->push_back(subscription);
            program.subscriptions_map.insert(std::make_pair(address::address(address), std::move(subscriptions)));
        }
    }

    template<typename T, typename P>
    void subscribe_event(P& program, const handler<T, P>& handler, const address& address, const std::shared_ptr<addressable>& subscriber)
    {
        subscribe_event5<T, P>(program, evt::provide_subscription_id(program), address, subscriber, handler);
    }

    template<typename T, typename P>
    void publish_event(P& program, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher)
    {
        auto subscriptions_opt(program.subscriptions_map.find(event_address));
        if (subscriptions_opt != program.subscriptions_map.end())
        {
            auto subscriptions_copy(*subscriptions_opt->second);
            for (const auto& subscription : subscriptions_copy)
            {
                const auto cascade(publish_subscription<T, P>(*subscription, program, event_data, event_address, publisher));
                if (!cascade) break;
            }
        }
    }
}

#endif
