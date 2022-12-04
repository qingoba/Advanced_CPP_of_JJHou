Memory Management 101 (内存管理 - 从平地到万丈高楼)

## 01 OVERVIEW

我们通过GNU C 4.9.2来看一些个性化的分配器

我们通过Visual C++ 6.0来看malloc函数

所有的内存管理动作最后都使用malloc, 该函数对我们的课程十分重要

参考网站https://gee.cs.oswego.edu/dl/

GNU C中malloc的算法完全来自Doug Lea, 其算法被称为DL malloc

## 02 C++ Memory Primitives

## 03 使用四种Primitives

```c++
// malloc() + free()
void *p1 = malloc(512);
free(p1);

// new + delete
complex<int> *p2 = new complex<int>;
delete p2;

// ::operator new  + ::operator delete
void *p3 = ::operator new(512);
::operator delete(p3);

// std::allocator, 两个函数都不是static的, 必须通过对象调用
std::allocator<int> alloc;
int *p4 = alloc.allocate(3);
alloc.deallocate(p4, 3);

// G2.91, alloc, 这两个函数都是static的, 可通过类名直接调用
void *p5 = alloc::allocate(512);
alloc::deallocate(p5, 512);

// G4.9.2
void *p6 = __gnu_cxx::__pool_alloc<int>().allocate(9);
__gnu_cxx::__pool_alloc().deallocate((int *)p6, 9);
```

## 04 new and delete expression/keyword

假设有两条语句``Complex *pc = new Complex(1, 2);`` 和 ``delete pc;``

编译器会将第一句翻译成如下形式

```c++
// 展开 Complex *pc = new Complex(1, 2);
Complex *pc;
try{
    void *mem = operator new (sizeof(Complex));	// allocate
    pc = static_cast<Complex*>(mem);	// cast
    pc->Complex::Complex(1, 2);	 // construct, 这样写合法吗? 怎样直接调用ctor? 使用placement new?
}catch(std::alloc){
    
}

// 上述的operator new是一个函数, 定义在VC98/CRT/SRC/newop2.cpp中
// 可以发现, operator new就是单纯的调用malloc()
// 该函数第二个参数表明不抛出异常, 如果没有可用内存, 则只是单纯返回空指针
// C++ 11已经有了更标准的不抛出异常的写法
void *operator new(size_t size, const std::nothrow_t&) _THROW0()
{	
	// try to allocate size bytes
	void *p;
	while ((p = malloc(size)) == 0)
	{	
		// buy more memory or return null pointer
		_TRY_BEGIN
			if (_callnewh(size) == 0)	// newh = new handler
				break;
		_CATCH(std::bad_alloc)
			return (0);
		_CATCH_END
	}
	return (p);
}
```

编译器会将第二句翻译成如下形式

```c++
// 展开 delete pc
pc->~Complex();
operator delete(pc);

// 上述operator delete是一个函数, 定义在VC98/CRT/SRC/delop.cpp中
// 可以发现, operator delete只是单纯的调用free()
void __cdecl operator delete(void *p) _THROW0()
{	
	// free an allocated object
	free(p);
}
```

## 05 不能直接调用``ctor``和``dtor``?

```c++
class A;
A *p = new A(1);

p->A::A(3);		// 显式调用构造函数, VC6编译通过, GCC编译失败(Cannot call constructor directly)
```

## 06 07 array new 和 array delete 和 placement new

array new 的含义是通过new创建一个数组, 比如``int *p = new int [10]``

```c++
Complex *pca = new Complex[3];	// 唤起三次ctor, 无法借由参数给予初始值; Complex必须要有default ctor
delete pca[];	// 唤起三次dtor

// 如果使用array new分配, 而不使用array delete销毁, 会怎么样?
// 如果类中没有堆区的内存, 则可能不会有影响
// 如果类中有堆区的内存, 则堆区的内存泄露
// 对于那些使用默认析构函数的类来说, 可以不对其使用array delete
// 为什么呢? 不使用array delete的话, 那些sizeof(obj)的内存怎么办? 他们也在堆区啊
```

使用placement new显式调用构造函数

```cpp
p = new A[3];
A *q = p;
for(int i = 0; i < 3; i++)
{
    new(q)A(i);     // 使用placement new显式调用ctor
    q++;
}
```

## 08 placement new

placement new允许我们将对象构造在已经分配的内存中, placement new本身并不分配内存

placement new也可以被称为``new(p)`` 或 ``::operator new(size, void*)``

使用placement new构造

```cpp
char *buf = new char[BUFSIZE];
Complex *pc = new(buf)Complex(1, 2); 	// 在buf的起始处构造一个Complex
```

## 09 10 重载 new 和 delete

new一个对象时, 编译器会调用operator new

```c++
// 展开 Complex *pc = new Complex(1, 2);
Complex *pc;
try{
    void *mem = operator new (sizeof(Complex));	// 调用全局或成员operator new
    pc = static_cast<Complex*>(mem);	// cast
    // 编译器通过一种方法, 在p处调用Complex的构造函数, 不是通过operator new(size_t, void *)来调用的
}catch(std::bad_alloc){ }
```

有两种operator new, 一种是成员的, 一种是全局的

如果成员没有自定义其operator new, 则在使用new表达式时, 会调用全局的operator new

如果成员自定义了其operator new, 则在使用new表达式时, 会调用该成员内部的operator new

如果想显式使用全局new, 则写法是`Complex *pc = ::new Complex`

也就是说, 在上述try包围的三条语句中, 重载operator new只会影响第一句



**现在的问题是: array new会被编译器翻译成什么? 比如``Complex *pc = new Complex[N]``**



**前十节课无非就是讲解了如下几个问题, 感觉可以直接看讲义**

1. new 和 delete 的编译器展开
2. 什么是 operator new, 有两种operator new, 两种operator new皆可以被重载
3. placement new用于在已经分配的内存中构造对象

## VS 2019 对于 new 和 new array 的编译

也可以参考https://blog.csdn.net/zhaxun/article/details/120139067, 其汇编的代码更简单

该网站可以实时对代码进行反汇编https://godbolt.org/, 而且可以选择编译器版本, 极佳

VS 2019 中 operator new 和 operator new[] 的声明和定义

```c++
// 声明位于Professional\VC\Tools\MSVC\14.29.30133\crt\src\vcruntime\vcruntime_new.h中
// 还有不抛出异常的版本, 此处未列出
void* __CRTDECL operator new(size_t _Size);
void* __CRTDECL operator new[](size_t _Size);
// operator new(size_t) 的定义, 位于crt\src\vcruntime\new_scalar.cpp中
void* __CRTDECL operator new(size_t const size)
{
    for (;;)
    {
        if (void* const block = malloc(size))
        {
            return block;
        }
        if (_callnewh(size) == 0)
        {
            if (size == SIZE_MAX)
            {
                __scrt_throw_std_bad_array_new_length();
            }
            else
            {
                __scrt_throw_std_bad_alloc();
            }
        }
        // The new handler was successful; try to allocate again...
    }
}
// operator new[](size_t) 的定义, 位于crt\src\vcruntime\new_array.cpp中
void* __CRTDECL operator new[](size_t const size)
{
    return operator new(size);
}
```



使用如下程序测试编译器对 new array 的处理过程, 对 delete array 的处理过程是类似的.

```c++
class A
{
public:
    A() { cout << "default ctor: " << this << endl; }
    ~A() { cout << "dtor called: " << this << endl; }
    int idx;
};

int main()
{
    A* p = new A[88];
    delete p[];
}
```

在 VS 中, 使用 Ctrl + F11 进入反汇编, 得到如下结果

```c++
				A* p = new A[88];	// 如下是这条指令的汇编过程
00007FF7D0A32907  mov         ecx,168h // 这里的0x168 = 360 = 88 x 4 + 8, 传给new的空间比实际对象的空间多了8
00007FF7D0A3290C  call        operator new[] (07FF7D0A31212h)  
00007FF7D0A32911  mov         qword ptr [rbp+188h],rax  
00007FF7D0A32918  cmp         qword ptr [rbp+188h],0  
00007FF7D0A32920  je          main+0A5h (07FF7D0A32975h)  
00007FF7D0A32922  mov         rax,qword ptr [rbp+188h]  
00007FF7D0A32929  mov         qword ptr [rax],58h  
00007FF7D0A32930  mov         rax,qword ptr [rbp+188h]  
00007FF7D0A32937  add         rax,8  
00007FF7D0A3293B  lea         rcx,[A::~A (07FF7D0A31348h)]  
00007FF7D0A32942  mov         qword ptr [rsp+20h],rcx  
00007FF7D0A32947  lea         r9,[A::A (07FF7D0A31415h)]  
00007FF7D0A3294E  mov         r8d,58h  
00007FF7D0A32954  mov         edx,4  
00007FF7D0A32959  mov         rcx,rax  
00007FF7D0A3295C  call        `eh vector constructor iterator' (07FF7D0A31055h) // 循环构造88个A对象

00007FF7D0A31055  jmp         `eh vector constructor iterator' (07FF7D0A32C70h)

// 如下指令这些指令循环执行, 顺序构造出88个A对象
				Test2.exe!`eh vector constructor iterator'
00007FF7D0A32C70  mov         qword ptr [success],r9
...... // 从0x32C98处的指令开始循环
00007FF7D0A32C98  mov         rax,qword ptr [i]  
00007FF7D0A32CA0  mov         qword ptr [i],rax  
00007FF7D0A32CA5  mov         rax,qword ptr [count]  
00007FF7D0A32CAA  cmp         qword ptr [i],rax  
00007FF7D0A32CAF  je          `eh vector constructor iterator'+7Ch (07FF7D0A32CECh)  
00007FF7D0A32CB1  mov         rax,qword ptr [constructor]  
00007FF7D0A32CB6  mov         qword ptr [rsp+30h],rax  
00007FF7D0A32CBB  mov         rax,qword ptr [rsp+30h]  
00007FF7D0A32CC0  mov         qword ptr [rsp+38h],rax  
00007FF7D0A32CC5  mov         rcx,qword ptr [ptr]  
00007FF7D0A32CCA  mov         rax,qword ptr [rsp+38h]  
00007FF7D0A32CCF  call        qword ptr [__guard_dispatch_icall_fptr (07FF7D0A46020h)] 	// 调用ctor 
00007FF7D0A32CD5  mov         rax,qword ptr [size]  
00007FF7D0A32CDA  mov         rcx,qword ptr [ptr]  
00007FF7D0A32CDF  add         rcx,rax  
00007FF7D0A32CE2  mov         rax,rcx  
00007FF7D0A32CE5  mov         qword ptr [ptr],rax  
00007FF7D0A32CEA  jmp         `eh vector constructor iterator'+28h (07FF7D0A32C98h)  // 跳到0x32C98, 继续循环
```

从以上反汇编代码可得出, 编译器对``Complex *pc = new Complex[N]``是如下解释的

```c++
// 展开Complex *pc = new Complex[N]
Complex *pc;
try{
	void *mem = operator new(N * sizeof(Complex));
	pc = static_cast<Complex*>(mem);
	for(int i = 0; i < N; i++) new(pc + i)Complex();	// 按顺序进行默认构造
}catch(std::bad_alloc) { }
```



## VS 2019 对于 delete 和 delete array 的编译

VS 2019 中 operator delete 和 operator delete[] 的声明和定义

```c++
// 声明, 位于 vcruntime_new.h 中
void __CRTDECL operator delete(void* _Block) noexcept;
void __CRTDECL operator delete[](void* _Block) noexcept;

// 定义, 位于 delete_scalar.h 中
void __CRTDECL operator delete(void* const block) noexcept
{
    #ifdef _DEBUG
    	_free_dbg(block, _UNKNOWN_BLOCK);	// DEBUG模式
    #else
    	free(block);	// RELEASE模式
    #endif
}
// 定义, 位于 delete_array.h 中
void __CRTDECL operator delete[](void* const block) noexcept
{
    operator delete(block);
}
```

使用如下程序测试编译器对 delete array 的处理过程.

```c++
class A
{
public:
    A() { cout << "default ctor: " << this << endl; }
    A(int x) : idx(x) { cout << "para ctor: " << this << " idx=" << idx << endl; }
    ~A() { cout << "dtor called: " << this << endl; }
    int idx;
};
int main()
{
    A* p = new A[88];
    delete[]p;
}
// 反汇编调试
    delete[]p;
00007FF6B77610F2  test        rdi,rdi  // rdi中存放的是p的地址, 如果p == nullptr, 则不执行delete操作
00007FF6B77610F5  je          main+91h (07FF6B7761121h)  
00007FF6B77610F7  mov         r9,rsi  
00007FF6B77610FA  mov         r8,qword ptr [rdi-8]  // Memory[rdi - 8]处存储了元素个数, 此处为88, qword是四字
00007FF6B77610FE  mov         edx,4  // 4是析构时循环的步长, 当改变类A的大小时, 该值随之改变
00007FF6B7761103  mov         rcx,rdi  // 设置函数入口参数
00007FF6B7761106  call        `eh vector destructor iterator' (07FF6B77615B4h)  
00007FF6B776110B  mov         rdx,qword ptr [rdi-8]  
00007FF6B776110F  lea         rdx,[rdx*4+8]  
00007FF6B7761117  lea         rcx,[rdi-8]  
00007FF6B776111B  call        operator delete[] (07FF6B7761570h) // 析构完所有对象后,调用operator delete[](p)
...

// 循环调用析构函数
00007FF6B77615E7  mov         rax,rbx  
00007FF6B77615EA  dec         rbx  
00007FF6B77615ED  mov         qword ptr [count],rbx  
00007FF6B77615F2  test        rax,rax  
00007FF6B77615F5  je          `eh vector destructor iterator'+5Ch (07FF6B7761610h)  
00007FF6B77615F7  sub         rdi,rsi  
00007FF6B77615FA  mov         qword ptr [ptr],rdi  
00007FF6B77615FF  mov         rcx,rdi  
00007FF6B7761602  mov         rax,r14  
00007FF6B7761605  mov         rdx,qword ptr [__guard_dispatch_icall_fptr (07FF6B7763250h)]  
00007FF6B776160C  call        rdx	// 这里面调用了析构函数
00007FF6B776160E  jmp         `eh vector destructor iterator'+33h (07FF6B77615E7h)  
```

根据上述过程, 可以得出

```cpp
// 编译器对delete pc的处理 (设pc为Complex类的指针)
{
    pc->~Complex();
    operator delete(pc);
}
// VS 2019, 64位环境下, 编译器对delete pc[]的处理
{
    qword N = *(pc - 8);
    qword x = sizeof(Complex);
    for(qword i = N - 1; i >= 0; i--) (pc + x * i)->~Complex();		// 循环析构
    operator delete[](pc);	// 和 operator delete(pc) 相同
}
```

所以, delete [] p和 delete p的区别在于: 前者对每个对象调用析构函数, 后者仅对p所指对象调用析构, 然后他们都调用free(p)

所以, 对于使用默认析构函数的类来说, delete [] 和 delete 的效果是相同的 (即需要使用 delete [] 的地方也可以使用 delete)

**并没有上述那么简单!!! 当你重载全局的delete[]时, 会发现, 调用delete[]和delete时, 编译器传入的第二个参数的大小不同!**

**这个问题只能等到对malloc和free的完全理解后, 才能解答**

```c++
void operator delete(void *block, size_t size)  // 如果不加第二个参数, 则编译器不识别
{
    cout << "Global operator delete, size = " << size << endl;
    free(block);
}
void operator delete[](void *block, size_t size)  // 如果不加第二个参数, 则编译器不识别
{
    cout << "Global operator delete[], size = " << size << endl;
    free(block);
}
A *p4 = ::new A[10];
::delete p4;	// 输出size=4
::delete[] p4;	// 输出size=48
```

## 10 11 探究 placement new

**placement new == 重载版本的 new**

**placement arguments  == 调用重载版本的 new 时传入的参数**

**事实: 在任何时候调用new, 无论是全局new, 或者局部new[], 编译器都会自动把size_t当成第一个参数传入**

假设有语句``Complex *p = new("hello", 6.7)Complex``, 则会调用有三个参数版本的new, 即会调用``void *operator new(size_t, string, double)``, 该``operator new``可能是全局的, 也可能是成员的

标准库 (或编译器) 提供了两个重载版本的`new` 和 两个重载版本的`new[]`, 

```c++
// 标准库提供的一种 placement new 和 placement new[]
// Default placement versions of operator new.
inline void* operator new(std::size_t, void* __p) _GLIBCXX_USE_NOEXCEPT  // 通过new(q)A(x)调用, 可以自定义构造
{ return __p; }
inline void* operator new[](std::size_t, void* __p) _GLIBCXX_USE_NOEXCEPT  // 通过new(q)A[3]调用, 只能默认构造
{ return __p; }
```

上述代码非常令人疑惑的是, ``operator new(size_t, void *)``返回的是一个指针, 然后后面就继续写A(x), 这样为什么是合法的? 编译器如何处理这样的语句?

**我认为, 编译器会对该语句先进行翻译, 就类似new关键字一样, 不会让其直接调用``operator new(size_t, void*)``那么简单, 有道理!!!**

**对任何地方出现的new, 编译器都会进行如下解释, 包括重载的new (即placement new)**

```c++
// 展开new(p)Complex(1, 2);
try{
    void *p = operator new (sizeof(Complex), p);	// 调用全局或成员operator new(size_t, void *)
    p = static_cast<Complex*>(mem);	// cast
    // 编译器通过一种方法, 在p处调用Complex的构造函数
}catch(std::bad_alloc){ }
```

**可以确定的是, 编译器在解析普通new时, 不是通过placement new来构造的对象**

```c++
		new(p)A(1);		// 如下对该语句进行反汇编
.L14:
        mov     rbx, QWORD PTR [rbp-56]
        mov     rsi, rbx
        mov     edi, 4
        call    operator new(unsigned long, void*)			// 调用placement new
        mov     r12, rax
        mov     r14d, 1
        mov     eax, DWORD PTR [rbp-60]
        mov     esi, eax
        mov     rdi, r12
        call    A::A(int) [complete object constructor]		// 构造
```

**所以, 可以这样认为: 默认的placement new提供给用户, 让其能在已经分配的内存中构造对象. 因为你没办法在某个地址显式调用构造函数, 但是可以借助operator new(size_t, void *)来实现这一愿望, 默认的placement new的功能仅此而已.**

但是, 如果从广义上看, 允许有多个重载版本的 new 为程序设计提供了更大灵活性. 

**而且, 对于new 和 重载版本的new, 其后必须跟一个对象, 因为编译器会执行sizeof(Complex)操作, 如果没有对象, 则参数不全. 这也就注定了, 在任何时候使用new, 都会有对象构造的操作**

## 12 13 per-class allocator

针对一个class写出其自己的内存管理函数

无论`malloc`执行的速度快或慢, 作为类的设计者, 我们总想让在new对象时, 对`malloc`的调用次数尽可能少 (时间角度).

除了减少`malloc`的调用次数, 作为类的设计者, 我们总想减小`cookie`的用量, 因为一次`malloc`就会产生一些`cookie` (空间角度). 

```c++
// Screen.cpp
#include <cstddef>
#include <iostream>

class Screen
{
public:
	Screen(int x) : val(x) {};
    int get() { return val; }
    void *operator new(size_t size)
    {
        Screen *p = nullptr;
        // 一次性分配screenChunk个对象(所占用的空间), 且把这些对象通过next指针串起来
        // 你可能会问: 你都没有构造Screen对象, 为什么能访问其next成员变量?
        // 答: 我们访问的是某个地址, 和该地址是否被构造对象无关, 即freeStore->next == *(Screen *)(freeStore + 4)
        // 新的问题: 你在未构造对象时就设置其所在内存的next成员变量, 当你构造Screen对象的时候, next会不会被初始化从而改变了其在内存中的值?
        // 新的答案: 在构造函数中, 没有指明如何初始化next, 则next会被执行默认初始化
        if(!freeStore)
        {
            freeStore = reinterpret_cast<Screen *>(new char[size * screenChunk]);
            for(p = freeStore; p != &freeStore[screenChunk - 1]; ++p)
                p->next = p + 1;
            p->next = nullptr;
        }
        p = freeStore;
        freeStore = freeStore->next;
        return p;
    }
    
    void operator delete(void *block, size_t size)
    {
        Screen *deadObj = (Screen *)block;
        deadObj->~Screen();
        deadObj->next = freeStore;  // 回收到freeStore的头部
        freeStore = deadObj;
    }
private:
    static Screen *freeStore;		// freeStore指向当前池中可用的第一个对象
    static const int screenChunk;	// 内存池中预分配对象的个数
private:
    int val;
    Screen *next;
};

// 一般不在头文件中初始化类的静态成员变量, 否则会重定义
// 一般在一个且仅一个CPP文件文件中初始化类的静态成员变量, 初始化时不允许写static关键字
const int Screen::screenChunk = 24;
Screen *Screen::freeStore = nullptr;
```

**构造函数默认初始化的验证**

**默认初始化, 对于非全局和非静态native变量来说, 就是默认内存中的原生值(垃圾值); 对于类变量, 执行默认初始化**

```c++
// default_initialized.cpp
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
    cout << p << endl;	// Hello World

    Foo *q = (Foo *)p;
    q = new(q)Foo('2','4');
    cout << q->c1 << q->c2 << q->c3 << q->c4 << endl;   // ouput: H2l4, 证明了默认初始化
}
```

上述的版本每一个Screen中都存储一个next指针, 代价高昂

我们可以使用union来实现embedded pointer. 在union中, 对象不被使用时, 表示的是next指针; 对象正在被使用时, 表示的是成员变量的数据

```c++
// <AirPlane.cpp>
#include <iostream>
#include <new>

class AirPlane
{
private:
    struct AirPlaneRep          // Rep = Representation
    {
        unsigned long miles;
        char type;
    };
    union                       // anonymous union
    {
        AirPlaneRep rep;
        AirPlane *next;
    };

public:
    void set(unsigned long m, char c) { rep.miles = m; rep.type = c; }
    unsigned long getMiles() { return rep.miles; }
    char getType() { return rep.type; }

private:
    static const int BLOCK_SIZE;
    static AirPlane *headOfFreeList;

public:
    static void *operator new(size_t size)
    {
        if(size != sizeof(AirPlane)) return ::operator new(size);   // 继承

        AirPlane *p = nullptr;
        if(headOfFreeList == nullptr)
        {
            headOfFreeList = static_cast<AirPlane*>(::operator new(size * BLOCK_SIZE));
            for(int i = 0; i < BLOCK_SIZE - 1; i++)
                headOfFreeList[i].next = headOfFreeList + i + 1;
            headOfFreeList[BLOCK_SIZE - 1].next = nullptr; 
        }
        p = headOfFreeList;
        headOfFreeList = headOfFreeList->next;
        return p;
    }
    static void operator delete(void *block, size_t size)
    {
        if(block == nullptr) return;
        if(size != sizeof(AirPlane))	// 继承
        {
            ::operator delete(block);
            return;
        }
        AirPlane *deadObj = static_cast<AirPlane*>(block);
        deadObj->~AirPlane();
        deadObj->next = headOfFreeList;
        headOfFreeList = deadObj;
    }
};

// initialize static member data
const int AirPlane::BLOCK_SIZE = 512;
AirPlane *AirPlane::headOfFreeList = nullptr;
```

上述两个类自己的构造器是这样工作的: 当用户需要new对象时, 类一次性分配出N个对象所占内存, 依次分发给用户; 当用户用完了N个内存时, freeStore会指向空, 这时用户需要new对象, 类又会一次性分配出N个对象所占内存; 然后用户释放了所有的对象, 这时候freeStore的链表中有2N个对象空间. 这2N个对象虽然没有还给系统, 但是不是内存泄漏, 因为还在freeStore手上.

freeStore是静态成员, 每次分配回收都是操作的一个变量, 会不会有线程安全问题?

## 14 static allocator

上述两个类的分配器非常相似, 在很多情况下, 我们对内存管理的写法都是这样的. 那么从软件工程的角度, 能不能把该功能抽取出来呢? 从而避免写很多重复代码. 为此, 我们设计一个类`allocator`, 提供两个方法`allocate(size_t), deallocate(void *, size_t)` .

```c++
class allocator
{
private:
    struct linker { linker *next; };  // embedded pointer
    linker *freeStore = nullptr;      // head of freelist
    int chunk = 5;                    // default block size, 标准库使用的chunk是20
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
```

其他程序想使用allocator的内存管理时, 直接在其类中引入一个allocator静态对象即可, 然后将其new和delete操作全部交给allocator处理. 就像下面这样.

```c++
#include "static_allocator.h"

class Foo
{
private:
    int x, y;   // 值得注意的是, 对象占用的空间必须比next指针大;
                // 在64位环境下执行, 指针占用8个字节, int只占用4个字节, 所以如果只有一个变量x, 则出错
public:
    static void *operator new(size_t size) 
    { return alloc.allocate(size); }
    static void operator delete(void *block, size_t size) 
    { return alloc.deallocate(block, size); }
protected:
    static allocator alloc;
};

// initialize static member data
allocator Foo::alloc;
```

## 15 macro for static allocator

上述类Foo对于allocator的使用, 只有在初始化静态成员变量时, 才会和类名有关. 为了更简化使用, 我们设计两个宏, 如下.

```c++
// DECLARE_POOL_ALLOC, used in calss definition
#define DECLARE_POOL_ALLOC \
public:	\
  static void *operator new(size_t size) { return myAlloc.allocate(size); } 	\
  static void operator delete(void *block, size_t size) { return myAlloc.deallocate(block, size); } \
protected: \
  static allocator myAlloc;

// IMPLEMENT_POOL_ALLOC, used in class implementation file
#define IMPLEMENT_POOL_ALLOC(class_name) \
allocator class_name::myAlloc
```

## 16 new handler

当new没有内存可返回时, 其会抛出std::bad_alloc异常

如果你使用`new (nothrow) Foo`, 则一定不会抛出异常, 如果没有内存返回空指针.

标准库对于new的实现中, 其抛出异常之前, 会不止一次调用可以由用户指定的handler, 如下是handler的形式和设定方法. 其中set_new_handler是C++标准规定的方法, 包含在头文件`<new>`中

```c++
typedef void (*new_handler)();
new_handler set_new_handler(new_handler p) throw();
```

使用set_new_handler

```c++
void noMoreMemory()
{
    cerr << "no more memory, abort()" << endl;
    // abort();		// 如果此处不终止程序, 则会一直输出no more memory
}
int main()
{
    set_new_handler(noMoreMemory);
    const long long maxn = 1e10 + 10;
    int *p = new int [maxn]; 
}
```

## 16 =deault and =delete

据说, 这两个关键字不仅可以用于拷贝控制函数, 还可以使用于operator new/new[] 和 operator delete/delete[] 和 他们的重载版本.

经过测试, 发现实际上不行. 因为你无法说一个默认的new应该执行什么操作.

事实上, 当你写出如下代码时, VSCode的C/C++扩展提示: "*'= default' can only appear on default constructors, copy/move constructors, copy/move assignment operators, and destructors C/C++(1774)*"

```
void *operator new(size_t size) = default; // g++: cannot be defaulted
void operator delete[](void *block) = delete; // g++: use of deleted function
```

# 第二讲 std::allocator

## 17 malloc() of VC6

VC6中的malloc的内存布局, 从上到下, 依次是`cookie, debug header, block, debug tail, pad, cookie`

cookie中存储的是这整块内存的大小 (好像还要加上1)

malloc返回的指着指向block的起始, 传给free的指针也是block的起始. free会找到整个内存块并释放, 而不是只释放block

## 18 19 20 std::allocator of VC6 / BC5 / G2.91 / G4.9.2

四者的``std::allocator``只是以操作符``new``和``delete``完成``allocate()``和``deallocate()``，没有任何特殊设计。

```cpp
// VC98/CRT/SRC/XMEMORY and VC98/INCLUDE/XMEMORY
// std::allocator of VC6
template<class _Ty>
class allocator 
{
public:
	pointer allocate(size_type _N, const void *)
		{return (_Allocate((difference_type)_N, (pointer)0)); }
	
	void deallocate(void _FARQ *_P, size_type)
		{operator delete(_P); }
};
// _Allocate()
template<class _Ty> inline
_Ty _FARQ *_Allocate(_PDFT _N, _Ty _FARQ *)
{
    if (_N < 0) _N = 0;
	return ((_Ty _FARQ *)operator new((_SIZT)_N * sizeof (_Ty)));
}
```

为什么要设计别样的allocator呢? 当你所需要的内存块有大有小时, 别样的allocator是没作用的, 因为不同大小的内存块需要cookie来记录大小. 但是, 容器中的元素大小是固定的, 那么就没必要为每个元素存cookie. 比如一个`list`, 其每个元素的大小都是一样的, 如果使用默认的new分配节点, 则每个节点都需要存cookie, 浪费至极. 

## 21 22 std::alloc of G2.91 == \_\_gnu_cxx::__pool_alloc of G4.9.2

## 23 std::alloc of G2.91

其预先开free_list[16], 其中free_list[0]挂接8字节的块, free_list[1]挂接16字节的块, 依次类推.

一个链默认开多大呢 (CHUNK的默认大小)? 其设置的是20, 不知道依据

## 27 implementation of std::alloc

当静态成员变量为`const`时, 可以执行类内静态成员变量初始化

```cpp
class Foo
{
	static int x = 5;	// error: a member with an in-class initializer must be const
    static const int y = 10;	// ok
}
```

refill函数中递归调用自己真的是太神了

在此之前, 我只有写搜索, 二叉树遍历等显然的递归过程才会写递归调用.

万万没想到递归函数还可以这样应用: 如果有则返回, 如果没有则分配然后递归调用自己!



当使用常量比较时, 比如`if(p == nullptr)`, 最好写成`if(nullptr == p)`, 后者能防止把等于号写成赋值号



看完整个源码, 感觉`std::alloc`可以自己尝试写一下!! 但是线程安全问题怎么解决?
