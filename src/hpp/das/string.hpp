#ifndef DAS_STRING_HPP
#define DAS_STRING_HPP

#include <string>
#include <vector>
#include <sstream>

#include "prelude.hpp"

// We need to allow operator""s in the global namespace...
#pragma warning(disable: 4455)

// This should have been in the global namespace...
inline std::string operator ""s(const char *str, std::size_t len)
{
    return std::string(str, len);
}

// Restore our warning, of course.
#pragma warning(default: 4455)

namespace das
{
    // Split a string on a char delimiter.
    inline std::vector<std::string> split_string(const std::string& str, char delimiter)
    {
        std::vector<std::string> strs{};
        std::stringstream ss(str);
        std::string token{};
        while (std::getline(ss, token, delimiter)) strs.push_back(token);
        return strs;
    }

    // Join a vector of string on a char delimiter.
    inline std::string join_strings(const std::vector<std::string>& strs, char delimiter)
    {
        std::string str_joined{};
        val& begin = std::begin(strs);
        val& end = std::end(strs);
        for (var iter = begin; iter != end; ++iter)
        {
            if (iter != begin) str_joined.push_back(delimiter);
            str_joined.append(*iter);
        }
        return str_joined;
    }

    // Convert a string to a boolean value.
    inline bool stob(const std::string& str)
    {
        return str != "false" && str != "0";
    }

    // Convert a boolean value to a string.
    inline std::string btos(bool b)
    {
        return b ? "true" : "false";
    }
}

#endif
