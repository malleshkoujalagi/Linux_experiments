#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/moduleparam.h>
#include <linux/rtnetlink.h>
#include <net/rtnetlink.h>

#define vNET_DEBUG
#define DRV_NAME 	"VNET"
#define DRV_VERSION 	"1.0"

#ifdef vNET_DEBUG
#define printk(fm1, fm2...)	printk(KERN_ALERT "%s:(%s:%d)  "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#define printk_e(fm1, fm2...) printk(KERN_ERR "%s:(%s:%d) "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#else
#define printk(fm1, fm2...) {}
#define printk_e(fm1, fm2...) {}
#endif






static void vNet_setup(struct net_device *dev)
{
	ether_setup(dev);

	/*Initialize the device structure*/
	/*FIXME*/

}

static int vNet_validate(struct nlattr *tb[], struct nlattr *data[])
{
	return 0;
}


static struct rtnl_link_ops vNet_link_ops = {
	.kind		= DRV_NAME,
	.setup		= vNet_setup,
	.validate	= vNet_validate,

};

static int __init vNet_init(void)
{
	int err = 0;

	printk("\n");
	
	rtnl_lock();
	err = __rtnl_link_register(&vNet_link_ops);
	if (err < 0)	
		goto out;
	
	rtnl_unlock();
out:
	rtnl_unlock();

	return err;
}

static void __exit vNet_exit(void)
{
	printk("\n");
	__rtnl_link_unregister(&vNet_link_ops);
}

module_init(vNet_init);
module_exit(vNet_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("vNet sample coding");
