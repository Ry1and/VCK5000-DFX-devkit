import os
import sys
import struct
from mmap import mmap, PROT_READ, PROT_WRITE, MAP_SHARED
import fcntl
import numpy as np

class VCK5000:
    def __init__(self, id):
        # NOC-PMC0 OFFSET: 0x001_0000_0000 [128MB]
        # NOC-PL OFFSET: 0x201_0000_0000 [2044GB]

        self.id = id
        self.h2c = os.open('/dev/xdma' + str(id) + '_h2c_' + str(0), os.O_WRONLY)
        self.c2h = os.open('/dev/xdma' + str(id) + '_c2h_' + str(0), os.O_RDONLY)
        self.bypass_rd = os.open('/dev/xdma' + str(id) + '_bypass_c2h_0', os.O_RDONLY)
        self.bypass_wr = os.open('/dev/xdma' + str(id) + '_bypass_h2c_0', os.O_WRONLY)
        #self.bypass_fd = open('/dev/xdma' + str(id) + '_bypass', 'r+b', 0)
   
        #self.bypass = mmap(self.bypass_fd.fileno(), flags=MAP_SHARED, prot=PROT_READ | PROT_WRITE, length=128*1024*1024)

        # self.bypass = np.memmap('/dev/xdma' + str(id) + '_bypass_h2c_0', mode='r+')

        self.bypass_offset = 0x0001_0000_0000
        # Slave Boot Interface
        self.sbi_ctrl_base_addr = 0x0001_0122_0000
        self.sbi_base_addr = 0x0001_0210_0000

     


    def bypass_read(self, addr, size):
        #return self.bypass[addr: addr + size]
        # self.bypass.seek(addr)
        # return self.bypass.read(size)
        return os.pread(self.bypass_rd, size, addr)
        
    def bypass_write(self, addr, data):
        self.bypass[addr: addr + len(data)] = np.frombuffer(data, dtype=np.uint8)
        # self.bypass.seek(addr)
        # print(data)
        # print(self.bypass.write(data))
        #return os.pwrite(self.bypass_wr, data, addr)

    def dma_read(self, addr, size):
        #print(os.lseek(self.c2h, 0, os.SEEK_CUR))
        return os.pread(self.c2h, size, addr)
  

    def dma_write(self, addr, data):
        return os.pwrite(self.h2c, data, addr)

    def get_sbi_mode(self):
        #print(os.lseek(self.bypass_rd, 0, os.SEEK_CUR))
        return int.from_bytes(self.dma_read(self.sbi_ctrl_base_addr, 4), 'little')

    def get_sbi_ctrl(self):
        return int.from_bytes(self.dma_read(self.sbi_ctrl_base_addr + 0x4, 4), 'little')

    def get_sbi_status(self):
        return int.from_bytes(self.dma_read(self.sbi_ctrl_base_addr + 0xc, 4), 'little')

    def get_sbi_irq_status(self):
        return int.from_bytes(self.dma_read(self.sbi_ctrl_base_addr + 0x300, 4), 'little')

    def enable_sbi(self):
        # print("before: ", self.bypass.tell())
        # self.bypass_write(self.sbi_ctrl_base_addr - self.bypass_offset + 0x4, (0xffffffff).to_bytes(4, 'little'))
        # print("after: ", self.bypass.tell())
        
        # print("before: ", os.SEEK_CUR)
        # self.dma_write(self.sbi_ctrl_base_addr + 0x4, (0x29).to_bytes(4, 'little'))
        # print("after: ", os.SEEK_CUR)

        self.dma_write(self.sbi_ctrl_base_addr + 0x4, (0x9).to_bytes(4, 'little'))
    def sbi_reconfigure(self, pdi_path):
        with open(pdi_path, 'rb') as pdi_file:
            while seg := pdi_file.read(4 * 1024):
                #print(seg)
                self.dma_write(self.sbi_base_addr, seg)
