#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>


static spinlock_t send_lock;
static struct sk_buff_head sk_blist;
static wait_queue_head_t send_wq;
static struct net_device *net_dev;


static struct sk_buff *skb0_create_skb(struct net_device *dev)
{
	struct sk_buff *skb = NULL;
	unsigned char *msg = NULL;
	struct ethhdr *eth;

	skb = dev_alloc_skb(dev->mtu);
	if(unlikely(!skb)) {
		printk(KERN_ALERT "SKB is not allocated \n");
		goto exit_skb;
	}
	skb->protocol = __constant_htons(ETH_P_ALL);
	skb->priority = 0;
	skb->next = skb->prev = NULL;
	/*skb->csum*/
	skb->dev = dev;
	msg = skb_put(skb, dev->mtu);

	memcpy(msg, "Hi Mallesh How are you!!!!", sizeof("Hi Mallesh How are you!!!!"));

	eth = (struct ethhdr *)skb_push(skb, sizeof(struct ethhdr *));
	eth->h_proto = htons(ETH_P_ALL);
	memcpy(eth->h_dest, "595800001010", ETH_ALEN);
	memcpy(eth->h_source, skb->dev->dev_addr, ETH_ALEN);

	return skb;
exit_skb:
	return NULL;	
}

static int skb0_create_blist(struct sk_buff *skb)
{
	unsigned long flags;

	if(likely(skb)) {

		spin_lock_irqsave(&send_lock, flags);
		skb_queue_tail(&sk_blist, skb);
		spin_unlock_irqrestore(&send_lock, flags);
	}
	return 0;
}

static int skb0_walk_and_send(void)
{
	struct sk_buff *skb;

	spin_lock_irq(&send_lock);
	while((skb = skb_dequeue(&sk_blist))) {
		dev_queue_xmit(skb);
	}
	printk(KERN_ALERT "Packet sends\n");
	spin_unlock_irq(&send_lock);

	return 0;
}

int skb0_init(void)
{
	int i;

	printk(KERN_ALERT "skb0_init called\n");
	
	read_lock(&dev_base_lock);
	for_each_netdev(&init_net, net_dev) {
		if(likely(!strcmp(net_dev->name, "eth0"))) {
			printk(KERN_ALERT "net device found %s %d \n", net_dev->name, net_dev->ifindex);
			break;
		}
		
		printk(KERN_ALERT "net device found %s %d \n", net_dev->name, net_dev->ifindex);
	}

	read_unlock(&dev_base_lock);
	spin_lock_init(&send_lock);
	skb_queue_head_init(&sk_blist);
	init_waitqueue_head(&send_wq);

	for(i = 0; i < 5 ; i++) {
		struct sk_buff *skb;
		skb = skb0_create_skb(net_dev);
		skb0_create_blist(skb);
		skb0_walk_and_send();		
	}
		
	return 0;
}

void skb0_exit(void)
{
	printk(KERN_ALERT "Goodbye skb\n");

}

module_init(skb0_init);
module_exit(skb0_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Skb sample coding");
