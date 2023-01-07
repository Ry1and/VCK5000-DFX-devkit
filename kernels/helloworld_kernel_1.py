from kernels.DummyKernel import DummyKernel
from functools import reduce

class helloworld_kernel_1(DummyKernel):
    def __init__(self, base_addr, dev):
        super().__init__(base_addr, dev)
       

    def get_kernel_result(self, in_arr):
        res = 1
        for e in in_arr:
            res *= e

        return res % pow(2, 32)