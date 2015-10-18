#ifdef TUT_CPP

#include <cstddef>
#include <functional>
#include <string>
#include <iostream>

#include "../hpp/tut/polymorphism.hpp"
#include "../hpp/tut/injection.hpp"
#include "../hpp/tut/safety.hpp"

void example()
{
    /// leveraging our data abstraction
    tut::data_abstraction da(10);
    std::cout << tut::func(da);
    std::cout << tut::extension(da);

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
}

void example2()
{
    /// here we simply create our dependency on the stack
    tut::pretend_messenger messenger;

    /// and then we create the world on the stack as well, injecting its needed dependencies
    tut::world world(messenger);

    /// software architecture can be surprisingly elegant and easy in C++ - if we set ourselves
    /// up properly :)
    std::cout << tut::run(world);
}

void example3()
{
    /// TODO: demonstrate exception safety with DAs
}

int main(int, char*[])
{
    example();
    example2();
    example3();
    return 0;
}

#endif
