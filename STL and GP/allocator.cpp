#include <list>
#include <string>
#include <memory>                       // std::allocator
#include <ext\array_allocator.h>
#include <ext\mt_allocator.h>           // mt == multi thread
#include <ext\debug_allocator.h>
#include <ext\pool_allocator.h>
#include <ext\bitmap_allocator.h>
#include <ext\malloc_allocator.h>
#include <ext\new_allocator.h>

using std::list;
using std::string;

int main()
{
    // std 和 __gnu_cxx 都是命名空间
    list<string, std::allocator<string>> c1;
    list<string, __gnu_cxx::malloc_allocator<string>> c2;
    list<string, __gnu_cxx::new_allocator<string>> c3;
    list<string, __gnu_cxx::__pool_alloc<string>> c4;

    // 直接使用分配器
    std::allocator<int> alloc;
    auto p = alloc.allocate(sizeof(int));
}