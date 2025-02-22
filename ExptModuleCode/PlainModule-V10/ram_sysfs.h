#ifndef SAIRAM_SYSFS_H
#define SAIRAM_SYSFS_H


/* Initialize the root directory and its files and sub-directories */
int ram_sysfs_init(void);


/* Cleanup - Remove all directories and files */
void ram_sysfs_exit(void);

int ram_sysfs_free_dir_1(void);
int ram_sysfs_free_dir_2(void);
int ram_sysfs_free_dir_3(void);


int ram_sysfs_init_dir_1(void);
int ram_sysfs_init_dir_2(void);
int ram_sysfs_init_dir_3(void);

#endif // SAIRAM_SYSFS_H


