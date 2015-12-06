#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops nf_h;

static unsigned int packet_hook(const struct nf_hook_ops *ops,
		struct sk_buff *skb,
		const struct net_device *in,
		const struct net_device *out,
		int (*okfn)(struct sk_buff *))
{
	struct sock *sk = skb->sk;
	
	printk(KERN_ALERT "Packet hook function called \n");
	return NF_ACCEPT;
}

int packet_init(void)
{
	printk(KERN_ALERT "Paket processing\n");
	nf_h.hook = packet_hook;
	nf_h.hooknum = 0;
	nf_h.pf	= PF_INET;
	nf_h.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&nf_h);

	return 0;
}

void packet_exit(void)
{
	nf_unregister_hook(&nf_h);
}

module_init(packet_init);
module_exit(packet_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Packet sample coding");
