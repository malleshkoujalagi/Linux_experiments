#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>




static int __init null_init(void)
{
	printk(KERN_DEBUG "Null init called\n");
	*(int *) 0 = 0; /*Null pointer reference */
	

	return 0;
}

static void __exit null_exit(void)
{


}

module_init(null_init);
module_exit(null_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Null pointer ref sample coding");
