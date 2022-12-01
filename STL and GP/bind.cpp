#include <iostream>
#include <functional>
using namespace std;
using std::placeholders::_1;
using std::placeholders::_2;

double divide(double x, double y) { return x / y; }


int main()
{
    auto fn_five = bind(divide, 10, 2);
    cout << fn_five() << endl;
    auto fn_half = bind(divide, _1, 2);
    cout << fn_half(88) << endl;
    auto fn_invert = bind(divide, _2, _1);
    cout << fn_invert(10, 2) << endl;
    auto fn_rounding = bind<int>(divide, _1, _2);
    cout << fn_rounding(10, 3) << endl;

}
