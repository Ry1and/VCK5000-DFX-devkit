#include "thread_class.cpp"
#include <thread>


int main() {
    thread_class *tc = new thread_class();
    std::thread th(&thread_class::start, tc, 3);
    th.join();
    return 0; 
}