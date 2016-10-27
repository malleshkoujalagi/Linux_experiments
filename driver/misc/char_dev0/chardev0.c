#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/version.h>


static dev_t device_no;

static __init int my_cdev_init(void)
{
	printk(KERN_ALERT "Enter character dev\n");
	if(alloc_chrdev_region(&device_no, 0,5,"my_cdev") < 0){
		printk(KERN_ALERT "Failed to allocate character device\n");
		return -1;
	}
	printk(KERN_ALERT "Hey dude\n");
	printk(KERN_ALERT "<Major Number:Minor Number><%d:%d>\n", MAJOR(device_no), MINOR(device_no));
	return 0;
}

static __exit void my_cdev_exit(void)
{
	unregister_chrdev_region(device_no, 5);
	printk(KERN_ALERT "Bye Bye\n");

}

module_init(my_cdev_init);
module_exit(my_cdev_exit);

MODULE_AUTHOR("Mallesh Koujalagi");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Experiment on charactor device \n");
