// 编译: g++ -o main test.cpp func.cpp
// 报错: multiple definition of `Foo::y'
#include "header.h"
void func();

int main()
{
    Foo *p = new Foo[10];
    delete [] p;
    func();
}