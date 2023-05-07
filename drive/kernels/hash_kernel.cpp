#include "hash_kernel.h"
#include "../xdma_io.h"
#include "../utils.h"

void HashKernel::set_input_offset(uint64_t offset_val){
    uint32_t lsb = offset_val & 0xFFFFFFFF;
    uint32_t msb = offset_val >> 32;

    XDMA_WRITE_DATA(base_addr + input_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + input_addr_offset_1, bytes_from_int(msb));



}


void HashKernel::set_output_offset(uint64_t offset_val){
    uint32_t lsb = offset_val & 0xFFFFFFFF;
    uint32_t msb = offset_val >> 32;

    XDMA_WRITE_DATA(base_addr + output_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + output_addr_offset_1, bytes_from_int(msb));



}

void HashKernel::set_batch_size(uint32_t size) {
    XDMA_WRITE_DATA(base_addr + batch_size_offset, bytes_from_int(size));

}

uint64_t HashKernel::get_input_offset() {
    long long out;
    dma_read(XDMA_C2H_0, base_addr + input_addr_offset_0, 0, 8, 0, 1, (char *)&out);
    return out;
}