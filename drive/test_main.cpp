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

int main() {
    VCK5000 vb = VCK5000(0);
    HashKernel hk = HashKernel(base_addr, dram_base_addr_0);
    DummyAdder da = DummyAdder(base_addr, bram_base_addr_0);
    DummyMultiplier dm = DummyMultiplier(base_addr, bram_base_addr_0);

    //ddr_dummy_test(dram_base_addr_0);
    //simple_kernel_test(dm);
    

    // direct writing to CFU and NPI
    // vb.cfu_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_wire512_inst_1_partial.rcdo");
    // char *buf = (char *)malloc(4);
    // XDMA_READ(0x1012b0000, buf);
    // printf("cfu interrupt: %d", buf);

    // kernel_test(hk);
    // vb.cfu_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_keccak512_inst_0_partial.rcdo");

    //kernel_test(hk);

    // sbi reconfiguration
    //vb.reset_sbi();
    vb.enable_sbi();
    printf("sbi mode: %x", vb.get_sbi_mode());


    int accumulated_size = 0;
    for (int i = 0; i < 1; i++) {
        printf("===========================reconfiguration round %d==============================\n", i);
        printf("common buffer size: %d\n", vb.get_sbi_status() & 0b1111111111000);
        printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
        printf("PPU_RST_MODE: %d\n", vb.get_ppu_rst_mode() & 0b10000);
        //vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+groestl/compressed/design_2_i_RP_HASH_0_groestl512_inst_0_partial.pdi");
        // vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/simple_kernel/compressed/simple_kernel_i_RP0_RM0_inst_0_partial.pdi");
        vb.sbi_reconfigure("/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/uncompressed/design_1_i_RP_HASH_0_keccak512_inst_0_partial.pdi");

        printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
        int com_buf_sz = vb.get_sbi_status() & 0b1111111111000;
        printf("common buffer size: %d\n", com_buf_sz);
        usleep(100000);

        com_buf_sz = vb.get_sbi_status() & 0b1111111111000;
        printf("common buffer size: %d\n", com_buf_sz);
        printf("PPU_RST_MODE: %d\n", vb.get_ppu_rst_mode() & 0b10000);
        //accumulated_size += 559312;
      
        // printf("SBI interrupt status: 0x%x\n", vb.get_sbi_irq_status());
    }


    // reconf_test(vb, "/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_wire512_inst_1_partial.pdi");

  

    // reconf_test(vb, "/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel_wire+keccak/compressed/design_1_i_RP_HASH_0_keccak512_inst_0_partial.pdi");
    
    
    // kernel_test(hk);
    return 0;
}