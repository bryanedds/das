#ifndef DAS_EVENT_SYSTEM_HPP
#define DAS_EVENT_SYSTEM_HPP

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <memory>

#include "prelude.hpp"
#include "id.hpp"
#include "castable.hpp"
#include "addressable.hpp"
#include "address.hpp"
#include "subscription.hpp"

namespace das
{
    template<typename P>
    using unsubscriber = std::function<void(P&)>;

    // A program mixin for enabling publisher-neutral events in a program. What is a program mixin?
    // Well, it's like any other C++ mixin, except it's intended for use on the type that end-user
    // will represent his program with. Program mixins are the good alternative to OOP Singletons.
    template<typename P>
    class eventable : public castable
    {
    private:

        std::unique_ptr<id_t> pred_id;
        subscriptions_map subscriptions_map;
        unsubscription_map unsubscription_map;

    protected:

        enable_cast(eventable<P>, castable);

        template<typename P>
        friend id_t get_subscription_id(P& program);

        template<typename P>
        friend void unsubscribe_event(P& program, id_t subscription_id);

        template<typename T, typename P, typename H>
        friend unsubscriber<P> subscribe_event5(P& program, id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler);

        template<typename T, typename P>
        friend void publish_event(P& program, const T& event_data, const address& address, const std::shared_ptr<addressable>& publisher);

    public:

        constraint(eventable);

        eventable() :
            castable(),
            pred_id(std::make_unique<id_t>()),
            subscriptions_map(),
            unsubscription_map()
        { }
    };

    template<typename P>
    id_t get_subscription_id(P& program)
    {
        constrain(P, eventable);
        val& pred_id = *program.pred_id;
        val& succ_id = succ(pred_id);
        program.pred_id = std::make_unique<id_t>(succ_id);
        return succ_id;
    }

    template<typename P>
    void unsubscribe_event(P& program, id_t subscription_id)
    {
        constrain(P, eventable);
        val& unsubscription_opt = program.unsubscription_map.find(subscription_id);
        if (unsubscription_opt != std::end(program.unsubscription_map))
        {
            val& subscriptions_opt = program.subscriptions_map.find(unsubscription_opt->second.first);
            if (subscriptions_opt != std::end(program.subscriptions_map))
            {
                var& subscriptions = *subscriptions_opt->second;
                subscriptions.erase(
                    std::remove_if(
                    std::begin(subscriptions),
                    std::end(subscriptions),
                    [unsubscription_opt](val& subscription)
                { return subscription->subscriber_opt.lock().get() == unsubscription_opt->second.second.lock().get(); }));
                program.unsubscription_map.erase(unsubscription_opt);
            }
        }
    }

    template<typename T, typename P, typename H>
    unsubscriber<P> subscribe_event5(P& program, id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler)
    {
        constrain(P, eventable);
        var subscription_detail_mvb = cast_unique<castable>(std::make_unique<subscription_detail<T, P>>(handler));
        var subscriptions_opt = program.subscriptions_map.find(address);
        if (subscriptions_opt != std::end(program.subscriptions_map))
        {
            val& subscription = std::make_shared<das::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
            subscriptions_opt->second->push_back(subscription);
        }
        else
        {
            val& subscription = std::make_shared<das::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
            var subscriptions_mvb = std::make_unique<subscription_list>();
            subscriptions_mvb->push_back(subscription);
            program.subscriptions_map.insert(std::make_pair(address::address(address), std::move(subscriptions_mvb)));
        }
        program.unsubscription_map.insert(std::make_pair(subscription_id, std::make_pair(address, subscriber)));
        return [subscription_id](P& program) { unsubscribe_event(program, subscription_id); };
    }

    template<typename T, typename P, typename H>
    unsubscriber<P> subscribe_event(P& program, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler)
    {
        constrain(P, eventable);
        return subscribe_event5<T, P>(program, get_subscription_id(program), address, subscriber, handler);
    }

    template<typename T, typename P>
    void publish_event(P& program, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher)
    {
        constrain(P, eventable);
        val& subscriptions_opt = program.subscriptions_map.find(event_address);
        if (subscriptions_opt != std::end(program.subscriptions_map))
        {
            val subscriptions_copy = *subscriptions_opt->second;
            for (val& subscription : subscriptions_copy)
            {
                val cascade = publish_subscription<T, P>(*subscription, event_data, event_address, publisher, program);
                if (!cascade) break;
            }
        }
    }
}

#endif
