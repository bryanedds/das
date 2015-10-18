#ifndef tut_safety_hpp
#define tut_safety_hpp

#include <cstddef>

namespace tut
{
    // Data Abstraction Style and Exception Safety.
    //
    // First, a preamble ramble -
    //
    // Why does the author take as read the need to avoid OOP in C++?
    //
    // Rather than attempting to communicate my experience with why object-orientation (as in
    // pervasive use of dynamic subtype polymorhpism) should be avoided, I will instead make an
    // appeal to the design of C++'s modern std library.
    //
    // C++'s std library scrupulously avoids dynamic subtype polymorphism for the same reason the
    // author of this article does - it makes code less generic, less efficient, more complex in
    // terms of execution, and harder to reason about behavior at run-time. In practice, these can
    // be huge trade-offs, and should not to be made without a clear need to do so (such as in the
    // case of plug-ins or user-definable simulation components).
    //
    // DAs aren't just for experts implementing container libraries and algorithmic functions,
    // they're for every day C++ programmers trying to build clean, elegant, efficient, and
    // decoupled code. And not just for common libraries, but all the way up the abstraction
    // ladder.
}

#endif
