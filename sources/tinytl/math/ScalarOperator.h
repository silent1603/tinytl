#ifndef SCALAR_H
#define SCALAR_H

template<typename T , typename U>
auto max(T a, U b) -> decltype(a > b ? a : b)
{
    return a > b ? a : b ;
}



#endif