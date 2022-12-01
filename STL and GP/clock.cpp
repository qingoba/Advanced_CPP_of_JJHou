// 在当前环境下, CLOCKS_PER_SEC = 1000;
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
using std::cout;
using std::endl;
int main()
{
    // clock(): Returns the processor time consumed by the program.
    // The value returned is expressed in clock ticks, 
    //   which are units of time of a constant but system-specific length 
    //   (with a relation of CLOCKS_PER_SEC clock ticks per second).
    // clock()返回的是该程序执行到现在为止经过的clock ticks数目, 如果用返回值除以CLOCKS_PER_SEC, 则得到秒数
    _sleep(1000);
    clock_t timeStart = clock();
    cout << timeStart << endl;
    return 0;
}