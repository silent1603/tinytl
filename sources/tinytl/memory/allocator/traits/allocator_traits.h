#ifndef ALLOCATORTRAITS_H
#define ALLOCATORTRAITS_H
//base traits 
// specialize behavior for constructing, destroying, or getting the address of objects
template <typename T>
class AllocatorTraits
{
public:
    template<typename U>
    struct rebind { typedef AllocatorTraits<U> other;}

    T* address(T& r) const {return &r;}
    const T* address(const T& r) const {return &r;}


    virtual void construct(T* p, const T& value) const = 0;
    virtual void destroy(T* p) const  = 0;
};
#endif