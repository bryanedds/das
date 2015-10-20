#ifndef xtd_string_hpp
#define xtd_string_hpp

#include <string>
#include <sstream>

#include "prelude.hpp"

// We need to allow operator""s in the global namespace...
#pragma warning(disable: 4455)

// This should also be in the global namespace...
inline std::string operator ""s(const char *str, std::size_t len)
{
    return std::string(str, len);
}

// Restore our warning, of course.
#pragma warning(default: 4455)

namespace xtd
{
    // Split a string on a char delimiter.
    // TODO: could be turned into a function template just called 'split' if implmeented with
    // generic iterators and delimiters.
    template<typename Cr>
    Cr split_string(std::string str, char delimiter)
    {
        Cr strs;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) strs.push_back(token);
        return strs;
    }
}

#endif
