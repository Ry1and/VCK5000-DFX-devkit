#ifndef __DMA_IO_H
#define __DMA_IO_H



/*
    XDMA description files
*/
#define XDMA_H2C_0 (char *)"/dev/xdma0_h2c_0"
#define XDMA_H2C_1 (char *)"/dev/xdma0_h2c_1"
#define XDMA_H2C_2 (char *)"/dev/xdma0_h2c_2"
#define XDMA_H2C_3 (char *)"/dev/xdma0_h2c_3"

#define XDMA_C2H_0 (char *)"/dev/xdma0_c2h_0"
#define XDMA_C2H_1 (char *)"/dev/xdma0_c2h_1"
#define XDMA_C2H_2 (char *)"/dev/xdma0_c2h_2"
#define XDMA_C2H_3 (char *)"/dev/xdma0_c2h_3"

#define XDMA_BYPASS (char *)"/dev/xdma0_bypass"
#define XDMA_BYPASS_H2C_0 (char *)"/dev/xdma0_bypass_h2c_0"


#define XDMA_BYPASS_C2H_0 (char *)"/dev/xdma0_bypass_c2h_0"
#define XDMA_BYPASS_C2H_1 (char *)"/dev/xdma0_bypass_c2h_1"
#define XDMA_BYPASS_C2H_2 (char *)"/dev/xdma0_bypass_c2h_2"
#define XDMA_BYPASS_C2H_3 (char *)"/dev/xdma0_bypass_c2h_3"

#define XDMA_READ(addr, buffer) dma_read(XDMA_C2H_0, addr, 0, 4, 0, 1, buffer);
#define XDMA_WRITE_DATA(addr, data) dma_write(XDMA_H2C_0, addr, 0, 4, 0, 1, data, 0);
#define XDMA_WRITE_FILE(addr, path) dma_write(XDMA_H2C_0, addr, 0, 4, 0, 1, data, 1);


int dma_write(char *devname, uint64_t addr, uint64_t aperture,
		    uint64_t size, uint64_t offset, uint64_t count,
		    char *in, int path);


int dma_read(char *devname, uint64_t addr, uint64_t aperture,
			uint64_t size, uint64_t offset, uint64_t count,
			char *outbuffer);

int dma_bypass_read(char *devname, uint64_t addr, char width, char *data);
int dma_bypass_write(char *devname, uint64_t addr, char width, char *data);

#endif