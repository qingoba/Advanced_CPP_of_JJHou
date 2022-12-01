#include <iostream>
#include <new>
using namespace std;

class A
{
public:
    A() { cout << "default ctor: " << this << endl; }
    A(int x) : idx(x) { cout << "para ctor: " << this << " idx=" << idx << endl; }
    ~A() { cout << "dtor called: " << this << endl; }
    void print() { cout << "for test only" << endl; }
    int idx;
};

A* func()
{
    A *p = new A(15);
    return p;
}

int main()
{
    // 1. new array 和 delete array 会按顺序逐个调用ctor和dtor
    A *p = new A[3];
    delete []p;
    puts("=========================================");

    // 2. 除非通过placement new, 否则不能显式调用构造函数; 但是可以显式调用析构函数
    p = new A(5);
    // p->A(5);        // g++ error: invalid use of 'A::A'
    // p->A::A(5);     // g++ error: cannot call constructor 'A::A' directly
    p->~A();
    delete p;   // 无论之前是否调用过析构函数, delete p都会按编译器的流程来
    puts("=========================================");

    // 3. 通过placement new来显式调用构造函数
    p = new A[3];
    A *q = p;
    for(int i = 0; i < 3; i++)
    {
        new(q)A(i);     // 使用placement new显式调用ctor
        q++;
    }
    new(q)A[3];

    func()->print();
}
