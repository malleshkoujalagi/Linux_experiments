#include<linux/init.h>
#include<linux/module.h>
#include <linux/kallsyms.h>

void *sys_newuname = NULL;

int a=20;
int b=30;
int c;
static int m; 
int hello_init(void)
{
	sys_newuname = (void*)kallsyms_lookup_name("sys_newuname");
	if (!sys_newuname) {
     		printk(KERN_DEBUG "Could not load address of sys_newuname\n");
     		return -1;
  	}
  

	printk(KERN_INFO "Address of sys_newuname is 0x%16lX\n", (unsigned long)&hello_init);

	printk(KERN_ALERT "Hello World, My fist module epxlorer example\n");
	return 0;
}


void hello_exit(void)
{
	printk(KERN_ALERT "Good Bye world\n");

}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Mallesh K");
MODULE_LICENSE("GPL");
