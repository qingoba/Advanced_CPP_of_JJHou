# 第一讲

## 01

C++ 标准库中并没有太多的OOP，更多的是泛型编程 (GP)，很少出现继承和虚函数，但是在版本迭代的过程中，出现了一些继承。

C++ 标准库 == C++ Standard Library  >  C++ 标准模板库 == C++ Standard Template Library

重要网站：https://gcc.gnu.org 和 https://gcc.gnu.org/onlinedocs/gcc-4.9.4/libstdc++/api/modules.html

其中侯捷对GNU的读法是'咯怒'

“胸中自有丘壑”

## 02	直接看笔记即可

STL六大Components：

- 容器Containers
- 分配器Allocators
- 算法Algorithms
- 迭代器Iterators
- 适配器Adapters；Adapter在英文中是变压器，所以Adapter的作用是帮我们转换，对容器做转换（容器适配器），对迭代器转换（迭代器适配器）
- 仿函数Functors

容器的内存通过分配器实现，算法通过迭代器操作容器

```c++
// 下述代码用到了STL的六大组件
int ia[6] = {27, 210, 15, 47, 109, 83};
vector<int, allocator<int>> vi(ia, ia+6);	// 指定分配器
// not1是函数适配器, bind2nd也是函数适配器, less<int>()是函数对象
// count_if的第三个参数是一个谓词predicate
count_if(vi.begin(), vi.end(), not1(bind2nd(less<int>(), 40))); 
```

侯捷: 我是一百万个数据哦，不是学校里的玩具程式.

时间复杂度 == Tiem Complexity，大O == Big-oh，区间 == Range，解引用 == dereference，插入 == insertion(n.)

尾后迭代器表示pass the end off by one

```c++
// range-based for statement
for(auto i : {2, 3, 5, 7, 8})
    cout << i << endl;
```

## 03

顺序容器 (Sequential Containers)

- array，C++ 11
- vector，扩容交给分配器处理
- deque，**读音是dek**
- list，双向链表
- forward_list，单向链表，C++ 11

关联容器 (Associative Containers)

- set/multiset
- map/multimap
- 标准库并没有规定set和map怎么实现，但各家编译器都不约而同使用红黑树
- multi-表示容器中的value可以重复 (对于set，其value就是key； 对于map，其value是整个pair)

无序容器 (Unordered Containers)，或称作不定序容器，可以被算作关联容器的一个子类

- unordered set/multiset
- unordered map/multimap
- 各家编译器都用哈希表 (Hash Table) 实现，且都用链式散列 (Separate Chaining)

C++ 11新增容器: array，forward_list，无序容器

## 05

4GB 应该读作 4 Giga Bytes，或简称为 4 Giga

标准库的算法有sort，但是某些容器自带sort，这种情况下，容器自带的sort要比标准库算法要快，比如``list.sort()``

标准库的算法有find，但是某些容器自带sort，这种情况下，容器自带的sort要比标准库算法要快，比如``set.find(val)``

GNU C额外提供了单向链表容器slist，非C++标准

添加空的作用域操作符前缀，能够以更明确的语义使用全局函数: `::sort(c.begin(), c.end())`

stack和queue都使用deque实现，以后用stack和queue的时候可以直接用deque，虽然写起来稍微复杂一点，但是对于理解原理非常有帮助

正因为stack和queue借助deque实现，所以人们常常不把stack和queue称为容器，而称为**容器适配器**

## 06

stack和queue只能使用规定的方式操作容器中的元素，两者都不提供任何迭代器，比如对于stack，只能执行push或pop操作，从外部不能访问到stack里面的元素，也不能修改里面元素的值。同理，也不能对stack或queue执行for-range循环，因为不支持迭代器操作。

deque支持迭代器操作，deque支持下标运算符

**对于一百万个元素，在高度平衡的BST上进行查找，只需要查找20次左右，真的太快了.**

除了之前提到的容器，还有两种：优先队列priority_queue和堆heap，其中heap不直接通过类提供，而通过一组函数来提供堆的操作

一些在C++ 11之前GNU C就提供的容器： hash_set，hash_map，hash_multiset，hash_multimap

## 07

本课程所介绍的分配器都是基于GNU C的，同样的名称或程序不一定适用于MSVC或Borland C

std::allocator是一个默认的分配器

能不能直接使用分配器? 能，分配器就是一个模板类，提供了一些方法，比如allocate，deallocate，construct，destroy

# 第二讲

## 08

标准库的接口相同，但是各家编译器实现不同。

本课程使用GNU C 2.91 和 4.9.1，侯捷用的Dev C++是GNU 4.9.2，我的Dev C++也是GNU 4.9.2。

## 09

标准库，尤其是GNU 2.91，没有太多继承和虚函数，不是用OOP设计的，而是用GP (泛型编程) 设计的。

OOP将 ``data`` 和 ``method`` 放在一起，GP将 ``data`` 和 ``method`` 分开放置。

采用GP：容器和算法各自闭门造车，两者通过迭代器连接; 算法通过迭代器确定操作范围，通过萃取器获取操作对象的类型和属性，并通过迭代器存取容器中的元素。

标准库算法``sort``假定传入的迭代器是Random Access的，因为其源码中有一句``first + (last - first) / 2``。

所以对于``list``来说，如果需要排序，则调用自身的排序算法，即``list.sort()``。

所有标准库的算法，其最终涉及元素本身的操作，无非就是比大小。

```c++
// 标准库: 接受一个谓词的max函数
template <class T, class Compare>
inline const T &max(const T &a, const T &b, Compare comp)
{
    return comp(a, b) ? b : a;
}
```

## 10

前14分钟，温习了操作符重载和类/函数/成员模板，直接跳过。

关于模板的书籍《C++ Template, the complete guide》

泛化Generalization，特化Specialization，全特化Full Sepcialization，偏特化Partial Sepcialization。

```c++
// 一个泛化的类模板
template <class type>
struct __type_traits { /* data members */ };
// 两个特化的类模板
template<> struct __type_traits<int> { /* data members */ }
template<> struct __type_traits<double> { /* data members */ }
// 当调用__type_traits<int> obj时, 会使用特化的版本, 如果没有特化版本, 则使用泛化版本
```

```c++
// 标准库中关于Hash的泛化与特化
template <class Key> struct hash { };
// 特化版本, 其中__STL_TEMPLATE_NULL == template<>
__STL_TEMPLATE_NULL struct hash<char> { /* overload operator() */ }
```

以上两者都是全特化，下面的例子是偏特化。当类不再是一个变参模板时，就是被全特化了。

```c++
// 泛化的vector
template <class T, class Alloc = alloc>
class vector {}
// 偏特化的vector
template <class Alloc = alloc>
class vector<bool> {}
```

上述的偏特化，指的是数量上'偏'。下述的两个偏特化是范围的偏特化。

值得注意的是，一旦一个类是特化版本，其类名后就会带有尖括号``<>``。

```c++
// Generalization
template <class Iterator>
struct iterator_traits { /* 定义迭代器的五种类型 */ }
// Partial Sepecialization
template <class T>
struct ierator_traits<T*> { /* 定义迭代器的五种类型 */ }			// 对于指针类型特化
template <class T>
struct ierator_traits<const T*> { /* 定义迭代器的五种类型 */ }	// 对于const指针类型特化
```

## 11 allocators

额外的开销 == overhead。比如，``malloc``分配内存时，并不是只分配用户指定的大小，还分配一些空间用于存储这块内存的信息，这些额外的信息就可以被称为overhead，在翻译中，overheads被翻译成经常性开支。

VC6中的默认分配器：``std::allocator.allocate()``会调用``new``操作符，``new``操作符会调用``malloc()``函数，``std::allocator.deallocate()``会调用``delete``操作符，``delete``操作符会调用``free()``函数。

VC6的``std::allocator``只是以操作符``new``和``delete``完成``allocate()``和``deallocate()``，没有任何特殊设计。

```c++
// 使用VC6的库
// 使用匿名allocator对象
// 必须要熟悉的写法: allocator<int>() 表示创建了一个匿名对象, 这种写法在后面经常用到
int *p = allocator<int>().allocate(512, (int *)0);
allocator<int>().deallocate(p, 512)
    
// VC6 STL对分配器的使用
template <class _Ty,  class _A = allocator<_Ty> >
class vector {};
```

BC5(Borland C++)的``std::allocator``只是以操作符``new``和``delete``完成``allocate()``和``deallocate()``，没有任何特殊设计。

GCC 2.91的``std::allocator``只是以操作符``new``和``delete``完成``allocate()``和``deallocate()``，没有任何特殊设计。

GCC 2.91的``std::allocator``被定义在文件``<defalloc.h>``中，该文件中有这样的注释：

```c++
// default allocator. It is provided only for backward compatibility.
// DO NOT USE THIS FILE unless you have an old container implementation
// that requires an allocator with the HP-style interface. SGI STL
// uses a different allocator interface. SGI-style allocators are not
// parametrized with respect to the object type; they traffic in void *
// pointers. This file is not included by any other SGI STL header.

// 其中, SGI == Silicon Graphics [Computer System] Inc. 硅图[计算机系统] 公司
// 因为GNU C的STL最初是由SGI这家公司发展的
```

所以，GCC 2.91的STL中使用的分配器不是``std::allocator``.

```c++
// G2.91 STL对分配器的使用, 可见其使用的默认构造器是alloc, 定义在头文件<stl_alloc.h>中
template <class T, class Alloc = alloc>
class vector { };
```

``malloc``分配内存时附加的额外数据结构我们称之为``cookie``。

G2.91所分配的内存都是8的倍数，alloc对象中有16个链表，从0编号到15，#0以8字节存储，#4以40字节存储。

但是，GCC 4.9.2的STL容器使用的分配器又变成了``std::allocator``，只是以操作符new和delete完成``allocate()``和``deallocate()``，没有任何特殊设计。原来G2.91的``alloc``呢? 还能用吗?

答案: G4.9.2 附带了一些extension allocator，其中``__pool_alloc``就是G2.91的``alloc``。

## 13 14 list

任何一个容器，都应该``typedef``自己的迭代器类型，比如``list``中，``typedef __list_iterator<T, T&, T*> iterator``。然后外界可以通过``list<int>::iterator``拿到迭代器的类型，从而声明或定义一个该容器的迭代器。

标准库规定，**每一个容器对应的迭代器类都应该至少做5个``typedef``**，如下是``__list_iterator``中的``typedef``定义。

**这五个``typedef``用于回答算法对迭代器的提问**。比如，算法可能问，你迭代器代表的元素类型是什么 (容器中元素的类型)，则算法可以通过``iterator::value_type``得到该问题的答案，从而通过该类型声明或定义变量。 比如，算法可能问，你迭代器能支持随机访问吗，则算法可以通过``iterator::iterator_category``得到该问题的答案。

```c++
template <class T, class Ref, class Ptr>
struct __list_iterator
{
    // 五个 typedef
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef ptrdiff_t difference_type;
    
    link_type node;   // list迭代器类中唯一的一个数据成员node, 类型是ListNode*
    reference operator*() const { return (*node).data; }
    // 为什么箭头运算符应该被重载成这般?
    // 参见CPP Primer 5th 和 https://blog.csdn.net/custa/article/details/5674434
    pointer operator->() const { return &(operator*()); }  
}
```

在``list``类中，只有一个唯一的成员变量``ListNode* node``， 对于``begin()``函数，其返回``node->next``，对于``end()``函数，其返回``node``，这也说明了``list``是双向循环链表。同时说明``node``当前指向的节点是一个额外的空节点 (dummy head)。这个额外的空节点什么时候分配? 在``list``的构造函数中被分配。其如何分配内存? 调用分配器。

在``list``类中，``node``永远指向空节点，不会移动 (即使调用成员函数，该指针也不动)。

在``__list_iterator``类中，``node``指针随着操作来回移动。

```c++
// list
template <class T, class Alloc = alloc>
class list 
{
public:
  typedef __list_iterator<T, T&, T*>             iterator;
protected:
  link_type node;   // 整个List类唯一的一个成员, 类型是ListNode*
                    // 应该疑惑, 为什么没有迭代器? 从下面可以发现, begin()和end()函数临时构造迭代器
                    // 因为list_iterator的其中一个构造函数接受一个ListNode*对象
                    // 当调用end()时, list对象通过node成员临时构造一个迭代器, 返回给用户
                    // 如果用户需要操作迭代器, 则用户会自己保存, 这样list中就无需存储迭代器对象
public:
  iterator begin() { return (link_type)((*node).next); }
  iterator end() { return node; }	// 双向循环链表
}
// ListNode
template <class T>
struct __list_node 
{
  typedef void* void_pointer;
  void_pointer next;
  void_pointer prev;
  T data;
};
```

``*``操作符和``->``操作符的解析是类似的：当``ptr``是一个指针时，``*ptr``就是普通的语法解析，得到``ptr``指向的对象；当``obj``是一个对象时，``*obj``会调用``obj``的重载的`*`操作符。

为什么箭头操作符要这样重载呢? 假设你有一个类`Foo`，类中有方法`method`，你现在有一个``list<Foo> L1``，还有一个迭代器``it = L1.begin()``，当你对``it``执行`->`操作时，你想干什么呢? 很显然是访问`Foo`中的变量或方法。因为迭代器是类似指针的，如果有``Foo *ptr``，则对`ptr`执行`->`操作时，就是访问`Foo`的变量或方法。既然迭代器的行为类似指针，其``it->action()``的语义就应该是调用``it``所指对象的``action`方法。

一个迭代器类，基本上由两大块组成，第一大块是各种``typedef``，第二大块是各种操作符重载。

从G2.91和G4.9.2的变化来看，可以总结一些常识：标准库的实现是不同的，编译器的实现也是不同的，只有 ISO C++ 和 ANSI C 定义的东西才是相同的。

## 15 iterator traits

`iterators`必须有能力回答`algorithms`的提问，比如：你能支持`+=`操作吗。

C++ 标准要求定义迭代器的五种相关类型 (Associated Type)，分别是

- value_type
- iterator_category
- difference_type
- reference (GNU C 标准库仅定义，未使用)
- pointer (GNU C 标准库仅定义，未使用)

这五种类型迭代器本身必须定义出来，以便回答算法的提问。

算法通过以下方式来询问迭代器：

```c++
template <typename I>
inline void algorithm(I first, I last)
{
    // 向I类型的迭代器询问
	I:iterator_category
    I:value_type
}
```

但是，如果`iterator`并不是一个`class`呢? 比如`native pointer`。

因为内置指针类型也可以被算法所用，但内置指针并没有五种相关类型，怎么办? **使用Traits。**

Traits就是一个中间层萃取器，输入可以是内置指针，也可以是迭代器类，其都能返回五种相关类型。

**解决计算机问题的尚方宝剑: 加一个中间层。**

加了中间层萃取器之后，算法的询问就变成：

```c++
template <typename I>
inline void algorithm(I first, I last)
{
    // I可能是内置指针类型, 也可能是迭代器类型
    // 将I抛给迭代器萃取器, 并向萃取器提问
    iterator_traits<I>::value_type
    iterator_traits<I>::iterator_category
}
```

在C++ 标准中，迭代器萃取器被包含在``<iterator>``头文件中。在GCC 2.91中，迭代器萃取器定义在头文件`<stl_iterator.h>`中，**迭代器萃取器是一个结构体，该结构体中没有任何成员，只包含类型别名定义 (使用typedef)。所以，当使用``iterator_traits<I>::value_type``时，得到的是一个类型名，不是一个成员变量或成员函数。**

那么萃取器应该怎么写呢? 如何区分内置指针和迭代器类呢? **使用泛化与特化**

```c++
// GCC 2.91 泛化的迭代器萃取器
template <class Iterator>
struct iterator_traits
{
    // 该结构体中不含有任何成员变量, 只含有类型别名
    // 因此iterator_traits<I>::value_type是一个类型名, 比如int或string
	typedef typename Iterator::iterator_category iterator_category;
  	typedef typename Iterator::value_type        value_type;
  	typedef typename Iterator::difference_type   difference_type;
  	typedef typename Iterator::pointer           pointer;
  	typedef typename Iterator::reference         reference;
}
// 特化的萃取器, 处理内置指针
template <class T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag iterator_category;
  	typedef T                          value_type;
  	typedef ptrdiff_t                  difference_type;
  	typedef T*                         pointer;
  	typedef T&                         reference;
}
// 特化的萃取器, 处理内置const指针
template <class T>
struct iterator_traits<const T*>
{
	// 注意value_type是T, 不是const T
    // 因为算法拿value_type主要用来声明变量, 而我们不想声明不能被赋值的变量
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef ptrdiff_t                  difference_type;
    typedef const T*                   pointer;
    typedef const T&                   reference;
}
```

在这种情况下，迭代器类匹配泛化萃取器，内置指针匹配特化萃取器。

不仅迭代器有萃取器，实际上标准库有各种萃取器

- type traits
- iterator traits
- allocator traits
- pointer traits
- array traits
- char traits

## 16 vector

C++ 标准并没有明确规定`vector`扩容的倍数，但各家编译器都使用两倍扩充。

`vector`类中只有三个迭代器对象，分别是``start, finish, end_of_storage``，除此之外没有任何其他成员变量。

通过源代码可以发现，`vector`类型的迭代器就是`T*`，其没有定义额外的迭代器类，直接使用内置指针当作迭代器。

在G2.91版本中，`vector`的默认大小是0。

```c++
template <class T, class Alloc = alloc>
class vector 
{
public:
  typedef T value_type;
  typedef value_type* iterator;
protected:
  iterator start;
  iterator finish;
  iterator end_of_storage;
public:
  iterator begin() { return start; }
  iterator end() { return finish; }
  size_type size() const { return size_type(end() - begin()); }
  size_type capacity() const { return size_type(end_of_storage - begin()); }
}
// 扩容
const size_type old_size = size();
const size_type len = old_size != 0 ? 2 * old_size : 1;
```

C++ 11对`vector`新增了``emplace_back()``方法,  与`push_back()`相比, 其有什么区别?

```c++
// GNU C <stl_vector.h>
void push_back(const value_type& __x)
{
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);	// copy ctor
}
void push_back(value_type&& __x) { emplace_back(std::move(__x)); }
void emplace_back(_Args&&... __args)	// 传入的是参数包
{
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::forward<_Args>(__args)...);
}
```

需要从传入的参数来看两者的区别:  

## 17 array and forward_list

GNU 2.91中没有array和forward_list，因为两者在C ++ 11才成为标准。

array没有ctor，也没有dtor，数组类型和数组大小都是通过模板参数传入的，其迭代器也是native pointer。

```c++
// TR1标准的array实现
template <typename T, std::size_t N>
struct array
{
  	typedef T value_type;
    typedef T* iterator;
    value_type _M_instance[N ? N : 1];
    iterator begin() { return iterator(&_M_instance[0]); }
    iterator end() { return iterator(&_M_instance[N]); } // 注意这里是N, 不是N-1, 因为end指向尾后
};
```

```c++
// GCC 4.9.2 forword_list实现
// forword_list类中仅有一个成员变量_M_head, 是一个指针
// forword_list初始化后有一个头节点, 该头节点不存储数据
// 其迭代器应该是自定义类型(和list类似)
iterator begin() noexcept { return iterator(_M_head._M_next); }	// 头节点的下一个节点才存放数据
iterator end() noexcept { return iterator(0); }	// 尾节点的next指针应该是nullptr, 且迭代器不支持--操作, 因此这样定义没有问题
```

## 18 19 deque/stack/queue

deque在内存中是分段连续组织的，当迭代器走到一个段的末尾时，其必须有能力跳到下一个段的开头；这通过迭代器类中的一个成员``node``来实现。

deque的迭代器是一个类，该迭代器类有四个成员变量，分别是``T *cur, *first, *last; map_pointer node;``。其中``cur, first, last``表示当前段的信息。

**关于deque的细节还可以再做更深入的研究。**

```c++
// deque类
// 一个段可以被称为一个buffer, 一般默认buffer_size = 512
template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque
{
protected:
    iterator start;
    iterator finish;
    map_pointer map;	// map_pointer == T**, 可以认为map[i][j]表示第i段的第j个元素
    size_type map_size;	// map_size表明现在有多少个段
public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return finish - start; }
    static size_type initial_map_size() { return 8; }
}
// 迭代器类
template <class T>
class deque_iterator
{
    T *cur, *first, *last;
    map_pointer node;	// 也是一个T**, 如果该迭代器在第二个段中(编号从0开始), 则node = map + 1;
    // 迭代器的运算符
    difference_type operator-(const self &x) const
    {
        return buffer_size() * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
    }  
	reference operator*() { return *cur; }
}

// 值得注意的是, iterator类并不确保迭代器总是指向正确的位置, 比如你的vector大小是10, 然后你调用it = it + 100,
// 是可以编译运行的, 也不会报错, 但是如果加法之后你*it, 或者通过it修改容器, 则会报错.
```

queue和stack默认是封装deque实现的。

queue的容器可以是list，deque，因为需要支持pop_front()，所以不能使用vector。

stack的容器可以是list，vector，deque。

stack和queue都不允许遍历，不提供迭代器。

```cpp
// queue, 所有的操作都是通过调用deque实现
template <class T, class Sequence = deque<T> >
class queue
{
protected:
    Sequence c;
};
// stack, 所有的操作都是通过调用deque实现
template <class T, class Sequence = deque<T> >
class stack
{
protected:
    Sequence c;
}
```

## 20 RB Tree

容器``rb_tree``提供遍历操作，提供迭代器，当你用迭代器遍历时，会得到递增序列。其``begin()``指向树中最左边元素，``end()``指向树中最右边元素。

``rb_tree``提供的迭代器允许对元素做修改，这主要是为了``map``中通过迭代器改元素的``value`` (而不是改`key`)。

``rb_tree``提供两种insertion操作，分别是``insert_unique()``和``insert_equal()``。

```c++
// value表示树中节点存储的元素类型, key表示通过KeyOfValue(value)得到的关键字的类型
// KeyOfValue是一个可调用对象
// Compare表示关键字如何做比较, 是一个函数对象
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree 
{
// 仅有的三个成员变量
protected:
	size_type node_count; 	// keeps track of size of tree
  	link_type header;  		// 树节点的指针, 指向一个虚空的根节点
  	Compare key_compare;	// 关键字比较准则, 应该是一个函数对象. (讲道理来说该内存为0, 但是编译器为变成1, 再加上对齐, 则sizeof(rb_tree) == 12
};

// 树节点结构: value, left, right, parent, color
struct __rb_tree_node_base
{
  color_type color; 
  base_ptr parent;
  base_ptr left;
  base_ptr right;
};
template <class Value>
struct __rb_tree_node : public __rb_tree_node_base
{
  typedef __rb_tree_node<Value>* link_type;
  Value value_field;
};
```

在`rb_tree`类中，每个节点只存储value，不存储Key。 当插入一个数时，需要比较节点之间的Key，则通过`KeyOfValue(val)`来临时获取Key，然后作比较，然后插入。

你会发现，`rb_tree`类里面没有修改元素的接口 (只有删除和插入)。 可以回想一下，在平常使用`set`和`map`的过程中，基本上不会有修改`key`的操作，而且`set`的`begin()`函数返回的就是`const`迭代器，不允许你修改其内部的元素。 但是`map`的`begin()`返回的不是`const`迭代器，因为外部要修改`key`对应的`value`，那是不是说，可以通过迭代器修改`map`的`key`了? 尝试一下，发现是不行的，因为当你定义`map<int，string> mp`时，`map`内部使用的数据结构是``pair<const int, string>``，所以`pair`的`first`只能初始化，不能修改。

## 21  set, multiset

`set`的`insert()`调用的是`rb_tree`的`insert_unique()`

`multiset`的`insert()`调用的是`rb_tree`的`insert_equal()`

```c++
// set的所有操作都是交给rb_tree来操作, 其迭代器也是用的rb_tree的迭代器
// 所以, set也可以看作一个容器适配器
// rb_tree的迭代器类已经定义了++, --, *, ->, 等操作符, 所以set都不需要定义这些操作符, 直接用rb_tree_iterator类即可
template <class Key, class Compare = less<Key>, class Alloc = alloc>
class set
{
public:
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
private:
  typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
  rep_type t;  // red-black tree representing set
  // set从rb_tree中拿到的迭代器是const迭代器, 所以根本没法通过迭代器修改元素
  typedef typename rep_type::const_iterator iterator;
};

// identity()可调用对象在G2.91定义在<stl_function.h>中, 返回其自己
template <class T>
struct identity : public unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};
```

## 22 map, multimap

**可见，set通过使用const iterator来防止修改key，map通过使用pair<const Key, T>来防止修改Key**

multimap不能使用下标运算符来添加/修改元素。

lower_bound()：试图在给定的范围内查找value，如果找到了，则返回第一个value出现的地方，没找到，则返回第一个大于value的地方 (最适合插入value的地方)。

map支持下标运算符，在G4.9.2中，下标操作符要先对key进行查找，找不到再执行insert，在这种情况下，insert的性能要比下标运算符要高，因为其实没必要对key查找，可以直接调用`insert_unique()`。

```c++
template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map 
{
public:
  typedef Key key_type;
  typedef T data_type;
  typedef T mapped_type;
  typedef pair<const Key, T> value_type;	// 注意这里的const; map自动将key设为const, 以确保只能增删, 不能修改
  typedef Compare key_compare;
  
private:
  typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
  rep_type t;  // red-black tree representing map
};

// 下面的仿函数实现从value中找到key, 该仿函数被map所使用
// select1st定义在G2.91的<stl_function.h>中
template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> 
{
  const typename Pair::first_type& operator()(const Pair& x) const
  {
    return x.first;
  }
};
```

## 23 24 HashTable

元素重新打散(rehashing)的规则: 当元素的个数大于bucket/slot的个数时，打散，变成两倍附近的质数.

在G2.9中，哈希表的默认大小是53，其后是97，这些数定义在``__stl_primer_list``中

G2.9中，用vector存储每个链，链表的节点为node，node的元素包括value和next

```c++
// 类hashtable
// 经过哈希函数得到的整数值称为HashCode
// ExtractKey用于提取Key, EqualKey用于比较Key
template <class Value, class Key, class HashFcn,
          class ExtractKey, class EqualKey,
          class Alloc>
class hashtable
{
private:
  hasher hash;
  key_equal equals;
  ExtractKey get_key;

  typedef __hashtable_node<Value> node;
  typedef simple_alloc<node, Alloc> node_allocator;

  vector<node*,Alloc> buckets;
  size_type num_elements;
};
```

使用hashtable对象必须先指明6个模板参数

#### 标准库提供的哈希函数

```c++
// 标准库提供的哈希函数

template <class Key> struct hash { };	// hash对象, 不是可调用的, 是泛化类型

// 整数的哈希函数, 偏特化, 其中__STL_TEMPLATE_NULL == template<>
__STL_TEMPLATE_NULL struct hash<int> 
{
  size_t operator()(int x) const { return x; }
};

// 字符串的哈希函数, 偏特化
__STL_TEMPLATE_NULL struct hash<const char*>
{
  size_t operator()(const char* s) const { return __stl_hash_string(s); }
};
inline size_t __stl_hash_string(const char* s)
{
  unsigned long h = 0; 
  for ( ; *s; ++s)
    h = 5*h + *s;
  
  return size_t(h);
}
```

G2.9没有为std::string提供现成的哈希函数

#### 向hashtable中插入一个元素

hashtable也提供了两种插入函数, 一个是insert_unique(), 另一个是insert_equal()

**modulus == 模数**

```c++
// 向hashtable对象中插入一个元素, 需要判断重复
// G2.9的做法是, 先扫一遍所在bucket的链表, 如果没有重复, 则插入到头部
template <class V, class K, class HF, class Ex, class Eq, class A>
pair<typename hashtable<V, K, HF, Ex, Eq, A>::iterator, bool> 
hashtable<V, K, HF, Ex, Eq, A>::insert_unique_noresize(const value_type& obj)
{
  const size_type n = bkt_num(obj);
  node* first = buckets[n];

  for (node* cur = first; cur; cur = cur->next) 
    if (equals(get_key(cur->val), get_key(obj)))
      return pair<iterator, bool>(iterator(cur, this), false);

  node* tmp = new_node(obj);
  tmp->next = first;
  buckets[n] = tmp;
  ++num_elements;
  return pair<iterator, bool>(iterator(tmp, this), true);
}

// 当插入一个元素时, 首先调用bkt_num(value)得到对应的桶的下标
// bkt_num(val)会调用模板中的ExtractKey对象得到val的关键字key,
// 然后对key进行hash, 最后模n
// 即 key = ExtractKey(val); idx = hash(key) % size();
```

#### hashtable中存储的元素

**所以, hashtable里究竟存的是什么东西? 是pair<key, value>吗? 还是单纯的value?**

发现, hashtable中存储的是一个一个的node, 每一个bucket中存放的都是node链起来的链表

```c++
// hashtable中存储的元素 (链表中的元素)

template <class Value>
struct __hashtable_node
{
  __hashtable_node* next;
  Value val;
};  

template <class Value, class Key, class HashFcn,
          class ExtractKey, class EqualKey,
          class Alloc>
class hashtable 
{
public:
  typedef Key key_type;
  typedef Value value_type;

private:
  typedef __hashtable_node<Value> node;
  vector<node*,Alloc> buckets;	// 一个bucket存放一个node的链表, node只存储value
  size_type num_elements;
};
```

**node中只存储value, 但插入时需要根据Key计算出桶所在的下标, 所以需要通过value临时计算出Key, 然后执行idx = hash(Key), 得到桶所在的下标. 当得到下标后, 判断链表中是否有重复的value, 没有就插入到头部(注意在链表中比较的是value!!!), 在同一个链表中的元素, Key都是相同的, 只是value不同**

所以, 在上层使用hashtable时, 比如unordered_map使用hashtable, 则其模板参数中的value就应该是pair类型

```c++
// 我假想的: unordered_map中的hashtable成员
hashtable< 	pair<const string, int>, 
		   	string,
		   	hash<string>, 
			select1st<pair<const string, int>>,
			equal_to<string>,
			alloc> ht;    
```

## 25 26 hash_set, hash_map, hash_multiset, hash_multimap, unordered

类hash_map的定义, 其哈希函数使用标准库定义的默认哈希函数

在hashtable中, 其默认的构造函数选用53作为bucket的初始数量; 而在hash_map中, 该数量变成了100, 不过其后续resize还会变成hashtable中的质数

hash_map中所有的操作均通过其hashtable成员rep来实现

hash_map调用insert_unique(), hash_multimap调用insert_equal()

```c++
template <class Key, class T, class HashFcn = hash<Key>,
          class EqualKey = equal_to<Key>,
          class Alloc = alloc>
class hash_map
{
private:
  typedef hashtable<pair<const Key, T>, Key, HashFcn,
                    select1st<pair<const Key, T> >, EqualKey, Alloc> ht;
  ht rep;
public:
  hash_map() : rep(100, hasher(), key_equal()) {}
}
```

类hash_set的定义

其提取key的方法是使用``identity<value>``, 所以其key就是value, 传给哈希函数的就是value

和set一样, 其通过把迭代器定义成const的, 来限制修改key

**再次重申: hashtable中传给哈希函数的是key, 不是value. 流程是: 通过value得到key, 再对key调用hash(key), 再执行hash(key) % n, 从而找到应该存放的地方, 而存放时, 存放的是最初的value, 所以有可能两个value映射到同一个key, 但是这两个value都应该被存储**

```cpp
template <class Value, class HashFcn = hash<Value>,
          class EqualKey = equal_to<Value>,
          class Alloc = alloc>
class hash_set
{
private:
  typedef hashtable<Value, Value, HashFcn, identity<Value>, 
                    EqualKey, Alloc> ht;
  ht rep;
public:
  typedef typename ht::const_iterator iterator;
};
```

# 第三讲

## 27 算法的形式

从语言的层面看，算法是函数模板，容器、迭代器、仿函数、适配器、分配器都是类模板

如下两种是标准库算法的一般形式

```c++
template <typename Iterator>
return-type ALGORITHM(Iterator it1, Iterator it2) { /* TODO */ }

// 第二个版本传入一个准测(criterion), Cmp应该是一个仿函数Functor
template <typename Iterator, typnename Cmp>
return-type ALGORITHM(Iterator it1, Iterator it2, Cmp comp) { /* TODO */ }
```

如果迭代器不能回答算法的提问，则编译就会失败。

## 28 iterator_category, 值得再看

``stl_iterator.h``头文件中定义了5种迭代器的``tag``, 如下所示

```c++
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};	 // forward_list, hashtable(根据桶中链表是否双向)
struct bidirectional_iterator_tag : public forward_iterator_tag {};		// list, rb_tree
struct random_access_iterator_tag : public bidirectional_iterator_tag {};	// vector, array, deque
```

那么, 标准库为什么不用1 2 3 4 5这种整数来表明迭代器类型呢, 为什么要使用类类型来表示? 比如可以如下定义：

```c++
#define input_iterator_tag 1
#define output_iterator_tag 2
```

原因见下一节中``__distance()``函数的两个版本, 迭代器有五种分类, 为什么只需要两个``__distacne()``函数? 正是因为对象的继承.

typename的用法参见: https://www.jianshu.com/p/17d65e71f548

下面描述一下算法得到迭代器类型的具体过程: 假设我们有一个list容器, 相对其执行find()算法, 那么

1. 类``list``中命名了自己的迭代器类型为``typedef __list_iterator<T, T&, T*> iterator;``
2. 模板类``__list_iterator``是``list``的迭代器类, 该类使用语句``typedef bidirectional_iterator_tag iterator_category;``表明了该迭代器的类型
3. 用户调用``auto it = find(list.begin(), list.end(), val);``, 其中``list.begin()``返回的迭代器的类型为``list::iterator``
4. ``find``函数接收到两个``I == list::iterator``类型的迭代器 (``I``是模板参数), 假设命名为``start``和``finish``
5. ``find``函数通过调用``iterator_traits<I>::category``得到传入迭代器的category, 假设定义类型别名为cagy
6. 调用更细分的函数, 比如``distance``或``advance``, 剩下的细节见下面第29节中``distance``函数的两种不同实现

## 29 iterator_category 和 type_traits 对算法的影响

distance()函数的两种不同实现

```c++
template <class InputIterator>
inline iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
  typedef typename iterator_traits<InputIterator>::iterator_category category;	// iterator_category
  return __distance(first, last, category());	// 临时对象, 调用__distance()函数
}
// 对于input_iterator_tag类型的迭代器的__distance
// input, forward, bidirectional 这三种迭代器都会调用此版本的__distance, 原因就在于继承
// 因此标准库使用类来表示各种迭代器类型, 而不是使用数值
template <class InputIterator>
inline iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
  iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first; ++n;
  }
  return n;
}

// 对于random_access_iterator_tag类型迭代器的__distance
template <class RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag) {
  return last - first;
}
```

## 30 标准库各种算法的实现

### accumulate()  in \<stl_numeric.h\> in \<numeric\>

```c++
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) 
{
  for ( ; first != last; ++first)
    init = init + *first;
  return init;
}

// binary_op is a Funtor, 接受两个参数
// 传给binary_op的实参只要能使用函数调用运算符且接受两个参数就可以
// callable identity == function object == functor
template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) 
{
  for ( ; first != last; ++first)
    init = binary_op(init, *first);
  return init;
}
```

### for_each() in \<stl_algo.h\>

```c++
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) 
{
  for ( ; first != last; ++first)
    f(*first);
  return f;
}
```

### replace() and replace_if() in \<stl_algo.h\>

```c++
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) 
{
  for ( ; first != last; ++first)
    if (*first == old_value) *first = new_value;
}

template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) 
{
  for ( ; first != last; ++first)
    if (pred(*first)) *first = new_value;
}
```

### count() and count_if() in \<stl_algo.h\>

使用``rb_tree``和``hashtable``定义的容器都自带了``count()``操作, 因此这些容器应该使用其内部版本, 而不应该使用``std::count()``

```c++
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for ( ; first != last; ++first)
    if (*first == value)
      ++n;
  return n;
}

template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for ( ; first != last; ++first)
    if (pred(*first))
      ++n;
  return n;
}
```

### find() and find_if() in <stl_algo.h>

使用``rb_tree``和``hashtable``定义的容器都自带了``find()``操作, 因此这些容器应该使用其内部版本, 而不应该使用``std::find()``

```c++
// 返回的是迭代器
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
  while (first != last && *first != value) ++first;
  return first;
}

template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last,
                      Predicate pred) {
  while (first != last && !pred(*first)) ++first;
  return first;
}
```

### sort() in <stl_algo.h>

对``vector``执行从大到小排序: ``sort(v.rbegin(), v.rend())``, 其中``rbegin()``和``rend()``返回的是``reverse_iterator``, ``reverse_iterator``是一种迭代器适配器
容器``list``和``forward_list``自带了``sort()``操作, 因此这两个容器应该使用其内部版本, 而不应该使用``std::sort()``
不要对使用``rb_tree``和``hashtable``的容器执行``sort()``操作

```c++
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first != last) {
    __introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
    __final_insertion_sort(first, last);
  }
}

template <class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
  if (first != last) {
    __introsort_loop(first, last, value_type(first), __lg(last - first) * 2,
                     comp);
    __final_insertion_sort(first, last, comp);
  }
}
```

### binary_search() in <stl_algo.h>

值得注意的是, 该算法接受的是一个forward_iterator, 该forward_iterator被传入lower_bound()中, 在lower_bound()中会调用distance()来计算``[first, last)``之间的元素个数, 然后调用``advance()``来找到``mid``指针, 然后进行一次二分操作, 如此迭代.

在上述调用distance()和advance()时, 会根据传入的迭代器类型执行不同的重载版本. 因此对于不能随机访问的容器来说, ``std::binary_search()``的效率还不如``std::find()``

```c++
template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last,
                   const T& value) {
  ForwardIterator i = lower_bound(first, last, value);
  return i != last && !(value < *i);
}

template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value,
                   Compare comp) {
  ForwardIterator i = lower_bound(first, last, value, comp);
  return i != last && !comp(value, *i);
}
```

### lower_bound() in <stl_algo.h>

如下函数不是G2.91中的版本, 是侯捷上课用的版本, 不知道是从哪里来的

```c++
template <class ForwardIterator,class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val)
{
	ForwardIterator it;
	iterator_traits<ForwardIterator>::difference_type count, step;
	count = distance(first, last);
	while (count>0)
	{
		it = first;
		step = count / 2;
		advance(it, step);
		if (*it < val)  // 或者可以是 if(comp (*it, val))
		{
			first = ++it;
			count -= step + 1;
		}
		else count = step;
	}
	return first;
}
```

## 31 Functor(仿函数 或 函数对象 或 可调用对象)
标准库提供的三大类Functor: 算术类(Arithmetic), 逻辑运算类(Logical), 相对关系类(Relational) 
```c++
// 算术类, 定义在<stl_function.h>中, 通过头文件<functional>包含
// plus, minus, multiplies, divides, negate
template <class T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x - y; }
};
template <class T>
struct multiplies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x * y; }
};
// negate是一元的, unary读作u-nary, binary读作bi-nary
template <class T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x) const { return -x; }
};
	// 使用方法
	auto f = minus<int>();		// 此时f是一个可调用对象, 接受二元int参数
	cout << f(6, 5) << endl;	// 6 - 5 = 1
	// 或
	cout << minus<int>()(6, 5) << endl; // 先构造一个匿名对象, 然后调用该匿名对象
```
**值得注意的是这些仿函数, 除了negate, 其他都是二元可调用对象**

```c++
// 相对关系类: equal_to, not_equal_to, greater, greater_equal, less, less_equal
template <class T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x == y; }
};
template <class T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x > y; }
};
template <class T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x <= y; }
};

	// 使用方法
	sort(v.begin(), v.end(), greater<int>());	// 升序排序数组v
	hashtable< ..., equal_to<T>, ...> ht;		// 声明一个hashtable, 其关键字比较使用类equal_to<T>
```

**如下是G2.91一些特有的仿函数, 如identity, select1st, select2nd**

```c++
template <class T>
struct identity : public unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};

template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
  const typename Pair::first_type& operator()(const Pair& x) const
  {
    return x.first;
  }
};

template <class Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> 
{
  const typename Pair::second_type& operator()(const Pair& x) const
  {
    return x.second;
  }
};
```

#### 仿函数的可适配 (adaptable) 条件

STL规定, 每个Adaptable Function都应挑选如下两者继承之. 因为Function Adapter将会提问一些仿函数的参数类型.

当unary_function或binary_function作为父类的时候, 其大小为0. 当单独sizeof(binary_function)的时候, 其大小可能为1.

```c++
template <class Arg, class Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

// 函数适配器的提问
// 所以, 如果你想让你的仿函数变成可适配的, 就要继承如上两者, 或者自定义回答适配器的提问
// 什么是可适配的? 适配器要对我的仿函数作什么? Adapter的原译是变压器, 适配器会对仿函数做改造
template <class Operation> 
class binder2nd
  : public unary_function<typename Operation::first_argument_type,
                          typename Operation::result_type> {
protected:
  Operation op;
  typename Operation::second_argument_type value;	// 函数适配器向仿函数提问
  }
};
```

## 32 Adapter (适配器 或 改造器)

改造仿函数的Adapter称为仿函数适配器, 同理有容器适配器和迭代器适配器

一个Component的Adapter是如何实现改造的呢? 通过内含该Component

## 33 binder2nd

新标准中, ``binder2nd, bind2nd, binder1st, bind1st``都已经被弃用了, 取而代之的是``bind``

``auto functor = bind2nd(less<int>(), 40)``, 将40绑定到``less<int>()``的第二个参数, 调用``funcotr(x)``会执行``less<int>()(x, 40)``

类``binder2nd`` 和 函数``bind2nd``

```c++
template <class Operation> 
class binder2nd
  : public unary_function<typename Operation::first_argument_type,	// binder2nd本事是可适配的仿函数
                          typename Operation::result_type> {
protected:
  Operation op;		// 实际调用的对象, 该对象必须是binary functor (有局限性)
  typename Operation::second_argument_type value;	// 调用op时, 将value绑定到op的第二个参数
public:
  binder2nd(const Operation& x,
            const typename Operation::second_argument_type& y) 	// 初始化该类, 传入一个functor和需要绑定的参数
      : op(x), value(y) {}
  typename Operation::result_type
  operator()(const typename Operation::first_argument_type& x) const // 该类被调用时, 向外部提供一个unary接口
  {
    return op(x, value); 
  }
};

// 用户调用函数bind2nd, 编译器进行模板推断, 然后用推断出的模板参数调用类binder2nd
template <class Operation, class T>
inline binder2nd<Operation> 	// 返回一个functor, 是一个临时binder2nd对象
bind2nd(const Operation& op, const T& x) 	// op是需要被修饰的functor, x是绑定到第二参数的值
{
  typedef typename Operation::second_argument_type arg2_type;	// 向op提问, op必须是可适配的
  return binder2nd<Operation>(op, arg2_type(x));
}

// 值得注意的是, 上述的整个过程, 都在用引用传递参数
```

同理有 类``binder1st`` 和 函数``bind1st``, 将第一个参数绑定到原有的binary接口

**调用上述两个函数时, 第一个参数必须是一个``binary functor``,  且该 functor 必须是 adaptable 的, 有非常大的局限性**

**一个``binary functor``经过 ``bind1st`` 或 ``bind2nd`` 修饰后, 得到的是一个``unary functor``**

## 34 not1

``not1``在新标准中仍然可用

函数``not1`` 和 类``unary_negate``

```c++
template <class Predicate>
class unary_negate
  : public unary_function<typename Predicate::argument_type, bool> {	// 该类本身是可适配的
protected:
  Predicate pred;
public:
  explicit unary_negate(const Predicate& x) : pred(x) {}
  bool operator()(const typename Predicate::argument_type& x) const {	// 传入的Predicate必须是可适配的
    return !pred(x);
  }
};

template <class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {
  return unary_negate<Predicate>(pred);
}
```

## 35 bind (C++ 11)

``auto newCallable = bind(oldCallable, xxx)``

``_1, _2, ... , _n``被称为placerholder, ``_1``表明在调用newCallable时, 这个位置的参数会被传到oldCallable的第一个位置

一般调用bind时不指定模板参数, 如果指定则一般指定第一个模板参数, 第一个模板参数表明返回值类型

```c++
#include <functional>
using namespace std;
using std::placeholders::_1;
using std::placeholders::_2;

	auto fn_five = bind(divide, 10, 2);
    cout << fn_five() << endl;		// 5
    auto fn_half = bind(divide, _1, 2);
    cout << fn_half(88) << endl;	// 44
    auto fn_invert = bind(divide, _2, _1);
    cout << fn_invert(10, 2) << endl;	// 0.2
    auto fn_rounding = bind<int>(divide, _1, _2);
    cout << fn_rounding(10, 3) << endl;	// 3

	count_if(v.begin(), v.end(), not1(bind(less<int>(), _1, 40)));	// 统计大于等于40的数的个数

// bind还可以修饰成员函数或成员变量, 此处不做展示; 详情参见cplusplus:  
```

## 36 reverse_iterator

类 ``reverse_iterator``

**Adapter 的本质是<u>包含</u>一个被修饰的组件, 记住该组件的状态, 然后决定如何操作**

```cpp
template <class Iterator>
class reverse_iterator 
{
protected:
  Iterator current;

public:
  // 解引用, 不能移动current, 取current的前面的元素
  reference operator*() const {
    Iterator tmp = current;
    return *--tmp;
  }
  // 前++ 和 后++
  self& operator++() {
    --current;
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    --current;
    return tmp;
  }
  // 前-- 和 后--
  self& operator--() {
    ++current;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    ++current;
    return tmp;
  }
  // 被该适配器修饰的迭代器必须要支持 +x 和 -x 操作, 下面的才能调用
  self operator+(difference_type n) const {
    return self(current - n);	// 这里会调用被修饰迭代器本身的 -n 操作, 然后构造一个新的reverse_iterator并返回
  }
  self& operator+=(difference_type n) {
    current -= n;	// 调用被修饰的迭代器本身的 -= 操作
    return *this;
  }
  self operator-(difference_type n) const {
    return self(current + n);
  }
  self& operator-=(difference_type n) {
    current += n;	// 调用被修饰的迭代器本身的 += 操作
    return *this;
  }
}; 
```

## 37 insert_iterator

inserter 让 assignment 变成 insertion

函数``inserter()`` 和 类``insert_iterator``

```c++
// 提供给用户的接口, inserter函数
template <class Container, class Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator i) 	
{
  typedef typename Container::iterator iter;
  return insert_iterator<Container>(x, iter(i));
}
// 实际执行操作的类
template <class Container>
class insert_iterator {
protected:
  Container* container;
  typename Container::iterator iter;
public:
  typedef output_iterator_tag iterator_category;	// 注意
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  insert_iterator(Container& x, typename Container::iterator i) 
    : container(&x), iter(i) {}
    
  // 将 assignment 变成 insertion, 从这种意义上来说, 操作符重载也算是C++的多态 
  insert_iterator<Container>&
  operator=(const typename Container::value_type& value) { 
    iter = container->insert(iter, value);
    ++iter;
    return *this;
  }
  insert_iterator<Container>& operator*() { return *this; }
  insert_iterator<Container>& operator++() { return *this; }
  insert_iterator<Container>& operator++(int) { return *this; }
};
```

## 38 ostream_iterator

使用 ostream_iterator

```c++
vector<int> v = {1, 2, 3, 4, 5};
ostream_iterator out_it(std::cout, ", ");
copy(v.begin(), v.end(), out_it);	// output: 1, 2, 3, 4, 5, 
```

类 ``ostream_iterator``

```c++
// 此处是G2.91的实现, 侯捷上课用的是最新版实现, 不过没有本质上的区别
template <class T>
class ostream_iterator 
{
protected:
  ostream* stream;	// cout是ostream对象, ofstream和ostringstream都继承自ostream
  const char* string;
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  // 注意: 构造 和 拷贝构造 传递的都是指针, 因为 ostream 对象本身就不允许拷贝
  ostream_iterator(ostream& s) : stream(&s), string(0) {}
  ostream_iterator(ostream& s, const char* c) : stream(&s), string(c)  {}
  ostream_iterator<T>& operator=(const T& value) { 
    *stream << value;
    if (string) *stream << string;
    return *this;
  }
  ostream_iterator<T>& operator*() { return *this; }
  ostream_iterator<T>& operator++() { return *this; } 
  ostream_iterator<T>& operator++(int) { return *this; } 
};
```

### C++ 的 IO 库概览

```c++
// 以下所说的IO类都是新版的GNU C定义的
// 在G2.91版本中, 没有basic前缀和多余的typedef, 类名直接就叫做istream 和 ostream
class ios_base;		// 基类, 定义在<ios_base.h>

template<typename _CharT, typename _Traits>
class basic_ios : public ios_base;	// 直接继承自ios_base, 定义在<basic_ios.h>

typedef basic_ios<char> ios;	// 声明在<iosfwd>

// istream 和 ostream 的初始类是 basic_istream 和 basic_ostream
template<typename _CharT, typename _Traits>
class basic_istream : virtual public basic_ios<_CharT, _Traits>		// <istream>
    
template<typename _CharT, typename _Traits>
class basic_ostream : virtual public basic_ios<_CharT, _Traits>		// <ostream>

// istream 和 ostream 的声明, <iosfwd>
typedef basic_ostream<char> 		ostream;
typedef basic_istream<char> 		istream;

// 读写通用的类 iostream 的初始类是 basic_iostream,  <istream>
template<typename _CharT, typename _Traits>
class basic_iostream: public basic_istream<_CharT, _Traits>, public basic_ostream<_CharT, _Traits>
    
typedef basic_iostream<char> 		iostream;	// <iosfwd>

// 其他:
// ifstream 的类名为 basic_ifstream, 继承自istream; 同理istringstream
// ofstream 的类名为 basic_ofstream, 继承自ostream; 同理ostringstream
// fstream 的类名为 basic_fstream, 继承自iostream; 同理stringstream

// 所以, 一个 ostream 引用可以绑定到 cout 或 ofstream 或 ostringstream 或 fstream 或 stringstream
// 以上道理同样适用于 istream 引用

// 最新版GNU C关于 cin 和 cout, 声明在<iostream>中
// 奇怪的是, basic_ostream不存在默认的构造函数, 所以cout是如何被构造的?
// 在G2.91中, istream和ostream类都存在默认的构造函数
extern istream cin;			/// Linked to standard input
extern ostream cout;		/// Linked to standard output
extern ostream cerr;		/// Linked to standard error (unbuffered)
extern ostream clog;		/// Linked to standard error (buffered)
```

## 39 istream_iterator

对 istream_iterator 进行带参初始化或++操作会执行读入G

G2.91中类``istream_iterator``的定义与现在使用的版本有很大差异, 如下是[cplusplus](http://www.cplusplus.com/reference/std/iterator/istream_iterator/)的版本

```c++
template <class T, class charT=char, class traits=char_traits<charT>, class Distance=ptrdiff_t>
class istream_iterator : public iterator<input_iterator_tag, T, Distance, const T*, const T&>
{
  basic_istream<charT,traits>* in_stream;		// 存的是pointer, 不能拷贝istream对象
  T value;
 
public:
  typedef charT char_type;
  typedef traits traits_type;
  typedef basic_istream<charT,traits> istream_type;
  istream_iterator() : in_stream(0) {}	// end of stream
  istream_iterator(istream_type& s) : in_stream(&s) { ++*this; }
  istream_iterator(const istream_iterator<T,charT,traits,Distance>& x)	// copy ctor
    : in_stream(x.in_stream), value(x.value) {}
  ~istream_iterator() {}
 
  const T& operator*() const { return value; }
  const T* operator->() const { return &value; }
  istream_iterator<T,charT,traits,Distance>& operator++() {
    if (in_stream && !(*in_stream >> value)) in_stream=0;
    return *this;
  }
  istream_iterator<T,charT,traits,Distance> operator++(int) {
    istream_iterator<T,charT,traits,Distance> tmp = *this;
    ++*this;
    return tmp;
  }

```

使用 istream_iterator 的一个例子

```c++
// 把新输入的元素附到vector末尾, 直到遇到end of stream
istream_iterator<int> eos, iit(cin);
copy(iit, eos, inserter(v, v.end()));
```

**同一个copy函数, 传入不同的组件, 得到不同的结果, 真乃神一般的设计, 这也可以称之为多态.**

# 第四讲 STL周边

## 40 一个万用的哈希函数

**勿在浮沙筑高台**

如何对一个复杂的对象进行哈希? 把该对象的每个元素进行哈希, 然后把哈希值相加? 类似下面这样

```c++
class Customer
{
	string fname, lname;
	long no;
};
class CustomerHash
{
public:
	size_t operator()(const Customer &c) const
	{
		return std::hash<string>()(c.fname) +
            std::hash<string>()(c.lname) +
            std::hash<long>()(c.no);
	}
};
unordered_set<Customer, CustomerHash> custset;
```

上述做法太过天真

TR1提供了一个好用的哈希函数, 其通过variadic templates实现递归计算哈希值

```c++
return hash_val(c.fname, c.lname, c.no);

// 在G4.9.2中没找到如下函数的定义, 可能换名了或者更改了哈希方式
template <typename... Types>
inline size_t hash_val(const Types&... args)
{
    size_t seed = 0;
    hash_val(seed, args...);
    return seed;
}

template <typename T, typename... Types>
inline void hash_val(size_t &seed, const T &val, const Types&... args)
{
    hash_combine(seed, val);	// recursive, 逐一取val, 改变seed
    hash_val(seed, args...);	// 当args只剩一个元素时, 会调用下面的hash_val
}

template <typename T>
inline void hash_val(size_t &seed, const T &val)
{
    hash_combine(seed, val);
}

template <typename T>
inline void hash_combine(size_t &seed, const T &val)
{
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
```

Golden Ratio 的十六进制表示为 1.9E3779B9... = 1.618... 

除了将哈希函数传入给unordered_map或unordered_set, 还可以直接偏特化struct hash, 让标准库自动调用该偏特化的版本

```c++
// 新版GNU C中对unordered_set的声明, 发现其默认的哈希函数是hash<_Value>
template<class _Value, class _Hash = hash<_Value>,class _Pred = std::equal_to<_Value>,
	   	 class _Alloc = std::allocator<_Value> >
class unordered_set
// 所以我们可以偏特化hash<Customer>, 使其成为可调用对象
// 使用如下写法, 可以直接声明unordered_set<Customer> custset;
namespace std
{
    template<>
    struct hash<Customer>
    {
        size_t operator()(const Customer &c) const noexcept
        {
            return hash_val(c.fname, c.lname, c.no);
        }
    }
}
```

## 41 Tuple

Tuple是从TR1开始引入的

G4.8 Tuple 的实现, 注意偏特化会让类名后面带尖括号<>

```c++
template <typename... Values> class tuple;		// 纯泛化版本
template <> class tuple<> {};	// 偏特化版本, 终止递归
template <typename Head, typename... Tail>
class tuple<Head, Tail...> : private tuple<Tail...>	// 偏特化版本, 递归继承
{
    typedef tuple<Tail...> inherited;
public:
   	tuple() {}
    tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail) {}
    typename Head::type head() { return m_head; }
    inherited &tail() { return *this; }		// 将本对象转型为父类的对象
protected:
    Head m_head;
}
```

## 42 43 Type Traits

trivial [ˈtrɪviəl] 琐碎的, 不重要的;  polymorphic [ˌpɒlɪˈmɔːfɪk] 多态的

G2.91实现了一个简单的type traits, 定义在文件``type_traits.h``中

```c++
struct __true_type {};
struct __false_type {};

// 泛化的type traits
template <class type>
struct __type_traits 
{ 
   typedef __true_type     this_dummy_member_must_be_first;
   typedef __false_type    has_trivial_default_constructor;		// 你的默认构造函数是不重要的吗? 不, 重要
   typedef __false_type    has_trivial_copy_constructor;		// 你的默认拷贝构造是不重要的吗? 不, 重要
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;		// POD = Plain Old Data, 就是C语言中的struct, 没有function
};
// int 特化的 type traits
__STL_TEMPLATE_NULL struct __type_traits<int> 
{
   typedef __true_type    has_trivial_default_constructor;		// 你的默认构造函数是不重要的吗? 是的, 不重要
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};
```

但是在这种情况下, 怎么使用呢? 写一个类, 还需要再写一个与之对应的type traits ?

**问题: ``__true_type`` 和 ``__false_type`` 都是一个类型, 其中没有数据和类型定义, 使用者如何提问?**

因此, C++ 11提供了新的type traits提问方法,  用户自定义的类无需特化自己的type traits, 也能回答标准库的提问.

这些新的type traits提问方法定义在头文件``<type_traits>``中, 比如`is_void<T>, is_class<T>, is_object<T>, is_ploymorphic<T>, is_trivially_copy_constructible<T>...`, 这些都是类模板, 模板参数是一个类型(Type), 该类中有静态成员``value``, 通过其value的值来得到type traits的提问.

C++ 11如何实现对任意类型都能进行type traits的回答?

两个基类: ``true_type`` 和 ``false_type``

```c++
/// These two type used as a compile-time boolean with true/false value.
typedef integral_constant<bool, true>     true_type;
typedef integral_constant<bool, false>    false_type;

// integral_constant, Tp是类型, v是该类型的值
template<typename _Tp, _Tp __v>
struct integral_constant
{
    static constexpr _Tp                  value = __v;	// 成员变量
    typedef _Tp                           value_type;
    typedef integral_constant<_Tp, __v>   type;
    constexpr operator value_type() const noexcept { return value; }
    #if __cplusplus > 201103L

    #define __cpp_lib_integral_constant_callable 201304

    constexpr value_type operator()() const noexcept { return value; }
    #endif
};

// 使用方法
	cout << is_void<int>::value << endl;			// 输出0
	cout << is_class<vector<int>>::value << endl;	// 输出1
```

如下是 G4.9.2 对于 ``is_void`` 的实现, 疯狂利用模板和特化. 侯捷: 变化的是类型, 而不是普通的变量.

```c++
// 以 is_void 为例子说明: 首先对传入的类型进行remove_cv操作, 然后传入is_void_helper来判断是否为void
// remove_cv = remove const and remove volatile
template<typename _Tp>
struct is_void : public __is_void_helper<typename remove_cv<_Tp>::type>::type {};

// remove_cv
template<typename _Tp>
struct remove_cv
{
    typedef typename remove_const<typename remove_volatile<_Tp>::type>::type type;
};

// remove_const, 泛化版本, 传入T返回T
template<typename _Tp>
struct remove_const { typedef _Tp     type; };
// remove_const, 特化版本, 当传入是const T时, 返回T
template<typename _Tp>
struct remove_const<_Tp const> { typedef _Tp     type; };

template<typename _Tp>
struct remove_volatile{ typedef _Tp     type; };	// 泛化
template<typename _Tp>
struct remove_volatile<_Tp volatile>{ typedef _Tp     type; };	// 特化

template<typename>
struct __is_void_helper: public false_type { };	// 泛化, 我不是空的
template<>
struct __is_void_helper<void>: public true_type { }; // 特化, 我是空的
```

同理, 如下是 ``is_integral`` 的实现

```c++
template<typename _Tp>
struct is_integral: public __is_integral_helper<typename remove_cv<_Tp>::type>::type {};

template<typename>
struct __is_integral_helper: public false_type { };

template<>
struct __is_integral_helper<bool>: public true_type { };

template<>
struct __is_integral_helper<char> : public true_type { };

// 下面还有很多特化版本, 像int, long等
```

## 44 ostream 与 cout

前面已经表述了 ``cout`` 是一个 `ostream` 类型的对象, 为什么 ``cout`` 能输出各种类型? 因为 ostream 中已经提前重载了`<<`

```c++
class ostream : virtual public ios
{
    ostream& operator<<(char c);
    ostream& operator<<(unsigned char c) { return (*this) << (char)c; }
    ostream& operator<<(signed char c) { return (*this) << (char)c; }
    ostream& operator<<(const char *s);
    ostream& operator<<(const unsigned char *s)
	{ return (*this) << (const char*)s; }
    ostream& operator<<(const signed char *s)
	{ return (*this) << (const char*)s; }
    ostream& operator<<(const void *p);
    ostream& operator<<(int n);
    ostream& operator<<(unsigned int n);
    ostream& operator<<(long n);
    ostream& operator<<(unsigned long n);
    // ... 还有其他, 未一一列举
};
```

标准库中已经重载了``<<``的类有: ``string, complex, thread, shared_ptr, sub_match, bitset``

其中, ``complex``类定义在头文件``<complex>``中;  ``sub_match``定义在头文件``<regex>``中, 和正则表达式有关

## 45 46 比较 copy 和 move 的性能

