import os
import numpy as np
from operator import xor
from functools import reduce
from time import sleep
from kernels.helloworld_kernel_0 import helloworld_kernel_0
from shells.simple_static_shell import simple_static_shell
from kernels.DummyKernel import DummyKernel
from kernels.helloworld_kernel_1 import helloworld_kernel_1



def test_sbi_reconfig():
    # enable sbi
    dev.enable_sbi()

    # Load kernel 0
    dev.sbi_reconfigure('./dfx_binaries/rm0.pdi')
    # Wait a while just in case
    sleep(0.1)
    print("RM0 Loaded")

    # Test kernel 0
    dev.dma_write(bram_buffer_base_addr, payload_bytes)
    kernel_0.set_fetch_offset(bram_buffer_base_addr)
    kernel_0.set_size(test_size)
    kernel_0.set_start()
    kernel_0.wait_on_done()
    res = reduce(lambda x, y: a + b % pow(2, 32), payload)
    assert(kernel_0.get_result() == res)
    
    # Load kernel 1
    dev.sbi_reconfigure('./dfx_binaries/rm1.pdi')
    # Wait a while just in case
    sleep(0.1)
    
    # Test kernel 1
    dev.dma_write(bram_buffer_base_addr, payload_bytes)
    kernel_0.set_fetch_offset(bram_buffer_base_addr)
    kernel_0.set_size(test_size)
    kernel_0.set_start()
    kernel_0.wait_on_done()
    res = reduce(lambda x, y: a * b % pow(2, 32), payload)
    assert(kernel_0.get_result() == res)

def simple_kernel_test(d, k):
    test_size = 2
    print('Generating ' + str(test_size * 4) + ' bytes of test data')
    payload = os.urandom(test_size * 4)

    d.dma_write(bram_buffer_base_addr, payload)
    k.set_fetch_offset(bram_buffer_base_addr)
    k.set_size(test_size)
    k.set_start()
    k.wait_on_done()
    #res = reduce(lambda x, y: (x + y) % pow(2, 32), [payload[i:i + 4] for i in range(0, len(payload), 4)])
    #for b in [payload[i:i + 4] for i in range(0, len(payload), 4)]:
    #    print (int.from_bytes(b, "little"))
    test_res = k.get_kernel_result([int.from_bytes(payload[i:i + 4], "little") for i in range(0, len(payload), 4)])

    print(test_res)
    print(k.get_result())

    #assert(k.get_result() == test_res)

def dummy_rw_test(d):
    test_size = 1
    print('Generating ' + str(test_size * 4) + ' bytes of test data')
    payload = os.urandom(test_size * 4)
    d.dma_write(dram_base_addr_1, payload)
    sleep(0.1)
    res = d.dma_read(dram_base_addr_1, test_size * 4)
    print(int.from_bytes(payload,"little"))
    print(int.from_bytes(res,"little"))


def xor_kernel_test(d):
    test_size = 1
    payload = os.urandom(test_size * 4)
    d.dma_write(axis_base_addr, payload)
    sleep(0.1)
   # res = d.dma_read(dram_base_addr_1, test_size * 4)
   # print(int.from_bytes(payload, "little") ^ 0b01101001011011010010101101010110)
   
   # print(int.from_bytes(res))
    

def get_binary(byte_arr):
    return "{:08b}".format(int(byte_arr.hex(),16))



def get_sbi_status():
    print('SBI_CSR_MODE: ' + str(hex(dev.get_sbi_mode())))
    print('SBI_CSR_STATUS: ' + str(hex(dev.get_sbi_status())))
    print('SBI_CSR_IRQ_STATUS: ' + str(hex(dev.get_sbi_irq_status())))
    print('SBI_CSR_CTRL: ' + str(hex(dev.get_sbi_ctrl())))


if __name__ == "__main__":
    kernel_0_base_addr = 0x0201_0001_0000
    kernel_1_base_addr = 0x0201_0001_0000
    
    bram_buffer_base_addr = 0x0201_0000_0000

    axis_base_addr = 0x0201_0000_0000


    dram_base_addr_1 = 0x0
    dram_base_addr_2 = 0x00C0_0000_0000

    # Init shell
    dev = simple_static_shell(0, dram_base_addr_1, bram_buffer_base_addr)



    # Kernel interfaces are the same for this demo
    kernel_0 = helloworld_kernel_0(kernel_0_base_addr, dev)
    kernel_1 = helloworld_kernel_1(kernel_1_base_addr, dev)

    #simple_kernel_test(dev, kernel_0)
    dummy_rw_test(dev)

    xor_kernel_test(dev)

    get_sbi_status()

    
    # enable sbi
    # dev.enable_sbi()

    # print('SBI_CSR_MODE: ' + str(hex(dev.get_sbi_mode())))
    # print('SBI_CSR_STATUS: ' + str(hex(dev.get_sbi_status())))
    # print('SBI_CSR_IRQ_STATUS: ' + str(hex(dev.get_sbi_irq_status())))
    # print('SBI_CSR_CTRL: ' + str(hex(dev.get_sbi_ctrl())))

    # Load kernel 0
    #dev.sbi_reconfigure('./dfx_binaries/rm1.pdi')
    
