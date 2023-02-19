#include <string>
#include <cstring>
#include <iostream>


#include "../dma_io.h"

using namespace std;

class vck5000
{
private:
    int id = 0;

    // Addresses
    int bypass_offset = 0x000100000000;
        
        
    // Slave Boot Interface
    int sbi_base_addr = 0x000101220000;
    int sbi_ctrl_offset = 0x4;

    int sbi_status_offset = 0xc;
    int sbi_irq_status_offset = 0x300; 
    int sbi_stream_addr = 0x000102100000;


    // cfu
    int cfu_base_addr = 0x0001012b0000;
    int cfu_fgcr_offset = 0x18;
    int cfu_status_offset = 0x100;
    
    
    int get_cfu_stream_busy(){



    }

    int cfu_monitor(){
        
    }

public:
    vck5000(int sid);
    ~vck5000();


    int get_sbi_mode();
    int get_sbi_ctrl();
    int get_sbi_status();
    int get_sbi_irq_status();
    int enable_sbi();
    int sbi_reconfigure(string path);
};

vck5000::vck5000(int sid)
{
    id = sid;

}

vck5000::~vck5000()
{

}


int vck5000::get_sbi_mode(){
    dma_read(XDMA_C2H_0, sbi_base_addr, 0, 4, 0, 1, )




}