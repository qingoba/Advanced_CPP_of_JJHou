#include <iostream>
#include <new>
#include <cstdlib>
#include <cstring>
using namespace std;

class Foo
{
public:
    Foo(char ch2, char ch4) : c2(ch2), c4(ch4) {}   // c1 和 c3 执行 default initialized
    char c1, c2, c3, c4;

};

int main()
{
    char *p = (char *)malloc(12);
    p[0] = 0;
    strncat(p, "Hello World", 11);
    cout << p << endl;

    Foo *q = (Foo *)p;
    q = new(q)Foo('2','4');
    cout << q->c1 << q->c2 << q->c3 << q->c4 << endl;   // ouput: H2l4, 证明了默认初始化
}