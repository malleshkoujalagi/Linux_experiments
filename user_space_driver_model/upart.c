#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV 		"/dev/uio0"
#define DEV_ADDR 	"/sys/class/uio/uio0/maps/map0/addr"
#define DEV_SIZE 	"/sys/class/uio/uio0/maps/map0/size"

char addr_buff[16],  size_buff[16];


int main()
{

	int uio_fd, uio_addr, uio_size, size;
	void *hw_addr, *la_addr;

	uio_fd = open(DEV, O_RDONLY);
	uio_addr = open(DEV_ADDR, O_RDONLY);
	uio_size = open(DEV_SIZE, O_RDONLY);

	if(uio_fd < 0 || uio_addr < 0 || uio_size < 0) {
		printf("Failed to open uio fd's %d, %d, %d \n", uio_fd, uio_addr, uio_size);
		return -1;
	}

	read(uio_addr, addr_buff, sizeof(addr_buff));
	read(uio_size, size_buff, sizeof(size_buff));

	printf("address: %s, size: %s \n", addr_buff, size_buff);
	hw_addr = (void *)strtoul(addr_buff, NULL, 0);
	size = (int)strtoul(size_buff, NULL, 0);

	la_addr = mmap(NULL, size, PROT_READ, MAP_SHARED, uio_fd, 0);

	printf("Hardware address %p, with size %d can access through mmap la address %p\n",
			hw_addr, size, la_addr);

	/*We can access hardware regitster from this point onwards*/

	return 0;
}
