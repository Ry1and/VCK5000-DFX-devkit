#ifndef __ADDR_H
#define __ADDR_H

#include <cstdint>


// s_axi_control
uint64_t hash_adaptor_base_addr = 0x20100010000;
uint64_t dfx_controller_base_addr = 0x201C0000000;


// offset
uint64_t input_addr_offset_0 = 0x10;
uint64_t input_addr_offset_1 = 0x14;
uint64_t output_addr_offset_0 = 0x1c;
uint64_t output_addr_offset_1 = 0x20;
uint64_t batch_size_offset = 0x28;


uint64_t dram_base_addr_0 = 0x000800000000;
uint64_t bram_base_addr_0 = 0x20100000000;

uint64_t ctrl_offset = 0x00;


// Addresses 
uint64_t bypass_offset = 0x000100000000;
    
    
// Slave Boot Interface
uint64_t sbi_base_addr = 0x000101220000;
uint64_t sbi_mode_offset = 0x0;
uint64_t sbi_ctrl_offset = 0x4;
uint64_t sbi_status_offset = 0xc;
uint64_t sbi_irq_status_offset = 0x300; 
uint64_t sbi_stream_addr = 0x000102100000;
uint64_t sbi_reset_addr = 0x101260324;

// cfu
uint64_t cfu_base_addr = 0x0001012b0000;
uint64_t cfu_fgcr_offset = 0x18;
uint64_t cfu_status_offset = 0x100;

// PPU
uint64_t ppu_rst_mode_addr = 0x00101110624;


// dfx controller
// offset
uint64_t data_source_0_offset = 0x10;
uint64_t data_source_1_offset= 0x14;
uint64_t pmc_in_0_offset = 0x1c;
uint64_t pmc_in_1_offset = 0x20;
uint64_t size_offset = 0x28;




#endif