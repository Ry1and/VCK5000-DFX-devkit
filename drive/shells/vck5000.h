#ifndef VCK_H__
#define VCK_H__

using namespace std;

class VCK5000
{
private:
    int id = 0;

    // Addresses 
    uint64_t bypass_offset = 0x000100000000;
        
        
    // Slave Boot Interface
    uint64_t sbi_base_addr = 0x000101220000;
    uint64_t sbi_ctrl_offset = 0x4;

    uint64_t sbi_status_offset = 0xc;
    uint64_t sbi_irq_status_offset = 0x300; 
    uint64_t sbi_stream_addr = 0x000102100000;


    // cfu
    uint64_t cfu_base_addr = 0x0001012b0000;
    uint64_t cfu_fgcr_offset = 0x18;
    uint64_t cfu_status_offset = 0x100;
    

    // timer
    timespec reconf_start;
    timespec reconf_end;

    // ddr

    
    int get_cfu_stream_busy();
    void cfu_monitor();

public:
    VCK5000(int sid);
    ~VCK5000();


    int get_sbi_mode();
    int get_sbi_ctrl();
    int get_sbi_status();
    int get_sbi_irq_status();
    int enable_sbi();
    int sbi_reconfigure(char *path);
 //   int ddr_test();


};

#endif