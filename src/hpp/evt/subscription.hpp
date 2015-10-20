#ifndef xtd_subscription_hpp
#define xtd_subscription_hpp

#include <cstddef>
#include <cassert>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

#include "../xtd/prelude.hpp"
#include "../xtd/id.hpp"
#include "../xtd/castable.hpp"

#include "addressable.hpp"
#include "address.hpp"
#include "event.hpp"

namespace evt
{
    template<typename T, typename P>
    using handler = std::function<bool(P&, const event<T>&)>;

    template<typename T, typename P>
    class subscription_detail : public xtd::castable
    {
    private:

        const handler<T, P> handler;

    protected:

        using subscription_detailTC = subscription_detail<T, P>;
        ENABLE_CAST(xtd::castable, subscription_detailTC)

    public:

        subscription_detail() = delete;
        subscription_detail(const evt::handler<T, P>& handler) : handler(handler) { }
        subscription_detail(const subscription_detail& that) = delete;
        subscription_detail(subscription_detail&& that) = delete;
        subscription_detail& operator=(const subscription_detail& that) = delete;

        bool operator()(P& program, const evt::event<T>& event) const
        {
            return handler(program, event);
        }
    };

    class subscription
    {
    protected:

        template<typename T, typename P>
        friend bool publish_subscription(const subscription& subscription, P& program, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher);

    public:

        const xtd::id_t id;
        const std::weak_ptr<addressable> subscriber_opt;
        const std::unique_ptr<xtd::castable> subscription_detail;

        subscription(
            xtd::id_t id,
            std::shared_ptr<addressable> subscriber,
            std::unique_ptr<xtd::castable> subscription_detail) :
            id(id),
            subscriber_opt(subscriber),
            subscription_detail(subscription_detail.release()) { }

        subscription(const subscription& that) = delete;
        subscription(subscription&& that) = delete;
        subscription& operator=(const subscription& that) = delete;
        bool operator==(const subscription& that) const = delete;
    };

    template<typename T, typename P>
    bool publish_subscription(const subscription& subscription, P& program, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher)
    {
        if (!subscription.subscriber_opt.expired())
        {
            const auto subscriber(subscription.subscriber_opt.lock());
            const auto event(event<T>(event_data, event_address, subscriber, publisher));
            const auto* subscription_detail_opt(xtd::try_cast_const<subscription_detail<T, P>>(subscription.subscription_detail.get()));
            if (subscription_detail_opt) return (*subscription_detail_opt)(program, event);
            return true;
        }
        return true;
    }

    using subscription_list = std::vector<std::shared_ptr<subscription>>;

    using subscriptions_map = std::unordered_map<address, std::unique_ptr<subscription_list>>;

    using unsubscription_map = std::unordered_map<xtd::id_t, std::pair<address, std::weak_ptr<addressable>>>;
}

#endif
