#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <vector>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "dma_io.h"
#include "utils.h"


#include "shells/vck5000.h"
#include "kernels/hash_kernel.h"


using namespace std;

uint64_t hash_kernel_base_addr = 0x020100010000;
uint64_t dram_base_addr_0 = 0x0;
uint64_t dram_base_addr_1 = 0x00C000000000;
uint64_t uu = 0xffffffffff;


uint64_t input_addr_offset_0 = 0x10;
uint64_t input_addr_offset_1 = 0x14;
uint64_t output_addr_offset_0 = 0x1c;
uint64_t output_addr_offset_1 = 0x20;
uint64_t ctrl_offset = 0x00;





int ddr_dummy_test() {
    char *test_data = gen_bytes(64);
    char *res = (char *)malloc(64);
    dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 64, 0, 1, test_data, 0);
    dma_read(XDMA_C2H_0, dram_base_addr_0, 0, 64,0,1, res);
    printf("test data: ");
    printhex(test_data, 64);
    printf("\n");
    printf("data read from dram: ");
    printhex(res, 64);
    printf("\n");
    return 0;
}


static void sbi_reconfigure() {




}


static int get_done(){
    int res;
    XDMA_READ(hash_kernel_base_addr + ctrl_offset, (char *)&res);
    return (res & 0x2) == 0x2;
}



int kernel_test() {
    int test_size = 3;
    int batch_size = 4 * 1024 * 1024;
    char *test_data = gen_bytes(64 * batch_size);

    for (int k = 0; k < test_size; k++) {
        printhex((char *)(test_data + k * 64), 64);
        printf("\n");
    }

    

    dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 64 * batch_size, 0, 1, test_data, 0);

    // set input and output address
    uint32_t lsb = dram_base_addr_0 & 0xFFFFFFFF;
    uint32_t msb = dram_base_addr_0 >> 32;

    XDMA_WRITE_DATA(hash_kernel_base_addr + input_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(hash_kernel_base_addr + input_addr_offset_1, bytes_from_int(msb));

    lsb = (dram_base_addr_0 + 0x10000000) & 0xFFFFFFFF;
    msb = (dram_base_addr_0 + 0x10000000) >> 32;

    XDMA_WRITE_DATA(hash_kernel_base_addr + output_addr_offset_0, bytes_from_int(lsb));
    XDMA_WRITE_DATA(hash_kernel_base_addr + output_addr_offset_1, bytes_from_int(msb));

    char *addr = (char *)malloc(8);
    dma_read(XDMA_C2H_0, hash_kernel_base_addr + input_addr_offset_0, 0, 8, 0, 1, addr);
    printhex(addr, 8);

    dma_read(XDMA_C2H_0, hash_kernel_base_addr + output_addr_offset_0, 0, 8, 0, 1, addr);
    printhex(addr, 8);


    // test_kernel.set_input_offset(dram_base_addr_0);
    // test_kernel.set_output_offset(dram_base_addr_0 + 0x10000000);

    //test_kernel.set_start();
    XDMA_WRITE_DATA(hash_kernel_base_addr + ctrl_offset, bytes_from_int(0x1));
    
    printf("Kernel start processing data\n");

    //test_kernel.wait_on_done();
    while (!get_done()){
        printf("kernel processing...\n");
        usleep(10000);
    }

    printf("Kernel finished processing\n");
    char *out_data = (char *)malloc(64);
    
    for (int i=0; i < test_size; i++) {
        dma_read(XDMA_C2H_0, dram_base_addr_0 + 0x10000000 + i * 64, 0, 64, 0, 1, out_data);
        printhex(out_data, 64);
        printf("\n");
    }
    
    // reconfiguration test/benchmark

}





int main(){
    ddr_dummy_test();

    kernel_test();


    return 0;
}