#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/spinlock.h>

#define MICROBENCHMARK_OVERHEAD_COUNT  	1000000
#define TOTAL_ITERATION 		100000000
#define BENCHMARK_DEBUG
#define KERNEL_CYCLES

#ifdef BENCHMARK_DEBUG
#define printk(fm1, fm2...)	printk(KERN_ALERT "%s:(%s:%d)  "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#define printk_e(fm1, fm2...) printk(KERN_ERR "%s:(%s:%d) "fm1"", __FILE__, __func__, __LINE__, ##fm2)
#else
#define printk(fm1, fm2...) {}
#define printk_e(fm1, fm2...) {}
#endif


static DEFINE_SPINLOCK(my_lock);


static __always_inline uint64_t microbenchmark_start(void) {
	unsigned msb, lsb;
	asm volatile (
		"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (msb), "=r" (lsb)::
		"%rax", "%rbx", "%rcx", "%rdx");
	return ((uint64_t)lsb) | (((uint64_t)msb) << 32);
}

static __always_inline uint64_t microbenchmark_end(void) {
	unsigned msb, lsb;
	asm volatile(
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		"CPUID\n\t": "=r" (msb), "=r" (lsb)::
		"%rax", "%rbx", "%rcx", "%rdx");
	return ((uint64_t)lsb) | (((uint64_t)msb) << 32);
}

#ifndef KERNEL_CYCLES

static uint64_t microbenchmark_overhead(void)
{
	uint64_t start, end, ovrhead = ~0;
	int i = 0;

	while(i < MICROBENCHMARK_OVERHEAD_COUNT) {
		start = microbenchmark_start();
		asm volatile ("nop");
		end = microbenchmark_end();
		if(end - start < ovrhead)
			ovrhead = end - start;
		i ++;
	}

	return ovrhead;
}
#endif
int benchmark_init(void)
{
#ifndef KERNEL_CYCLES
	uint64_t start, end, overhead=0,total;
#else
	u64 start, end, total;
#endif
	int i;

	printk("\n");
	preempt_disable();
	printk("CPU no :%d\n", smp_processor_id());
	preempt_enable();

#ifndef KERNEL_CYCLES
	overhead = microbenchmark_overhead();

	printk("Overhead %llu\n", overhead);
	start = microbenchmark_start();
#else
	rdtscll(start);
#endif

	for(i = 0; i < TOTAL_ITERATION; i ++) {
		//asm volatile ("nop"::);
		spin_lock(&my_lock);
		barrier();
		spin_unlock(&my_lock);
	}
#ifndef KERNEL_CYCLES
	end = microbenchmark_end();
	total = (end - start - overhead) / TOTAL_ITERATION;
#else
	rdtscll(end);
	total = (end - start) / TOTAL_ITERATION;
#endif
	printk("Spin_lock and unlock took %llu cycles \n", total);
	/*2.10 GHz, check cpu freq from cat /proc/cpu | grep -i mhz*/

	return 0;
}

void benchmark_exit(void)
{
	printk("\n");
}

module_init(benchmark_init);
module_exit(benchmark_exit);

MODULE_AUTHOR("Mallesh Koujalagi <mallesh.koujalagi@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Micro benchmark sample code");
