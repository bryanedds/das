#ifndef xtd_castable_hpp
#define xtd_castable_hpp

#include <cstddef>
#include <typeinfo>
#include <memory>

#include "prelude.hpp"

namespace xtd
{
    // A mixin for enabling down casts without resorting to inefficient dynamic_casts.
    class castable
    {
    protected:

        virtual void const* try_cast_const(const char* type_name) const
        {
            if (type_name == typeid(castable).name()) return static_cast<void const*>(this);
            return nullptr;
        }

        virtual void* try_cast(const char* type_name)
        {
            if (type_name == typeid(castable).name()) return static_cast<void*>(this);
            return nullptr;
        }

        template<typename T, typename S>
        void const* try_cast_const_impl(const char* type_name) const
        {
            if (type_name == typeid(T).name()) return static_cast<const void*>(this);
                return S::try_cast_const(type_name);
        }

        template<typename T, typename S>
        void* try_cast_impl(const char* type_name)
        {
            if (type_name == typeid(T).name()) return static_cast<void*>(this);
                return S::try_cast(type_name);
        }

        template<typename T>
        friend const T* try_cast_const(const castable* castable);

        template<typename T>
        friend T* try_cast(castable* castable);

    public:

        castable() = default;
        castable(const castable& that) = delete;
        castable(castable&& that) = delete;
        virtual ~castable() = default;
        castable& operator=(const castable& castable) = delete;
        bool operator==(const castable& that) const = delete;
    };

    template<typename T>
    const T* try_cast_const(const castable* castable)
    {
        const char* type_name = typeid(T).name();
        return static_cast<const T*>(castable->try_cast_const(type_name));
    }

    template<typename T>
    T* try_cast(castable* castable)
    {
        const char* type_name = typeid(T).name();
        return static_cast<T*>(castable->try_cast(type_name));
    }

    template<typename T>
    const T* cast_const(const castable* castable)
    {
        const T* t_opt = try_cast_const<T>(castable);
        if (t_opt) return t_opt;
        throw std::logic_error("Invalid cast.");
    }

    template<typename T>
    T* cast(castable* castable)
    {
        T* t_opt = try_cast<T>(castable);
        if (t_opt) return t_opt;
        throw std::logic_error("Invalid cast.");
    }

    template<typename T>
    const T& cast_ref_const(const castable& castable)
    {
        return *cast_const<T>(&castable);
    }

    template<typename T>
    T& cast_ref(castable& castable)
    {
        return *cast<T>(&castable);
    }

    template<typename U, typename T>
    std::shared_ptr<U> try_cast_shared(const std::shared_ptr<T>& source)
    {
        U* u_opt = try_cast<U>(source.get());
        if (u_opt) return std::shared_ptr<U>(source, u_opt);
        return std::shared_ptr<U>();
    }

    template<typename U, typename T>
    std::shared_ptr<U> cast_shared(const std::shared_ptr<T>& source)
    {
        U* u_opt = try_cast<U>(source.get());
        if (u_opt) return std::shared_ptr<U>(source, u_opt);
        throw std::logic_error("Invalid cast.");
    }

    template<typename U, typename T>
    std::unique_ptr<U> try_cast_unique(std::unique_ptr<T> source)
    {
        U* u_opt = try_cast<U>(source.release());
        if (u_opt) return std::unique_ptr<U>(source, u_opt);
        return std::unique_ptr<U>();
    }

    template<typename U, typename T>
    std::unique_ptr<U> cast_unique(std::unique_ptr<T> source)
    {
        U* u_opt = try_cast<U>(source.release());
        if (u_opt) return std::unique_ptr<U>(u_opt);
        throw std::logic_error("Invalid cast.");
    }
}

#endif
