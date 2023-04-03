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
    AppKernel()
    : DummyKernel()
    {
        ctrl_offset = 0x00;
    }
    ~AppKernel() {};

    void set_start();
    void wait_on_done();
    void get_ctrl_reg();
};



#endif

