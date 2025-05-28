#ifndef ALLOCATOR_H
#define ALLOCATOR_H
//base Allcator
#include "policy/alloc_policy.h"
#include "traits/allocator_traits.h"
template< typename T, typename Policy, typename Trails>
class Allocator : public Policy,public Trails
{
public:
    typedef typename Policy::value_type value_type;
    typedef typename Policy::pointer pointer;
    typedef typename Policy::size_type size_type;

    template<typename U>
    struct rebind
    {
        typedef Allocator<U,typename Policy::template rebind<U>::other,
        typename Trails::template rebind<U>::other> other;
    }

    Allocator() {}
    template<typename U>
    Allocator(const Allocator<U, typename Policy::template rebind<U>::other,
                                    typename Traits::template rebind<U>::other>&) {}
};
#endif