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
int ram_open1(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_open2(inode, file);
}

// Release function
int ram_release1(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_release2(inode, file);
}

// Read function
ssize_t ram_read1(struct file *file,
			char __user *buf, size_t len, loff_t *offset) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_read2(file, buf, len, offset);
}

// Write function
ssize_t ram_write1(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_write2(file, buf, len, offset);
}


// Open function
int ram_open2(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_open3(inode, file);;
}

// Release function
int ram_release2(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_release3(inode, file);;
}

// Read function
ssize_t ram_read2(struct file *file,
			char __user *buf, size_t len, loff_t *offset) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_read3(file, buf, len, offset);
}

// Write function
ssize_t ram_write2(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return ram_write3(file, buf, len, offset);
}



// Open function
int ram_open3(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return 0;
}

// Release function
int ram_release3(struct inode *inode, struct file *file) {
    pr_err("Sairam_8: %s, Device opened\n", __func__);
    return 0;
}

// Read function
ssize_t ram_read3(struct file *file,
			char __user *buf, size_t len, loff_t *offset) {
    int bytes_read = 0;

    pr_err("Sairam_8: %s, Device opened\n", __func__);

    // If the offset is beyond the buffer size, we return 0 to indicate EOF
    if (*offset >= BUFFER_SIZE) {
        return 0;
    }

    // Limit the number of bytes to read
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from the kernel buffer to user space
    if (copy_to_user(buf, device_buffer + *offset, len)) {
        pr_err("Sairam_8: %s Failed to send data to user space\n", __func__);
        return -EFAULT;
    }

    // Update the offset and number of bytes read
    *offset += len;
    bytes_read = len;

    pr_err("Sairam_8: %s Read %d bytes from the device\n", __func__, bytes_read);
    return bytes_read;
}

// Write function
ssize_t ram_write3(struct file *file,
			const char __user *buf, size_t len, loff_t *offset) {
    int bytes_written = 0;

    pr_err("Sairam_8: %s, Device opened\n", __func__);

    // Limit the number of bytes to write
    len = min(len, (size_t)(BUFFER_SIZE - *offset));

    // Copy data from user space to the kernel buffer
    if (copy_from_user(device_buffer + *offset, buf, len)) {
        pr_err("Sairam_8: %s Failed to receive data from user space\n", __func__);
        return -EFAULT;
    }

    // Update the offset and number of bytes written
    *offset += len;
    bytes_written = len;

    pr_err("Sairam_8: %s Wrote %d bytes to the device\n", __func__, bytes_written);
    return bytes_written;
}



