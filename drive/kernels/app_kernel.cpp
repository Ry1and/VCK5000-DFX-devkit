#include <iostream>
#include <unistd.h>

#include "app_kernel.h"
#include "../xdma_io.h"
#include "../utils.h"


int AppKernel::get_done(){
    int res;
    XDMA_READ_WORD(base_addr + ctrl_offset, (char *)&res);
    printf("%08x\n", res);
    return (res & 0x2) == 0x2;

}


void AppKernel::set_start(){
    XDMA_WRITE_DATA(base_addr + ctrl_offset, bytes_from_int(0x1));
}

void AppKernel::get_ctrl_reg(){
    char *buf = (char *)malloc(4);
    XDMA_READ_WORD(base_addr + ctrl_offset, buf);
    printf("%08x\n", int_from_bytes(buf));
}


void AppKernel::wait_on_done() {

    while (!get_done()){
        printf("kernel processing...\n");
        usleep(10000);
    }
}