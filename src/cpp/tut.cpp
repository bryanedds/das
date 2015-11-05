#ifdef TUT_CPP

#include <cstddef>
#include <memory>
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

    /// make a widget, and cast it back and forth
    auto widget = std::make_unique<tut::widget>(12345, 101.0f, true);
    auto castable = xtd::cast_unique<xtd::castable>(std::move(widget));
    auto widget_again = xtd::cast_unique<tut::widget>(std::move(castable));

    /// check its replacability :)
    std::cout << tut::should_replace_with(*widget_again, 100.0f, 12345);
    return 0;
}

#endif
