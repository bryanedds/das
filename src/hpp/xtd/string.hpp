#ifndef xtd_string_hpp
#define xtd_string_hpp

#include <string>
#include <sstream>

#include "prelude.hpp"

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
