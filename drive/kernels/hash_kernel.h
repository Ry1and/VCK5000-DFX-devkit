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
    uint64_t batch_size_offset;
public:
    HashKernel()
    : AppKernel()
    {
        input_addr_offset_0 = 0x10;
        input_addr_offset_1 = 0x14;
        output_addr_offset_0 = 0x1c;
        output_addr_offset_1 = 0x20;
        batch_size_offset = 0x28;
    }

    ~HashKernel() {}

    void set_input_offset(uint64_t offset_val);
    void set_output_offset(uint64_t offset_val);
    void set_batch_size(uint32_t size);
};

#endif