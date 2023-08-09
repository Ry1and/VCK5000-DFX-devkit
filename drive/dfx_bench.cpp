#include "tests/hash_test.h"
#include "kernels/hash_kernel.h"
#include "shells/vck5000.h"
#include "tests/dma_test.h"
#include "tests/simple_kernel_test.h"
#include "kernels/adder_kernel.h"
#include "kernels/multiplier_kernel.h"
#include "address.h"
#include "xdma_io.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#include <thread>


#define IMAGE_NAME "/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/wire/uncompressed/keccak_basic_i_hash_core_wire512_inst_0_partial_2.pdi"
#define N_ROUNDS 10

using namespace std;

double diff_timespec(const struct timespec *time1, const struct timespec *time0) {
  return (time1->tv_sec - time0->tv_sec)
      + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}


int main() {
    VCK5000 vb = VCK5000(0);
    HashKernel hk = HashKernel(hash_adaptor_base_addr, dram_base_addr_0);

    struct timespec reconf_start;
    struct timespec reconf_end;

    printf("sbi ctrl: 0x%x\n", vb.get_sbi_ctrl_bypass());
    vb.enable_sbi();
    printf("sbi ctrl: 0x%x\n", vb.get_sbi_ctrl_bypass());

    io_stat stat_list[N_ROUNDS]; 

    for (int i = 0; i < N_ROUNDS; i++){
        vb.sbi_reconfigure(IMAGE_NAME, &stat_list[i]);
        usleep(50000);
        kernel_test(hk);
        usleep(50000);
    }

    long total_time = 0;
    float total_rate = 0;
    uint64_t total_size = 0;

    for(int i = 0; i < N_ROUNDS; i++) {
        total_time += stat_list[i].time;
        total_rate += stat_list[i].rate;
        total_size += stat_list[i].nbytes;
    }

    printf("===============================================================================================\n");
    printf("In %d rounds, avg size:%lu avg time taken to reconfigure: %f, avg throughput: %f\n", N_ROUNDS, total_size/N_ROUNDS, (float)total_time/(float)N_ROUNDS, total_rate/(float)N_ROUNDS);
    printf("===============================================================================================\n");
    

    return 0;
}