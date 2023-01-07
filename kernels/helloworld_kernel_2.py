from kernels.DummyKernel import DummyKernel

class helloworld_kernel_2(DummyKernel):
    def __init__(self, base_addr, dev):
        super().__init__(base_addr, dev)
       

    def get_kernel_result(self, input):
        res = input ^ 0b01101001011011010010101101010110

        return res