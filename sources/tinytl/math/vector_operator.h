#ifndef VECTOROPERATOR_H
#define VECTOROPERATOR_Hs
#include "tinytl/container/Vector.h"
template<typename Type, int dimension>
Vector<Type, dimension> Add(const Vector<Type, dimension>& a, const Vector<Type, dimension>& b) {
    Vector<Type, dimension> result;
    for (size_t i = 0; i < dimension; ++i)
        result.data[i] = a.data[i] + b.data[i];
    return result;
}

#endif