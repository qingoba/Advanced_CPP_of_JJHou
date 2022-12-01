#include <iostream>
#include <cstdlib>      // malloc(), free()
#include <vector>
using namespace std;

// 尝试重载:
// 1. operator new 和 operator delete
// 2. operator new[] 和 operator delete[]
// 3. ::operator new 和 ::operator delete
// 4. ::operator new[] 和 ::operator delete[]

// 在operator delete(void *, size_t)中, 第二个参数是可选的, 如果带了第二个参数,
// 则编译器会传入, 该参数表明free释放的空间大小
class A
{
public:
    A() { cout << "default ctor: " << this << endl; }
    A(int x) : idx(x) { cout << "para ctor: " << this << ", idx=" << idx << endl; }
    ~A() { cout << "dtor called: " << this << endl; }
    int idx;

    // 1. operator new 和 operator delete
    static void *operator new(size_t size)
    {
        cout << "operator new, size = " << size << endl;
        return malloc(size);
    }
    static void operator delete(void *block, size_t size)   // 第二个参数是可选的
    {
        cout << "operator delete, size = " << size << endl;
        free(block);
    }

    // 2. operator new[] 和 operator delete[]
    static void *operator new[](size_t size)
    {
        cout << "operator new[], size = " << size << endl;
        return malloc(size);
    }
    static void operator delete[](void *block, size_t size) // 第二个参数是可选的
    {
        cout << "operator delete[], size = " << size << endl;
        free(block);
    }

};

// 3. ::operator new 和 ::operator delete
void *operator new(size_t size)
{
    cout << "Global operator new, size = " << size << endl;
    return malloc(size);
}
void operator delete(void *block, size_t size)  // 如果不加第二个参数, 则编译器不识别
{
    cout << "Global operator delete, size = " << size << endl;
    free(block);
}

// 4. ::operator new[] 和 ::operator delete[]
void *operator new[](size_t size)
{
    cout << "Global operator new[], size = " << size << endl;
    return malloc(size);
}
void operator delete[](void *block, size_t size)  // 如果不加第二个参数, 则编译器不识别
{
    cout << "Global operator delete[], size = " << size << endl;
    free(block);
}

int main()
{
    // 结合编译器对new的处理过程可知这样写是正确的
    // 先分配空间, 然后在返回的指针处构造元素
    A *p = new A(15);
    A *q = new A();
    // 先析构, 然后删除对象本身的空间(即删除sizeof(A));
    delete(p);
    delete(q);
    puts("=========================================");

    p = new A[3];   // 编译器处理, 分配N*sizeof(A) + 8的空间, 当delete p时, [p-8]中存放大小
    delete [] p;
    puts("=========================================");

    vector<int> *p3 = ::new vector<int>;    // 通过::显式调用全局new和delete
    ::delete p3;
    vector<int> *q3 = ::new vector<int> [10];
    ::delete []q3;
    A *p4 = ::new A[10];
    ::delete [] p4;
}