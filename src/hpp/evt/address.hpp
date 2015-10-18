#ifndef xtd_address_hpp
#define xtd_address_hpp

#include <cstddef>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include "../xtd/prelude.hpp"
#include "../xtd/string.hpp"
#include "../xtd/hash.hpp"
#include "../xtd/name.hpp"

namespace evt
{
    // The address of an event or a participant.
    class address
    {
    private:

        const std::size_t hash_code;
        const xtd::names_t names;

    protected:

        friend std::size_t get_hash_code(const address& address);
        friend xtd::names_t get_names(const address& address);

    public:

        address(const xtd::name_t& name) :
            hash_code(xtd::get_hash(name)),
            names { name } { }

        address(const xtd::names_t& names) :
            hash_code(xtd::get_hash_range<xtd::name_t>(names.cbegin(), names.cend())),
            names(names) { }

        address(const std::vector<std::string>& names) :
            address(std::transform<std::vector<xtd::name_t>>(names.cbegin(), names.cend(),
                [](const auto& name) { return xtd::name_t(name); })) { }

        address(const std::string& names_str) :
            address(xtd::split_string<std::vector<std::string>>(names_str, '/')) { }

        address(const address& that) :
            hash_code(that.hash_code),
            names(that.names) { }

        address(address&& that) :
            hash_code(std::move(that.hash_code)),
            names(std::move(that.names)) { }

        address& operator=(const address& that) = delete;

        bool operator==(const address& that) const
        {
            return
                hash_code == that.hash_code &&
                names == that.names;
        }

        address operator+(const address& right) const
        {
            xtd::names_t names_summed(names.cbegin(), names.cend());
            for (const auto& name : right.names) names_summed.push_back(name);
            return address(names_summed);
        }
    };

    std::size_t get_hash_code(const address& address)
    {
        return address.hash_code;
    }

    xtd::names_t get_names(const address& address)
    {
        return address.names;
    }
}

namespace std
{
    template<>
    struct hash<evt::address>
    {
        std::size_t operator()(const evt::address& address) const
        {
            return evt::get_hash_code(address);
        }
    };

    template<>
    struct equal_to<evt::address>
    {
        std::size_t operator()(const evt::address& left, const evt::address& right) const
        {
            return left == right;
        }
    };
}

#endif
