#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "ram_sysfs.h"

#define RAM_ROOT_DIR  "ram_sysfs_root"
#define RAM_CHILD_DIR_1 "ram_sysfs_dir_1"
#define RAM_CHILD_DIR_2 "ram_sysfs_dir_2"
#define RAM_CHILD_DIR_3 "ram_sysfs_dir_3"

static struct kobject *ram_sysfs_root;
static struct kobject *ram_sysfs_dir_1;
static struct kobject *ram_sysfs_dir_2;
static struct kobject *ram_sysfs_dir_3;


/* File a - Encodes a char */

static char file_a_value = 'R';

// Allow user space to READ a character value
static ssize_t file_a_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a char value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a char value of: %c\n",
            __func__, file_a_value);
    return sprintf(buf, "%c", file_a_value);
}

// Allow user space to WRITE a character value
static ssize_t file_a_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a char value\n", __func__);
    sscanf(buf, "%c", &file_a_value);
    pr_err("Sairam 10: %s Read from userspace a char value of: %c\n",
            __func__, file_a_value);
    
    return count;
}

static struct kobj_attribute file_a_attr = __ATTR(file_a, 0664, file_a_show, file_a_store);

/* File b - Encodes an integer */

static int file_b_value = 1123;

// Allow user space to READ a integral value
static ssize_t file_b_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a integral value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a integral value of: %d\n",
            __func__, file_b_value);
    return sprintf(buf, "%d", file_b_value);
}

// Allow user space to WRITE a integral value
static ssize_t file_b_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a integral value\n", __func__);
    sscanf(buf, "%d", &file_b_value);
    pr_err("Sairam 10: %s Read from userspace a integral value of: %d\n",
        __func__, file_b_value);
    return count;
}

static struct kobj_attribute file_b_attr = __ATTR(file_b, 0664, file_b_show, file_b_store);

/* File c - Encodes a string */

static char file_c_value[108] = "Om Sri Sairam";

// Allow user space to READ a string value
static ssize_t file_c_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a string value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a string value of: %s\n",
        __func__, file_c_value);
    return sprintf(buf, "%s", file_c_value);
}

// Allow user space to WRITE a string value
static ssize_t file_c_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a string value\n", __func__);
    strncpy(file_c_value, buf, sizeof(file_c_value) - 1);
    file_c_value[sizeof(file_c_value) - 1] = '\0';
    pr_err("Sairam 10: %s Read from userspace a string value of: %s\n",
        __func__, file_c_value);
    return count;
}

static struct kobj_attribute file_c_attr = __ATTR(file_c, 0664, file_c_show, file_c_store);




/* File i - Encodes a char */

static char file_i_value = 'R';

// Allow user space to READ a character value
static ssize_t file_i_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a char value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a char value of: %d\n",
            __func__, file_i_value);
    return sprintf(buf, "%c", file_i_value);
}

// Allow user space to WRITE a character value
static ssize_t file_i_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a char value\n", __func__);
    sscanf(buf, "%c", &file_i_value);
    pr_err("Sairam 10: %s Read from userspace a char value of: %c\n",
            __func__, file_i_value);
    return count;
}

static struct kobj_attribute file_i_attr = __ATTR(file_i, 0664, file_i_show, file_i_store);

/* File j - Encodes an integer */

static int file_j_value = 1926;

// Allow user space to READ a character value
static ssize_t file_j_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a integral value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a integral value of: %d\n",
            __func__, file_j_value);
    return sprintf(buf, "%d", file_j_value);
}

// Allow user space to WRITE a character value
static ssize_t file_j_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a integral value\n", __func__);
    sscanf(buf, "%d", &file_j_value);
    pr_err("Sairam 10: %s Read from userspace a integral value of: %d\n",
        __func__, file_j_value);
    
    return count;
}

static struct kobj_attribute file_j_attr = __ATTR(file_j, 0664, file_j_show, file_j_store);


/* File k - Encodes a char */

static char file_k_value = 'R';

// Allow user space to READ a character value
static ssize_t file_k_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a char value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a char value of: %c\n",
            __func__, file_k_value);
    return sprintf(buf, "%c", file_k_value);
}

// Allow user space to WRITE a character value
static ssize_t file_k_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a char value\n", __func__);
    sscanf(buf, "%c", &file_k_value);
    pr_err("Sairam 10: %s Read from userspace a char value of: %c\n",
            __func__, file_k_value);
    return count;
}

static struct kobj_attribute file_k_attr = __ATTR(file_k, 0664, file_k_show, file_k_store);

/* File x - Encodes a char */

static char file_x_value = 'R';

// Allow user space to READ a character value
static ssize_t file_x_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a char value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a char value of: %c\n",
            __func__, file_x_value);
    return sprintf(buf, "%c", file_x_value);
}

// Allow user space to WRITE a character value
static ssize_t file_x_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a char value\n", __func__);
    sscanf(buf, "%c", &file_x_value);
    pr_err("Sairam 10: %s Read from userspace a char value of: %c\n",
            __func__, file_x_value);
    
    return count;
}

static struct kobj_attribute file_x_attr = __ATTR(file_x, 0664, file_x_show, file_x_store);

/* File y - Encodes an integer */

static int file_y_value = 424;

// Allow user space to READ a integral value
static ssize_t file_y_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a integral value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a integral value of: %d\n",
            __func__, file_y_value);
    return sprintf(buf, "%d", file_y_value);
}

// Allow user space to WRITE a integral value
static ssize_t file_y_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a integral value\n", __func__);
    sscanf(buf, "%d", &file_y_value);
    pr_err("Sairam 10: %s Read from userspace a integral value of: %d\n",
        __func__, file_y_value);
    return count;
}

static struct kobj_attribute file_y_attr = __ATTR(file_y, 0664, file_y_show, file_y_store);

/* File z - Encodes a string */

static char file_z_value[108] = "Om Sri Sairam";

// Allow user space to READ a string value
static ssize_t file_z_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to read a string value\n", __func__);
    pr_err("Sairam 10: %s Returning to userspace a string value of: %s\n",
        __func__, file_z_value);
    return sprintf(buf, "%s", file_z_value);
}

// Allow user space to WRITE a string value
static ssize_t file_z_store(struct kobject *kobj,
						struct kobj_attribute *attr,
						const char *buf, size_t count)
{
    // Ensure the buffer is not null
    if (buf == NULL)
        pr_err("Sairam 10: %s Input buffer is null\n", __func__);

    pr_err("Sairam 10: %s Call to write a string value\n", __func__);
    strncpy(file_z_value, buf, sizeof(file_z_value) - 1);
    file_z_value[sizeof(file_z_value) - 1] = '\0';
    pr_err("Sairam 10: %s Read from userspace a string value of: %s\n",
        __func__, file_z_value);
    return count;
}

static struct kobj_attribute file_z_attr = __ATTR(file_z, 0664, file_z_show, file_z_store);




int ram_sysfs_free_dir_1(void)
{
    sysfs_remove_file(ram_sysfs_dir_1, &file_a_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_a from directory dir_1\n", __func__);

    sysfs_remove_file(ram_sysfs_dir_1, &file_b_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_b from directory dir_1\n", __func__);
    
    sysfs_remove_file(ram_sysfs_dir_1, &file_c_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_c from directory dir_1\n", __func__);
    
    return 0;
}

int ram_sysfs_init_dir_1(void)
{
    int retval;

    /* Create file_x, file_y, and file_z in the dir_1 directory */
    retval = sysfs_create_file(ram_sysfs_dir_1, &file_a_attr.attr);
    if (retval)
        return retval;

    retval = sysfs_create_file(ram_sysfs_dir_1, &file_b_attr.attr);
    if (retval)
        goto error_b;

    retval = sysfs_create_file(ram_sysfs_dir_1, &file_c_attr.attr);
    if (retval)
        goto error_c;

    return 0;

error_c:
    sysfs_remove_file(ram_sysfs_dir_1, &file_b_attr.attr);
error_b:
    sysfs_remove_file(ram_sysfs_dir_1, &file_c_attr.attr);
    
    return retval;
}

int ram_sysfs_free_dir_2(void)
{
    sysfs_remove_file(ram_sysfs_dir_2, &file_i_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_i from directory dir_2\n", __func__);
    
    sysfs_remove_file(ram_sysfs_dir_2, &file_j_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_j from directory dir_2\n", __func__);

    sysfs_remove_file(ram_sysfs_dir_2, &file_k_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_k from directory dir_2\n", __func__);

    return 0;
}

int ram_sysfs_init_dir_2(void)
{
    int retval;

    /* Create file_i, file_j, and file_k in the dir_2 directory */
    retval = sysfs_create_file(ram_sysfs_dir_2, &file_i_attr.attr);
    if (retval)
        return retval;

    retval = sysfs_create_file(ram_sysfs_dir_2, &file_j_attr.attr);
    if (retval)
        goto error_j;

    retval = sysfs_create_file(ram_sysfs_dir_2, &file_k_attr.attr);
    if (retval)
        goto error_k;

    return 0;

error_k:
    sysfs_remove_file(ram_sysfs_dir_2, &file_j_attr.attr);
error_j:
    sysfs_remove_file(ram_sysfs_dir_2, &file_i_attr.attr);
    
    return retval;
}

int ram_sysfs_free_dir_3(void)
{
    sysfs_remove_file(ram_sysfs_dir_3, &file_x_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_x from directory dir_3\n", __func__);
    
    sysfs_remove_file(ram_sysfs_dir_3, &file_y_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_y from directory dir_3\n", __func__);
    
    sysfs_remove_file(ram_sysfs_dir_3, &file_z_attr.attr);
    pr_err("Sairam 10: %s Tearing down file_z from directory dir_3\n", __func__);
    
    return 0;
}

int ram_sysfs_init_dir_3(void)
{
    int retval;

    /* Create file_a, file_b, and file_c in the dir_3 directory */
    retval = sysfs_create_file(ram_sysfs_dir_3, &file_x_attr.attr);
    if (retval)
        return retval;

    retval = sysfs_create_file(ram_sysfs_dir_3, &file_y_attr.attr);
    if (retval)
        goto error_y;

    retval = sysfs_create_file(ram_sysfs_dir_3, &file_z_attr.attr);
    if (retval)
        goto error_z;

    return 0;

error_z:
    sysfs_remove_file(ram_sysfs_dir_3, &file_y_attr.attr);
error_y:
    sysfs_remove_file(ram_sysfs_dir_3, &file_z_attr.attr);
    
    return retval;
}


/* Initialize the root directory and its files and sub-directories */
int ram_sysfs_init(void)
{
    int retval;

    /* Create root directory: The symbol kernel_kobj is
     * globally available and is defined by kernel core
     */
    ram_sysfs_root = kobject_create_and_add(RAM_ROOT_DIR, kernel_kobj);
    if (!ram_sysfs_root)
        return -ENOMEM;
    
    /* Create a sub-directory */
    ram_sysfs_dir_1 = kobject_create_and_add(RAM_CHILD_DIR_1, ram_sysfs_root);
    if (!ram_sysfs_dir_1)
        goto error_1;
    
    /* Create a sub-directory */
    ram_sysfs_dir_2 = kobject_create_and_add(RAM_CHILD_DIR_2, ram_sysfs_root);
    if (!ram_sysfs_dir_2)
        goto error_2;
    
    /* Create a sub-directory */
    ram_sysfs_dir_3 = kobject_create_and_add(RAM_CHILD_DIR_3, ram_sysfs_root);
    if (!ram_sysfs_dir_3)
        goto error_3;

    retval = ram_sysfs_init_dir_1();
    if (retval)
        goto error_4;

    retval = ram_sysfs_init_dir_2();
    if (retval)
        goto error_5;

    retval = ram_sysfs_init_dir_3();
    if (retval)
        goto error_6;

    return 0;

error_6:
    ram_sysfs_free_dir_2();
error_5:
    ram_sysfs_free_dir_1();
error_4:
    kobject_put(ram_sysfs_dir_3);
error_3:
    kobject_put(ram_sysfs_dir_2);
error_2:
    kobject_put(ram_sysfs_dir_1);
error_1:
    kobject_put(ram_sysfs_root);

    return -ENOMEM;

}


/* Cleanup - Remove all directories and files */
void ram_sysfs_exit(void)
{
    ram_sysfs_free_dir_3();
    kobject_put(ram_sysfs_dir_3);
    pr_err("Sairam 10: %s Tearing down directory dir_3\n", __func__);

    ram_sysfs_free_dir_2();
    kobject_put(ram_sysfs_dir_2);
    pr_err("Sairam 10: %s Tearing down directory dir_2\n", __func__);
    
    ram_sysfs_free_dir_1();
    kobject_put(ram_sysfs_dir_1);
    pr_err("Sairam 10: %s Tearing down directory dir_1\n", __func__);
    
    kobject_put(ram_sysfs_root);
}






