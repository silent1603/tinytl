#ifndef VECTOR_H
#define VECTOR_H

#define DEFINE_VECTOR(Type, Dimension, Name) \
    typedef struct Name {                    \
        Type data[Dimension];                \
} Name;


#endif