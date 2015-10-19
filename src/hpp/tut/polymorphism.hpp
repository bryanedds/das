#ifndef tut_polymorphism_hpp
#define tut_polymorphism_hpp

#include <cstddef>
#include <functional>

namespace tut
{
    // This tutorial gives an overview of the data abstraction-style in C++ (DAS).
    //
    // DAS is a simple, powerful, composable, and efficient style of C++ programming. It is the
    // combination of four major implementation techniques -
    //
    // 1) data abstractions rather than OOP objects
    // 2) stand-alone functions rather than member functions
    // 3) opaque mixins rather than OOP inheritance
    // 4) the following types of polymorphism rather than OOP polymorphism -
    //      a) ad-hoc polymorphism (function overloading)
    //      b) structural polymorphism (function templates without specialization)
    //      c) static polymorphisms (function template with specialization)
    //
    // Subtype polymorphism should be used only when necessary, such as in mixins and plugins.
    // 
    // In these tutorial and with the library code provided in this repository, I hope to
    // demonstrate the outright superiority of DAS programming in C++ over its alternatives.
    //
    // So let's get started!

    // Here we have a data abstraction, or 'DA'. You can tell it is different from an OOP object as
    // it has no public functions (save for ctors, dtors, and operators). Its advantages are that
    // it should keep you from using OOP unnecessarily, and has better generic usage (especially
    // with respect to the following forms of polymorphism).
    //
    // Also unlike objects, DA's only do what they must in order to implement a localized
    // semantics, rather than as much as they can for the wider system.
    //
    // For example, if you have a simulant DA in a simulation, it won't be that simulant's role to
    // hold on to a shader and texture handle to render itself. Instead, the DA will be a bag of
    // data properties with invariants that preseve consistency. To perform sepcialized tasks like
    // rendering or physics, the simulant DA sends data messages that describe the desired
    // rendering or physics results to a renderer or physics system. It is up to these
    // specialized systems to manage resources and perform the tasks that the data messages
    // describe.
    //
    // The downside to DAs is that they are considered novel in some C++ shops, and their adoption
    // may therefore pose political difficulties.
    class data_abstraction
    {
    private:

        // A private field. Note that having a single, const private field is rarely a good enough
        // reason to use a DA, but we're just giving a toy example to keeps things simple.
        const int value;

    protected:

        // Here we have a friend declaration that allows us to implement our interface. I make
        // protected because that seems to appease the ordering in more involved types, but you
        // can make them private if your prefer.
        friend int func(const data_abstraction&);

    public:

        // A public constructor that initializes the private value.
        data_abstraction(int value) : value(value) { }
    };

    // Here we write forwarding functions for our DA's functions. Exposing the interface like
    // this will provide us the additional genericity that is accompanied by ad-hoc polymorphism
    // (AKA, operator-overloading). Stand-alone functions are considered preferable to member
    // functions for many reasons. See http://www.gotw.ca/gotw/084.htm and
    // http://www.drdobbs.com/cpp/how-non-member-functions-improve-encapsu/184401197
    int func(const data_abstraction& da)
    {
        return da.value * 5;
    }

    // Unlike with OOP in C++, extending a DA is easy; just add a function in the same namesapce
    // as the DA, and you're done! Look ma, no inheritance!
    int extension(const data_abstraction& da)
    {
        return func(da) + 5;
    }

    // Structural, or 'static duck-type', polymorphism is easy, useful, and usually politically
    // viable in C++ shops. However, it's not the most powerful form of static polymorphism in
    // C++, and its applicability is therefore limited.
    template<typename T>
    int pow(const T& t)
    {
        return func(t) * func(t);
    }

    // An extended pow function.
    template<typename T>
    int pow_ex(const T& t)
    {
        return extension(t) * extension(t);
    }

    // Template specialization provides a powerful form of static polymorphism in C++. On the plus
    // side, it's generally efficient, and in simpler forms, is easy to understand. On the minus,
    // once this approach starts invoking multiple different functions on the generalized type, it
    // starts becoming more like a limited form of type classes. That, in itself is quite a good
    // thing, but considering that C++ Concepts still haven't made it into the language, code that
    // leverages this form of polymorphism in complicated ways can be increasingly difficult for
    // many devs to deal with.
    template<typename T>
    T op(const T& a, const T& b)
    {
        return a + b;
    }

    // Int specialization of op.
    template<>
    int op(const int& a, const int& b)
    {
        return a * b;
    }

    // Float specialization of op.
    template<>
    float op(const float& a, const float& b)
    {
        return a / b;
    }

    // Note that function templates shadow rather than overload, so if you want to have the same
    // generic function with a different number of parameters, you do as the standard library does
    // and suffix the name with the number parameters. This is the same approach as taken in most
    // functional languages. Usually the original function remains un-numbered, so you can apply
    // this after-the-fact.
    template<typename T>
    T op3(const T& a, const T& b, const T& c)
    {
        return a + b + c;
    }
}

#endif
