#ifndef __MEMDEV_H
#define __MEMDEV_H


#ifndef MAJOR_NO
#define MAJOR_NO 255
#endif

#ifndef MEMDEV_NUMBER
#define MEMDEV_NUMBER 2
#endif



struct memdev {
	char *data;
	unsigned long size;

};

#endif
