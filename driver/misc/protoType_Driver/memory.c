#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/errno.h>





static ssize_t memory_read(struct file *filep, char __user *buf, size_t count, loff_t *ppos)
{

	return 0;
}
static ssize_t memory_write(struct file *filep, const char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static int memory_open(struct inode *inode, struct file *filep)
{
	return 0;
}


static int memory_release(struct inode *inode, struct file *filep)
{

	return 0;
}

struct file_operations memory_ops={
	.open=memory_open,
	.release=memory_release,
	.read=memory_read,
	.write=memory_write,
};


static int memory_init(void)
{

	return 0;
}


static void memory_exit(void)
{


}


module_init(memory_init);
module_exit(memory_exit);

MODULE_LICENSE("GPL");




