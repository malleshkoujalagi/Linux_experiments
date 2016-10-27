#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#define KPART_DEFINE
#ifdef KPART_DEFINE
#define printk(fm1, fm2...) printk(KERN_ALERT "%s:(%s:%d) "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#endif


static struct platform_device *uio_device;
static struct uio_info kuio_info = {
	.name = "kuio",
	.version = "1.0",
	.irq = UIO_IRQ_NONE,
};

static int drv_kuio_probe(struct device *dev)
{
	printk("\n");
	kuio_info.mem[0].addr = (unsigned long )kmalloc(512, GFP_KERNEL);
	if(!kuio_info.mem[0].addr) {
		printk("Failled to alloc memory\n");
		return -ENOMEM;
	}
	kuio_info.mem[0].memtype = UIO_MEM_LOGICAL;
	kuio_info.mem[0].size = 512;
	if(uio_register_device(dev, (&kuio_info)))
		return -ENODEV;

	return 0;
}


static int drv_kuio_remove(struct device *dev)
{
	printk("\n");
	uio_unregister_device(&kuio_info);
	return 0;
}

static struct device_driver kuio_driver = {
	.name = "kuio",
	.bus = &platform_bus_type,
	.probe = drv_kuio_probe,
	.remove = drv_kuio_remove,
};





static int kpart_init(void)
{
	printk("\n");

	uio_device = platform_device_register_simple("kuio", -1, NULL, 0);

	if(IS_ERR(uio_device)) {
		printk("UIO device register failed\n");
		return PTR_ERR(uio_device);
	}

	return driver_register(&kuio_driver);

}

static void kpart_exit(void)
{
	printk("\n");
	driver_unregister(&kuio_driver);
	platform_device_unregister(uio_device);
}


module_init(kpart_init);
module_exit(kpart_exit);
MODULE_AUTHOR("mallesh.koujalagi@gmail.com");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel part");
