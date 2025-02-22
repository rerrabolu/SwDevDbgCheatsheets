#ifndef SAIRAM_IMPL_H
#define SAIRAM_IMPL_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>           // For file operations
#include <linux/cdev.h>
#include <linux/uaccess.h>      // For copying To and From User space
#include <linux/device.h>
#include <linux/init.h>


// Prototypes for file operations
int ram_open1(struct inode *, struct file *);
int ram_release1(struct inode *, struct file *);
ssize_t ram_read1(struct file *, char __user *, size_t, loff_t *);
ssize_t ram_write1(struct file *, const char __user *, size_t, loff_t *);

int ram_open2(struct inode *, struct file *);
int ram_release2(struct inode *, struct file *);
ssize_t ram_read2(struct file *, char __user *, size_t, loff_t *);
ssize_t ram_write2(struct file *, const char __user *, size_t, loff_t *);

int ram_open3(struct inode *, struct file *);
int ram_release3(struct inode *, struct file *);
ssize_t ram_read3(struct file *, char __user *, size_t, loff_t *);
ssize_t ram_write3(struct file *, const char __user *, size_t, loff_t *);

#endif // SAIRAM_IMPL_H


