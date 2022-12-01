#include <iostream>
#include <iterator>  // 迭代器, 迭代器萃取器
#include <typeinfo>  // typeid
#include <vector>
#include <unordered_map>
#include <set>
#include <forward_list>
using namespace std;

void _display_category(input_iterator_tag)
{
    cout << "input_iterator" << endl;
}
void _display_category(output_iterator_tag)
{
    cout << "output_iterator" << endl;
}
void _display_category(forward_iterator_tag)
{
    cout << "forward_iterator" << endl;
}
void _display_category(bidirectional_iterator_tag)
{
    cout << "bidirectional_iterator" << endl;
}
void _display_category(random_access_iterator_tag it)
{
    cout << "random_access_iterator" << endl;
}
void func(int)
{
    puts("hello");
}


template <typename I>
void display_category(I itr)    // 传进来一个迭代器对象itr
{
    // 此处的cagy是一个对象, 其类型是iterator_traits<I>::iterator_category
    // typename并不是给类型换一个名字, 而是显式表明其后面跟的是一个类型, 而不是其他元素
    // 具体参见: https://www.jianshu.com/p/17d65e71f548
    typename iterator_traits<I>::iterator_category cagy;
    _display_category(cagy);
    cout << "typeid(itr).name(): " << typeid(itr).name() << endl;
}

int main()
{
    // vector<int>::iterator是一个迭代器类型, 
    // 加上()后相当于临时构造该类型的对象
    display_category(vector<int>::iterator());
    display_category(unordered_map<int, int>::iterator());
    display_category(set<int>::iterator());
    display_category(forward_list<int>::iterator());
    display_category(istream_iterator<int>());
    display_category(ostream_iterator<int>(cout));
}