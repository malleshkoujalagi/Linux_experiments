#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/kernel.h>

#define DEV_NAME "hack"
struct fake_device {
	char data[100];
	struct mutex mtex;
}virtual_device;

static struct class *hack_class;

struct cdev *mydev;
dev_t dev_no;
int major;

static void dev_exit(void);

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
	mutex_lock(&virtual_device.mtex);
	return 0;
}
int dev_close (struct inode *inodep, struct file *filep)
{
	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	printk(KERN_ALERT "kobject name-->%s", inodep->i_cdev->kobj.name);
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
	//cdev_init(mydev, &fops);
	if(!mydev){
		printk(KERN_ALERT "Failled alloc cdev structure\n");	
		result = -ENOMEM;
		goto err_reg;
	}

	mydev->owner=THIS_MODULE;
	mydev->ops=&fops;
	result=cdev_add(mydev,dev_no,1);
	if(result < 0){
		printk(KERN_ALERT "Failed to add cdev\n");
		goto err_reg;
	}
	mutex_init(&virtual_device.mtex);
	
	hack_class = class_create(THIS_MODULE, "hack");
	if(!hack_class){
		printk(KERN_ALERT "failed to create clase\n");
		result = PTR_ERR(hack_class);
	}	goto err_reg;
	
	device_create(hack_class, NULL, dev_no, NULL, "hackctrl");
	
	dump_stack();		
	return result;
err_reg:
	if(mydev)	
		cdev_del(mydev);
	unregister_chrdev_region(dev_no,1);

	return result;
}

static void dev_exit(void)
{
	printk(KERN_ALERT "Inside function:%s line:%d\n",__FUNCTION__,__LINE__);
	device_destroy(hack_class, dev_no);
	class_destroy(hack_class);
	cdev_del(mydev);
	unregister_chrdev_region(dev_no,1);
	dump_stack();
}


module_init(dev_init);
module_exit(dev_exit);


MODULE_LICENSE("GPL");
