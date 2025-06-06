#ifndef ARRAY_H
#define ARRAY_H

template <typename T, size_t N>
class Array
{
public:
    Array() : m_Data {}
    size_t size() const
    {
        return N;
    }

    T &operator[](size_t i)
    {
        assert(i < N);

        return m_Data[i];
    }

    const T &operator[](size_t i) const
    {
        assert(i < N);

        return m_Data[i];
    }

private:
    T m_Data[N];
};

//Template Specialization

template<> //Partial Specialization
class Array<float, 4>
{
public:
    Array()
    : m_Vec(_mm_setzero_ps())
    {}
 
    size_t size() const
    {
        return 4;
    }
 
    float& operator[](size_t i)
    {
        assert(i < 4);
 
        return m_Data[i];
    }
 
    const float& operator[](size_t i) const
    {
        assert(i < 4);
 
        return m_Data[i];
    }
 
private:
    union
    {
        __m128 m_Vec;     // Vectorized data.
        float  m_Data[4]; // Float data.
    };
};

template<typename T> //Partial Specialization
class Array<T, 4>
{
public:
    Array()
    : m_Vec(_mm_setzero_ps())
    {}
 
    size_t size() const
    {
        return 4;
    }
 
    T& operator[](size_t i)
    {
        assert(i < 4);
 
        return m_Data[i];
    }
 
    const T& operator[](size_t i) const
    {
        assert(i < 4);
 
        return m_Data[i];
    }
 
private:
    union
    {
        __m128 m_Vec;     // Vectorized data.
        T  m_Data[4]; // Float data.
    };
};

template<size_t N> //Partial Specialization
class Array<float, N>
{
public:
    Array()
    : m_Vec(_mm_setzero_ps())
    {}
 
    size_t size() const
    {
        return N;
    }
 
    float& operator[](size_t i)
    {
        assert(i < N);
 
        return m_Data[i];
    }
 
    const float& operator[](size_t i) const
    {
        assert(i < N);
 
        return m_Data[i];
    }
 
private:

    float  m_Data[N]; // Float data.

};




//Partial Specialization
template<typename T,size_t N>
template<typename T, size_t N>
class Array<T*, N>
{
public:
    explicit Array(T* data)
        : m_Data(data)
    {}

    T& operator[](size_t i)
    {
        assert(i < N);

        return m_Data[i];
    }

    const T& operator[](size_t i) const
    {
        assert(i < N);

        return m_Data[i];
    }

private:
    T* m_Data;
};

#endif