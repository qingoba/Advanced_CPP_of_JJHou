#include <iostream>

class allocator
{
private:
    struct linker { linker *next; };  // embedded pointer
    linker *freeStore = nullptr;      // head of freelist
    int chunk = 5;                    // default block size
public:
    void setChunk(int x) { chunk = x; }
    void *allocate(size_t size)
    {
        linker *p = nullptr;
        if(freeStore == nullptr)
        {
            p = freeStore = (linker *)malloc(size * chunk);
            for(int i = 0; i < chunk - 1; i++)
            {
                p->next = (linker *)((char *)p + size);
                p = p->next;
            }
            p->next = nullptr; 
        }
        p = freeStore;
        freeStore = freeStore->next;
        return p;
    }
    void deallocate(void *block, size_t size)
    {
        if(block == nullptr) return;
        linker *p = (linker *)block;
        p->next = freeStore;
        freeStore = p;
    }
};

/* test staic_allocator

#include "static_allocator.h"

class Foo
{
private:
    int x, y;   // 值得注意的是, 对象占用的空间必须比next指针大;
                // 在64位环境下执行, 指针占用8个字节, int只占用4个字节, 所以如果只有一个变量x, 则出错
public:
    static allocator alloc;
    static void *operator new(size_t size) 
    { return alloc.allocate(size); }
    static void operator delete(void *block, size_t size) 
    { return alloc.deallocate(block, size); }
};

// initialize static member data
allocator Foo::alloc;

using std::cout;
using std::endl;

int main()
{
    int N = 12;
    Foo *p[N];
    Foo::alloc.setChunk(10);
    for(int i = 0; i < N; i++)  
    {
        p[i] = new Foo;
        cout << p[i] << endl;   // 输出的前十个地址是连续的, 后续就不连续了
    }
}
*/