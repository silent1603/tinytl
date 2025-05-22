// #include "tinytl/container/Vector.h"
// #include "tinytl/math/VectorOperator.h"
// #include "tinytl/math/MathConstant.h"
// #include "tinytl/formatter/formatter.h"
// #include "tinytl/Platforms.h"

// constexpr int bufferSize = 512;
// int main(int argc, char* argv[]) 
// {

//     HANDLE hConsole = NULL;
//     DWORD written = 0;

//     // Get the console handle
//     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//     Vector<float, 3> v1 = {{1, 2, 3}};
//     Vector<float, 3> v2 = {{4, 5, 6}};
//     auto result = Add(v1, v2); //runtime


//     static_assert(PI<float> > 3.14f && PI<float> < 3.15f, "PI must be evaluated at compile time");

//     TCHAR buffer[bufferSize];
//     ZeroMemory(buffer,bufferSize);
//     StringCchPrintf(buffer, bufferSize, TEXT("PI<int>: %d\n"), PI<int>);
//     SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//     WriteConsole(hConsole,buffer,lstrlen(buffer),&written,NULL);
//     OutputDebugString(buffer);
    
//     ZeroMemory(buffer,bufferSize);
//     StringCchPrintf(buffer, bufferSize, TEXT("PI<int>: %.3f\n"), PI<float>);
//     SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//     WriteConsole(hConsole,buffer,lstrlen(buffer),&written,NULL);
//     OutputDebugString(buffer);

//     ZeroMemory(buffer,bufferSize);
//     StringCchPrintf(buffer, bufferSize, TEXT("PI<int>: %.6f\n"),PI<double>);
//     SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
//     WriteConsole(hConsole,buffer,lstrlen(buffer),&written,NULL);
//     OutputDebugString(buffer);
    

//     return 0;
// }

#include <memory>
#include <iostream>
#include <vector>

template<typename T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() = default;

    template<typename U>
    MyAllocator(const MyAllocator<U>&) {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) {
        ::operator delete(p);
    }


    bool operator==(const MyAllocator&) const { return true; }
    bool operator!=(const MyAllocator&) const { return false; }
};

int main() {
    MyAllocator<int> alloc;

    std::vector<int, MyAllocator<int>> vec(alloc);

    std::cout << "Adding elements to vector:" << std::endl;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "Vector contents: ";
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Resizing vector to 1 element:" << std::endl;
    vec.resize(1);

    std::cout << "Clearing vector:" << std::endl;
    vec.clear();

    return 0;
}