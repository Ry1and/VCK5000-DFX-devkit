#include <iostream>
#include <unistd.h>

#include "app_kernel.h"
#include "../dma_io.h"
#include "../utils.h"


int AppKernel::get_done(){
    int res;
    XDMA_READ(base_addr + ctrl_offset, (char *)&res);
    return (res & 0x2) == 0x2;

}


void AppKernel::set_start(){
    XDMA_WRITE_DATA(base_addr + ctrl_offset, bytes_from_int(0x1));
}



void AppKernel::wait_on_done() {

    while (!get_done()){
        printf("kernel processing...\n");
        usleep(1);
    }
}