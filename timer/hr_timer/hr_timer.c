#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/hrtimer.h>

struct hrtimer timer;
static ktime_t period;

static enum hrtimer_restart watchdog(struct hrtimer *timer) 
{

	hrtimer_forward_now(timer, period); /*seconds and nanoseconds*/
	printk(KERN_ALERT "HR timer restarted\n");
	return HRTIMER_RESTART;
}



static int __init hr_timer_init(void)
{

	
	printk(KERN_ALERT "Hr timer Init\n");
	timer.function = watchdog;
	hrtimer_init(&timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	period = ktime_set(1, 0);
	hrtimer_start(&timer, period, HRTIMER_MODE_REL);
	return 0;
}

static void __exit hr_timer_exit(void)
{

	printk(KERN_ALERT "Hr timer Init\n");
	hrtimer_cancel(&timer);

}

module_init(hr_timer_init);
module_exit(hr_timer_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("High resolution timer sample coding");
