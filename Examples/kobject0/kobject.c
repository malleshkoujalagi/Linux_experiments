#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/kernel.h>


extern struct kobject *power_kobj;

static struct kobject *my_kobj;
static int x_one;
static int y_one;
static int z_one;

static ssize_t x_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	
	return sprintf(buf, "%d\n",x_one);
}
			
static ssize_t x_store(struct kobject *kobj, struct kobj_attribute *attr,
	 const char *buf, size_t count)
{
	sscanf(buf,"%du",&x_one);	
	return count;

}

static ssize_t y_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	int c;
	if(strcmp(attr->attr.name, "y_one"))
		c = y_one;
	else
		c = z_one;
	
	return sprintf(buf, "%d\n",c);
}

static ssize_t y_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	if(strcmp(attr->attr.name,"y_one"))
		sscanf(buf, "%du", &y_one);
	else
		sscanf(buf, "%du", &z_one);

	return count;
}

static struct kobj_attribute x_attribute = 	
	__ATTR(x_one, 0666, x_show, x_store);
static struct kobj_attribute y_attribute =
	__ATTR(y_one, 0666, y_show, y_store);
static struct kobj_attribute z_attribute =
	__ATTR(z_one, 0666, y_show, y_store);

static struct attribute *attrs[] = {
	&x_attribute.attr,
	&y_attribute.attr,
	&z_attribute.attr,
	NULL,
};

static const struct attribute_group attr_grp = {
	.attrs = attrs,
};



static ssize_t my_bin_read(struct file *filep, struct kobject *kobj, struct bin_attribute *bin_attr,
			char *buf, loff_t ppos, size_t count)
{

	return count;
}

static struct bin_attribute bin_attribute = {
	.attr = { .name = "my_binary",
		  .mode = S_IRUGO,},
	.read = my_bin_read,	
};


static int my_kobj_init(void)
{

	int result;
	
	//my_kobj = kobject_create_and_add("my_kobj", NULL);
	my_kobj = kobject_create_and_add("my_kobj", kernel_kobj);
	//my_kobj = kobject_create_and_add("my_kobj", firmware_kobj);
	//my_kobj = kobject_create_and_add("my_kobj", hypervisor_kobj);
	//my_kobj = kobject_create_and_add("my_kobj", power_kobj);
	//my_kobj = kobject_create_and_add("my_kobj", mm_kobj);
	if(!my_kobj) {
		result = -ENOMEM;
		goto exit;
	}
	
	result = sysfs_create_group(my_kobj, &attr_grp);
	if(result)
		goto kset_exit;
	
	result = sysfs_create_bin_file(my_kobj, &bin_attribute);
	if(result)
		goto group_exit;

	return 0;

group_exit:
 	sysfs_remove_group(my_kobj, &attr_grp);
kset_exit:
	kobject_put(my_kobj);
exit:
	return result;
}

static void my_kobj_exit(void)
{

	//sysfs_remove_group(my_kobj, &attr_grp);
	kobject_put(my_kobj);
}



module_init(my_kobj_init);
module_exit(my_kobj_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Explore Kobject usage");
MODULE_AUTHOR("Mallesh Koujalagi mallesh.koujalagi@gmail.com");
