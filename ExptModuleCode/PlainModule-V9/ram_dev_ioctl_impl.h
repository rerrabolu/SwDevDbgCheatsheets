// ram_ioctl_impl.h
#ifndef RAM_IOCTL_IMPL_H
#define RAM_IOCTL_IMPL_H

#include <linux/types.h>
#include <linux/ioctl.h> // For _IOR, _IOW macros

struct ram_dev_process;

// Implements a read request from the user space
int ram_dev_ioctl_read_str(struct file *filep,
                struct ram_dev_process *proc, void *data);

// Implements a write request from the user space
int ram_dev_ioctl_write_str(struct file *filep,
                struct ram_dev_process *proc, void *data);

// Implements a read and write request from the user space
int ram_dev_ioctl_read_write_str(struct file *filep,
                        struct ram_dev_process *proc, void *data);

// Implements a request to allocate in system memory
int ram_dev_ioctl_alloc_sys_memory(struct file *filep,
                        struct ram_dev_process *proc, void *data);

// Implements a request to free in system memory
int ram_dev_ioctl_free_sys_memory(struct file *filep,
                        struct ram_dev_process *proc, void *data);


#endif  // RAM_IOCTL_IMPL_H


