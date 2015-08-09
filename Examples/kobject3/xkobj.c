#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/kobject.h>

struct xkobj {
	char *xname;
	struct list_head entry;
	struct xkobj *parent;
	struct kset *xkset;
	struct ktype *xktype;
	struct kernfs_node *sd;
	struct kref kref;

	unsigned int state_inititialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
};

static int xkobj_init(void)
{

	return 0;
}
static void xkobj_exit(void)
{

}

module_init(xkobj_init);
module_exit(xkobj_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi<mallesh.koujalagi@gmail.com>");
