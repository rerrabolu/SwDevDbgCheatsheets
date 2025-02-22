#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // For file operations
#include <linux/cdev.h>
#include <linux/uaccess.h>      // For copying To and From User space
#include <linux/device.h>
#include <linux/init.h>

#include "ram_fops_core.h"
#include "ram_fops_impl.h"

// Open function
int dev_open(struct inode *inode, struct file *file) {
    pr_err("Sairam_10: %s Device opened\n", __func__);
    return ram_open1(inode, file);;
}

// Release function
int dev_release(struct inode *inode, struct file *file) {
    pr_err("Sairam_10: %s Device opened\n", __func__);
    return ram_release1(inode, file);;
}

// Read function
ssize_t dev_read(struct file *file,
			char __user *buf, size_t len, loff_t *offset)
{
    pr_err("Sairam_10: %s Device opened\n", __func__);
    return ram_read1(file, buf, len, offset);
}

// Write function
ssize_t dev_write(struct file *file,
			const char __user *buf, size_t len, loff_t *offset)
{
    pr_err("Sairam_10: %s Device opened\n", __func__);
    return ram_write1(file, buf, len, offset);
}



