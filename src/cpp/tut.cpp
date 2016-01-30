#ifdef TUT_CPP

#include <cstddef>
#include <memory>
#include <functional>
#include <string>
#include <iostream>

#include "../hpp/tut/tut.hpp"

int main(int, char*[])
{
    /// leveraging our data abstraction
    tut::data_abstraction da(10);
    std::cout << tut::func(da) << std::endl;
    std::cout << tut::func_ex(da) << std::endl;

    /// we don't need std::bind for stand-alone functions - they're much easier to pass around than
    /// member functions.
    const auto fp(&tut::func);
    std::cout << fp(da) << std::endl;

    /// leveraging structural polymorphism
    std::cout << tut::pow(da) << std::endl;
    std::cout << tut::pow_ex(da) << std::endl;

    /// leveraging static polymorphism
    std::cout << tut::op(3, 5) << std::endl;
    std::cout << tut::op(3.0f, 5.0f) << std::endl;
    std::cout << tut::op('3', '5') << std::endl;

    /// make a widget, and cast it back and forth
    auto widget = std::make_unique<tut::widget>(12345, 101.0f, true);
    auto castable = das::cast_unique<das::castable>(std::move(widget));
    auto widget_again = das::cast_unique<tut::widget>(std::move(castable));

    /// check its replacability :)
    std::cout << tut::should_replace_with(*widget_again, 100.0f, 12345) << std::endl;
    return 0;
}

#endif
