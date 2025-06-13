#ifndef VECTOR_H
#define VECTOR_H

#define DEFINE_VECTOR(Type, Dimension, Name) \
    typedef struct Name {                    \
        Type data[Dimension];                \
} Name;
DEFINE_VECTOR(int,2,Vector2i)

#endif