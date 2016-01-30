#ifndef DAS_ADDRESS_HPP
#define DAS_ADDRESS_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include "prelude.hpp"
#include "string.hpp"
#include "hash.hpp"
#include "name.hpp"

namespace das
{
    // The address of an event or a participant.
    class address
    {
    private:

        std::size_t hash_code;
        std::vector<name_t> names;

    protected:

        friend const std::vector<name_t>& get_names(const address& address);

    public:

        address() = default;
        address(const address& that) = default;
        address(address&& that_mvb) = default;
        address& operator=(const address&) = default;
        address& operator=(address&&) = default;

        explicit address(const name_t& name) : hash_code(get_hash(name)), names({ name }) { }
        explicit address(const std::vector<name_t>& names) : hash_code(get_hash_range<name_t>(names.cbegin(), names.cend())), names(names) { }
        explicit address(std::vector<name_t>&& names_mvb) : hash_code(get_hash_range<name_t>(names_mvb.cbegin(), names_mvb.cend())), names(names_mvb) { }
        explicit address(const std::vector<std::string>& names) : address(std::transform<std::vector<name_t>>(names.cbegin(), names.cend(), [](val& name) { return name_t(name); })) { }
        explicit address(const char* names_str) : address(std::string(names_str)) { }
        explicit address(const std::string& names_str) : address(split_string(names_str, '/')) { }

        bool address::operator==(const address& that) const
        {
            return
                hash_code == that.hash_code && // OPTIMIZATION: assuming hash_code check could hasten eq chacks
                names == that.names;
        }

        address address::operator+(const address& right) const
        {
            std::vector<name_t> names_summed(names.cbegin(), names.cend());
            for (val& name : right.names) names_summed.push_back(name);
            return address(names_summed);
        }

        explicit operator std::size_t() const { return hash_code; }
    };


    // Get the names of which an address consists.
    const std::vector<name_t>& get_names(const address& address)
    {
        return address.names;
    }
}

namespace std
{
    template<>
    struct hash<das::address>
    {
        std::size_t operator()(const das::address& address) const
        {
            return static_cast<std::size_t>(address);
        }
    };

    template<>
    struct equal_to<das::address>
    {
        std::size_t operator()(const das::address& left, const das::address& right) const
        {
            return left == right;
        }
    };
}

#endif
