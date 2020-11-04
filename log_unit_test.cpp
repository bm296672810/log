#include "log_manger.h"
#include <iostream>

#include <thread>
#include <atomic>

int main()
{
    long long a = 1;
    int b = 2;
    b = (int)a;


    atomic_bool b1(true),b2(true),b3(true);

    LOG(INFO) << "hello info test" << " hello info test2" << "\n";
    LOG(WARNING) << "warning test";
    LOG(ERROR) << "error test";
    bool c = true;
    char c_arr[20] = "char arr test";
    LOG(INFO) << "string test" << a << b << c << c_arr << "\n";

    return 0;
}