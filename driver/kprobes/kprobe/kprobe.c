#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/moduleparam.h>


#define SYMBOL_STRING_LEN 128 

static char symbol_name[SYMBOL_STRING_LEN] = "sys_open";

module_param_string(symbol_name, symbol_name, sizeof(symbol_name), 0644);

#define KPROBE_DEBUG

#ifdef KPROBE_DEBUG
#define printk(fm1, fm2...)	printk(KERN_ALERT "%s:(%s:%d)  "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#define printk_e(fm1, fm2...) printk(KERN_ERR "%s:(%s:%d) "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#else
#define printk(fm1, fm2...) {}
#define printk_e(fm1, fm2...) {}
#endif

static struct kprobe kpp = {
	.symbol_name = symbol_name,
};


static int kprobe_pre_handler(struct kprobe *pkp, struct pt_regs *reg)
{
	printk("Symbol name:%s-->addr:0x%p, ip = %lx, sp = %lx, flags = 0x%lx\n",
                pkp->symbol_name, pkp->addr, reg->ip, reg->sp, reg->flags);

	dump_stack();

	return 0;
}

static void kprobe_post_handler(struct kprobe *pkp, struct pt_regs *reg, unsigned long flags)
{


}

int kprobe_init(void)
{
	int err = 0;
	printk("\n");
	kpp.pre_handler = kprobe_pre_handler;
	kpp.post_handler = kprobe_post_handler;

	
	err =  register_kprobe(&kpp);
	if (err < 0)
		printk_e("Failed to register kprobe\n");

	
	return err;
}

void kprobe_exit(void)
{
	unregister_kprobe(&kpp);
	printk("\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kprobe sample coding");
