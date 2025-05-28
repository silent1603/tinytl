#include <vector>
#include <tinytl/memory/allocator/allocator.h>
#include <tinytl/memory/allocator/policy/stardard_alloc_policy.h>
#include <tinytl/memory/allocator/traits/stardard_allocator_traits.h>
int main(int argc,char* argv[])
{
    std::vector<int, Allocator<int, StardardAllocatorPolicy<int>>> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }
    v.clear();
    v.shrink_to_fit(); // triggers deallocation
    return 0;
}
