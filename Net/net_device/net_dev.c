#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/rtnetlink.h>

struct net_device *g_net_dev_ptr;

static int net_dev_ops_open(struct net_device *dev)
{

	return 0;
}

static int net_dev_ops_stop(struct net_device *dev)
{

	return 0;
}

static netdev_tx_t net_dev_ops_start_xmit(struct sk_buff *skb, 
					  struct net_device *dev)
{

	return NETDEV_TX_OK;
}

static int net_dev_ops_change_mtu(struct net_device *dev, int new_mtu)
{

	return 0;
}

static int net_dev_ops_set_mac_address(struct net_device *dev, void *addr)
{

	return 0;
}

static struct net_device_stats *net_dev_ops_get_stats(struct net_device *dev)
{

	return NULL;
}

static struct net_device_ops net_dev_ops = {
	.ndo_open		= net_dev_ops_open,
	.ndo_stop		= net_dev_ops_stop,
	.ndo_start_xmit		= net_dev_ops_start_xmit,
	.ndo_change_mtu		= net_dev_ops_change_mtu,
	.ndo_set_mac_address 	= net_dev_ops_set_mac_address,
	.ndo_get_stats		= net_dev_ops_get_stats,		
};

static struct rtnl_link_ops net_dev_rtnl_ops __read_mostly= {
	.kind = "net_dev_exp",
};

static void net_dev_setup(struct net_device *dev)
{
	ether_setup(dev);
	
	dev->netdev_ops = &net_dev_ops;
	dev->rtnl_link_ops = &net_dev_rtnl_ops;
	dev->destructor = free_netdev;
	dev->tx_queue_len = 0;
	dev->flags |= IFF_NOARP;
	dev->flags &= ~IFF_MULTICAST;
	
	random_ether_addr(dev->dev_addr);	
}


int net_dev_init(void)
{
	int err;
	g_net_dev_ptr = alloc_netdev(0, "net_dev_exp", NET_NAME_UNKNOWN, net_dev_setup);
	
	if(unlikely(!g_net_dev_ptr))
		return -ENOMEM;

	err = register_netdevice(g_net_dev_ptr);
	if(err < 0)
		goto err_reg;
	return 0;

err_reg:
	free_netdev(g_net_dev_ptr);
	return err;
}



void net_dev_exit(void)
{
	unregister_netdev(g_net_dev_ptr);
	free_netdev(g_net_dev_ptr);
}

module_init(net_dev_init);
module_exit(net_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_DESCRIPTION("Net device explore");

