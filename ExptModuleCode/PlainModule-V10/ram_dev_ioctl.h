// ram_ioctl.h
#ifndef RAM_IOCTL_H
#define RAM_IOCTL_H

#include <linux/types.h>

// If compiling for kernel space
#ifdef __KERNEL__
    #include <linux/ioctl.h>
#endif

// If not compiling in kernel space
#ifndef __KERNEL__
    #include <stdint.h> 
    typedef uint64_t u64; // Kernel type
#endif

// BASE ID that is used to compute the ID of an IOCTL
#define RAM_DEV_CMD_BASE     'S'


// Define the structure to pass READ_ONLY data
struct ram_ioctl_data_1 {
    int rd_size;
    char *rd_buff_addr;  // Example field
};

// Define the structure to pass WRITE_ONLY data
struct ram_ioctl_data_2 {
    int wr_size;
    char *wr_buff_addr;  // Example field
};

// Define the structure to pass READ and WRITE data
struct ram_ioctl_data_3 {
    int rd_size;
    int wr_size;
    char *rd_buff_addr;  // Example field
    char *wr_buff_addr;  // Example field
};

// Define the structure to allocate memory
struct ram_ioctl_data_4 {
    int mem_size;
    u64 *buff_addr;
};

// Define the structure to free memory
struct ram_ioctl_data_5 {
    u64 *buff_addr;
};

// Define the IOCTL command numbers
#define RAM_IOCTL_READ_STR          _IOR(RAM_DEV_CMD_BASE, 1, struct ram_ioctl_data_1)
#define RAM_IOCTL_WRITE_STR         _IOW(RAM_DEV_CMD_BASE, 2, struct ram_ioctl_data_2)
#define RAM_IOCTL_READ_WRITE_STR    _IOWR(RAM_DEV_CMD_BASE, 3, struct ram_ioctl_data_3)
#define RAM_IOCTL_ALLOC_SYS_MEMORY  _IOWR(RAM_DEV_CMD_BASE, 4, struct ram_ioctl_data_4)
#define RAM_IOCTL_FREE_SYS_MEMORY   _IOWR(RAM_DEV_CMD_BASE, 5, struct ram_ioctl_data_5)

#define RAM_DEV_START_ID    0
#define RAM_DEV_STOP_ID     6

#endif

