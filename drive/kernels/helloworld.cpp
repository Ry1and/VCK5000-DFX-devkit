#include "helloworld.h"
#include "dma_io.h"
#include "utils.h"


int Helloworld::get_done(){
    int res;
    XDMA_READ(base_addr + ctrl_offset, (char *)&res);
    return (res & 0x2) == 0x2;

}


void Helloworld::set_start(){

    
  
    XDMA_WRITE_DATA(base_addr + ctrl_offset, bytes_from_int(0x1));
}


void Helloworld::set_size(int size) {


    XDMA_WRITE_DATA(base_addr + size_offset, bytes_from_int(size));


}



void Helloworld::set_fetch_offset(__uint64_t val){
    uint64_t lsb = val & 0xFFFFFFFF;
    uint64_t msb = val >> 32;

    XDMA_WRITE_DATA(base_addr + in_r1_offset, bytes_from_int(lsb));
    XDMA_WRITE_DATA(base_addr + in_r1_offset, bytes_from_int(lsb));



}

int Helloworld::get_result() {

    int res;
    XDMA_READ(base_addr + ctrl_offset, (char *)&res);
    return res;

}


void Helloworld::wait_on_done() {

    while (!get_done()){
        printf("kernel processing...\n");
        usleep(1);
    }




}
