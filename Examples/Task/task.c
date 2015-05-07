#include<linux/init.h>
#include<linux/module.h>
#include<linux/sched.h>


void task_info(void)
{
	struct task_struct *mytask=current;
	struct thread_info *mythread=current_thread_info();

	printk(KERN_ALERT "Task name: %s-->pid:%d\n", mytask->comm, mytask->pid);
	printk(KERN_ALERT "status:%d, CPU:%d, flag%d, preempt_count:%d\n",
		mythread->status, mythread->cpu, mythread->flags,
		mythread->saved_preempt_count);
	
}


static int __init task_init(void)
{
        task_info();
	printk(KERN_ALERT "Task information\n");
	return 0;
}


static void __exit task_exit(void)
{
	printk(KERN_ALERT "Task info End\n");

}

module_init(task_init);
module_exit(task_exit);

MODULE_AUTHOR("Mallesh K");
MODULE_LICENSE("GPL");
