#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>

#define DEV_NAME "hack"
struct fake_device {
	char data[100];
	struct mutex mtex;
}virtual_device;


struct cdev *mydev;
dev_t dev_no;
int major;


ssize_t dev_read (struct file *filep, char __user *buf, size_t len, loff_t *ppos)
{
	int ret;

	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	printk(KERN_ALERT "data:%s\n",virtual_device.data);		
	ret = copy_to_user(buf,virtual_device.data, len);
	return ret;
}
ssize_t dev_write (struct file *filep, const char __user *buf, size_t len, loff_t *ppos)
{
	int ret;

	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	printk(KERN_ALERT "buf:%s\n",buf);		
	ret=copy_from_user(virtual_device.data, buf, len);
	return ret;
}


int dev_open(struct inode *inodep, struct file *filep)
{

	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	printk(KERN_ALERT "Kernel object:%s\n",inodep->i_cdev->kobj.name);
	printk(KERN_ALERT "i_rdev:%d\n",inodep->i_rdev);
	mutex_lock(&virtual_device.mtex);
	return 0;
}
int dev_close (struct inode *inodep, struct file *filep)
{
	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	mutex_unlock(&virtual_device.mtex);
	return 0;
}


struct file_operations fops={
	.owner   = THIS_MODULE,
	.open 	 = dev_open,
	.release = dev_close,
	.read 	 = dev_read,
	.write 	 = dev_write,
};


static int dev_init(void)
{
	int result;
	
	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	result = alloc_chrdev_region(&dev_no,0,1,DEV_NAME);
	if(result < 0){
		printk(KERN_ALERT "Failed to allocate chardev\n");	
		return result;	
	}
	major=MAJOR(dev_no);
	printk(KERN_ALERT "major number%d\n",major);

	mydev = cdev_alloc();
	if(!mydev){
		printk(KERN_ALERT "Failled alloc cdev structure\n");	
		return -ENOMEM;
	}

	mydev->owner=THIS_MODULE;
	mydev->ops=&fops;
	result=cdev_add(mydev,dev_no,1);
	if(result < 0){
		printk(KERN_ALERT "Failed to add cdev\n");
		return result;
	}
	mutex_init(&virtual_device.mtex);
	
	return 0;
}

static void dev_exit(void)
{
	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	cdev_del(mydev);
	unregister_chrdev_region(dev_no,1);

}


module_init(dev_init);
module_exit(dev_exit);


MODULE_LICENSE("GPL");
