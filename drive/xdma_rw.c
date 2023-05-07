#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>


#include <byteswap.h>
#include <ctype.h>

#include "dma_utils.c"

#include "cdev_sgdma.h"

#include "xdma_io.h"

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ltohl(x)       (x)
#define ltohs(x)       (x)
#define htoll(x)       (x)
#define htols(x)       (x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define ltohl(x)     __bswap_32(x)
#define ltohs(x)     __bswap_16(x)
#define htoll(x)     __bswap_32(x)
#define htols(x)     __bswap_16(x)
#endif



int eop_flush = 0;

int dma_write(char *devname, uint64_t addr, uint64_t aperture,
		    uint64_t size, uint64_t offset, uint64_t count,
		    char *in, int path)
{
	uint64_t i;
	ssize_t rc;
	size_t bytes_done = 0;
	size_t out_offset = 0;
	char *buffer = NULL;
	char *allocated = NULL;
	struct timespec ts_start, ts_end;
	int infile_fd = -1;
	int outfile_fd = -1;
	int fpga_fd = open(devname, O_RDWR);
	long total_time = 0;
	float result;
	float avg_time = 0;
	int underflow = 0;

	if (fpga_fd < 0) {
		fprintf(stderr, "unable to open device %s, %d.\n",
			devname, fpga_fd);
		perror("open device");
		return -EINVAL;
	}

	posix_memalign((void **)&allocated, 4096 /*alignment */ , size + 4096);
	if (!allocated) {
		fprintf(stderr, "OOM %lu.\n", size + 4096);
		rc = -ENOMEM;
		goto out;
	}
	buffer = allocated + offset;
	if (verbose)
		fprintf(stdout, "host buffer 0x%lx = %p\n",
			size + 4096, buffer); 

	if (path) {
		if (in) {
			infile_fd = open(in, O_RDONLY);
			if (infile_fd < 0) {
				fprintf(stderr, "unable to open input file %s, %d.\n",
					in, infile_fd);
				perror("open input file");
				rc = -EINVAL;
				goto out;
			} else {
				rc = read_to_buffer(in, infile_fd, buffer, size, 0);
				if (rc < 0 || rc < size)
					goto out;
			}
		}
	} else {
		// copy the input buffer to the host buffer
		memcpy(buffer, in, size);
	}


	for (i = 0; i < count; i++) {
		/* write buffer to AXI MM address using SGDMA */
		rc = clock_gettime(CLOCK_MONOTONIC, &ts_start);

		if (aperture) {
			struct xdma_aperture_ioctl io;

			io.buffer = (unsigned long)buffer;
			io.len = size;
			io.ep_addr = addr;
			io.aperture = aperture;
			io.done = 0UL;

			rc = ioctl(fpga_fd, IOCTL_XDMA_APERTURE_W, &io);
			if (rc < 0 || io.error) {
				fprintf(stdout,
					"#%ld: aperture W ioctl failed %ld,%d.\n",
					i, rc, io.error);
				goto out;
			}

			bytes_done = io.done;
		} else {
			rc = write_from_buffer(devname, fpga_fd, buffer, size,
				      	 	addr);
			if (rc < 0)
				goto out;

			bytes_done = rc;
		}

		rc = clock_gettime(CLOCK_MONOTONIC, &ts_end);

		if (bytes_done < size) {
			printf("#%ld: underflow %ld/%ld.\n",
				i, bytes_done, size);
			underflow = 1;
		}

		/* subtract the start time from the end time */
		timespec_sub(&ts_end, &ts_start);
		total_time += ts_end.tv_nsec;
		/* a bit less accurate but side-effects are accounted for */
		if (verbose)
		fprintf(stdout,
			"#%lu: CLOCK_MONOTONIC %ld.%09ld sec. write %ld bytes\n",
			i, ts_end.tv_sec, ts_end.tv_nsec, size); 
	
	}

	if (!underflow) {
		avg_time = (float)total_time/(float)count;
		result = ((float)size)*1000/avg_time;
		if (verbose)
			printf("** Avg time device %s, total time %ld nsec, avg_time = %f, size = %lu, BW = %f \n",
			devname, total_time, avg_time, size, result);
		printf("%s ** Average BW = %lu, %f\n", devname, size, result);
	}

out:
	close(fpga_fd);

	if (rc < 0)
		return rc;
	/* treat underflow as error */
	return underflow ? -EIO : 0;
}


int dma_bypass_read(char *devname, uint64_t addr, char width, char *data) {
	int fd;
	int err = 0;
	void *map;
	uint32_t read_result;
	off_t target;
	off_t pgsz, target_aligned, offset;
	/* access width */
	char access_width = 'w';
	char *device;

	pgsz = sysconf(_SC_PAGESIZE);
	offset = addr & (pgsz - 1);
	target_aligned = addr & (~(pgsz - 1));


	printf("device: %s, address: 0x%lx (0x%lx+0x%lx), access %s.\n",
		devname, addr, target_aligned, offset, "read");

	access_width = tolower(width);

	switch(access_width){
		case 'b':
			printf("byte (8-bits)\n");
			break;
		case 'h':
			printf("half word (16-bits)\n");
			break;
		case 'w':
			printf("word (32-bits)\n");
			break;
		default:
			printf("default to word (32-bits)\n");
			access_width = 'w';
			break;
	}



	if ((fd = open(devname, O_RDWR | O_SYNC)) == -1) {
		printf("character device %s opened failed: %s.\n",
			devname, strerror(errno));
		return -errno;
	}
	printf("character device %s opened.\n", devname);

	map = mmap(NULL, offset + 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
		       	target_aligned);
	if (map == (void *)-1) {
		printf("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
		err = 1;
		goto close;
	}
	printf("Memory 0x%lx mapped at address %p.\n", target_aligned, map);

	map += offset;

	switch (access_width) {
		case 'b':
			memcpy(data, map, 1);
			break;
		case 'h':
			memcpy(&read_result, map, 2);
			/* swap 16-bit endianess if host is not little-endian */
			read_result = ltohs(read_result);
			memcpy(data, &read_result, 2);
			break;
		case 'w':
			memcpy(&read_result, map, 2);
			/* swap 32-bit endianess if host is not little-endian */
			read_result = ltohl(read_result);
			memcpy(data, &read_result, 2);
			break;
		default:
			fprintf(stderr, "Illegal data type '%c'.\n",
				access_width);
			err = 1;
			goto unmap;
		}

	printf("finish reading\n");


	unmap:
		map -= offset;
		if (munmap(map, offset + 4) == -1) {
			printf("Memory 0x%lx mapped failed: %s.\n",
				target, strerror(errno));
		}
	close:
		close(fd);

		return err;
}


int dma_bypass_write(char *devname, uint64_t addr, char width, char *data) {
	int fd;
	int err = 0;
	void *map;
	uint32_t writeval;
	off_t target;
	off_t pgsz, target_aligned, offset;
	/* access width */
	char access_width = 'w';
	char *device;

	pgsz = sysconf(_SC_PAGESIZE);
	offset = addr & (pgsz - 1);
	target_aligned = addr & (~(pgsz - 1));


	printf("device: %s, address: 0x%lx (0x%lx+0x%lx), access %s.\n",
		devname, addr, target_aligned, offset, "write");

	access_width = tolower(width);

	switch(access_width){
	case 'b':
		printf("byte (8-bits)\n");
		break;
	case 'h':
		printf("half word (16-bits)\n");
		break;
	case 'w':
		printf("word (32-bits)\n");
		break;
	default:
		printf("default to word (32-bits)\n");
		access_width = 'w';
		break;
	}

	if ((fd = open(devname, O_RDWR | O_SYNC)) == -1) {
		printf("character device %s opened failed: %s.\n",
			devname, strerror(errno));
		return -errno;
	}
	printf("character device %s opened.\n", devname);

	map = mmap(NULL, offset + 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
		       	target_aligned);
	if (map == (void *)-1) {
		printf("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
		err = 1;
		goto close;
	}
	printf("Memory 0x%lx mapped at address %p.\n", target_aligned, map);

	map += offset;

	switch (access_width) {
	case 'b':
		memcpy(map, data, 1);
		break;
	case 'h':
		memcpy(&writeval, data, 2);
		/* swap 16-bit endianess if host is not little-endian */
		writeval = htols(writeval);
		memcpy(map, &writeval, 2);
		break;
	case 'w':
		memcpy(&writeval, data, 4);
		/* swap 16-bit endianess if host is not little-endian */
		writeval = htols(writeval);
		memcpy(map, &writeval, 4);
		break;
	default:
		fprintf(stderr, "Illegal data type '%c'.\n",
			access_width);
		err = 1;
		goto unmap;
	}

	printf("finish writing\n");

	unmap:
		map -= offset;
		if (munmap(map, offset + 4) == -1) {
			printf("Memory 0x%lx mapped failed: %s.\n",
				target, strerror(errno));
		}
	close:
		close(fd);

		return err;


}


int dma_read(char *devname, uint64_t addr, uint64_t aperture,
			uint64_t size, uint64_t offset, uint64_t count,
			char *outbuffer)
{
	ssize_t rc = 0;
	size_t out_offset = 0;
	size_t bytes_done = 0;
	uint64_t i;
	char *buffer = NULL;
	char *allocated = NULL;
	struct timespec ts_start, ts_end;
	int out_fd = -1;
	int fpga_fd;
	long total_time = 0;
	float result;
	float avg_time = 0;
	int underflow = 0;



	/*
	 * use O_TRUNC to indicate to the driver to flush the data up based on
	 * EOP (end-of-packet), streaming mode only
	 */
	if (eop_flush)
		fpga_fd = open(devname, O_RDWR | O_TRUNC);
	else
		fpga_fd = open(devname, O_RDWR);

	if (fpga_fd < 0) {
                fprintf(stderr, "unable to open device %s, %d.\n",
                        devname, fpga_fd);
		perror("open device");
                return -EINVAL;
        }

	/* create file to write data to */


	posix_memalign((void **)&allocated, 4096 /*alignment */ , size + 4096);
	if (!allocated) {
		fprintf(stderr, "OOM %lu.\n", size + 4096);
		rc = -ENOMEM;
		goto out;
	}

	buffer = allocated + offset;
	if (verbose)
	fprintf(stdout, "host buffer 0x%lx, %p.\n", size + 4096, buffer);

	for (i = 0; i < count; i++) {
		rc = clock_gettime(CLOCK_MONOTONIC, &ts_start);
		if (aperture) {
			struct xdma_aperture_ioctl io;

			io.buffer = (unsigned long)buffer;
			io.len = size;
			io.ep_addr = addr;
			io.aperture = aperture;
			io.done = 0UL;

			rc = ioctl(fpga_fd, IOCTL_XDMA_APERTURE_R, &io);
			if (rc < 0 || io.error) {
				fprintf(stderr,
					"#%ld: aperture R failed %ld,%d.\n",
					i, rc, io.error);
				goto out;
			}

			bytes_done = io.done;
		} else {
			rc = read_to_buffer(devname, fpga_fd, buffer, size, addr);
			if (rc < 0)
				goto out;
			bytes_done = rc;

		}
		clock_gettime(CLOCK_MONOTONIC, &ts_end);

		if (bytes_done < size) {
			fprintf(stderr, "#%ld: underflow %ld/%ld.\n",
				i, bytes_done, size);
			underflow = 1;
		}

		/* subtract the start time from the end time */
		timespec_sub(&ts_end, &ts_start);
		total_time += ts_end.tv_nsec;
		/* a bit less accurate but side-effects are accounted for */
		if (verbose)
		fprintf(stdout,
			"#%lu: CLOCK_MONOTONIC %ld.%09ld sec. read %ld/%ld bytes\n",
			i, ts_end.tv_sec, ts_end.tv_nsec, bytes_done, size);

		
        memcpy(outbuffer, buffer, bytes_done);
        
	}

	if (!underflow) {
		avg_time = (float)total_time/(float)count;
		result = ((float)size)*1000/avg_time;
		if (verbose)
			printf("** Avg time device %s, total time %ld nsec, avg_time = %f, size = %lu, BW = %f \n",
				devname, total_time, avg_time, size, result);
		printf("%s ** Average BW = %lu, %f\n", devname, size, result);
		rc = 0;
	} else if (eop_flush) {
		/* allow underflow with -e option */
		rc = 0;
	} else 
		rc = -EIO;

out:
	close(fpga_fd);
	if (out_fd >= 0)
		close(out_fd);
	free(allocated);

	return rc;
}
