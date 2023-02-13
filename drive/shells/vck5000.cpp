#include <string>
#include <cstring>
#include <iostream>


#include "../dma_rw.c"


using namespace std;

class vck5000
{
private:
    int id = 0;
    string dma_h2c_dname =  "/dev/xdma0_h2c_0";
    string dma_c2h_dname =  "/dev/xdma0_c2h_0";
    string dma_c2h_bypass_dname = "/dev/xdma0_bypass_c2h_0";
    char* dma_h2c_dname_c = strcpy(new char[dma_h2c_dname.length() + 1], dma_h2c_dname.c_str());
    char* dma_c2h_dname_c = strcpy(new char[dma_c2h_dname.length() + 1], dma_c2h_dname.c_str());
    char* dma_c2h_bypass_dname_c = strcpy(new char[dma_c2h_bypass_dname.length() + 1], dma_c2h_bypass_dname.c_str());

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


