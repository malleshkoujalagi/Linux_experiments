#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <locale.h>
#include <stdint.h>
#include <rte_cycles.h>
#include <rte_ring.h>
#include <rte_memcpy.h>


int main(int argc, char *argv[])
{
    	int socket_id;
        int ret;		
    	/* Initialize DPDK EAL*/
	ret = rte_eal_init(argc, argv);
	if(ret <  0)
		printf("EAL error \n");	
        
	socket_id = rte_lcore_to_socket_id(rte_get_master_lcore());
	if(socket_id < 0)
		printf("Socket id issue\n");

	setlocale(LC_NUMERIC, "");

	printf("Hello DPDK World \n");
	
	rte_eal_mp_wait_lcore();
	return 0;
}
