#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>


#include "ram_dev_ioctl.h"

#define DEVICE_NAME "/dev/re_device_10"

int main(int argc, char **argv) {


    // Open the device file
    int fd;
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    printf("\n");
    printf("Opened the device file with FD = %d\n", fd);


    /* Section to READ from Driver Object */
    int rdSize = 108;
    char *rdBuffer = malloc(rdSize);
    struct ram_ioctl_data_1 ioRdData;

    // Prepare the ioctl data structure
    ioRdData.rd_size = rdSize;
    ioRdData.rd_buff_addr = rdBuffer;
    
    printf("\n");
    printf("Initialized ARG for READ IOCTL\n");
    printf("\tSize of rdBuffer: %d\n", rdSize);

    // Call the ioctl to get data from the kernel
    if (ioctl(fd,  RAM_IOCTL_READ_STR, &ioRdData) < 0) {
        perror("Read Ioctl Failed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Print the received data
    printf("\tSize of string received from kernel: %d\n", ioRdData.rd_size);
    printf("\tString received from kernel: %s\n", ioRdData.rd_buff_addr);


    /* Section to WRITE from Driver Object */
    int wrSize = 0;
    struct ram_ioctl_data_2 ioWrData;
    char *wr_mesg = "Om Jaga Janani Sai Mata!";

    // Prepare the ioctl data structure
    wrSize = strlen(wr_mesg) + 1;
    ioWrData.wr_size = wrSize;
    ioWrData.wr_buff_addr = wr_mesg;
    
    printf("\n");
    printf("Initialized ARG for WRITE IOCTL\n");
    printf("\tSize of wrBuffer: %d\n", wrSize);

    // Call the ioctl to get data from the kernel
    if (ioctl(fd,  RAM_IOCTL_WRITE_STR, &ioWrData) < 0) {
        perror("Write Ioctl Failed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Print the received data
    printf("\tSize of string sent to kernel: %d\n", ioWrData.wr_size);
    printf("\tString sent to kernel: %s\n", ioWrData.wr_buff_addr);


    /* Section to READ and WRITE from Driver Object */
    int rdwrSize = 0;
    struct ram_ioctl_data_3 ioRdWrData;
    char *rdwr_mesg = "Chandrasekharaya Namah Om, Gangahdaraya Namah Om!";

    // Prepare the ioctl data structure's read part
    ioRdWrData.rd_size = rdSize;
    ioRdWrData.rd_buff_addr = rdBuffer;
    
    printf("\n");
    printf("Initialized READ part of IOCTL ARG\n");
    printf("\tSize of rdBuffer: %d\n", rdSize);

    // Prepare the ioctl data structure's write part
    rdwrSize = strlen(rdwr_mesg) + 1;
    ioRdWrData.wr_size = rdwrSize;
    ioRdWrData.wr_buff_addr = rdwr_mesg;
    
    printf("Initialized WRITE part of IOCTL ARG\n");
    printf("\tSize of wrBuffer: %d\n", rdwrSize);

    // Call the ioctl to get data from the kernel
    if (ioctl(fd,  RAM_IOCTL_READ_WRITE_STR, &ioRdWrData) < 0) {
        perror("Read_Write Ioctl Failed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Print the received data
    printf("\n");
    printf("\tSize of string received from kernel: %d\n", ioRdWrData.rd_size);
    printf("\tString received from kernel: %s\n", ioRdWrData.rd_buff_addr);
    printf("\tSize of string sent to kernel: %d\n", ioRdWrData.wr_size);
    printf("\tString sent to kernel: %s\n", ioRdWrData.wr_buff_addr);
    printf("\n");


    /* Section to ALLOCATE SYSTEM MEMORY from Driver Object */
    uint64_t *buffAddr;
    int memSize = (4 * 1024 * 1024);
    struct ram_ioctl_data_4 ioAllocData;

    // Prepare the ioctl data structure
    ioAllocData.mem_size = memSize;
    ioAllocData.buff_addr = NULL;

    printf("\n");
    printf("Initialized ARG for ALLOC SYS MEM IOCTL\n");
    printf("\tSize of alloc request: %#x\n", memSize);

    // Call the ioctl to get data from the kernel
    if (ioctl(fd,  RAM_IOCTL_ALLOC_SYS_MEMORY, &ioAllocData) < 0) {
        perror("Alloc Ioctl Failed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Save address to be used as arg to free ioctl
    buffAddr = (uint64_t *)ioAllocData.buff_addr;

    // Print the received data
    printf("\tSize of alloc request sent to kernel: %#x\n", ioAllocData.mem_size);
    printf("\tBuffer Addr received from kernel: %#lx\n", (uint64_t)ioAllocData.buff_addr);


    /* Section to FREE SYSTEM MEMORY from Driver Object */
    uint64_t *tempAddr;
    struct ram_ioctl_data_5 ioFreeData;

    // Prepare the ioctl data structure
    ioFreeData.buff_addr = buffAddr;

    printf("\n");
    printf("Initialized ARG for FREE SYS MEM IOCTL\n");
    printf("\tAddr of Free request: %#lx\n", (uint64_t)buffAddr);

    // Call the ioctl to get data from the kernel
    if (ioctl(fd,  RAM_IOCTL_FREE_SYS_MEMORY, &ioFreeData) < 0) {
        perror("Free Ioctl Failed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Save address to be used as arg to free ioctl
    tempAddr = (uint64_t *)ioFreeData.buff_addr;

    // Print the received data
    printf("\tBuffer Addr sent kernel: %#lx\n", (uint64_t)buffAddr);
    printf("\tBuffer Addr received from kernel: %#lx\n", (uint64_t)tempAddr);

    // Close the device file and free resources
    printf("\n");
    close(fd);

    return EXIT_SUCCESS;
}


