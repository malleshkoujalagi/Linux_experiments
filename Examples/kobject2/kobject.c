#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>

static struct kset *kset;

struct my_kobject {
	struct kobject kobj;
	int x;
};

struct my_kobject *kobject2;

struct g_kobject2_attribute {
	struct attribute attr;
	ssize_t (*show)(struct my_kobject *kobj, struct g_kobject2_attribute *attr,
	char *buf);
	ssize_t (*store)(struct my_kobject *kobj, struct g_kobject2_attribute *attr,
	const char *buf, size_t len);
};

static ssize_t x_show(struct my_kobject *kobj, struct g_kobject2_attribute *attr,
	char *buf)
{
	return sprintf(buf, "%d\n", kobj->x);
}
static ssize_t x_store(struct my_kobject *kobj, struct g_kobject2_attribute *attr,
	const char *buf, size_t len)
{
	sscanf(buf, "%d\n", &kobj->x);
	return len; 
}

struct g_kobject2_attribute x_attr =
	__ATTR(x, 0644, x_show, x_store);

static struct attribute *kobject2_attr [] = {
	&x_attr.attr,
	NULL,
};

static ssize_t kobject2_sysfs_show(struct kobject *kobj, struct attribute *attr,
	char *buf) 
{
	struct my_kobject *kobj1; 
	struct g_kobject2_attribute *attr_temp;
	
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	kobj1 = container_of(kobj, struct my_kobject, kobj);
	attr_temp = container_of(attr, struct g_kobject2_attribute, attr);
	
	//printk(KERN_ALERT "show:%s\n",buf);
	if(!attr_temp->show)
		return -EIO;
	return	attr_temp->show(kobj1, attr_temp, buf);
}
static ssize_t kobject2_sysfs_store(struct kobject *kobj, struct attribute *attr,
	const char *buf, size_t len)
{
	struct my_kobject *kobj1; 
	struct g_kobject2_attribute *attr_temp;

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	kobj1 = container_of(kobj, struct my_kobject, kobj);
	attr_temp = container_of(attr, struct g_kobject2_attribute, attr);

	//printk(KERN_ALERT "store:%s\n",buf);
	if(!attr_temp->store)
		return -EIO;

	attr_temp->store(kobj1, attr_temp, buf, len);
	return len;
}	

static struct sysfs_ops sys_kobject2_ops = {
	.show	= kobject2_sysfs_show,
	.store	= kobject2_sysfs_store,
};

static void kobject2_release(struct kobject *kobj)
{
	struct my_kobject *temp;
	temp = container_of(kobj, struct my_kobject, kobj);
	kfree(temp);
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
}

static struct kobj_type kobject2_type = {
	.release 	= kobject2_release, 
	.sysfs_ops 	= &sys_kobject2_ops,
	.default_attrs	= kobject2_attr,
};
 
static int kobj2_init(void)
{
	int err=0;

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);

	kset = kset_create_and_add("kset_hack", NULL, kernel_kobj);

	if(!kset) {
		err = -ENOMEM;
		goto kobj_exit;
	}

	kobject2 = kzalloc(sizeof(struct my_kobject), GFP_KERNEL);
	//kobject_init_and_add(kobject2, &kobject2_type, NULL, "kobj_hack3");
	//kobject2 = kobject_create_and_add("kobj_hack2", kernel_kobj);	
	//kobject2->kset= kset;
	if(!kobject2) {
		err = -ENOMEM;
		printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
		goto exit;
	}
	
	kobject2->kobj.kset = kset;
	//kobject2->kobj.ktype = &kobject2_type;
	//kobject2->kobj.parent = kernel_kobj;
	
	err = kobject_init_and_add(&kobject2->kobj, &kobject2_type, NULL, "kobj_hack");
	//kobject2 = kobject_create_and_add("kobj_hack2", kernel_kobj);	
	//err = sysfs_create_and_add(kobject2, &kobject2_type);
	if(err < 0) {
		goto kobj_init_exit;		
	}
	
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	kobject_uevent(&kobject2->kobj, KOBJ_ADD);
		
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	return 0;

kobj_init_exit:
	kfree(kobject2);
exit:	
	kset_unregister(kset);
kobj_exit:
	return err;	
}

static void kobj2_exit(void)
{

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	kobject_put(&kobject2->kobj);
	kset_unregister(kset);
}


module_init(kobj2_init);
module_exit(kobj2_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
