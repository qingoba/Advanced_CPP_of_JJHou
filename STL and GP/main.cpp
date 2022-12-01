#include <list>
#include <algorithm>
#include <memory>
#include <iostream>
#include <map>
#include <forward_list>
#include <stack>
#include <functional>
#include <iterator>
#include <vector>
#include <tr1/functional_hash.h>
#include <type_traits>
#include <cmath>
#include <numeric>
#include <complex>
#include <memory>
using namespace std;

static int y;
int y = 6;

int func()
{
	static int x = 5;
    ++x;
    return x;
}


int main()
{
	cout << func() << endl;	// 6
	cout << func() << endl;	// 7
	cout << func() << endl;	// 8

	vector<int> v = {1, 2, 3, 4, 5};
	ostream_iterator<int> out_it(std::cout, ", ");
	copy(v.begin(), v.end(), out_it);
	cout << endl;

	// 把新输入的元素附到vector末尾, 直到遇到end of stream
	istream_iterator<int> eos, iit(cin);
	copy(iit, eos, inserter(v, v.end()));

	copy(v.begin(), v.end(), out_it);
	cout << endl;

	hash<int> hs;
	hs(6);
	
	cout << is_void<int>::value << endl;			// 输出0
	cout << is_class<vector<int>>::value << endl;	// 输出1

	unique_ptr<int> P(new int(5));
	v.emplace_back(1);
	v.push_back(2);

}