#!/usr/bin/env bash
#
# Builds cscope and ctags databases for the AMD GPU driver
# source code (amdgpu, amdkfd, etc) of the ROCm
# amdgpu kernel tree, to browse and jump to symbol definitions
# using vim.
#
# The list of files to TAG is collected into a temporary file
# and kept as rockSrcList.txt next to the databases.
#
ROCK_ROOT=${HOME}/Work_Repos/Rock
AMDGPU_ROOT="${ROCK_ROOT}/amdgpu"
CODENAV_ROOT="${ROCK_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
rockCTags="${CODENAV_ROOT}/rockCTags"
rockCscope="${CODENAV_ROOT}/rockCscope.out"
rockSrcList="${CODENAV_ROOT}/rockSrcList.txt"

#
# Directories whose C header and implementation
# files are tagged. Each directory is scanned
# recursively, including its subdirectories
#
AMD_ROOT="${AMDGPU_ROOT}/drivers/gpu/drm/amd"
srcDirs=(
    # Source code of amdgpu driver
    "${AMD_ROOT}/amdgpu"

    # Source code of amdkfd (compute) driver
    "${AMD_ROOT}/amdkfd"

    # Headers of hardware registers and ASICs shared
    # by the drivers
    "${AMD_ROOT}/include"

    # Source code of partitioning support
    "${AMD_ROOT}/amdxcp"

    # Source code of kernel compatibility layer. Exists
    # only in the ROCm tree to build on older kernels
    "${AMD_ROOT}/amdkcl"

    # Source code backported from newer kernels. Exists
    # only in the ROCm tree
    "${AMD_ROOT}/backport"

    # Headers of DKMS packaging. Exists only in the ROCm tree
    "${AMD_ROOT}/dkms"

    # Source code of Display Core. Uncomment to tag
    # "${AMD_ROOT}/display"

    # Source code of power management. Uncomment to tag
    # "${AMD_ROOT}/pm"

    # Source code of RAS error handling. Uncomment to tag
    # "${AMD_ROOT}/ras"

    # Source code of audio co-processor. Uncomment to tag
    # "${AMD_ROOT}/acp"
)

#
# Directories under srcDirs that are NOT tagged.
# Uncomment to skip them
#
excludeDirs=(
    # Headers of hardware register definitions. They make
    # up most of the tags database. Comment out to tag them
    "${AMD_ROOT}/include/asic_reg"
)

#
# Individual files that are tagged. Headers exported to
# user space carrying the ioctl definitions of the drivers
#
srcFiles=(
    # ioctls of amdgpu driver
    "${AMDGPU_ROOT}/include/uapi/drm/amdgpu_drm.h"

    # ioctls and types of DRM used by amdgpu_drm.h
    "${AMDGPU_ROOT}/include/uapi/drm/drm.h"

    # ioctls of amdkfd driver
    "${AMDGPU_ROOT}/include/uapi/linux/kfd_ioctl.h"

    # Topology properties of amdkfd exported via sysfs
    "${AMDGPU_ROOT}/include/uapi/linux/kfd_sysfs.h"
)

echo ""
echo "Generating Tags for AMDGPU Source Code"

#
# Stop if any of the directories or files to tag is missing
#
for dir in "${srcDirs[@]}"; do
    if [ ! -d "${dir}" ]; then
        echo "Error: directory not found: ${dir}"
        exit 1
    fi
done
for file in "${srcFiles[@]}"; do
    if [ ! -f "${file}" ]; then
        echo "Error: file not found: ${file}"
        exit 1
    fi
done

mkdir -p "${CODENAV_ROOT}"
rm -f "${srcList}" "${rockCTags}" "${rockCscope}" "${rockCscope}.in" "${rockCscope}.po"

#
# Build the find arguments that skip the excluded directories
#
pruneArgs=()
for dir in "${excludeDirs[@]}"; do
    pruneArgs+=( -path "${dir}" -prune -o )
done

#
# Generate the list of source files to TAG. Files that
# appear more than once are listed only once
#
{
    find "${srcDirs[@]}" "${pruneArgs[@]}" -type f \( -name '*.c' -o -name '*.h' \) -print
    printf '%s\n' "${srcFiles[@]}"
} | sort -u > "${srcList}"

#
# Edit the source file list to remove files we are not
# interested in e.g. products of a kernel build
#
sed -i '/build/d' "${srcList}"

numFiles=$(wc -l < "${srcList}")
if [ "${numFiles}" -eq 0 ]; then
    echo "Error: no .c or .h files found"
    rm -f "${srcList}"
    exit 1
fi
echo "Found ${numFiles} source files"
echo ""

#
# Build the TAGS database files
#
echo "Building AMDGPU Tags database"
if ! cscope -i "${srcList}" -q -b -f "${rockCscope}" 2> /dev/null || [ ! -s "${rockCscope}" ]; then
    echo "Error: cscope failed to build ${rockCscope}"
    exit 1
fi
if ! ctags --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px --extras=+f \
           -L "${srcList}" -f "${rockCTags}" 2> /dev/null || [ ! -s "${rockCTags}" ]; then
    echo "Error: ctags failed to build ${rockCTags}"
    exit 1
fi

#
# Completed the building of AMDGPU Tags file. Keep the
# list of files that were tagged
#
mv "${srcList}" "${rockSrcList}"
echo "Completed the building of AMDGPU Tags"
echo ""
echo "To use in vim:"
echo "    :set tags+=${rockCTags}"
echo "    :cs add ${rockCscope}"
echo ""
