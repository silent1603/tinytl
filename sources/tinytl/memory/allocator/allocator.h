#ifndef ALLOCATOR_H
#define ALLOCATOR_H
// base Allcator
//  Layer Roles:
//  Layer	Function
//  Policy	Allocates raw memory (allocate(n)) and returns a pointer
//  Allocator	Combines policy.allocate() and traits.construct()
//  Traits	Constructs the object using placement new: new(p) T(...)
#include "policy/alloc_policy.h"
#include "traits/allocator_traits.h"

struct AllocatorMetaData
{
    std::size_t m_totalSize = 0;
    std::size_t m_used = 0;
    std::size_t m_peak = 0;
    std::size_t m_alignment = 0;
};

template <typename T, std::size_t Alignment, typename Policy = AllocatorPolicy<T, Alignment>, typename Traits = AllocatorTraits<T>, std::size_t MemoryBlockedSize = 0>
class Allocator : public Policy, public Traits
{
public:
    typedef typename Policy::value_type value_type;
    typedef typename Policy::pointer pointer;
    typedef typename Policy::size_type size_type;

    template <typename U, std::size_t Alignment = alignof(U)>
    struct rebind
    {
        typedef Allocator<U, Alignment, typename Policy::template rebind<U, Alignment>::other, typename Traits::template rebind<U>::other, MemoryBlockedSize> other;
    };

    explicit Allocator(const std::size_t totalSize = MemoryBlockedSize) noexcept
        : m_metaData{totalSize, 0, 0, Alignment}
    {
    }

    // Copy constructor from another allocator
    template <typename U, std::size_t Alignment>
    Allocator(const Allocator<U, Alignment, typename Policy::template rebind<U, Alignment>::other,
                              typename Traits::template rebind<U>::other, MemoryBlockedSize> &) noexcept
        : m_metaData{MemoryBlockedSize, 0, 0, Alignment}
    {
    }

protected:
    AllocatorMetaData m_metaData;
};
#endif