#ifndef __HW_H
#define __HW_H



#include "app_kernel.h"

class Helloworld : public AppKernel
{
private:
    
    
    /* kernel */
    uint64_t ret_offset;
    uint64_t size_offset;
    uint64_t in_r1_offset;
    uint64_t in_r2_offset;

public:
    uint64_t bram_base_addr;
    Helloworld(uint64_t base_addr, uint64_t bram_base_addr)
    : AppKernel(base_addr)
    {
        this->bram_base_addr = bram_base_addr;
        ret_offset = 0x10;
        size_offset = 0x24;
        in_r1_offset = 0x18;
        in_r2_offset = 0x1c;
    }
    ~Helloworld() {}

    void set_size(int size);
    void set_fetch_offset(uint64_t offset);

    int get_result();


    virtual int compute_result(int *in_arr, int size);
    
    

};

#endif