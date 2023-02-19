




/*
    XDMA description files
*/
#define XDMA_H2C_0 "/dev/xdma0_h2c_0"
#define XDMA_H2C_1 "/dev/xdma0_h2c_1"
#define XDMA_H2C_2 "/dev/xdma0_h2c_2"
#define XDMA_H2C_3 "/dev/xdma0_h2c_3"

#define XDMA_C2H_0 "/dev/xdma0_c2h_0"
#define XDMA_C2H_1 "/dev/xdma0_c2h_1"
#define XDMA_C2H_2 "/dev/xdma0_c2h_2"
#define XDMA_C2H_3 "/dev/xdma0_c2h_3"

#define XDMA_BYPASS "/dev/xdma0_bypass"




#define printhex(chars) for (int i = 0; i < sizeof(chars); i++) printf("%02hhx", chars[i]); 

static int dma_write(char *devname, uint64_t addr, uint64_t aperture,
		    uint64_t size, uint64_t offset, uint64_t count,
		    char *inbuffer);

static int dma_read(char *devname, uint64_t addr, uint64_t aperture,
			uint64_t size, uint64_t offset, uint64_t count,
			char *outbuffer);
