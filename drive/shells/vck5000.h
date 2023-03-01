#include <unistd.h>
#include <string>

class VCK5000
{
private:
    int id = 0;

    // Addresses 
    __uint64_t bypass_offset = 0x000100000000;
        
        
    // Slave Boot Interface
    __uint64_t sbi_base_addr = 0x000101220000;
    __uint64_t sbi_ctrl_offset = 0x4;

    __uint64_t sbi_status_offset = 0xc;
    __uint64_t sbi_irq_status_offset = 0x300; 
    __uint64_t sbi_stream_addr = 0x000102100000;


    // cfu
    __uint64_t cfu_base_addr = 0x0001012b0000;
    __uint64_t cfu_fgcr_offset = 0x18;
    __uint64_t cfu_status_offset = 0x100;
    

    // timer
    timespec reconf_start;
    timespec reconf_end;
    
    int get_cfu_stream_busy();
    int cfu_monitor();

public:
    VCK5000(int sid);
    ~VCK5000();


    int get_sbi_mode();
    int get_sbi_ctrl();
    int get_sbi_status();
    int get_sbi_irq_status();
    int enable_sbi();
    int sbi_reconfigure(string path);


};
