#ifndef BASEALLOCATOR_H
#define BASEALLOCATOR_H

template <typename T>
class BaseAllocatorTrails
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T value_type;

    template <class U>
    struct rebind
    {
        typedef allocator<U> other;
    }
};

// template<typename T> struct allocator {
// template<typename U> struct rebind {
// typedef allocator<U> other;
// };
// };
// allocator<int>::rebind<char>::other char_alloc
template <typename T>
class BaseAllocatorPolicy
{
public:
    T *allocate(size_type size_t, const T *hint = 0);
    void deallocate(T *p, size_t size);
    size_t max_size() const;
};

template <typename T>
class BaseAllocatorTypeTrails
{
public:
    void construct(T *p, const t &val);
    void destroy(T *p);

};

template <typename T,
          typename AllocatorTrails = BaseAllocatorTrails<T>,
          typename AllocatorPolicy = BaseAllocatorPolicy<T>,
          typename AllocatorTypeTrails = BaseAllocatorTypeTrails<T>>
class BaseAllocator : AllocatorTrails, AllocatorPolicy, AllocatorTypeTrails
{
public:
    BaseAllocator();
    BaseAllocator(const BaseAllocator &);

    template <class U>
    BaseAllocator(const BaseAllocator<U> &);
    ~BaseAllocator();


};

template<typename T>
bool operator == (const BaseAllocator<T>& a, const BaseAllocator<T>& b);

template<typename T>
bool operator != (const BaseAllocator<T>& a, const BaseAllocator<T>& b);
#endif