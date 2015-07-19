#ifndef __MEMDEV_H
#define __MEMDEV_H


#ifndef MAJOR_NO
#define MAJOR_NO 	0
#endif

#define MEMDEV_NUMBER	1
#define MEMDEV_SIZE	4096 


struct memdev_t {
	char *data;
	unsigned long size;

};

#endif
