#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> // for copy_to_user and copy_from_user
#include <linux/device.h>
#include <linux/init.h>

#define DEVICE_NAME "re_device_3"
#define CLASS_NAME  "re_class_3"

static struct device *re_device = NULL;
static struct class *re_class = NULL;
static struct cdev re_cdev;
static dev_t dev_number;
static int major_number;

// Buffer for storing data
#define BUFFER_SIZE 1024
static char device_buffer[BUFFER_SIZE] = {0};

// Prototypes for file operations
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

// File operations structure
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// The init function for the module
static int __init rw_cdevice3_init(void) {
    int ret;

    // Allocate a major number dynamically
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Sairam_3: Failed to allocate major number\n");
        return ret;
    }

    // Create the class
    re_class = class_create(CLASS_NAME);
    if (IS_ERR(re_class)) {
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_3: Failed to register device class\n");
        return PTR_ERR(re_class);
    }

    // Initialize the character device
    cdev_init(&re_cdev, &fops);
    ret = cdev_add(&re_cdev, dev_number, 1);
    if (ret < 0) {
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_3: Failed to add character device\n");
        return ret;
    }

    // Create the device file
    re_device = device_create(re_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(re_device)) {
        cdev_del(&re_cdev);
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_3: Failed to create device file\n");
        return PTR_ERR(re_device);
    }

    major_number = MAJOR(dev_number);
    pr_err("Sairam_3: Char device's major number is: %d\n", major_number);
    pr_err("Sairam_3: Load a char device that supports R/W operations\n");
    return 0;
}

// The cleanup function for the module
static void __exit rw_cdevice3_exit(void) {

    dev_t dev_number = MKDEV(major_number, 0);

    device_destroy(re_class, dev_number);
    cdev_del(&re_cdev);
    class_destroy(re_class);
    unregister_chrdev_region(dev_number, 1);

    pr_err("Sairam_3: Unload a char device that supports R/W operations\n");
}

// Open function
static int dev_open(struct inode *inode, struct file *file) {
    pr_err("Sairam_3: Device opened\n");
    return 0;
}

// Release function
static int dev_release(struct inode *inode, struct file *file) {
    pr_err("Sairam_3: Device closed\n");
    return 0;
}

// Read function
static ssize_t dev_read(struct file *file,
			char __user *buf, size_t len, loff_t *offset) {
    int bytes_read = 0;

    // If the offset is beyond the buffer size, we return 0 to indicate EOF
    if (*offset >= BUFFER_SIZE) {
        return 0;
    }

    // Limit the number of bytes to read
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from the kernel buffer to user space
    if (copy_to_user(buf, device_buffer + *offset, len)) {
        pr_err("Sairam_3: Failed to send data to user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes read
    *offset += len;
    bytes_read = len;

    pr_err("Sairam_3: Read %d bytes from the device\n", bytes_read);
    return bytes_read;
}

// Write function
static ssize_t dev_write(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    int bytes_written = 0;

    // Limit the number of bytes to write
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from user space to the kernel buffer
    if (copy_from_user(device_buffer + *offset, buf, len)) {
        pr_err("Sairam_3: Failed to receive data from user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes written
    *offset += len;
    bytes_written = len;

    pr_err("Sairam_3: Wrote %d bytes to the device\n", bytes_written);
    return bytes_written;
}

module_init(rw_cdevice3_init);
module_exit(rw_cdevice3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char device that can read and write data");

