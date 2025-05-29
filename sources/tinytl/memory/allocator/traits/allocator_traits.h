#ifndef ALLOCATORTRAITS_H
#define ALLOCATORTRAITS_H
//base traits 
// specialize behavior for constructing, destroying, or getting the address of objects
// Object Construction & Destruction
// Traits define how to:
// Construct an object in the given memory using placement new
// Destroy an object (i.e., call the destructor)
// construct() uses placement new: new(p) T(value)
// destroy()uses destructor directly: p->~T();
// These are important for non-trivial types that:
//  Need copy/move constructors
//  Hold resources (e.g., file handles, pointers, etc.)

template <typename T>
class AllocatorTraits
{
public:
    template<typename U>
    struct rebind { typedef AllocatorTraits<U> other;};

    T* address(T& r) const {return &r;}
    const T* address(const T& r) const {return &r;}


    virtual void construct(T* p, const T& value) const = 0;
    virtual void destroy(T* p) const  = 0;
};
#endif