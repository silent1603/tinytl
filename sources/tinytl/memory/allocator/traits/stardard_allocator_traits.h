#ifndef STARDARDALLOCATORTRAITS_H
#define STARDARDALLOCATORTRAITS_H
#include "allocator_traits.h"
template <typename T>
class StardardAllocatorTraits : public AllocatorTraits<T>
{
public:
    void construct(T* p, const T& value) const override { new(p) T(value); }
    void destroy(T* p) const override { p->~T(); }
};
#endif