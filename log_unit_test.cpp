#include "log_manger.h"
#include <iostream>

#include <thread>
#include <atomic>

using namespace std;
namespace bm
{
log_manger s_log_manger;
}
int main()
{
    long long a = 1;
    int b = 2;
    b = (int)a;

    std::cout << b << std::endl;

    atomic_bool b1(true),b2(true),b3(true);
    (new thread([&](){
        b1 = true;
        for(int i = 0; i < 1000; i++)
        {
            BMLOG(INFO) << "=======================1 =====" << i << "========================\n";
            BMLOG(INFO) << "hello info test" << " hello info test2" << std::endl;
            BMLOG(WARNING) << "warning test";
            BMLOG(ERROR) << "error test";
            bool c = true;
            char c_arr[20] = "char arr test";
            BMLOG(INFO) << "string test" << a << b << c << c_arr << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        b1 = false;
    }))->detach();

    (new thread([&](){
        b2 = true;
        for(int i = 0; i < 1000; i++)
        {
            BMLOG(INFO) << "=======================2 =====" << i << "========================\n";
            BMLOG(INFO) << "hello info test" << " hello info test2" << "\n";
            BMLOG(WARNING) << "warning test";
            BMLOG(ERROR) << "error test";
            bool c = true;
            char c_arr[20] = "char arr test";
            BMLOG(INFO) << "string test" << a << b << c << c_arr << "\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        b2 = false;
    }))->detach();

    (new thread([&](){
        b3 = true;
        for(int i = 0; i < 1000; i++)
        {
            BMLOG(INFO) << "=======================3 =====" << i << "========================\n";
            BMLOG(INFO) << "hello info test" << " hello info test2" << "\n";
            BMLOG(WARNING) << "warning test";
            BMLOG(ERROR) << "error test";
            bool c = true;
            char c_arr[20] = "char arr test";
            BMLOG(INFO) << "string test" << a << b << c << c_arr << "\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        b3 = false;
    }))->detach();
    s_log_manger.stream().close();
    log_manger log;

    while(b1 || b2 || b3);

    std::cout << "all write log complate!" << std::endl;

    return 0;
}