#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>


struct my_kobj {
	struct kobject kobj;
	int a;
	int b;
	int c;
};

static struct kset *my_kset;
static struct my_kobj *a;
static struct my_kobj *b;
static struct my_kobj *c;


struct my_kobj_attribute {

	struct attribute attr;
	ssize_t (*show)(struct my_kobj *mykobj, struct my_kobj_attribute *attr, 
	char *buf);
	ssize_t (*store)(struct my_kobj *mykobj, struct my_kobj_attribute *attr,
	const char *buf, size_t count);
};




static void my_release(struct kobject *kobj)
{
	struct my_kobj *temp;
	printk(KERN_ALERT "Inside:%s\n", __FUNCTION__);
	temp = container_of(kobj, struct my_kobj, kobj);
	kfree(temp);
}



static ssize_t my_sysfs__show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	struct my_kobj *kobj_temp;
	struct my_kobj_attribute *attr_temp;

	kobj_temp = container_of(kobj, struct my_kobj, kobj);
	attr_temp = container_of(attr, struct my_kobj_attribute, attr);
	if(!attr_temp->show)
		return -EIO;

	return attr_temp->show(kobj_temp, attr_temp, buf);
}

static ssize_t my_sysfs_store(struct kobject *kobj, struct attribute *attr, 
	const char *buf, size_t count)
{
	struct my_kobj *kobj_temp;
	struct my_kobj_attribute *attr_temp;

	kobj_temp = container_of(kobj, struct my_kobj, kobj);
	attr_temp = container_of(attr, struct my_kobj_attribute, attr);

	if(!attr_temp->store)
		return -EIO;	
	return attr_temp->store(kobj_temp, attr_temp, buf, count);
}



static struct sysfs_ops my_sysfs_ops = {
	.show = my_sysfs__show,
	.store = my_sysfs_store,
};

static ssize_t a_show(struct my_kobj *kobj, struct my_kobj_attribute *attr,
	char *buf)
{
	return sprintf(buf,"%d\n", kobj->a);
}

static ssize_t a_store(struct my_kobj *kobj, struct my_kobj_attribute *attr,
	const char *buf, size_t count)
{
	sscanf(buf, "%du", &kobj->a);
	return count;
}

static ssize_t b_show(struct my_kobj *kobj, struct my_kobj_attribute *attr,
	char *buf)
{
	int temp;
	if(strcmp(attr->attr.name, "b"))
		temp = kobj->b;
	else
		temp = kobj->c;
	
	return sprintf(buf, "%d\n", temp);
}

static ssize_t b_store(struct my_kobj *kobj, struct my_kobj_attribute *attr,
	const char *buf, size_t count)
{
	int var;
	
	sscanf(buf, "%du",&var);
	if(strcmp(attr->attr.name, "b"))
		kobj->b = var;
	else
		kobj->c = var;
	
	return count;
}

static struct my_kobj_attribute a_attr =
	__ATTR(a, 0644, a_show, a_store);

static struct my_kobj_attribute b_attr = 
	__ATTR(b, 0644, b_show, b_store);

static struct my_kobj_attribute c_attr =
	__ATTR(c, 0644, b_show, b_store);


static struct attribute *my_default_attrs [] = {
	&a_attr.attr,
	&b_attr.attr,
	&c_attr.attr,
	NULL,
};

static struct kobj_type my_kobj_type = {
	.release = my_release,
	.sysfs_ops = &my_sysfs_ops,
	.default_attrs = my_default_attrs,
};

static struct my_kobj *my_create_kobject(const char *name)
{
	struct my_kobj *temp;
	int error;

	temp = kzalloc(sizeof(*temp), GFP_KERNEL);
	if(!temp){
		error=-ENOMEM;
		return NULL;
	}

	temp->kobj.kset = my_kset;
	error = kobject_init_and_add(&temp->kobj, &my_kobj_type, NULL, name);
	if(error) {
		kobject_put(&temp->kobj);
		//kfree(temp);
		return NULL;
	}
	
	kobject_uevent(&temp->kobj, KOBJ_ADD);		
	
	return temp;
}


static void my_destroy_kobject(struct my_kobj *my_kobj)
{
	kobject_put(&my_kobj->kobj);
}
static int my_kset_init(void)
{
	int error;
	
	//my_kset = kset_create_and_add("my_kset", NULL, kernel_kobj);
	my_kset = kset_create_and_add("my_kset", NULL, kernel_kobj);	
	if(!my_kset) {
		error=-ENOMEM;
		goto exit;
	}

	a = my_create_kobject("kobj1");
	if(!a)
		goto a_exit;

	b = my_create_kobject("kobj2");
	if(!b)
		goto b_exit;

	c = my_create_kobject("kobj3");
	if(!c)
		goto c_exit;
	
	
	return 0;
c_exit:
	my_destroy_kobject(b);
b_exit:
	my_destroy_kobject(a);
a_exit:
	kset_unregister(my_kset);	

exit:
	return -EINVAL;
}


static void my_kset_exit(void)
{

	my_destroy_kobject(c);
	my_destroy_kobject(b);
	my_destroy_kobject(a);
	kset_unregister(my_kset);
}


module_init(my_kset_init);
module_exit(my_kset_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Explore on kset feature");
MODULE_AUTHOR("Mallesh Koujalgi <mallesh.koujalagi@gmail.com>");
