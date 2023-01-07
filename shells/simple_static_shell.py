from shells.VCK5000  import VCK5000

class simple_static_shell(VCK5000):
    def __init__(self, id, dram_base, bram_base):
        super().__init__(id)
        self.dram_base_addr = dram_base
        self.bram_base_addr = bram_base

