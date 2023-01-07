# virtual class
class DummyKernel:
    def __init__(self, base_addr, dev):
        self.base_addr = base_addr
        self.dev = dev
        # Register Offsets
        self.ctrl_offset = 0x00
        self.ret_offset = 0x10
        self.size_offset = 0x24
        self.in_r1_offset = 0x18
        self.in_r2_offset = 0x1c

    def set_start(self):
        self.dev.dma_write(self.base_addr + self.ctrl_offset, int(0x1).to_bytes(4, "little"))

    def get_done(self):
        return int.from_bytes(self.dev.dma_read(self.base_addr + self.ctrl_offset, 4), 'little') & 0x2 == 0x2

    def set_size(self, size):
        self.dev.dma_write(self.base_addr + self.size_offset, size.to_bytes(4, "little"))

    def set_fetch_offset(self, offset):
        lsb = offset & 0xFFFF_FFFF
        msb = offset >> 32
        self.dev.dma_write(self.base_addr + self.in_r1_offset, lsb.to_bytes(4, "little"))
        self.dev.dma_write(self.base_addr + self.in_r2_offset, msb.to_bytes(4, "little"))

    def get_result(self):
        return int.from_bytes(self.dev.dma_read(self.base_addr + self.ret_offset, 4), 'little')

    def wait_on_done(self):
        while not self.get_done():
            pass

    # virtual method
    def get_kernel_result(self, in_arr):
        return