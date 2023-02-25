#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <thread>

#include "dma_io.h"
#include "utils.h"
#include "vck5000.h"

using namespace std;


VCK5000::VCK5000(int sid)
{
    id = sid;

}

VCK5000::~VCK5000()
{

}

/*
    Private method
*/
int VCK5000::get_cfu_stream_busy(){
        char *buf = (char *)malloc(4);
        dma_read(XDMA_BYPASS, cfu_base_addr + cfu_status_offset, 0,4,0,1, buf);
        return *buf & 0x1;

    }


int VCK5000::cfu_monitor(){
    while (!get_cfu_stream_busy()) {
        printf("timer waiting cfu...");
        usleep(1);
    }

    while (get_cfu_stream_busy()) {
        printf("cfu busy...");
        usleep(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &reconf_end);

}


/*
    Public method
*/
int VCK5000::get_sbi_mode(){
    char *buf = (char *)malloc(4);
    dma_read(XDMA_C2H_0, sbi_base_addr, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::get_sbi_ctrl(){
    char *buf = (char *)malloc(4);
    dma_read(XDMA_C2H_0, sbi_base_addr + 0x4, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::get_sbi_status() {
    char *buf = (char *)malloc(4);
    dma_read(XDMA_C2H_0, sbi_base_addr + 0xc, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::get_sbi_irq_status() {
    char *buf = (char *)malloc(4);
    dma_read(XDMA_C2H_0, sbi_base_addr + 0x300, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::enable_sbi() {
    dma_write(XDMA_H2C_0, sbi_base_addr + 0x4, 0, 4, 0, 1, bytes_from_int((char *)malloc(4), 0x9), 0);

    return 0;
}

int VCK5000::sbi_reconfigure(string path) {
    uintmax_t pdi_size = filesystem::file_size(path);
    printf("Size of reconf image: %lu\n", pdi_size);
    printf("start reconfiguration");

    thread conf_timer(cfu_monitor);
    clock_gettime(CLOCK_MONOTONIC, &reconf_start);

    

    dma_write(XDMA_H2C_0, 0x102100000, 4096, pdi_size, 0, 1, string_to_char_array(path), 1);

    conf_timer.join();

    printf("reconfiguration successful");
    printf("reconf began on: %ld", reconf_start.tv_nsec);
    printf("reconf ended on: %ld", reconf_end.tv_nsec);
    printf("time taken in nano second: %ld", reconf_end.tv_nsec - reconf_start.tv_nsec);


    return 0;
}