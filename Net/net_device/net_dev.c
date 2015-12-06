#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/netdevice.h>



int net_dev_init(void)
{

	if(likely(1))
		printk(KERN_ALERT "---true\n");
	else
		printk(KERN_ALERT "false\n");

	if(likely(0))
		printk(KERN_ALERT "true\n");
	else
		printk(KERN_ALERT "---false\n");


	if(unlikely(1))
		printk(KERN_ALERT "----true\n");
	else
		printk(KERN_ALERT "false\n");


	if(unlikely(0))
		printk(KERN_ALERT "true\n");
	else
		printk(KERN_ALERT "---false\n");

	return 0;
}



void net_dev_exit(void)
{

}

module_init(net_dev_init);
module_exit(net_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_DESCRIPTION("Net device explore");

