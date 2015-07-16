#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/slab.h>

struct kobject *my_parent;
struct kobject *my_kobj;
struct kset *my_kset;
static unsigned int flag = 1;

static struct attribute my_kobj_attrs = {
	.name = "my_kobj_attr",	
	.mode = S_IRUGO | S_IWUSR,
};


static ssize_t my_kobj_sysfs_show(struct kobject *kobj, struct attribute *attr,
	char *buf)
{
	size_t count = 0;
    	count += sprintf(&buf[count], "%du\n", flag);
    	return count;
}

static ssize_t my_kobj_sysfs_store(struct kobject *kobj, struct attribute *attr, 
	const char *buf, size_t count)
{

    flag = buf[0] - '0';

    switch (flag) {
    case 0:
        kobject_uevent(kobj, KOBJ_ADD);
        break;

    case 1:
        kobject_uevent(kobj, KOBJ_REMOVE);
        break;

    case 2:
        kobject_uevent(kobj, KOBJ_CHANGE);
        break;

    case 3:
        kobject_uevent(kobj, KOBJ_MOVE);
        break;

    case 4:
        kobject_uevent(kobj, KOBJ_ONLINE);
        break;

    case 5:
        kobject_uevent(kobj, KOBJ_OFFLINE);
        break;

    case 6:
        kobj->uevent_suppress = 1;
        break;

    case 7:
        kobj->uevent_suppress = 0;
        break;
    }

    return count;
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
	
	//kobject_uevent(my_kobj, KOBJ_ADD);

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

