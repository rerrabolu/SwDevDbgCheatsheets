#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // For file operations
#include <linux/cdev.h>
#include <linux/uaccess.h>      // For copying To and From User space
#include <linux/device.h>
#include <linux/init.h>


#define DEVICE_NAME "re_device_4"
#define CLASS_NAME "re_class_4"


static struct device *re_device = NULL;
static struct class *re_class = NULL;
static struct cdev re_cdev;
static dev_t dev_number;
static int major_number;

int create_module_4(void);
void teardown_module_4(void);

// Prototypes for file operations
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

// File operations structure
static const struct file_operations fops = {
	.owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// Buffer for storing data
#define BUFFER_SIZE 1024
static char device_buffer[BUFFER_SIZE] = {0};

int create_module_4(void)
{

	int ret;

    // Allocate a major number dynamically
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Sairam_4: Failed to allocate major number\n");
        return ret;
    }
    major_number = MAJOR(dev_number);
    pr_err("Sairam_4: Obtained Major number for Char device: %d\n", major_number);

    // Create the class
    re_class = class_create(CLASS_NAME);
    if (IS_ERR(re_class)) {
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_4: Failed to register device class\n");
        return PTR_ERR(re_class);
    }
    pr_err("Sairam_4: Created Class for Char device\n");

    // Initialize the character device
    cdev_init(&re_cdev, &fops);
    ret = cdev_add(&re_cdev, dev_number, 1);
    if (ret < 0) {
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_4: Failed to add character device\n");
        return ret;
    }
    pr_err("Sairam_4: Created and Added CDEV for Char device\n");

    // Create the device file
    re_device = device_create(re_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(re_device)) {
        cdev_del(&re_cdev);
        class_destroy(re_class);
        unregister_chrdev_region(dev_number, 1);
        pr_err("Sairam_4: Failed to create device file\n");
        return PTR_ERR(re_device);
    }

    pr_err("Sairam_4: Char device's major number is: %d\n", major_number);
    pr_err("Sairam_4: Load a char device that supports R/W operations\n");
    
    return 0;
}

void teardown_module_4(void)
{
    pr_err("Sairam_4: Begin Teardown the Char device\n");

    dev_t dev_number = MKDEV(major_number, 0);

    device_destroy(re_class, dev_number);
    cdev_del(&re_cdev);
    class_destroy(re_class);
    unregister_chrdev_region(dev_number, 1);
    
    pr_err("Sairam_4: End Teardown the Char device\n");
}

// Open function
static int dev_open(struct inode *inode, struct file *file) {
// int dev_open(struct inode *inode, struct file *file) {
    pr_err("Sairam_4: Device opened\n");
    return 0;
}

// Release function
static int dev_release(struct inode *inode, struct file *file) {
// int dev_release(struct inode *inode, struct file *file) {
    pr_err("Sairam_4: Device closed\n");
    return 0;
}

// Read function
static ssize_t dev_read(struct file *file,
// ssize_t dev_read(struct file *file,
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
        pr_err("Sairam_4: Failed to send data to user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes read
    *offset += len;
    bytes_read = len;

    pr_err("Sairam_4: Read %d bytes from the device\n", bytes_read);
    return bytes_read;
}

// Write function
static ssize_t dev_write(struct file *file,
// ssize_t dev_write(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    int bytes_written = 0;

    // Limit the number of bytes to write
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from user space to the kernel buffer
    if (copy_from_user(device_buffer + *offset, buf, len)) {
        pr_err("Sairam_4: Failed to receive data from user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes written
    *offset += len;
    bytes_written = len;

    pr_err("Sairam_4: Wrote %d bytes to the device\n", bytes_written);
    return bytes_written;
}


