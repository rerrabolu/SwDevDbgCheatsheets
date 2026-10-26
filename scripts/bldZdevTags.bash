#!/usr/bin/env bash
#
# Builds cscope and ctags databases for the zdev (lszdev/chzdev)
# source code of s390-tools, to browse and jump to symbol
# definitions using vim.
#
# The list of files to TAG is collected into a temporary file
# and kept as zdevSrcList.txt next to the databases.
#
ZDEV_ROOT=${HOME}/Work_Repos/ZTools
S390_ROOT="${ZDEV_ROOT}/s390-tools"
CODENAV_ROOT="${ZDEV_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
zdevCTags="${CODENAV_ROOT}/zdevCTags"
zdevCscope="${CODENAV_ROOT}/zdevCscope.out"
zdevSrcList="${CODENAV_ROOT}/zdevSrcList.txt"

# Documentation of various tools of s390 is located
# here: cd $S390_ROOT/zdev

#
# Directories whose C header and implementation
# files are tagged
#
srcDirs=(
    # Source code for general LSZDEV support
    "${S390_ROOT}/zdev/src"

    # Headers of LSZDEV support
    "${S390_ROOT}/zdev/include"

    # Headers of s390 libraries
    "${S390_ROOT}/include/lib"

    # Source code of lspci mock up. It appears to
    # encode the logic used to enumerate PCIe devices
    "${S390_ROOT}/libzpci"

    # Source code for tests of LSZDEV support. They appear
    # to be absent in public repository. Uncomment to tag
    # "${S390_ROOT}/zdev/tests"
)

echo ""
echo "Generating Tags for ZDEV Source Code"

#
# Stop if any of the directories to tag is missing
#
for dir in "${srcDirs[@]}"; do
    if [ ! -d "${dir}" ]; then
        echo "Error: directory not found: ${dir}"
        exit 1
    fi
done

mkdir -p "${CODENAV_ROOT}"
rm -f "${srcList}" "${zdevCTags}" "${zdevCscope}" "${zdevCscope}.in" "${zdevCscope}.po"

#
# Generate the list of source files to TAG
#
find "${srcDirs[@]}" -type f \( -name '*.c' -o -name '*.h' \) | sort > "${srcList}"
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
echo "Building ZDEV Tags database"
if ! cscope -i "${srcList}" -q -b -f "${zdevCscope}" 2> /dev/null || [ ! -s "${zdevCscope}" ]; then
    echo "Error: cscope failed to build ${zdevCscope}"
    exit 1
fi
if ! ctags --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px --extras=+f \
           -L "${srcList}" -f "${zdevCTags}" 2> /dev/null || [ ! -s "${zdevCTags}" ]; then
    echo "Error: ctags failed to build ${zdevCTags}"
    exit 1
fi

#
# Completed the building of ZDEV Tags file. Keep the
# list of files that were tagged
#
mv "${srcList}" "${zdevSrcList}"
echo "Completed the building of ZDEV Tags"
echo ""
echo "To use in vim:"
echo "    :set tags+=${zdevCTags}"
echo "    :cs add ${zdevCscope}"
echo ""
