#include <mutex>
#include <thread>
#include <iostream>

std::mutex m;
int i = 0;

void add() {

    for (int j = 0; j < 1000000; ++j) {

        std::lock_guard<std::mutex> l(m);
        ++i;
    }
}

int main() {
    std::thread t1(add);
    std::thread t2(add);
    t1.join();
    t2.join();
    std::cout << i << std::endl;
}