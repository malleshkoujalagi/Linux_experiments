#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/slab.h>

static struct kobject *kobject2;
static struct kset *kset;
//int x;
//int y;




static ssize_t kobject2_sysfs_show(struct kobject *kobj, struct attribute *attr,
	char *buf) 
{

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	return 0;
}

static ssize_t kobject2_sysfs_store(struct kobject *kobj, struct attribute *attr,
	const char *buf, size_t len)
{

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	return len;
}	


static struct sysfs_ops kobject2_ops = {
	.show	= kobject2_sysfs_show,
	.store	= kobject2_sysfs_store,
};

static void kobject2_release(struct kobject *kobj)
{
	kobject_put(kobj);
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
}

static struct kobj_type kobject2_type = {
	.release 	= kobject2_release, 
	.sysfs_ops 	= &kobject2_ops,
//	.default_attrs	= kobject2_attr,
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

	kobject2 = kzalloc(sizeof(*kobject2), GFP_KERNEL);
	//kobject_init_and_add(kobject2, &kobject2_type, NULL, "kobj_hack3");
	//kobject2 = kobject_create_and_add("kobj_hack2", kernel_kobj);	
	//kobject2->kset= kset;
	if(!kobject2) {
		err = -ENOMEM;
		printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
		goto exit;
	}
	
	kobject2->kset = kset;
	kobject2->ktype = &kobject2_type;
	//err = sysfs_create_and_add(kobject2, &kobject2_type);
	//if(err < 0) {
	//	goto exit;		
	//}
	
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	kobject_uevent(kobject2, KOBJ_ADD);
		
	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	return 0;

exit:
	kset_unregister(kset);
kobj_exit:
	return err;	
}

static void kobj2_exit(void)
{

	printk(KERN_ALERT "Fun:%s--%d\n", __FUNCTION__,__LINE__);
	//kobject_put(kobject2);
	kset_unregister(kset);
}


module_init(kobj2_init);
module_exit(kobj2_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
