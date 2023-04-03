#ifndef DUMMY_KERNEL_H__
#define DUMMY_KERNEL_H__

#include <cstdint>

class DummyKernel
{
private:
    /* data */

protected:
    uint64_t base_addr;
    
public:
    DummyKernel(){
        base_addr = 0x020100010000;
    };
    ~DummyKernel() {};
};

#endif