#include "tests/hash_test.h"
#include "tests/reconf_test.h"
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


using namespace std;

double diff_timespec(const struct timespec *time1, const struct timespec *time0) {
  return (time1->tv_sec - time0->tv_sec)
      + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}


int main() {
    VCK5000 vb = VCK5000(0);
    HashKernel hk = HashKernel(hash_adaptor_base_addr, dram_base_addr_0);
    DummyAdder da = DummyAdder(hash_adaptor_base_addr, bram_base_addr_0);
    DummyMultiplier dm = DummyMultiplier(hash_adaptor_base_addr, bram_base_addr_0);

    //ddr_dummy_test(dram_base_addr_0);
    //simple_kernel_test(dm);
    struct timespec reconf_start ;
    struct timespec reconf_end ;


    // direct writing to CFU and NPI
    // vb.cfu_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_wire512_inst_1_partial.rcdo");
    // char *buf = (char *)malloc(4);
    // XDMA_READ(0x1012b0000, buf);
    // printf("cfu interrupt: %d", buf);

    //kernel_test(hk);
    // vb.cfu_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_keccak512_inst_0_partial.rcdo");

    //kernel_test(hk);
    //kernel_test(hk);
    
    // transfer PDI file to 


    // sbi reconfiguration
    //vb.reset_sbi();


    printf("sbi ctrl: 0x%x\n", vb.get_sbi_ctrl_bypass());
    vb.enable_sbi();
    printf("sbi ctrl: 0x%x\n", vb.get_sbi_ctrl_bypass());

    // set dfx_controller input address
    uint32_t lsb = dram_base_addr_0 & 0xFFFFFFFF;
    uint32_t msb = dram_base_addr_0 >> 32;

    XDMA_WRITE_WORD(dfx_controller_base_addr + data_source_0_offset, bytes_from_int(lsb));
    XDMA_WRITE_WORD(dfx_controller_base_addr + data_source_1_offset, bytes_from_int(msb));

    lsb = sbi_stream_addr & 0xFFFFFFFF;
    msb = sbi_stream_addr >> 32;

    XDMA_WRITE_WORD(dfx_controller_base_addr + pmc_in_0_offset, bytes_from_int(lsb));
    XDMA_WRITE_WORD(dfx_controller_base_addr + pmc_in_1_offset, bytes_from_int(msb));


    
    dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 238683, 0, 1, "/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/wire/uncompressed/keccak_basic_i_hash_core_wire512_inst_0_partial.pdi", 1);
    //dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 8842592, 0, 1, "/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak_big/XDMA/uncompressed/keccak_basic_i_hash_core_keccak512_inst_0_partial.pdi", 1);
    
    //XDMA_WRITE_WORD(dfx_controller_base_addr + size_offset, bytes_from_int(2183392));
    XDMA_WRITE_WORD(dfx_controller_base_addr + size_offset, bytes_from_int(138914));


    XDMA_WRITE_WORD(dfx_controller_base_addr + ctrl_offset, bytes_from_int(0x1));
    
    int res;
    clock_gettime(CLOCK_MONOTONIC, &reconf_start);
    XDMA_READ_WORD(dfx_controller_base_addr + ctrl_offset, (char *)&res);
    while((res & 0x2) != 0x2) {
        //printf("dfx controller running\n");
        usleep(10);
        XDMA_READ_WORD(dfx_controller_base_addr + ctrl_offset, (char *)&res);
    }
    clock_gettime(CLOCK_MONOTONIC, &reconf_end);

    printf("dfx controller finished, time taken to reconfigure: %f\n", diff_timespec(&reconf_end, &reconf_start));
    kernel_test(hk);


    // //dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 866128, 0, 1, "/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak_big/XDMA/compressed/keccak_basic_i_hash_core_wire512_inst_0_partial.pdi", 1);
    // dma_write(XDMA_H2C_0, dram_base_addr_0, 0, 8842592, 0, 1, "/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak_big/XDMA/uncompressed/keccak_basic_i_hash_core_wire512_inst_0_partial.pdi", 1);
    
    
    // //XDMA_WRITE_WORD(dfx_controller_base_addr + size_offset, bytes_from_int(866128));
    // XDMA_WRITE_WORD(dfx_controller_base_addr + size_offset, bytes_from_int(8842592));


    // XDMA_WRITE_WORD(dfx_controller_base_addr + ctrl_offset, bytes_from_int(0x1));
    
    // clock_gettime(CLOCK_MONOTONIC, &reconf_start);
    // XDMA_READ_WORD(dfx_controller_base_addr + ctrl_offset, (char *)&res);
    // while((res & 0x2) != 0x2) {
    //     //printf("dfx controller running\n");
    //     usleep(10);
    //     XDMA_READ_WORD(dfx_controller_base_addr + ctrl_offset, (char *)&res);
    // }
    // clock_gettime(CLOCK_MONOTONIC, &reconf_end);

    // printf("dfx controller finished, time taken to reconfigure: %f\n", diff_timespec(&reconf_end, &reconf_start));
    // kernel_test(hk);

    // sleep(1);
    // printf("common buffer size: %d\n", vb.get_sbi_status() & 0b1111111111000);
    // printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    // printf("PPU_RST_MODE: %x\n", vb.get_ppu_rst_mode());
    //vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak_big/compressed/keccak_basic_i_hash_core_keccak512_inst_0_partial.pdi");


    // printf("common buffer size: %d\n", vb.get_sbi_status() & 0b1111111111000);
    
    // printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    // printf("PPU_RST_MODE: %x\n", vb.get_ppu_rst_mode());
    //kernel_test(hk);
    //printf("sbi mode: 0x%x\n", vb.get_sbi_mode());
    //printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    //vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak/uncompressed/keccak_basic_i_RP_HASH_0_keccak512_inst_0_partial.pdi");
    
    // int accumulated_size = 0;
    // for (int i = 0; i < 1; i++) {
    //     printf("===========================reconfiguration round %d==============================\n", i);
    //     printf("common buffer size: %d\n", vb.get_sbi_status() & 0b1111111111000);
    //     printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    //     printf("PPU_RST_MODE: %x\n", vb.get_ppu_rst_mode());
    //     //vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+groestl/compressed/design_2_i_RP_HASH_0_groestl512_inst_0_partial.pdi");
    //     // vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/simple_kernel/compressed/simple_kernel_i_RP0_RM0_inst_0_partial.pdi");
    //     vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/ubuntu22.04+2023.1/keccak/uncompressed/keccak_basic_i_RP_HASH_0_keccak512_inst_0_partial.pdi");

    //     printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    //     int com_buf_sz = vb.get_sbi_status() & 0b1111111111000;
    //     printf("common buffer size: %d\n", com_buf_sz);
    //     usleep(100000);

    //     com_buf_sz = vb.get_sbi_status() & 0b1111111111000;
    //     printf("common buffer size: %d\n", com_buf_sz);
    //     printf("PPU_RST_MODE: %d\n", vb.get_ppu_rst_mode() & 0b10000);
    //     accumulated_size += 559312;
      
    //     printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    // }


    // reconf_test(vb, "/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_wire512_inst_1_partial.pdi");

  

    // reconf_test(vb, "/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_keccak512_inst_0_partial.pdi");
    
    
    // kernel_test(hk);
    return 0;
}