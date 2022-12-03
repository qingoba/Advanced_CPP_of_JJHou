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

using std::cout;
using std::endl;
int main()
{
    const int N = 10;
    // 使用自己的分配器, 间隔16
    Screen *p[N];
    for(int i = 0; i < N; i++)
    {
        p[i] = new Screen(i);
        cout << p[i] << endl;
    }
    // 使用默认分配器, 间隔32
    for(int i = 0; i < N; i++)
    {
        p[i] = ::new Screen(i);
        cout << p[i] << endl;
    }
    return 0;
}