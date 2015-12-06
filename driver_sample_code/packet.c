#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>




int packet_init(void)
{

	return 0;
}

void packet_exit(void)
{


}

module_init(packet_init);
module_exit(packet_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Packet sample coding");
