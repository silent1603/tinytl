#ifndef STARDARDALLOCATORPOLICY_H
#define STARDARDALLOCATORPOLICY_H

#include "alloc_policy.h"
#include <stdlib.h> // for malloc ,free()

template <typename T>
class CAllocatorPolicy : public AllocatorPolicy<T>
{
public:
    template <typename U>
    struct rebind
    {
        typedef CAllocatorPolicy<U> other;
    };

#if __cplusplus < 201703L

    pointer allocate(size_type size, const void *hint = 0) override
    {
        return malloc(size);
    }

#elif __cplusplus <= 202002L

    pointer allocate(size_type size) override
    {
        return malloc(size);
    }

#elif __cplusplus >= 202002L

    constexpr pointer allocate(size_type size)
    {
        return malloc(size);
    }
#endif

    void deallocate(pointer p, size_type) override
    {
        ::operator delete(p);
    }
};
#endif