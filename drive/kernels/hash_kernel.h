#ifndef HASH_K_H__
#define HASH_K_H__

#include "app_kernel.h"

class HashKernel : public AppKernel
{
private:
    uint64_t input_addr_offset_0;
    uint64_t input_addr_offset_1;
    uint64_t output_addr_offset_0;
    uint64_t output_addr_offset_1;
public:
    HashKernel(uint64_t addr)
    : AppKernel(base_addr)
    {
        input_addr_offset_0 = 0x10;
        input_addr_offset_1 = 0x14;
        output_addr_offset_0 = 0x1c;
        output_addr_offset_1 = 0x20;
    }

    ~HashKernel();

    void set_input_offset(uint64_t offset_val);
    void set_output_offset(uint64_t offset_val);

};

#endif