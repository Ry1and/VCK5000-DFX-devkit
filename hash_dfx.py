import os
from kernels.xor_kernel import xor_kernel
from shells.simple_static_shell import simple_static_shell
import numpy as np
from operator import xor
from functools import reduce
from time import sleep
import kernels
import shells
import pdb



def dummy_rw_test(d):
    test_size = 1
    print('Generating ' + str(test_size * 4) + ' bytes of test data')
    payload = os.urandom(test_size * 4)
    d.dma_write(dram_base_addr_1, payload)
    sleep(0.1)
    res = d.dma_read(dram_base_addr_1, test_size * 4)
    print(int.from_bytes(payload,"little"))
    print(int.from_bytes(res,"little"))


def kernel_test(d, k):
    test_size = 1
    payload = os.urandom(64)
    d.dma_write(dram_base_addr_1, payload)
    print("dram input: ", hex(int.from_bytes(d.dma_read(dram_base_addr_1, 64), "little")))

    #k.test()
    k.set_input_offset(dram_base_addr_1)
    k.set_output_offset(dram_base_addr_1 + 0x100)
    print("input_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x10, 8), "little")))
    print("output_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x1c, 8), "little")))
    
    k.set_start()


    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    
    k.wait_on_done()
    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    res = d.dma_read(dram_base_addr_1 + 0x100, 64)
    print(hex(int.from_bytes(payload,"little")))
    print(hex(int.from_bytes(res,"little")))

    
    # res = d.dma_read(dram_base_addr_1, test_size * 64)
    # print(int.from_bytes(payload, "little"))
   
    # print(int.from_bytes(res, "little"))
    

def get_binary(byte_arr):
    return "{:08b}".format(int(byte_arr.hex(),16))



def get_sbi_status(dev):
    print('SBI_CSR_MODE: ' + str(hex(dev.get_sbi_mode())))
    print('SBI_CSR_STATUS: ' + str(hex(dev.get_sbi_status())))
    print('SBI_CSR_IRQ_STATUS: ' + str(hex(dev.get_sbi_irq_status())))
    print('SBI_CSR_CTRL: ' + str(hex(dev.get_sbi_ctrl())))


if __name__ == "__main__":
    hash_kernel_0_base_addr = 0x0201_0001_0000

    
   
    dram_base_addr_1 = 0x0
    dram_base_addr_2 = 0x0008_0000_0000

    # Init shell
    versal_dev = simple_static_shell(0, dram_base_addr_1, 0)

    # Kernel interfaces are the same for this demo
    hash_kernel_0 = xor_kernel(versal_dev, hash_kernel_0_base_addr)



    #simple_kernel_test(dev, kernel_0)
    #dummy_rw_test(versal_dev)

    
    kernel_test(versal_dev, hash_kernel_0)

    
    
    #get_sbi_status()
    
    # enable sbi
    #dev.enable_sbi()
    #get_sbi_status()

    # print('SBI_CSR_MODE: ' + str(hex(dev.get_sbi_mode())))
    # print('SBI_CSR_STATUS: ' + str(hex(dev.get_sbi_status())))
    # print('SBI_CSR_IRQ_STATUS: ' + str(hex(dev.get_sbi_irq_status())))
    # print('SBI_CSR_CTRL: ' + str(hex(dev.get_sbi_ctrl())))

    # Load kernel 0
    #dev.sbi_reconfigure('/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/simple_kernel_i_RP0_RM1_inst_0_partial.pdi')

    #simple_kernel_test(dev, kernel_0)
    
