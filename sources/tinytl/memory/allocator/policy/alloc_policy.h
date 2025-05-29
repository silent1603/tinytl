#ifndef ALLOCATORPOLICY_H
#define ALLOCATORPOLICY_H
// base policy
// Allocator Policy = Memory Resource Management
// Allocator policy provides low-level memory operations:
// How to allocate a block of raw memory (e.g. malloc, new, VirtualAlloc, memory pool)
// How to deallocate that block
// These operate on uninitialized raw memory — no objects are constructed or destroyed yet.
// Think of this like: “Give me N bytes” and “Free this memory.”
template <typename T, std::size_t Alignment>
class AllocatorPolicy
{
protected:
    size_t m_alignment = Alignment;
public:
    typedef T value_type;
    typedef T *pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <typename U,std::size_t Alignment>
    struct rebind
    {
        typedef AllocatorPolicy<U,Alignment> other;
    };

#if __cplusplus < 201703L
    // C++98 to C++14
    virtual pointer allocate(size_type size, const void *hint = 0) = 0;

#elif __cplusplus <= 202002L
    // C++17
    virtual pointer allocate(size_type size) = 0;
#endif

    virtual void deallocate(pointer p, size_type) = 0;

#if __cplusplus < 201703L
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
#endif

    constexpr const std::size_t GetAlignment() const
    {
        return m_alignment;
    }
};
#endif