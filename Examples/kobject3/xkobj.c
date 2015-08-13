#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/kobject.h>

struct xkobject {
	/* To add as kobject**/
	struct kobject kobj;

	char *xname;
	struct list_head entry;
	struct xkobject *parent;
	struct xkset *xkset;
	struct xktype *xktype;
	struct kernfs_node *sd;
	struct kref kref;

	unsigned int state_inititialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
};


struct xkset {
	struct list_head list;
	spinlock_t list_lock;
	struct xkobject xkobj;
	const struct kset_uevent_ops *uevent_ops;
};

struct xkobj_type {
	void (*release)(struct xkobject *xkobj);
	const struct sysfs_ops *sysfs_ops;
	struct attribute **default_attrs;
	const void *(*namespace)(struct xkobject *xkobj);
};

struct xkobj_attribute {
	struct attribute attr;
	ssize_t (*show)(struct xkobject *, struct xkobj_attribute *, char *);
	ssize_t (*store)(struct xkoject *, struct xkobj_attribute *, const char *, size_t );
};	

static void xxx_release(struct xkobject *xkobj)
{

}

static ssize_t xxx_show(struct kobject *kobj, struct attribute *attr, char *buf)
{

	return 0;
}

static ssize_t xxx_store(struct kobject *kobj, struct attribute *attr, 
	const char *buf, size_t count)
{

	return count;
}
const struct sysfs_ops xxx_sysfs_ops = {
	.show 	= xxx_show,
	.store	= xxx_store,
};

struct xkobj_type x_type = {
	.release 	= xxx_release,
	.sysfs_ops	= &xxx_sysfs_ops,
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
