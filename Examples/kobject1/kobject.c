#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/slab.h>

struct kobject *my_parent;
struct kobject *my_kobj;
struct kset *my_kset;


static struct attribute my_kobj_attrs = {
	.name = "my_kobj_attr",	
	.mode = S_IRUGO | S_IWUSR,
};


static ssize_t my_kobj_sysfs_show(struct kobject *kobj, struct attribute *attr,
	char *buf)
{

	return 0;
}

static ssize_t my_kobj_sysfs_store(struct kobject *kobj, struct attribute *attr, 
	const char *buf, size_t count)
{

	return 0;
}
static struct sysfs_ops my_sysfs_ops = {
	.show  = my_kobj_sysfs_show, 
	.store = my_kobj_sysfs_store,
};
static struct kobj_type my_kobj_type = {
	.sysfs_ops = &my_sysfs_ops,
};

static int demo1_kobj_init(void)
{
	int err;

	my_parent = kobject_create_and_add("my_parent", kernel_kobj);
	
	if(!my_parent) {
		err = -ENOMEM;
		goto exit;
	}

	my_kobj = kzalloc(sizeof(*my_kobj), GFP_KERNEL);
	if(!my_kobj) {
		err = -ENOMEM;
		goto kobj_exit;	
	}

	my_kset = kset_create_and_add("my_kset", NULL, my_parent);
	if(!my_kset) {
		err = -ENOMEM;
		goto kset_exit;		
	}
		
	my_kobj->kset = my_kset;


	err = kobject_init_and_add(my_kobj, &my_kobj_type, my_parent, "my_kobj");
	if(err)
		goto kobj_init_exit;

	err = sysfs_create_file(my_kobj, &my_kobj_attrs);
	
	if(!err)
		goto kobj_init_exit;
	
	return 0;

kobj_init_exit:
	kset_unregister(my_kset);

kset_exit:
	kobject_del(my_kobj);

kobj_exit:
	kobject_del(my_parent);

exit:
	return err;
}

static void demo1_kobj_exit(void)
{
	sysfs_remove_file(my_kobj, &my_kobj_attrs);
	kset_unregister(my_kset);
	kobject_del(my_kobj);
	kobject_del(my_parent);
}


module_init(demo1_kobj_init);
module_exit(demo1_kobj_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");

