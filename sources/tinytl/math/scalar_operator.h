#ifndef SCALAR_H
#define SCALAR_H


template<typename T , typename U> //function template
auto max(T a, U b) -> decltype(a > b ? a : b) 
{
    return a > b ? a : b ;
}

// Partial Specialization
template <typename T ,typename U>
auto max(const T* a,const U* b) -> decltype(*a > *b ? *a : *b)
{
    return *a > *b ? *a: *b;
}


template<typename T, typename U>
auto add(T a, U b) -> decltype( a + b)
{
    return a + b;
}


//Template Specialization - Function overloading
template<>
double add<double,double>(double a, double b)
{
    return a + b;   
}


#endif