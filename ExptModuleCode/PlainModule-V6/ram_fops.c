#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // For file operations
#include <linux/cdev.h>
#include <linux/uaccess.h>      // For copying To and From User space
#include <linux/device.h>
#include <linux/init.h>

#include "ram_fops_impl.h"

#define DEVICE_NAME "re_device_6"
#define CLASS_NAME "re_class_6"

static struct device *re_device = NULL;
static struct class *re_class = NULL;
static struct cdev re_cdev;
static dev_t dev_number;
static int major_number;

int create_module_6(void);
void teardown_module_6(void);

extern const struct file_operations fops;

int create_module_6(void)
{

	int ret;

    // Allocate a major number dynamically
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Sairam_6: Failed to allocate major number\n");
        return ret;
    }
    major_number = MAJOR(dev_number);
    pr_err("Sairam_6: Obtained Major number for Char device: %d\n", major_number);

    // Create the class
    re_class = class_create(CLASS_NAME);
    if (IS_ERR(re_class)) {
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_6: Failed to register device class\n");
        return PTR_ERR(re_class);
    }
    pr_err("Sairam_6: Created Class for Char device\n");

    // Initialize the character device
    cdev_init(&re_cdev, &fops);
    ret = cdev_add(&re_cdev, dev_number, 1);
    if (ret < 0) {
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_6: Failed to add character device\n");
        return ret;
    }
    pr_err("Sairam_6: Created and Added CDEV for Char device\n");

    // Create the device file
    re_device = device_create(re_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(re_device)) {
        cdev_del(&re_cdev);
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_6: Failed to create device file\n");
        return PTR_ERR(re_device);
    }

    pr_err("Sairam_6: Char device's major number is: %d\n", major_number);
    pr_err("Sairam_6: Load a char device that supports R/W operations\n");
    
    return 0;
}

void teardown_module_6(void)
{
    pr_err("Sairam_6: Begin Teardown the Char device\n");

    dev_t dev_number = MKDEV(major_number, 0);

    device_destroy(re_class, dev_number);
    cdev_del(&re_cdev);
    class_destroy(re_class);
    unregister_chrdev_region(dev_number, 1);
    
    pr_err("Sairam_6: End Teardown the Char device\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char device that can read and write data");


