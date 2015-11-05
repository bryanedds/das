#ifndef tut_polymorphism_hpp
#define tut_polymorphism_hpp

#include <cstddef>
#include <functional>

#include "../xtd/castable.hpp"

namespace tut
{
    // This tutorial gives an introduction to the data abstraction-style in C++ (DAS).
    //
    // DAS is a simple, powerful, composable, and efficient style of C++ programming. It is the
    // combination of four major implementation techniques -
    //
    // 1) data abstractions rather than OOP objects
    // 2) stand-alone functions rather than member functions
    // 3) composable mixins rather than OOP inheritance
    // 4) the following types of polymorphism rather than OOP polymorphism -
    //      a) ad-hoc polymorphism (function overloading)
    //      b) structural polymorphism (function templates without specialization)
    //      c) static polymorphisms (function template with specialization)
    //
    // Subtype polymorphism should be used only when necessary, such as in mixins and plugins.
    // 
    // In these tutorial and with the library code provided in this repository, I hope to
    // demonstrate the validity of DAS programming in C++ as an alternative to OOP.
    //
    // So let's get started!

    // Here we have a data abstraction, or 'DA'. You can tell it is different from an OOP object as
    // it has no public functions (save for ctors, dtors, and operators). Its has better generic
    // usage (especially with respect to the above preferred forms of polymorphism) and exposes a
    // much simpler and more extensible interface. Adding a new function to a DA is as easy as
    // opening up its containing namespace and plopping one in.
    //
    // Also unlike objects, DA's do only what they must in order to implement a localized
    // semantics rather than doing as much as possible for the wider system.
    //
    // For example, if you have a simulant DA in a simulation, it won't be that simulant's role to
    // hold on to a shader and texture handle to in order to render itself. Instead, the DA will be
    // a bag of data properties with consistency invariants. To perform specialized tasks like
    // rendering or physics, the simulant DA sends data messages that describe the desired
    // rendering or physics results to a renderer or physics system. It is up to these
    // specialized systems to manage resources and perform the tasks that the data messages
    // describe.
    //
    // The downside to DAs is that they are considered novel in some C++ shops, and may therfore
    // pose adoption difficulties in practice.
    class data_abstraction
    {
    private:

        // A private field. Note that having a single, const private field is rarely a good enough
        // reason to use a DA, but we're just giving a toy example to keeps things simple.
        const int value;

    protected:

        // Here we have a friend declaration that allows us to implement our interface. I make it
        // protected because that seems to gel with the ordering in more involved DA types, but you
        // can make them private if your prefer.
        friend int func(const data_abstraction&);

    public:

        // A public constructor that initializes the private value.
        data_abstraction(int value) : value(value) { }
    };

    // Here we expose our DA's interface with stand-alone functions. For more on why stand-alone
    // functions are considered superior to OO-style member functions, see -
    // http://www.gotw.ca/gotw/084.htm and -
    // http://www.drdobbs.com/cpp/how-non-member-functions-improve-encapsu/184401197
    int func(const data_abstraction& da)
    {
        return da.value * 5;
    }

    // Unlike with OO interfaces, extending a DA is easy; even if you're in a different file or
    // code base, just open up the DA's namespace and add functions to you heart's content!
    int func_ex(const data_abstraction& da)
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
        return func_ex(t) * func_ex(t);
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

    // As you can see, a great variety of computation and program characteristics can be cleanly,
    // efficiently, and most importantly _generically_ expressed using just the above concepts.
    // However, we will still need late-binding to achieve certain types of abstractions. This
    // is where DAS mixins come in.
    //
    // What is a mixin in the context of data-abstraction style?
    //
    // Mixins in this context are a bit different than what most C++ articles about mixins present.
    // Rather than being programming components that allow the injection of capabilities into a
    // class a posteriori via template arguments, our mixins are simple data abstractions that each
    // provide a single orthogonal capability to another data abstraction via inheritance.
    //
    // A good example of a mixin is implemented in '../hpp/xtd/castable.hpp'. By inheriting from
    // this mixin, you get dynamic castability capabilities via -
    // 1) A common base type xtd::castable.
    // 2) Virtual try_cast functions, with default template impl functions to ease overriding.
    // 3) A completely general public interface, including casting with smart ptrs.
    //
    // The following is a type that leverages the castable mixin -
    class widget : public xtd::castable
    {
    private:

        const int upc;
        const float age;
        const bool replacable;

    protected:

        // Override xtd::castable::try_cast_const from mixin.
        void const* try_cast_const(const char* type_name) const override
        {
            return try_cast_const_impl<castable>(this, type_name);
        }

        // Override xtd::castable::try_cast from mixin.
        void* try_cast(const char* type_name) override
        {
            return try_cast_impl<castable>(this, type_name);
        }

        friend int get_upc(const widget& widget);
        friend bool should_replace(const widget& widget, float age_max);

    public:

        // Construct our widget.
        widget(int upc, float age, bool replacable) :
            upc(upc),
            age(age),
            replacable(replacable) { }
    };

    // Get the upc of a widget.
    int get_upc(const widget& widget)
    {
        return widget.upc;
    }

    // Query that a widget should be replaced.
    bool should_replace(const widget& widget, float age_max)
    {
        return widget.replacable && widget.age > age_max;
    }

    // Query that a widget should be replaced with a give product.
    bool should_replace_with(const widget& widget, float age_max, int upc)
    {
        return should_replace(widget, age_max) && get_upc(widget) == upc;
    }
}

#endif
