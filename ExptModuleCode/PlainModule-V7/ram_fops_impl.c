#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // For file operations
#include <linux/cdev.h>
#include <linux/uaccess.h>      // For copying To and From User space
#include <linux/device.h>
#include <linux/init.h>

#include "ram_fops_impl.h"

// Buffer for storing data
#define BUFFER_SIZE 1024
static char device_buffer[BUFFER_SIZE] = {0};

// Open function
int dev_open(struct inode *inode, struct file *file) {
    pr_err("Sairam_7: Device opened\n");
    return 0;
}

// Release function
int dev_release(struct inode *inode, struct file *file) {
    pr_err("Sairam_7: Device closed\n");
    return 0;
}

// Read function
ssize_t dev_read(struct file *file,
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
        pr_err("Sairam_7: Failed to send data to user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes read
    *offset += len;
    bytes_read = len;

    pr_err("Sairam_7: Read %d bytes from the device\n", bytes_read);
    return bytes_read;
}

// Write function
ssize_t dev_write(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    int bytes_written = 0;

    // Limit the number of bytes to write
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from user space to the kernel buffer
    if (copy_from_user(device_buffer + *offset, buf, len)) {
        pr_err("Sairam_7: Failed to receive data from user space\n");
        return -EFAULT;
    }

    // Update the offset and number of bytes written
    *offset += len;
    bytes_written = len;

    pr_err("Sairam_7: Wrote %d bytes to the device\n", bytes_written);
    return bytes_written;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char device that can read and write data");


