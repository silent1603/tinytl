#ifndef VECTOR_H
#define VECTOR_H

template<typename Type,int dimension>
union Vector
{
    Type data[dimension];
};


#endif