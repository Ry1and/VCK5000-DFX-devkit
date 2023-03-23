#include "hash_kernel.h"
#include "../dma_io.h"
#include "../utils.h"

void HashKernel::set_input_offset(uint64_t offset_val){
    uint32_t lsb = offset_val & 0xFFFFFFFF;
    uint32_t msb = offset_val >> 32;

    XDMA_WRITE_DATA(base_addr + input_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + input_addr_offset_1, bytes_from_int(lsb));



}


void HashKernel::set_output_offset(uint64_t offset_val){
    uint32_t lsb = offset_val & 0xFFFFFFFF;
    uint32_t msb = offset_val >> 32;

    XDMA_WRITE_DATA(base_addr + output_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + output_addr_offset_1, bytes_from_int(lsb));



}