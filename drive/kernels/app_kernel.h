#ifndef APP_KERNEL_H__
#define APP_KERNEL_H__

#include "dummy_kernel.h"



class AppKernel : public DummyKernel
{
private:
    /* data */
protected:
    uint64_t ctrl_offset;
    int get_done();

public:
    AppKernel(uint64_t base_addr)
    : DummyKernel(base_addr)
    {
        ctrl_offset = 0x00;
    }
    ~AppKernel();

    void set_start();
    void wait_on_done();
};



#endif

