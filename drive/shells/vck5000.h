#ifndef VCK_H__
#define VCK_H__

#include <sys/time.h>
#include "../io_bench.h"

using namespace std;

class VCK5000
{
private:
    int id = 0;

    // Addresses 
    uint64_t bypass_offset = 0x000100000000;
        
        
    // Slave Boot Interface
    uint64_t sbi_base_addr = 0x000101220000;
    uint64_t sbi_mode_offset = 0x0;
    uint64_t sbi_ctrl_offset = 0x4;
    uint64_t sbi_status_offset = 0xc;
    uint64_t sbi_irq_status_offset = 0x300; 
    uint64_t sbi_stream_addr = 0x000102100000;
    uint64_t sbi_reset_addr = 0x101260324;

    // cfu
    uint64_t cfu_base_addr = 0x0001012b0000;
    uint64_t cfu_fgcr_offset = 0x18;
    uint64_t cfu_status_offset = 0x100;
    
    // PPU
    uint64_t ppu_rst_mode_addr = 0x00101110624;


    // timer
    struct timespec reconf_start ;
    struct timespec reconf_end ;

    // ddr

    
    int get_cfu_stream_busy();
    void cfu_monitor();
    int wake_up_ppu();

public:
    VCK5000(int sid);
    ~VCK5000();

    // SBI
    int get_sbi_mode();
    int get_sbi_ctrl();
    int get_sbi_ctrl_bypass();
    int get_sbi_status();
    int get_sbi_irq_status();
    int enable_sbi_bypass();
    int enable_sbi();
    int reset_sbi();
    int sbi_reconfigure(char *path, io_stat* io_st);

    // PPU
    int get_ppu_rst_mode();

    int cfu_reconfigure(char *path);

 //   int ddr_test();


};

#endif