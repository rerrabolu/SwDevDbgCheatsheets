#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static struct device * re_device = NULL;
static struct class *re_class = NULL;
static struct cdev re_cdev;
static dev_t dev_number;
static int major_number;

#define DEVICE_NAME "re_device_2"
#define CLASS_NAME "re_class_2"

// File operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    // No operations implemented (open, read, write, etc.)
    .open = NULL,
    .release = NULL,
    .read = NULL,
    .write = NULL,
};

// Initialization function
static int __init ram_module_2_init(void) {

    int ret;

	pr_err("Sairam_2: Loading a simple character device module\n");

    // Dynamically allocate a major number
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Sairam_2: Failed to allocate major number\n");
        return ret;
    }
    major_number = MAJOR(dev_number);
    pr_err("Sairam_2: Allocated a Major number for Char device: %d\n", major_number);

    // Register the device class
    re_class = class_create(CLASS_NAME);
    if (IS_ERR(re_class)) {
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_2: Failed to register device class\n");
        return PTR_ERR(re_class);
    }
    pr_err("Sairam_2: Obtained Class for Char device\n");

    // Initialize the character device
    cdev_init(&re_cdev, &fops);
    ret = cdev_add(&re_cdev, dev_number, 1);
    if (ret < 0) {
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_2: Failed to add character device\n");
        return ret;
    }
    pr_err("Sairam_2: Initialized and Added CDEV for Char device\n");

    // Create device file
    re_device = device_create(re_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (re_device == NULL) {
        cdev_del(&re_cdev);
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_2: Failed to create device file\n");
        return PTR_ERR(re_device);
    }

    pr_err("Sairam_2: A simple character device registered with major number %d\n", major_number);
    return 0;
}

// Exit function
static void __exit ram_module_2_exit(void) {

	pr_err("Sairam_2: Unloading a simple character device module\n");

    // Remove device and unregister class
    device_destroy(re_class, dev_number);
    cdev_del(&re_cdev);
    class_destroy(re_class);
    unregister_chrdev_region(dev_number, 1);
}

module_init(ram_module_2_init);
module_exit(ram_module_2_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device module");


