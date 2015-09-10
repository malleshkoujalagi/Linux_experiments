#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/types.h>



int virt_drv_init(void)
{

	return 0;
}

void virt_drv_exit(void)
{

}

module_init(virt_drv_init);
module_exit(virt_drv_exit);
MODULE_AUTHOR("Mallesh Koujalagi<mallesh.koujalagi@gmail.com>");


