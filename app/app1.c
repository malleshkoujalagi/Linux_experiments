#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#define DEV_NAME "/dev/hackctrl"

int main()
{
	
	int fd, i;
	char ch, wbuf[100], rbuf[100];

	fd = open(DEV_NAME, O_RDWR);
	if (fd == -1){
		printf("failed to open\n");	
		exit(-1);
	}
	printf("r:read from device and w:write to device:\n");
	scanf("%c",&ch);
	
	switch(ch){

		case 'w':
			printf("Enter data\n");
			scanf("%s",wbuf);
			write(fd, wbuf,sizeof(wbuf));
			break;
		case 'r':
			printf("Read data from driver \n");
			read(fd, rbuf,sizeof(rbuf));
			printf("read--->%s\n",rbuf);
			break;
		default:
			printf("Command not found\n");
			break;
	}
	
	close(fd);	
	return 0;
}
