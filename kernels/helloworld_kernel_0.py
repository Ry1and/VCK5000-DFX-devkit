from kernels.helloworld_kernel_base import HelloworldKernelBase
from functools import reduce

class HelloworldKernel0(HelloworldKernelBase):
    def __init__(self, dev, base_addr):
        super().__init__(dev, base_addr)
       

    def get_kernel_result(self, in_arr):
        res = 0
        for e in in_arr:
            res += e

        return res % pow(2, 32)