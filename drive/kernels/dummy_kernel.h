#include "vck5000.h"

#include <cstdint>

class DummyKernel
{
private:
    /* data */

protected:
    uint64_t base_addr;
    
public:
    DummyKernel(__uint64_t addr);
    ~DummyKernel();
};

DummyKernel::DummyKernel(uint64_t addr)
{
    base_addr = addr;
}

DummyKernel::~DummyKernel()
{
}



 