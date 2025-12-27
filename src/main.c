/* main.c - Hello World demo */

/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>


/* size of stack area used by each thread */
#define THREAD_A_STACKSIZE 1024
#define THREAD_B_STACKSIZE 1024

/* scheduling priority used by each thread */
#define THREAD_A_PRIORITY 8
#define THREAD_B_PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 500


K_THREAD_STACK_DEFINE(threadA_stack_area, THREAD_A_STACKSIZE);
K_THREAD_STACK_DEFINE(threadB_stack_area, THREAD_B_STACKSIZE);

static struct k_thread threadA_data;
//static struct k_thread threadB_data;


/* threadA is a static thread that is spawned automatically */

void threadA(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	printk("thread_a: thread started \n");

	for(int i=5;i>0;i--){
		printk("thread_a: thread loop \n");
		k_msleep(SLEEPTIME);
	}

	printk("thread_a: sleep for 5000 ms.\n");
	k_sleep(K_MSEC(5000));

	printk("thread_a: thread abort\n");
	k_thread_abort(k_current_get());

}

void threadB(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	printk("thread_b: thread started \n");

	while (1)
	{
		printk("thread_b: thread loop \n");
		k_msleep(SLEEPTIME);
	}

}

K_THREAD_DEFINE(THREAD_B_TID, THREAD_B_STACKSIZE, threadB, NULL, NULL, NULL, THREAD_B_PRIORITY, 0, 5000);

void main(void)
{
	printk("Main loop started \n");

	k_thread_create(&threadA_data, threadA_stack_area,
			K_THREAD_STACK_SIZEOF(threadA_stack_area),
			threadA, NULL, NULL, NULL,
			THREAD_A_PRIORITY, 0, K_FOREVER);
	k_thread_name_set(&threadA_data, "thread_a");

	k_thread_start(&threadA_data);
}