DAS - Data Abstraction Style in C++
===

A Tutorial, Library, and Sample Program written in the Data Abstraction Style.
--------------------------------------------------------------------------

In the 'Notes to the Reader' section 1.2.1 on page 11, Bjarne Stroustrup mentions the four major supported styles of programming in C++ -

- Procedural programming
- Data abstraction
- Object-oriented programming
- Generic programming

He later attempts to describe data abstraction in terms of classes, esp. with respect to abstract classes. Unfortunately, he does not draw a clear distinction between classes as used for data abstraction and as used for object-orientation. This is particularly wanting because PLT shows quite clearly that not only is data abstraction different from object-orientation, they are in fact duals. It is a further confounding that the mechanisms used to implement each style have a surprising degree of overlap.

For the rest of the book, and for the rest of all C++ literature I've encountered, the concept of data abstraction remains unexplored.

However, as the world wakes up to the disastrous results of using object-orientation as the golden hammer to our modern software problems, it is the time to revisit this lesser discussed concept of data abstraction.

In this repository, I give a strong and precise definition of data abstraction style in C++. Additionally, I build both a very small standard library called 'xtd' as well as a publisher-neutral event system called 'evt', as well as an toy demo program, all of which are implemented in the data abstraction style. The result has been code that is significanly simpler, more efficient, more powerful, and more reliable than code implemented in the object-oriented style.

To get started, begin by reading the first literate tutorial here - das/src/hpp/tut/polymorphism.hpp

Have fun, and please report any feeback, criticism, oversights, or errors as a new issue here - https://github.com/bryanedds/das/issues
