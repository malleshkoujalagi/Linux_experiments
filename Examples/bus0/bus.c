#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/device.h>



static ssize_t show_bus_attr(struct bus_type *bus_type,
	char *buf)
{

	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	printk(KERN_ALERT "Subsys_private p member %p \n",((struct subsys_private *)bus_type->p));
	return sprintf(buf, "%s","1.0");
}



static struct bus_attribute my_bus_attr = 
__ATTR(bus_attr, 0644, show_bus_attr, NULL);

static int my_bus_match(struct device *dev, struct device_driver *drv)
{

	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, drv->name);
	
	return !strncmp(dev_name(dev),drv->name,strlen(drv->name));
}

	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
static int my_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	
	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return 0;
}

static int my_bus_probe(struct device *dev)
{

	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return 0;
}

static int my_bus_release(struct device *dev)
{
	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return 0;
}

struct bus_type my_bus_type = {
	.name 		= "my_bus",
	.match		= my_bus_match,
	.uevent 	= my_bus_uevent,
	.probe		= my_bus_probe,
	.remove		= my_bus_release,
};


static int my_bus_init(void)
{
	int err;

	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	err = bus_register(&my_bus_type);
	if(err < 0)
		goto exit;

	err = bus_create_file(&my_bus_type, &my_bus_attr);
	if(err < 0) {
		printk("<0> Bus create file failed\n");
		goto bus_err_exit;
	}
		
	return 0;

bus_err_exit:	
	bus_unregister(&my_bus_type);
exit:
	return err;
}

static void my_bus_exit(void)
{
	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	bus_unregister(&my_bus_type);
}


module_init(my_bus_init);
module_exit(my_bus_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalgi <mallesh.koujalagi@gmail.com>");

