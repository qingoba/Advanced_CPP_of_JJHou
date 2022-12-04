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

class Goo
{
private:
    int x, y;
    DECLARE_POOL_ALLOC;
};

// initialize static member data
allocator Foo::alloc;
IMPLEMENT_POOL_ALLOC(Goo);

using std::cout;
using std::endl;

template <class T>
void test_static_allocator()
{
    int N = 12;
    T *p[N];
    // T::alloc.setChunk(10);
    for(int i = 0; i < N; i++)  
    {
        p[i] = new T;
        cout << p[i] << endl;   // 输出的前5个地址是连续的, 后续就不连续了
    }
    for(int i = 0; i < N; i++) delete p[i];
    for(int i = 0; i < N; i++)
    {
        p[i] = new T;
        cout << p[i] << endl;   // 倒序复用上面的地址
    }
}

int main()
{
    test_static_allocator<Foo>();
    cout << endl;
    test_static_allocator<Goo>();
}