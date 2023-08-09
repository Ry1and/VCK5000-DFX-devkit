#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <sys/stat.h>
#include <pthread.h>


#include "../xdma_io.h"
#include "../utils.h"
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
        dma_bypass_read(XDMA_BYPASS, cfu_base_addr + cfu_status_offset, 'w', buf);
        //dma_read(XDMA_BYPASS, cfu_base_addr + cfu_status_offset, 0,4,0,1, buf);
        return *buf & 0x1;

    }


void VCK5000::cfu_monitor(){
    while (!this->get_cfu_stream_busy()) {
        printf("timer waiting cfu...");
        usleep(1);
    }

    while (this->get_cfu_stream_busy()) {
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
    XDMA_BYPASS_READ_WORD(sbi_base_addr + sbi_mode_offset - bypass_offset, buf);
    //dma_read(XDMA_C2H_0, sbi_base_addr, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::get_sbi_ctrl(){
    char *buf = (char *)malloc(4);
    //XDMA_BYPASS_READ_WORD(sbi_base_addr + 0x4 - bypass_offset, buf);
    dma_read(XDMA_C2H_0, sbi_base_addr + 0x4, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}


int VCK5000::get_sbi_ctrl_bypass(){
    char *buf = (char *)malloc(4);
    XDMA_BYPASS_READ_WORD(sbi_base_addr + 0x4 - bypass_offset, buf);
    //dma_read(XDMA_C2H_0, sbi_base_addr + 0x4, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}


int VCK5000::get_sbi_status() {
    char *buf = (char *)malloc(4);
    XDMA_BYPASS_READ_WORD(sbi_base_addr + 0xc - bypass_offset, buf);
    //dma_read(XDMA_C2H_0, sbi_base_addr + 0xc, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::get_sbi_irq_status() {
    char *buf = (char *)malloc(4);
    XDMA_BYPASS_READ_WORD(sbi_base_addr + 0x300 - bypass_offset, buf);
    //dma_read(XDMA_C2H_0, sbi_base_addr + 0x300, 0, 4, 0, 1, buf);
    return int_from_bytes(buf);

}

int VCK5000::enable_sbi_bypass() {
    //dma_write(XDMA_H2C_0, sbi_base_addr + 0x4, 0, 4, 0, 1, bytes_from_int(0x29), 0);
    //XDMA_BYPASS_WRITE_WORD(sbi_base_addr + sbi_mode_offset - bypass_offset, bytes_from_int(0x0));
    XDMA_BYPASS_WRITE_WORD(sbi_base_addr + 0x4 - bypass_offset, bytes_from_int(0x9));
    return 0;
}

int VCK5000::enable_sbi() {
    dma_write(XDMA_H2C_0, sbi_base_addr + 0x4, 0, 4, 0, 1, bytes_from_int(0x9), 0);
    return 0;
}

int VCK5000::reset_sbi() {
    dma_bypass_write(XDMA_BYPASS, sbi_reset_addr, 'b', bytes_from_int(0x1));
    printf("sbi reset finished");
    return 0;
}


int VCK5000::get_ppu_rst_mode() {
    char *buf = (char *)malloc(4);
    XDMA_BYPASS_READ_WORD(ppu_rst_mode_addr - bypass_offset, buf);
    return int_from_bytes(buf);
}


int VCK5000::wake_up_ppu() {
    XDMA_BYPASS_WRITE_WORD(sbi_base_addr + 0x4 - bypass_offset, bytes_from_int(0x9));
}

int VCK5000::sbi_reconfigure(char *path, io_stat* io_st) {
    struct stat st;
    long pdi_size = 0;
    if (!stat(path, &st)){
        pdi_size = st.st_size;
    } else {
        perror("stat");
    }
    printf("Size of reconf image: %ld\n", pdi_size);
    printf("start reconfiguration\n");
    //thread conf_timer(&VCK5000::cfu_monitor, this);



    if(dma_write_stat(XDMA_H2C_0, sbi_stream_addr, 65536, pdi_size, 0, 1, path, 1, io_st)) {
        printf("reconfiguration unsuccessful\n");
    } else {
        printf("reconfiguration successful\n");
    }

    // for (int ptr = 0; ptr < pdi_size; ptr++) {
    //     dma_write(XDMA_H2C_0, sbi_stream_addr + ptr % 4096, 0, pdi_size, 0, 1, path, 1)
    // }

    return 0;
}

// int VCK5000::cfu_reconfigure(char *path) {
//     struct stat st;
//     long rcdo_size = 0;
//     if (!stat(path, &st)){
//         rcdo_size = st.st_size;
//     } else {
//         perror("stat");
//     }
//     printf("Size of reconf image: %ld\n", rcdo_size);
//     printf("start reconfiguration\n");

//     if(dma_write(XDMA_H2C_0, 0x1012C0000, 4096, rcdo_size, 0, 1, path, 1)) {
//         printf("reconfiguration unsuccessful\n");
//     } else {
//         printf("reconfiguration successful\n");
//     }

    

//     return 0;
// }
