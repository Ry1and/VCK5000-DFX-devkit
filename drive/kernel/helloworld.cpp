#include "helloworld.h"
#include "dma_io.h"
#include "utils.h"


void Helloworld::set_start(){

    
    dma_write(XDMA_H2C_0, base_addr, 0, 4,0,1, bytes_from_int((char *)malloc(4), 0x1), 0);

}


void set_size(int size) {





}



void set_fetch_offset(__uint64_t offset){







}

int get_result() {









}


void wait_on_done() {






}
