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
    DummyKernel(uint64_t addr){
        base_addr = addr;
    };
    ~DummyKernel();
};

#endif