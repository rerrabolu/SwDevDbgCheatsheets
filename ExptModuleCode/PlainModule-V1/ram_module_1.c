// Required for all kernel modules
#include <linux/module.h>

// Required for printk()
#include <linux/kernel.h>

// Required for module init
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World kernel module");

/**
 * A Simple kernel module. All that the module
 * will do upon loading is to print a statement
 * into kernel log
 */
static int __init ram_device_1_init(void) {
    pr_err("Sairam_1: Loading a Simple module with no device\n");
    return 0;
}

/**
 * A Simple kernel module. All that the module
 * will do upon unloading is to print a statement
 * into kernel log
 */
static void __exit ram_device_1_exit(void) {
    pr_err("Sairam_1: UNLoading a Simple module with no device\n");
}

/* Name of the module is - simple_module */
module_init(ram_device_1_init);
module_exit(ram_device_1_exit);

