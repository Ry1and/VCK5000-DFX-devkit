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



test_data = [0xc57108dd95a45d3f1fe3b5eb3cc691dedbb1450f679f2b54c291a3682ef820f65f4542c5737b440fd7afdda19ebf3735d7d98a013e75de330cd6fb12a817159d
 ,0x6710a9421a5a88b568eb0c8763dde93274d594c279203265ffd6e1a5b3affd8b26058962cdeab0269d386be65c76953992d4f8f44c68084a29ab55c207f8ec0c
,0x2ab0eb5ebf8d874dc702b63eb94e5a0ebc1189919210753217d5701e2ffc4af734d8613ac13077cff7771dbfe900cf67c2e3a47a4696fa2350e8096d793d4ed4]


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
    batch_size = 4 * 1024 * 1024
    payload = os.urandom(64 * (batch_size - len(test_data)))
    
    for i in range(len(test_data)):
        d.dma_write(dram_base_addr_1 + i * 64, test_data[i].to_bytes(64, "little"))
        print("input", i, ": ", hex(test_data[i]))
    
    d.dma_write(dram_base_addr_1 + len(test_data) * 64, payload)
    



    #k.test()
    k.set_input_offset(dram_base_addr_1)
    k.set_output_offset(dram_base_addr_1 + 0x10000000)
    print("input_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x10, 8), "little")))
    print("output_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x1c, 8), "little")))
    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    k.set_start()


    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    print("kernel processing")
    k.wait_on_done()
    print("done")
    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    
    
    
    res = d.dma_read(dram_base_addr_1 + 0x10000000, 64)
    

    for i in range(len(test_data)):
        print("output ", i, ": ", hex(int.from_bytes(d.dma_read(dram_base_addr_1 + 0x10000000 + i * 64, 64), "little")))

    #print(hex(int.from_bytes(payload,"little")))
    # print("output0: " + hex(int.from_bytes(res,"little")))
    # res = d.dma_read(dram_base_addr_1 + 0x10000000 + 64, 64)
    # print("output1: " + hex(int.from_bytes(res,"little")))
    # res = d.dma_read(dram_base_addr_1 + 0x10000000 + 128, 64)
    # print("output2: " + hex(int.from_bytes(res,"little")))
    # res = d.dma_read(dram_base_addr_1 + 0x10000000 + 192, 64)
    # print("output3: " + hex(int.from_bytes(res,"little")))
    # res = d.dma_read(dram_base_addr_1 + 0x10000000 + 256, 64)
    # print("output4: " + hex(int.from_bytes(res,"little")))

    
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
    dummy_rw_test(versal_dev)

    
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
    
