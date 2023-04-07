import os
import time
from kernels.hash_kernel import hash_kernel
from shells.simple_static_shell import simple_static_shell
import numpy as np
from operator import xor
from functools import reduce
from time import sleep
import kernels
import shells
import pdb



test_data = [0x54686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f672e2054686520717569636b2062726f776e20666f78,
            0x6710a9421a5a88b568eb0c8763dde93274d594c279203265ffd6e1a5b3affd8b26058962cdeab0269d386be65c76953992d4f8f44c68084a29ab55c207f8ec0c,
            0x2ab0eb5ebf8d874dc702b63eb94e5a0ebc1189919210753217d5701e2ffc4af734d8613ac13077cff7771dbfe900cf67c2e3a47a4696fa2350e8096d793d4ed4]

# ground truth for different kernels
keccak_gt = [0xc2ccf276c558eb625b3eeab33a813bf5a888bd49d49966773a5a208e36d54cd834c3dc488d8580038736d4cf5bbf44fd88627629f42accde954c9081641ec18d,
             0x0,
             0x0]

groestl_gt = [0xa66d7e14a882b406bbb2329e38b25eeab82759a1f3cc84d073a6e02fc4664c005b0149a9a59eaccfe445f79cbf676f47c956d8b2f2a12d8c9aac632763855cce,
              0x0,
              0x0]

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
    batch_size = 32
    payload = os.urandom(64 * (batch_size - len(test_data)))
    
    for i in range(len(test_data)):
        d.dma_write(dram_base_addr_1 + i * 64, test_data[i].to_bytes(64, "little"))
        print("input", i, ": ", hex(test_data[i]))
    
    d.dma_write(dram_base_addr_1 + len(test_data) * 64, payload)
    
    for i in range(len(test_data)):
        print("input", i, "in ddr: ", hex(int.from_bytes(d.dma_read(dram_base_addr_1 + i * 64, 64), "little")))

    #k.test()
    k.set_batch_size(batch_size)
    k.set_input_offset(dram_base_addr_1)
    k.set_output_offset(dram_base_addr_1 + 0x10000000)
    print("input_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x10, 8), "little")))
    print("output_address: ", hex(int.from_bytes(d.dma_read(hash_kernel_0_base_addr + 0x1c, 8), "little")))
    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    k.set_start()
    print("kernel processing")

    #time.sleep(1)
    #print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    
    #print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    
    k.wait_on_done()
    print("done")
    print("axi_control_register: ", bin(int.from_bytes(d.dma_read(hash_kernel_0_base_addr, 4), "little")))
    
    
    
    res = d.dma_read(dram_base_addr_1 + 0x10000000, 64)
    

    for i in range(len(test_data) + 33):
        print("output ", i, ": ", hex(int.from_bytes(d.dma_read(dram_base_addr_1 + 0x10000000 + i * 64, 64), "little")))

    
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
    hash_kernel_0_base_addr = 0x0201_0000_0000

    
   
    dram_base_addr_1 = 0x0008_0000_0000

    # Init shell
    versal_dev = simple_static_shell(0, dram_base_addr_1, 0)

    # Kernel interfaces are the same for this demo
    hash_kernel_0 = hash_kernel(versal_dev, hash_kernel_0_base_addr)



    #simple_kernel_test(dev, kernel_0)
    dummy_rw_test(versal_dev)

    
    kernel_test(versal_dev, hash_kernel_0)

    
    
    #get_sbi_status()
    
    # enable sbi
    #versal_dev.enable_sbi()
    #get_sbi_status(versal_dev)

    # Load kernel 0
    #versal_dev.sbi_reconfigure('/home/neutronmgr/backup/dfx_binaries/wsl-ubuntu22.04+2022.2/hash_kernel/design_1_i_RP_HASH_0_keccak512_inst_0_partial.pdi')

    # 

    #kernel_test(versal_dev, hash_kernel_0)

    #simple_kernel_test(dev, kernel_0)
    
