#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/version.h>


#define PACKET_DEBUG

#ifdef PACKET_DEBUG
#define dprint(fm1, fm2...)	printk(KERN_ALERT "%s:(%s:%d)  "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#define derror(fm1, fm2...) printk(KERN_ERR "%s:(%s:%d) "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#else
#define dprint(fm1, fm2...) {}
#define derror(fm1, fm2...) {}
#endif


int packet_init(void)
{
	dprint("\n");

	return 0;
}

void packet_exit(void)
{
	dprint("\n");
}

module_init(packet_init);
module_exit(packet_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Packet sample coding");
