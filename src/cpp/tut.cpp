#ifdef TUT_CPP

#include <cstddef>
#include <functional>
#include <string>
#include <iostream>

#include "../hpp/tut/intro.hpp"

int main(int, char*[])
{
    /// leveraging our data abstraction
    tut::data_abstraction da(10);
    std::cout << tut::func(da);
    std::cout << tut::func_ex(da);

    /// we don't need std::bind for stand-alone functions - they're much easier to pass around than
    /// member functions.
    const auto fp(&tut::func);
    std::cout << fp(da);

    /// leveraging structural polymorphism
    std::cout << tut::pow(da);
    std::cout << tut::pow_ex(da);

    /// leveraging static polymorphism
    std::cout << tut::op(3, 5);
    std::cout << tut::op(3.0f, 5.0f);
    std::cout << tut::op('3', '5');
    return 0;
}

#endif
