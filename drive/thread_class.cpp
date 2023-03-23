#include <iostream>


class thread_class
{
private:
    /* data */
public:
    thread_class(/* args */);
    ~thread_class();
    void start(int i);
};

thread_class::thread_class(/* args */)
{
}

thread_class::~thread_class()
{
}

void thread_class::start(int i){
    printf("%d\n", i);
}