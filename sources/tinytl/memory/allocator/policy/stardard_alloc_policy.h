#ifndef STARDARDALLOCATORPOLICY_H
#define STARDARDALLOCATORPOLICY_H

#include "alloc_policy.h"
template <typename T, std::size_t Alignment>
class StardardAllocatorPolicy : public AllocatorPolicy<T,Alignment>
{
public:
    template <typename U,std::size_t Alignment>
    struct rebind
    {
        typedef StardardAllocatorPolicy<U,Alignment> other;
    };

#if __cplusplus < 201703L

    pointer allocate(size_type size, const void *hint = 0) override
    {
        return reinterpret_cast<pointer>(::operator new(size * sizeof(T)));
    }

#elif __cplusplus <= 202002L

    pointer allocate(size_type size) override
    {
        return reinterpret_cast<pointer>(::operator new(size * sizeof(T)));
    }

#elif __cplusplus >= 202002L

    constexpr pointer allocate(size_type size)
    {
        return reinterpret_cast<pointer>(::operator new(size * sizeof(T)));
    }

#endif

    void deallocate(pointer p, size_type) override
    {
        ::operator delete(p);
    }
};
#endif