#include "dummy_kernel.h"

class Helloworld : DummyKernel
{
private:
    /* data */
    uint64_t ctrl_offset;
    uint64_t ret_offset;
    uint64_t size_offset;
    uint64_t in_r1_offset;
    uint64_t in_r2_offset;
    int get_done();
public:
    Helloworld(__uint64_t base_addr);
    ~Helloworld();
    void set_start();
    void set_size(int size);
    void set_fetch_offset(__uint64_t offset);

    int get_result();
    void wait_on_done();

    virtual int get_result(int *in_arr, int size);
    
    

};

Helloworld::Helloworld(__uint64_t base_addr)
: DummyKernel(base_addr)
{
    ctrl_offset = 0x00;
    ret_offset = 0x10;
    size_offset = 0x24;
    in_r1_offset = 0x18;
    in_r2_offset = 0x1c;
}

Helloworld::~Helloworld()
{
}
