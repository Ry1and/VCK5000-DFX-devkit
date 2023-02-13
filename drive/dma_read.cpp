

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

#include "dma_rw.c"

using namespace std;
using random_bytes_engine = independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;


uint64_t hash_kernel_0_base_addr = 0x020100010000;
uint64_t dram_base_addr_1 = 0x0;
uint64_t dram_base_addr_2 = 0x00C000000000;
uint64_t uu = 0xffffffffff;


uint64_t input_addr_offset_0 = 0x10;
uint64_t input_addr_offset_1 = 0x14;
uint64_t output_addr_offset_0 = 0x1c;
uint64_t output_addr_offset_1 = 0x20;
uint64_t ctrl_offset = 0x00;

//int dma_h2c = open("/dev/xdma0_h2c_0", O_WRONLY);
//int dma_c2h = open("/dev/xdma0_c2h_0", O_RDONLY);
string dma_h2c_dname =  "/dev/xdma0_h2c_0";
char* dma_h2c_dname_c = strcpy(new char[dma_h2c_dname.length() + 1], dma_h2c_dname.c_str());

string dma_c2h_dname =  "/dev/xdma0_c2h_0";
string dma_c2h_bypass_dname = "/dev/xdma0_bypass_c2h_0";
char* dma_c2h_dname_c = strcpy(new char[dma_c2h_dname.length() + 1], dma_c2h_dname.c_str());
char* dma_c2h_bypass_dname_c = strcpy(new char[dma_c2h_bypass_dname.length() + 1], dma_c2h_bypass_dname.c_str());


// static printHexString(char *buf){
//     for (int i = 0; i < ; i++){
//         printf("%02hhx", test_buf2[i]);
//     }
// }

// create a copy of the string that is in the form of char array
static char* string_to_char_array(string in){
    return strcpy(new char[in.length() + 1], in.c_str());
}


static int dummy_rw_test(){
    int test_size = 4;
    printf("Generating %d bytes of test data\n", test_size);
    random_bytes_engine rbe;
    vector<unsigned char> data(test_size);
    generate(begin(data), end(data), ref(rbe));
    for (char i : data){
        printf("%02hhx", i);
    }

    cout << endl;
    char *buffer = reinterpret_cast<char*> (&data[0]);
    char *test_buf = new char[4];


    // dma write
    //dma_write(dma_h2c_dname_c, dram_base_addr_1, 0, test_size, 0, 1, buffer);
    //dma_read(dma_c2h_dname_c, dram_base_addr_1, 0, test_size,0,1,test_buf);
    for (int i = 0; i < 4; i++) {
        printf("%02hhx", test_buf[i]);
    }

    cout << endl;
    uint32_t lsb = 0x2b2b2b2b3c3c3c3c & 0xffffffff;
    uint32_t msb = 0x2b2b2b2b3c3c3c3c >> 32;

    char lsb_c[sizeof(lsb)];
    char msb_c[sizeof(msb)];

    memcpy(lsb_c, &lsb, sizeof(lsb));
    memcpy(msb_c, &msb, sizeof(msb));

    for (int i = 0; i < 4; i++){
        printf("%02hhx", lsb_c[i]);
    }
    cout << endl;
    for (int i = 0; i < 4; i++){
        printf("%02hhx", msb_c[i]);
    }

    cout << endl;

    char test_buf2[4]; 
    
    //dma_read(dma_c2h_dname_c, hash_kernel_0_base_addr, 0, 1,  0,1,test_buf2);


    //dma_write(dma_h2c_dname_c, hash_kernel_0_base_addr + input_addr_offset_0, 0, 4, 0,1,lsb_c);
    //dma_read(dma_c2h_bypass_dname_c, hash_kernel_0_base_addr, 0, 1,  0,1,test_buf2);
    
    dma_write(dma_h2c_dname_c, hash_kernel_0_base_addr, 0, 4, 0,1,lsb_c);
    dma_read(dma_c2h_dname_c, hash_kernel_0_base_addr, 0, 4,  0,1,test_buf2);

    
    printf("%d", sizeof(test_buf2));

    cout << endl;
    
    for (int i = 0; i < 4; i++){
        printf("%02hhx", test_buf2[i]);
    }

    return 0;

}



int main(){
    
    //int dma_h2c_bypass = open("/dev/xdma0_bypass_h2c_0", O_WRONLY);
    //int dma_c2h_bypass = open("/dev/xdma0_bypass_c2h_0", O_RDONLY);

    dummy_rw_test();


    return 0;
}