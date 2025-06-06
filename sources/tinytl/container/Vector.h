#ifndef VECTOR_H
#define VECTOR_H

template <typename T, size_t N>
struct Vector
{
    T Data[N];
};

//class Template Specialization

template <>
struct Vector<float,4>
{
    union 
    {
        __m128 Vec;
        float Data[4];
        
    };
    
};


#endif