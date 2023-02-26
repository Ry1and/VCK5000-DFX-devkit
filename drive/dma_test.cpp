

#include <string>
#include <cstring>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <vector>


#include <sys/stat.h>
#include <fcntl.h>

#include "dma_io.h"
#include "utils.h"


using namespace std;
using random_bytes_engine = independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;


#define string_to_char_array(in) strcpy(new char[in.length() + 1], in.c_str())

uint64_t hash_kernel_0_base_addr = 0x020100010000;
uint64_t dram_base_addr_1 = 0x0;
uint64_t dram_base_addr_2 = 0x00C000000000;
uint64_t uu = 0xffffffffff;


uint64_t input_addr_offset_0 = 0x10;
uint64_t input_addr_offset_1 = 0x14;
uint64_t output_addr_offset_0 = 0x1c;
uint64_t output_addr_offset_1 = 0x20;
uint64_t ctrl_offset = 0x00;

// simple dfx
uint64_t kernel_0_base_addr = 0x020100010000;
uint64_t kernel_1_base_addr = 0x020100010000;
uint64_t ret_offset = 0x10;
uint64_t size_offset = 0x24;
uint64_t in_r1_offset = 0x18;
uint64_t in_r2_offset = 0x1c;

// static printHexString(char *buf){
//     for (int i = 0; i < ; i++){
//         printf("%02hhx", test_buf2[i]);
//     }
// }

// create a copy of the string that is in the form of char array
// char* string_to_char_array(string in){
//     return strcpy(new char[in.length() + 1], in.c_str());
// }


int dummy_rw_test(){
    // int test_size = 4;
    // printf("Generating %d bytes of test data\n", test_size);


    // char *test_buf = new char[4];


    // // dma write
    // //dma_write(dma_h2c_dname_c, dram_base_addr_1, 0, test_size, 0, 1, buffer);
    // //dma_read(dma_c2h_dname_c, dram_base_addr_1, 0, test_size,0,1,test_buf);
    // for (int i = 0; i < 4; i++) {
    //     printf("%02hhx", test_buf[i]);
    // }

    

    // cout << endl;
    // uint32_t lsb = 0x2b2b2b2b3c3c3c3c & 0xffffffff;
    // uint32_t msb = 0x2b2b2b2b3c3c3c3c >> 32;

    // char lsb_c[sizeof(lsb)];
    // char msb_c[sizeof(msb)];

    // memcpy(lsb_c, &lsb, sizeof(lsb));
    // memcpy(msb_c, &msb, sizeof(msb));

    // for (int i = 0; i < 4; i++){
    //     printf("%02hhx", lsb_c[i]);
    // }
    // cout << endl;
    // for (int i = 0; i < 4; i++){
    //     printf("%02hhx", msb_c[i]);
    // }

    // cout << endl;

    char *dma_buf = (char *)malloc(4);
    char *bypass_buf = (char *)malloc(4);


    //dma_read(dma_c2h_dname_c, hash_kernel_0_base_addr, 0, 1,  0,1,test_buf2);


    //dma_write(dma_h2c_dname_c, hash_kernel_0_base_addr + input_addr_offset_0, 0, 4, 0,1,lsb_c);
    //dma_read(dma_c2h_bypass_dname_c, hash_kernel_0_base_addr, 0, 1,  0,1,test_buf2);
    
    //dma_write(XDMA_H2C_0, hash_kernel_0_base_addr, 0, 4, 0, 1, lsb_c);
    
    
    dma_read(XDMA_C2H_0, 0x0001012b0100, 0, 4, 0, 1, dma_buf);
    dma_bypass_read(XDMA_BYPASS, 0x100, 'w', bypass_buf);

    
    printf("%08x\n", int_from_bytes(dma_buf));
    printf("%08x\n", int_from_bytes(bypass_buf));

    // cout << endl;
    
    // for (int i = 0; i < 4; i++){
    //     printf("%02hhx", test_buf2[i]);
    // }

    return 0;

}



int main(){
    
    //int dma_h2c_bypass = open("/dev/xdma0_bypass_h2c_0", O_WRONLY);
    //int dma_c2h_bypass = open("/dev/xdma0_bypass_c2h_0", O_RDONLY);

    dummy_rw_test();


    return 0;
}