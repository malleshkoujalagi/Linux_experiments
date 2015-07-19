#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "memdev.h"


static int mem_major = MAJOR_NO; 
struct cdev cdev;
struct memdev_t *memdev_p;
dev_t dev_no;

static ssize_t memdev_read(struct file *filep, char __user *buf,
	 size_t count, loff_t *ppos)
{
	int ret = 0;
	int len = count;
	long int pos = *ppos;
	struct memdev_t *dev = filep->private_data;


	if( pos > MEMDEV_SIZE)
		return ret;
	if(len > (MEMDEV_SIZE - pos))
		len = MEMDEV_SIZE - pos;

	if(copy_to_user(buf, (dev->data+pos), len))
		ret = -EFAULT;
	else {
		*ppos += len;
		ret = len;
		printk(KERN_ALERT "Read %d bytes from %ld\n", len, pos);
	}
	
	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	return len;
}


static ssize_t memdev_write(struct file *filep, const char __user *buf,
	size_t count, loff_t *ppos)
{	
	int ret = 0;
	int len = count;
	long int pos = *ppos;
	struct memdev_t *dev = filep->private_data;
	
	if (pos > MEMDEV_SIZE)
		return ret;

	if(len > (MEMDEV_SIZE - pos))
		len = MEMDEV_SIZE - pos;

	if(copy_from_user((dev->data+pos), buf, len)) 
		ret = -EFAULT;
	else {
		*ppos += len;
		ret = len;
		printk(KERN_ALERT "Write %d bytes from %ld\n", len, pos);
	
	}
	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	return ret;
}


	loff_t (*llseek) (struct file *, loff_t, int);
static loff_t memdev_llseek(struct file *filep, loff_t offset, int whence)
{
	int newpos;

	switch(whence) {
		case 0:
			newpos = offset;
		break;
		case 1:
			newpos = filep->f_pos + offset;
		break;
		case 2:
			newpos = MEMDEV_SIZE - 1 + offset;
		break;
		default:
			return -EINVAL;
	}

	if((newpos < 0) || (newpos > MEMDEV_SIZE))
		return -EINVAL;

	filep->f_pos = newpos;
	return newpos;

}

static int memdev_open(struct inode *inodep, struct file *filep)
{
	int num;
	
	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	num = MINOR(inodep->i_rdev);
	filep->private_data = &memdev_p[num];
	return 0;
}

static int memdev_close(struct inode *inode, struct file *filep)
{
	
	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	return 0;
} 


static struct file_operations memdev_ops = {
	.open 		= memdev_open,
	.release	= memdev_close,
	.read		= memdev_read,
	.write		= memdev_write,
	.llseek		= memdev_llseek,
};


static int memdev_init(void)
{
	int err;
	int i;
	//dev_t dev_no;

	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	dev_no = MKDEV(mem_major, 0);

	if(mem_major) {
		err = register_chrdev_region(dev_no, MEMDEV_NUMBER, "memdev");
	}
	else {
		err = alloc_chrdev_region(&dev_no, 0, MEMDEV_NUMBER, "memdev");
		mem_major = MAJOR(dev_no);
	}
	
	if(err < 0) 
		return err;
	
	cdev_init(&cdev, &memdev_ops);	
	cdev.owner = THIS_MODULE;
	cdev.ops = &memdev_ops;

	cdev_add(&cdev, MKDEV(mem_major, 0), MEMDEV_NUMBER);

	memdev_p = kmalloc((sizeof(struct memdev_t)*MEMDEV_NUMBER), GFP_KERNEL);	
	
	if(!memdev_p) {
		err = -ENOMEM;
		printk(KERN_ALERT "Faile alloc %s--%d\n", __FUNCTION__, __LINE__);
		goto exit;
	}

	memset(memdev_p, 0x0, sizeof(struct memdev_t)*MEMDEV_NUMBER);
	for(i=0; i < MEMDEV_NUMBER; i++) {
		memdev_p[i].size = MEMDEV_SIZE;
		memdev_p[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
		if(!memdev_p[i].data) {
			err = -ENOMEM;
			printk(KERN_ALERT "Faile alloc %s--%d\n", __FUNCTION__, __LINE__);
			goto exitmem;
		}
	
		memset(memdev_p[i].data, 0x0, MEMDEV_SIZE);
	}

	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	return 0;
exitmem:
	kfree(memdev_p);

exit:
	cdev_del(&cdev);
	unregister_chrdev_region(dev_no, mem_major);
	return err;
}

static void memdev_exit(void)
{
	int i;
	
	printk(KERN_ALERT "Called %s--%d\n", __FUNCTION__, __LINE__);
	for(i=0; i < MEMDEV_NUMBER; i++) {
		if(memdev_p[i].data) {
			kfree(memdev_p[i].data);
			memdev_p[i].data=NULL;
		}
	}
	
	kfree(memdev_p);
	cdev_del(&cdev);
	unregister_chrdev_region(dev_no, MEMDEV_NUMBER);
}

module_init(memdev_init);
module_exit(memdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi` <mallesh.koujalagi@gmail.com>");
