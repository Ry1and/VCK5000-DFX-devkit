import os
import sys
import struct
import time
from mmap import mmap, PROT_READ, PROT_WRITE, MAP_SHARED
import fcntl
import numpy as np
import subprocess
import threading

# global timer
threadlock=threading.Lock()

reconf_start = 0
reconf_end = 0

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
        self.sbi_base_addr = 0x0001_0122_0000
        self.sbi_ctrl_offset = 0x4

        self.sbi_status_offset = 0xc
        self.sbi_irq_status_offset = 0x300 
        self.sbi_stream_addr = 0x0001_0210_0000


        # cfu
        self.cfu_base_addr = 0x0001_012b_0000
        self.cfu_fgcr_offset = 0x18
        self.cfu_status_offset = 0x100

      



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
        return int.from_bytes(self.dma_read(self.sbi_base_addr, 4), 'little')

    def get_sbi_ctrl(self):
        return int.from_bytes(self.dma_read(self.sbi_base_addr + 0x4, 4), 'little')

    def get_sbi_status(self):
        return int.from_bytes(self.dma_read(self.sbi_base_addr + 0xc, 4), 'little')

    def get_sbi_irq_status(self):
        return int.from_bytes(self.dma_read(self.sbi_base_addr + 0x300, 4), 'little')

    def enable_sbi(self):
        # print("before: ", self.bypass.tell())
        # self.bypass_write(self.sbi_ctrl_base_addr - self.bypass_offset + 0x4, (0xffffffff).to_bytes(4, 'little'))
        # print("after: ", self.bypass.tell())
        
        # print("before: ", os.SEEK_CUR)
        # self.dma_write(self.sbi_ctrl_base_addr + 0x4, (0x29).to_bytes(4, 'little'))
        # print("after: ", os.SEEK_CUR)

        self.dma_write(self.sbi_base_addr + 0x4, (0x9).to_bytes(4, 'little'))

    def get_cfu_stream_busy(self):
        return int.from_bytes(self.dma_read(self.cfu_base_addr + self.cfu_status_offset, 4), "little") & 0x1 == 0x1

    def cfu_monitor(self):
        global threadlock, reconf_end
        # first wait for cfu stream to get busy
        while not self.get_cfu_stream_busy():
            print("timer thread waiting cfu...")
            time.sleep(0.0001)

        while self.get_cfu_stream_busy():
            print("Reconfiguring...")
            time.sleep(0.0001)

        threadlock.acquire()
        reconf_end = time.clock_gettime_ns(time.CLOCK_MONOTONIC)
        threadlock.release()

    def sbi_reconfigure(self, pdi_path):
        global reconf_start, reconf_end, threadlock
        pdi_size = os.path.getsize(pdi_path)
        print(pdi_size)
        print("start reconfiguration")

        reconf_start = time.clock_gettime_ns(time.CLOCK_MONOTONIC)
        conf_timer = threading.Thread(target=self.cfu_monitor)
        conf_timer.start()
        subprocess.run(["./dma-tools/dma_to_device", "-v", "-d", "/dev/xdma0_h2c_0", "-k", "4096", "-f", pdi_path, "-s" ,str(pdi_size), "-a", "0x102100000"])

        conf_timer.join()

        print("reconfiguration success")
        print("reconf began at: ", reconf_start)
        print("reconf ended at: ", reconf_end)
        print("time taken in nanosec: ", reconf_end - reconf_start)
        # with open(pdi_path, 'rb') as pdi_file:
        #     while seg := pdi_file.read(4 * 1024):
        #         #print(seg)
        #         self.dma_write(self.sbi_base_addr, seg)
