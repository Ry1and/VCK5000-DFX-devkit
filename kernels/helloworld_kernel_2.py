from kernels.dummy_kernel import DummyKernel

class HelloworldKernel2(DummyKernel):
    def __init__(self, dev, base_addr):
        super().__init__(dev, base_addr)
       

    def get_kernel_result(self, input):
        res = input ^ 0b01101001011011010010101101010110

        return res