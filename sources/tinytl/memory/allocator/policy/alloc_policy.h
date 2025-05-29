#ifndef ALLOCATORPOLICY_H
#define ALLOCATORPOLICY_H
// base policy
// Allocator Policy = Memory Resource Management
// Allocator policy provides low-level memory operations:
// How to allocate a block of raw memory (e.g. malloc, new, VirtualAlloc, memory pool)
// How to deallocate that block
// These operate on uninitialized raw memory — no objects are constructed or destroyed yet.
// Think of this like: “Give me N bytes” and “Free this memory.”
template <typename T>
class AllocatorPolicy
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;


    template <typename U>
    struct rebind { typedef AllocatorPolicy<U> other;};

    virtual pointer allocate(size_type n, const void* = 0) = 0;
    virtual void deallocate(pointer p, size_type) = 0;

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};
#endif