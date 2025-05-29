#ifndef STARDARDALLOCATORPOLICY_H
#define STARDARDALLOCATORPOLICY_H

#include "alloc_policy.h"
template <typename T>
class StardardAllocatorPolicy : public AllocatorPolicy<T>
{
public:

    template <typename U>
    struct rebind { typedef StardardAllocatorPolicy<U> other;};

    pointer allocate(size_type n, const void* = 0) override {
        return reinterpret_cast<pointer>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type) override {
        ::operator delete(p);
    }
};
#endif