#ifndef xtd_name_hpp
#define xtd_name_hpp

#include <cstddef>
#include <string>
#include <numeric>
#include <vector>

#include "prelude.hpp"
#include "hash.hpp"

namespace xtd
{
    // A name value implemented as a data abstraction. Its hash is cached for true constant-time
    // lookup. This may not seem like a good candidate for being a data abstraction, but when we
    // decide to make hashing lazy and the hash member therefore mutable, we'll be happy for the
    // barrier.
    class name_t
    {
    private:

        const std::size_t hash_code;
        const std::string name_str;

    protected:

        friend std::size_t get_hash_code(const name_t& name);
        friend std::string get_name_str(const name_t& name);

    public:

        name_t(const std::string& name_str) : hash_code(xtd::get_hash(name_str)), name_str(name_str) { }
        name_t(const name_t& that) = default;
        name_t(name_t&& that) = default;
        name_t& operator=(const name_t& that) = delete;
        bool operator==(const name_t& that) const { return name_str == that.name_str; }
    };

    std::size_t get_hash_code(const name_t& name)
    {
        return name.hash_code;
    }

    std::string get_name_str(const name_t& name)
    {
        return name.name_str;
    }

    using names_t = std::vector<xtd::name_t>;
}

namespace std
{
    template<>
    struct hash<xtd::name_t>
    {
        std::size_t operator()(const xtd::name_t& name) const
        {
            return xtd::get_hash_code(name);
        }
    };

    template<>
    struct equal_to<xtd::name_t>
    {
        std::size_t operator()(const xtd::name_t& left, const xtd::name_t& right) const
        {
            return left == right;
        }
    };
}

#endif
