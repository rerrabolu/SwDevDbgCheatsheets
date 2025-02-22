#!/usr/bin/bash

# set -x

# SysFs directories
dir_1='/sys/kernel/ram_sysfs_root/ram_sysfs_dir_1'
dir_2='/sys/kernel/ram_sysfs_root/ram_sysfs_dir_2'
dir_3='/sys/kernel/ram_sysfs_root/ram_sysfs_dir_3'

# 
# Operations of files of directory 1
#

# Read and write a file that encapsulates a char
cat ${dir_1}/file_a
echo ""
echo 'S' | tee ${dir_1}/file_a
echo ""

# Read and write a file that encapsulates an integer
cat ${dir_1}/file_b
echo ""
echo 23 | tee ${dir_1}/file_b
echo ""

# Read and write a file that encapsulates a string
cat ${dir_1}/file_c
echo ""
echo "Jaya Narayana Jaya Hari Om" | tee ${dir_1}/file_c
echo ""


# 
# Operations of files of directory 2
#

# Read and write a file that encapsulates a char
cat ${dir_2}/file_i
echo ""
echo 'M' | tee ${dir_2}/file_i
echo ""

# Read and write a file that encapsulates an integer
cat ${dir_2}/file_j
echo ""
echo 26 | tee ${dir_2}/file_j
echo ""

# Read and write a file that encapsulates a char
cat ${dir_2}/file_k
echo ""
echo 'I' | tee ${dir_2}/file_k
echo ""

# 
# Operations of files of directory 3
#

# Read and write a file that encapsulates a char
cat ${dir_3}/file_x
echo ""
echo 'P' | tee ${dir_3}/file_x
echo ""

# Read and write a file that encapsulates an integer
cat ${dir_3}/file_y
echo ""
echo 23 | tee ${dir_3}/file_y
echo ""

# Read and write a file that encapsulates a string
cat ${dir_3}/file_z
echo ""
echo "Jagan Mate Jaga Janani Sairam" | tee ${dir_3}/file_z
echo ""



