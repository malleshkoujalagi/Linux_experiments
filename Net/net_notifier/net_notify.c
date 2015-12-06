#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>

int net_dev_event(struct notifier_block *nb,
	unsigned long action, void *data)
{
	printk(KERN_ALERT "Net dev event\n");
	
	return 0;

}

struct notifier_block net_notifier = {
	.notifier_call = net_dev_event	
};

int net_notify_init(void) 
{
	int err;

	printk(KERN_ALERT "Net device notifier usage \n");
	err = register_netdevice_notifier(&net_notifier);
	if(err < 0)
		goto err_net_notifier;

	return 0;

err_net_notifier:
	return err;
}



void net_notify_exit(void)
{
	unregister_netdevice_notifier(&net_notifier);
}


module_init(net_notify_init);
module_exit(net_notify_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Network notification sample driver");

