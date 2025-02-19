#include <linux/types.h>
#include <linux/fs.h>           // For file operations

#include "ram_fops_core.h"
#include "ram_dev_ioctl.h"
#include "ram_dev_ioctl_impl.h"

// Struct to encapsulate process info
struct ram_dev_process {
    int pid;
};

typedef int ram_dev_ioctl_t(struct file *filep,
                        struct ram_dev_process *proc, void *data);

// Struct to encapsulate info about an ioctl
struct ram_dev_ioctl_desc {
    unsigned int cmd;
    int flags;
    ram_dev_ioctl_t *func;
    const char *name;
};

// Defines a ioctl descriptor object. The macro retrieves
// from user facing IOCTL Id, the ID given by driver object
// and populates an instance of IOCTL descriptor at the
// index given by driver object
#define RAM_DEV_IOCTL_DEF(ioctl, _func, _flags) \
        [_IOC_NR(ioctl)] = { .cmd = ioctl,      \
                            .func = _func,      \
                            .flags = _flags,    \
                            .name = #ioctl }
 

// Build a list of ioctl descriptor using the struct
// that is defined to encapsulate ioctl info 
const struct ram_dev_ioctl_desc desc_list[] = {

    RAM_DEV_IOCTL_DEF(RAM_IOCTL_READ_STR,
                            ram_dev_ioctl_read_str, 0),

    RAM_DEV_IOCTL_DEF(RAM_IOCTL_WRITE_STR,
                            ram_dev_ioctl_write_str, 0),

    RAM_DEV_IOCTL_DEF(RAM_IOCTL_READ_WRITE_STR,
                            ram_dev_ioctl_read_write_str, 0),

    RAM_DEV_IOCTL_DEF(RAM_IOCTL_ALLOC_SYS_MEMORY,
                            ram_dev_ioctl_alloc_sys_memory, 0),

    RAM_DEV_IOCTL_DEF(RAM_IOCTL_FREE_SYS_MEMORY,
                            ram_dev_ioctl_free_sys_memory, 0),

};


/**
 * Root method called by kernel to service an IOCTL
 * call from userspace
 */
long ram_dev_ioctl(struct file *filep,
                   unsigned int cmd, unsigned long arg) {
    const struct ram_dev_ioctl_desc *desc;
    ram_dev_ioctl_t *func;
    char kdata[128];
	u32 ioctl_id;
    u32 ret;
    
    pr_err("%s - Value of cmd recvd: %u\n", __func__, cmd);
    pr_err("%s - Value of ioctl_id recvd: %u\n", __func__, _IOC_NR(cmd));
    pr_err("%s - Magic number: 0x%x\n", __func__, _IOC_TYPE(cmd));
    pr_err("%s - Data size: %u\n", __func__, _IOC_SIZE(cmd)); 
    pr_err("%s - Data direction: %s\n", __func__,
            (_IOC_DIR(cmd) == _IOC_NONE)? "None":
            (_IOC_DIR(cmd) == _IOC_WRITE)? "Write":
            (_IOC_DIR(cmd) == _IOC_READ)? "Read":
            (_IOC_DIR(cmd) == (_IOC_READ | _IOC_WRITE))? "Read/Write": "Unknown");


    // Validate the command passed into ioctl
    ioctl_id = _IOC_NR(cmd);
    if ((ioctl_id <= RAM_DEV_START_ID) || (ioctl_id>= RAM_DEV_STOP_ID)) {
        pr_err("Sairam_9: %s, IOCTL cmd is invalid: %u\n", __func__, cmd);
        return -EINVAL;
    }
 
    // Get the handle of ioctl descriptor
    desc = &desc_list[ioctl_id];
    if (desc == NULL) {
        pr_err("Didn't find a valid ioctl descriptor\n");
    } else {
        pr_err("Name of valid ioctl descriptor: %s\n", desc->name);
    }
            
    // Get the size of this ioctl's args parameter
    // as given by user and defined by driver object
    u32 usr_arg_size = _IOC_SIZE(cmd);
    u32 drv_arg_size = _IOC_SIZE(desc->cmd);

    pr_err("%s Size per IOC_SIZE(cmd): %u\n", __func__, usr_arg_size);
    pr_err("%s Size per IOC_SIZE(desc->cmd): %u\n", __func__, drv_arg_size);

    // The two should match, else it is
    // an error condition
    if (usr_arg_size != drv_arg_size) {
        pr_err("Sairam_9: %s, IOCTL cmd arg is invalid: %u\n", __func__, cmd);
        return -EINVAL;
    }

    // At this point it is reasonable to consider
    // the IOCTL call as being valid. So copy the
    // arguments of the IOCTL and call its handler
	if (copy_from_user(kdata, (void __user *)arg, drv_arg_size) != 0) {
        pr_err("Sairam_9: %s, IOCTL cmd copy encountered error: %u\n", __func__, cmd);
        return -EINVAL;
    }

    // Get hold of the handler that implements the command
    func = desc->func;
    ret = func(filep, NULL, kdata);
    if (ret != 0) {
        pr_err("Sairam_9: %s, IOCTL cmd copy to user space encountered error: %u\n", __func__, cmd);
        return -EFAULT;
    }

	//if ((cmd & IOC_OUT) || (cmd & IOC_INOUT)) {
	if (cmd & IOC_OUT) {
        pr_err("Handling passing of kernel data to user space\n");
        if (copy_to_user((void __user *)arg, kdata, drv_arg_size) != 0) {
            pr_err("Sairam_9: %s, IOCTL cmd copy to user space encountered error: %u\n", __func__, cmd);
            return -EFAULT;
        }
    }

    return ret;
}

// Implements a read request from the user space
int ram_dev_ioctl_read_str(struct file *filep,
                        struct ram_dev_process *proc, void *data) {

    char *mesg = "Om Sri Sai Ram, Jai Sai Ram!\n";
    struct ram_ioctl_data_1 *arg= (struct ram_ioctl_data_1 *)data;
    u32 size;
    s32 ret;

    size = strlen(mesg) + 1;
    if (size > arg->rd_size) {
        pr_err("sairam_9: %s, user buffer is too small\n", __func__);
        return -EINVAL;
    }

    pr_err("Sairam_9: %s, IOCTL cmd to read\n", __func__);
    pr_err("Sairam_9: %s, IOCTL cmd returning string size: %u\n", __func__, size);
    pr_err("Sairam_9: %s, IOCTL cmd returning string: %s\n", __func__, mesg);
    arg->rd_size = size;
    ret = copy_to_user((void __user *)arg->rd_buff_addr, mesg, size);
    
    return 0;
}

// Implements a write request from the user space
int ram_dev_ioctl_write_str(struct file *filep,
                        struct ram_dev_process *proc, void *data) {

    struct ram_ioctl_data_2 *arg= (struct ram_ioctl_data_2 *)data;
    char mesg[108];

    if (sizeof(mesg) < arg->wr_size) {
        pr_err("sairam_9: %s, user buffer is too big: %u\n", __func__, arg->wr_size);
        return -EINVAL;
    }

    pr_err("Sairam_9: %s, IOCTL cmd to write\n", __func__);
    pr_err("Sairam_9: %s, IOCTL cmd writing string size: %u\n", __func__, arg->wr_size);
	if (copy_from_user(&mesg[0], (void __user *)arg->wr_buff_addr, arg->wr_size) != 0) {
        pr_err("%s - Error in copying user buffer to kernel buffer\n", __func__);
        return -EFAULT;
    }
    pr_err("Sairam_9: %s, IOCTL cmd writing string: %s\n", __func__, &mesg[0]);
    
    return 0;
}

// Implements a read and write request from the user space
int ram_dev_ioctl_read_write_str(struct file *filep,
                        struct ram_dev_process *proc, void *data) {

    struct ram_ioctl_data_3 *arg= (struct ram_ioctl_data_3 *)data;
    char *rd_mesg = "Sai Pita Aur Mata Sai, Deena Dayala Datha Sai!\n";
    char wr_mesg[108];
    u32 rd_size;

    rd_size = strlen(rd_mesg) + 1;
    if (rd_size > arg->rd_size) {
        pr_err("sairam_9: %s, user buffer is too small\n", __func__);
        return -EINVAL;
    }

    pr_err("Sairam_9: %s, IOCTL cmd to read\n", __func__);
    pr_err("Sairam_9: %s, IOCTL cmd returning string size: %u\n", __func__, rd_size);
    pr_err("Sairam_9: %s, IOCTL cmd returning string: %s\n", __func__, rd_mesg);
    arg->rd_size = rd_size;
    if (copy_to_user((void __user *)arg->rd_buff_addr, rd_mesg, rd_size) != 0) {
        pr_err("%s - Error in copying kernel buffer to user buffer\n", __func__);
        return -EFAULT;
    }
    

    if (sizeof(wr_mesg) < arg->wr_size) {
        pr_err("sairam_9: %s, user buffer is too big: %u\n", __func__, arg->wr_size);
        return -EINVAL;
    }

    pr_err("Sairam_9: %s, IOCTL cmd to write\n", __func__);
    pr_err("Sairam_9: %s, IOCTL cmd writing string size: %u\n", __func__, arg->wr_size);
	if (copy_from_user(&wr_mesg[0], (void __user *)arg->wr_buff_addr, arg->wr_size) != 0) {
        pr_err("%s - Error in copying user buffer to kernel buffer\n", __func__);
        return -EFAULT;
    }
    pr_err("Sairam_9: %s, IOCTL cmd writing string: %s\n", __func__, &wr_mesg[0]);
    

    return 0;
}

// Implements a request to allocate in system memory
int ram_dev_ioctl_alloc_sys_memory(struct file *filep,
                        struct ram_dev_process *proc, void *data) {

    struct ram_ioctl_data_4 *arg= (struct ram_ioctl_data_4 *)data;
    u32 alloc_size = arg->mem_size;
    char *buffer = NULL;

    // Allocate memory of said size
    buffer = kmalloc(alloc_size, GFP_KERNEL);
    if (buffer == NULL) {
        pr_err("Sairam_9: %s, IOCTL cmd to allocate memory of size failed: %u\n", __func__, alloc_size);
        return -EFAULT;
    }

    arg->buff_addr = (u64 *)buffer;
    return 0;
}


// Implements a request to free in system memory
int ram_dev_ioctl_free_sys_memory(struct file *filep,
                        struct ram_dev_process *proc, void *data) {

    struct ram_ioctl_data_5 *arg= (struct ram_ioctl_data_5 *)data;

    kfree(arg->buff_addr);
    arg->buff_addr = 0;

    return 0;

}




