#!/usr/bin/env bash
#
# Builds cscope and ctags databases for the PCIe source code
# of the Linux kernel, with focus on s390 support, to browse
# and jump to symbol definitions using vim.
#
# The list of files to TAG is collected into a temporary file
# and kept as pciSrcList.txt next to the databases.
#
PCI_ROOT=${HOME}/Work_Repos/LnxKrnl
LINUX_ROOT="${PCI_ROOT}/linux"
CODENAV_ROOT="${PCI_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
pciCTags="${CODENAV_ROOT}/pciCTags"
pciCscope="${CODENAV_ROOT}/pciCscope.out"
pciSrcList="${CODENAV_ROOT}/pciSrcList.txt"

# Documentation of various aspects of s390 systems
# is located at the following location. The one on
# PCIe is called pci.rst
# cd $LINUX_ROOT/Documentation/arch/s390/

#
# Directories whose C header and implementation
# files are tagged. Each directory is scanned
# recursively, including its subdirectories
#
srcDirs=(
    # Source code for general PCIe support. Includes
    # pcie, endpoint, hotplug and controller subdirectories
    "${LINUX_ROOT}/drivers/pci"

    # Source code for general PCIe support. Already
    # included by drivers/pci above
    # "${LINUX_ROOT}/drivers/pci/pcie"

    # Source code for general PCIe Endpoint support.
    # Already included by drivers/pci above
    # "${LINUX_ROOT}/drivers/pci/endpoint"

    # Source code for PCIe on s390
    "${LINUX_ROOT}/arch/s390/pci"

    # Source code for s390 PCIe headers
    "${LINUX_ROOT}/arch/s390/include/asm"
)

#
# Individual files that are tagged
#
srcFiles=(
    # Source code for s390 Hotplug support. Already
    # included by drivers/pci above
    "${LINUX_ROOT}/drivers/pci/hotplug/s390_pci_hpc.c"
)

echo ""
echo "Generating Tags for PCIe Source Code"

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
rm -f "${srcList}" "${pciCTags}" "${pciCscope}" "${pciCscope}.in" "${pciCscope}.po"

#
# Generate the list of source files to TAG. Files that
# appear more than once are listed only once
#
{
    find "${srcDirs[@]}" -type f \( -name '*.c' -o -name '*.h' \)
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
echo "Building PCIe Tags database"
if ! cscope -i "${srcList}" -q -b -f "${pciCscope}" 2> /dev/null || [ ! -s "${pciCscope}" ]; then
    echo "Error: cscope failed to build ${pciCscope}"
    exit 1
fi
if ! ctags --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px --extras=+f \
           -L "${srcList}" -f "${pciCTags}" 2> /dev/null || [ ! -s "${pciCTags}" ]; then
    echo "Error: ctags failed to build ${pciCTags}"
    exit 1
fi

#
# Completed the building of PCIe Tags file. Keep the
# list of files that were tagged
#
mv "${srcList}" "${pciSrcList}"
echo "Completed the building of PCIe Tags"
echo ""
echo "To use in vim:"
echo "    :set tags+=${pciCTags}"
echo "    :cs add ${pciCscope}"
echo ""
