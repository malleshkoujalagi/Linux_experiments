#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "memdev.h"


static int mem_major = MAJOR_NO; 
struct cdev cdev;
struct memdev *memdev_p;


static ssize_t memdev_read(struct file *filep, char __user *buf,
	 size_t count, loff_t *ppos)
{

	return count;
}


static ssize_t memdev_write(struct file *filep, const char __user *buf,
	size_t count, loff_t *ppos)
{
	return count;
}

static int memdev_open(struct inode *inodep, struct file *filep)
{

	return 0;
}

static int memdev_close(struct inode *inode, struct file *filep)
{

	return 0;
} 


static struct file_operations memdev_ops = {
	.open 		= memdev_open,
	.release	= memdev_close,
	.read		= memdev_read,
	.write		= memdev_write,
};


static int memdev_init(void)
{
	int err;
	//int i;
	dev_t dev_no;

	dev_no = MKDEV(mem_major, 0);

	if(mem_major) {
		err = register_chrdev_region(dev_no, 2, "memdev");
	}
	else {
		err = alloc_chrdev_region(&dev_no, 0, 2, "memdev");
		mem_major = MAJOR(dev_no);
	}
	
	if(err < 0) 
		return err;
	

	cdev_init(&cdev, &memdev_ops);	
	cdev.owner = THIS_MODULE;
	cdev.ops = &memdev_ops;

	cdev_add(&cdev, MKDEV(mem_major, 0), MEMDEV_NUMBER);
	
	memdev_p = kzalloc(sizeof(struct memdev)*MEMDEV_NUMBER, GFP_KERNEL);	
	
	if(memdev_p) {
		err = -ENOMEM;
		goto exit;
	}

	return 0;

exit:
	cdev_del(&cdev);
	unregister_chrdev_region(dev_no, mem_major);
	return err;
}

static void memdev_exit(void)
{
	kfree(memdev_p);
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(mem_major, 0), mem_major);
}

module_init(memdev_init);
module_exit(memdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi` <mallesh.koujalagi@gmail.com>");
