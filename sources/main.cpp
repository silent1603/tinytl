#include "tinytl/container/Vector.h"
#include "tinytl/math/VectorOperator.h"

int main(int argc, char* argv[]) 
{
    Vector<float, 3> v1 = {{1, 2, 3}};
    Vector<float, 3> v2 = {{4, 5, 6}};
    auto result = Add(v1, v2);

    return 0;
}