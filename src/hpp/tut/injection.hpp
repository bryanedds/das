#ifndef tut_injection_hpp
#define tut_injection_hpp

#include <cstddef>
#include <string>
#include <iostream>

namespace tut
{
    // Dependency injection in C++ is easy, if you do it carefully. The quickest way to do it
    // wrong is to use a Singleton. A slower way to do it wrong is to use objects (that will
    // often just suck you into the OOP quagmire). Instead you should use an interface implemented
    // with a data abstraction. Interfaces destined for use with DAs are a little different. Like
    // DAs, they have no public instance functions, but rather public functions instead in their
    // containing namespace.
    class messenger
    {
    protected:

        // Encrypt a message.
        virtual std::string encrypt(const std::string& message) const = 0;

        // Send a message.
        virtual void send(const std::string& message) = 0;

        // Befriend stand-alone function to provide it with the internals it needs.
        friend void send_message(messenger& messenger, const std::string& message);
    };

    // Data abstraction-style interfaces are non-members to enable ad-hoc polymorphism, easy
    // extension, and enhanced genericity.
    void send_message(messenger& messenger, const std::string& message)
    {
        const auto encrypted(messenger.encrypt(message));
        messenger.send(encrypted);
    }

    // Here we implement the above interface with an abstraction whose implementation here is mostly
    // faked. We don't need to write any forwarding functions since the forwarding function
    // written for the above interface already took care of it :)
    class pretend_messenger : public messenger
    {
    private:

        const int connection_handle;

    protected:

        std::string encrypt(const std::string& message) const override
        {
            /// here we pretend we are encrypting a message fit for a specific type of connection
            return std::string(message.crbegin(), message.crend());
        }

        void send(const std::string& message) override
        {
            /// here we pretend to send the message over a connection with the given handle
            std::cout << message;
        }

    public:

        // Here we just pretend the constructor gets a real connection handle.
        pretend_messenger() : connection_handle(0) { }
    };

    // Finally, like in the previous gist article, we implement an example abstraction, except this time
    // we call it something more specific so as not to confuse it with the abstraction above.
    class world
    {
    private:

        // Reference to the injected dependency.
        messenger& messenger;

    protected:

        // The stand-alone friend.
        friend int run(world& world);

    public:

        // Our world with the messenger dependency injected into it.
        world(class messenger& messenger) : messenger(messenger) { }
    };

    // Our DA's interface is exposed as a simple function for ad-hoc polymorphism and genericity :)
    int run(world& world)
    {
        send_message(world.messenger, std::string("Returning a number!"));
        return 25;
    }
}

#endif
