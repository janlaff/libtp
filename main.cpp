#include <iostream>

#include "thread_pool.h"
#include "future_task.h"

int main() {
    tp::thread_pool pool;

    pool.post([&pool]() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        pool.resume();
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.pause();

    std::cout << pool.post<tp::future_task<std::string>>([]() { return "Hello World"; })->get() << std::endl;

    pool.wait();

    return 0;
}