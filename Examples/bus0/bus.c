#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/device.h>



/*************BUS START*********************/
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

static int my_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	
	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return 0;
}
#if 0
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
#endif 
struct bus_type my_bus_type = {
	.name 		= "my_bus",
	.match		= my_bus_match,
	.uevent 	= my_bus_uevent,
	//.probe		= my_bus_probe,
	//.remove		= my_bus_release,
};

EXPORT_SYMBOL(my_bus_type);

/*****************BUS END*****************************/

/*****************Device START************************/
static ssize_t my_dev_show(struct device *dev,struct device_attribute *attr,
	char *buf)
{

	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return 0;
}

static ssize_t my_dev_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t len)
{

	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
	return len;
}



//struct device_attribute dev_attr_read = 
//__ATTR(read, 0644, my_dev_show, my_dev_store);

DEVICE_ATTR(read, 0644, my_dev_show, my_dev_store);

static void my_bus_device_release(struct device *dev)
{
	
	printk(KERN_ALERT "Inside Fun:%s and %d-->%s\n",__FUNCTION__, __LINE__, dev_name(dev));
}
struct device my_bus_device = {
	.init_name	= "my_driver",
	.release	= my_bus_device_release,
};

EXPORT_SYMBOL(my_bus_device);
/*************Device END************************/

/*************Driver START*********************/


static ssize_t show_my_bus_driver(struct device_driver *drv, char *buf)
{

	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	return 0;
}

//struct driver_attribute driver_attr_read =
//__ATTR(read, 0644, show_my_bus_driver, NULL);

DRIVER_ATTR(read, 0644, show_my_bus_driver, NULL);

static int my_bus_driver_probe(struct device *dev)
{

	printk(KERN_ALERT "Inside Fun:%s and %d\n device:%s",__FUNCTION__, __LINE__ , dev_name(dev));
	return 0;
}

static int my_bus_driver_remove(struct device *dev)
{

	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	return 0;
}

struct device_driver my_bus_driver = {
	.name		= "my_driver",
	.bus		= &my_bus_type,
	.probe		= my_bus_driver_probe,
	.remove		= my_bus_driver_remove
};
/*************Driver END***********************/

/*************Class Start***********************/
struct class *my_class;

/************Class END*************************/

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

	my_class = class_create(THIS_MODULE, "my_class");

	if(my_class < 0) {
		err = PTR_ERR(my_class);
		goto bus_err_exit;
	}
	
	err = device_register(&my_bus_device);	
	if(err < 0)
		goto dev_reg_exit;
	
	err = device_create_file(&my_bus_device, &dev_attr_read);
	
	if(err < 0)
		goto dev_exit;

	err = driver_register(&my_bus_driver);
	if(err < 0)
		goto dev_exit;

	err = driver_create_file(&my_bus_driver, &driver_attr_read);

	if(err < 0)
		goto drv_exit;

	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	return 0;
drv_exit:
	driver_unregister(&my_bus_driver);

dev_exit:
	device_unregister(&my_bus_device);
dev_reg_exit:
	class_destroy(my_class);
bus_err_exit:
	bus_unregister(&my_bus_type);	
exit:
	return err;
}

static void my_bus_exit(void)
{
	printk(KERN_ALERT "Inside Fun:%s and %d\n",__FUNCTION__, __LINE__ );
	driver_unregister(&my_bus_driver);
	device_unregister(&my_bus_device);
	class_destroy(my_class);
	bus_unregister(&my_bus_type);

}


module_init(my_bus_init);
module_exit(my_bus_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalgi <mallesh.koujalagi@gmail.com>");

