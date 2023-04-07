#include "tests/hash_test.h"
#include "tests/reconf_test.h"
#include "kernels/hash_kernel.h"
#include "shells/vck5000.h"



int main() {
    VCK5000 vb = VCK5000(0);
    HashKernel hk = HashKernel();
    //ddr_dummy_test();
    //kernel_test(hk);
    reconf_test(vb);
    //kernel_test(hk);
}