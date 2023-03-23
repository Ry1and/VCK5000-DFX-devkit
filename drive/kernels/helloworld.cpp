#include "helloworld.h"
#include "../dma_io.h"
#include "../utils.h"




void Helloworld::set_size(int size) {


    XDMA_WRITE_DATA(base_addr + size_offset, bytes_from_int(size));


}


void Helloworld::set_fetch_offset(uint64_t val){
    uint32_t lsb = val & 0xFFFFFFFF;
    uint32_t msb = val >> 32;

    XDMA_WRITE_DATA(base_addr + in_r1_offset, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + in_r1_offset, bytes_from_int(lsb));



}

int Helloworld::get_result() {

    int res;
    XDMA_READ(base_addr + ctrl_offset, (char *)&res);
    return res;

}

