#include "app_kernel.h"

class Helloworld : public AppKernel
{
private:
    /* data */
    uint64_t ret_offset;
    uint64_t size_offset;
    uint64_t in_r1_offset;
    uint64_t in_r2_offset;

public:
    Helloworld(uint64_t base_addr)
    : AppKernel(base_addr)
    {
        ret_offset = 0x10;
        size_offset = 0x24;
        in_r1_offset = 0x18;
        in_r2_offset = 0x1c;
    }
    ~Helloworld();

    void set_size(int size);
    void set_fetch_offset(uint64_t offset);

    int get_result();


    virtual int get_result(int *in_arr, int size);
    
    

};

