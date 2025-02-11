#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>      // for copy_to_user and copy_from_user
#include <linux/device.h>
#include <linux/init.h>

extern int create_module_5(void);
extern void teardown_module_5(void);

// The init function for the module
static int __init ram_module_5_init(void) {

    int retval;

    pr_err("Sairam_5: Calling helper method to create a Char device\n");
    retval = create_module_5();
    pr_err("Sairam_5: Return value of helper method: %d\n", retval);

    return retval;
}

// The cleanup function for the module
static void __exit ram_module_5_exit(void) {

    teardown_module_5();
    pr_err("Sairam_5: Unload a char device that supports R/W operations\n");
}

module_init(ram_module_5_init);
module_exit(ram_module_5_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char device that can read and write data");


