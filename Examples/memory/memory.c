#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>


static void memory_exit(void);

int major=60;
char *memory_buf;

static ssize_t memory_read(struct file *filep, char __user *buf, size_t count, loff_t *ppos)
{
	
	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
	//copy_to_user(buf, memory_buf,1);
	if(*ppos==0){
		//printk(KERN_ALERT "position%ld",*ppos);
		*ppos+=1;
		return 1;
	}else{
		return 0;
	}
}
static ssize_t memory_write(struct file *filep, const char __user *buf, size_t count, loff_t *ppos)
{
	//char *temp;
	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
	//temp=buf+count-1;
	//copy_from_user(memory_buf, temp,1);
	return 1;
}

static int memory_open(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
	return 0;
}


static int memory_release(struct inode *inode, struct file *filep)
{

	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
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
	int result=0;

	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
	result = register_chrdev(major, "memory", &memory_ops);
	if(result < 0){
		printk(KERN_ALERT "Failed to register memory driver\n");
		return	result;
	}	
	memory_buf = kmalloc(1, GFP_KERNEL);
	if(!memory_buf){
		printk(KERN_ALERT "Failed to allocate memory\n");
		result = -ENOMEM;	
		goto fail;
	}

	printk(KERN_ALERT "memory init end\n");	
	return result;
	fail:
	memory_exit();
	return result;
}


static void memory_exit(void)
{
	printk(KERN_ALERT "Inside function %s\n", __FUNCTION__ );	
	unregister_chrdev(major, "memory");
	if(memory_buf){
		kfree(memory_buf);
	}
}


module_init(memory_init);
module_exit(memory_exit);

MODULE_LICENSE("GPL");




