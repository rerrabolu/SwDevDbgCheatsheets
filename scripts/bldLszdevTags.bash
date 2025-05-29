#
# The list of files to TAG by CSCOPE is collected
# into a file called srcList.txt in the current directory.
#
currDir=${PWD}
ZDEV_ROOT=${HOME}/Work_Repos
CODENAV_ROOT="${ZDEV_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
lszDevCTags="${CODENAV_ROOT}/lszDevCTags"
lszDevCscope="${CODENAV_ROOT}/lszDevCscope.out"
lszDevSrcList="${CODENAV_ROOT}/lszDevSrcList.txt"

# Documentation of various tools of s390 is located
# here: cd $ZDEV_ROOT/s390-tools/zdev


#
# Generate the list of source files to TAG.
#
# @note: Syntax is kind of weird, mixing Windows
# and Unix styles.
#
echo ""
mkdir -p ${CODENAV_ROOT}
echo "Generating Tags for LSZDEV Source Code"

rm -f $srcList
rm -f ${lszDevCTags}*
rm -f ${lszDevCscope}*

#
# Tag source code for general LSZDEV support
#
cd $ZDEV_ROOT/s390-tools/zdev/src
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for s390 headers
#
cd $ZDEV_ROOT/s390-tools/zdev/include
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for s390 headers
#
cd $ZDEV_ROOT/s390-tools/include/lib
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for tests of LSZDEV support
#
cd $ZDEV_ROOT/s390-tools/zdev/tests
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code of pci ioctls
#
cd $ZDEV_ROOT/s390-tools/zpcictl
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code of lspci mock up
# It appears to encode the logic used
# to enumerate PCIe devices
#
cd $ZDEV_ROOT/s390-tools/libzpci
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Edit the source file list to remove files we are not
# interested in
#
echo "Removing unwanted files - e.g. obj, sym, map, etc"

# Remove object files from the list

sed -i '/\.obj/d' $srcList
sed -i '/\.lib/d' $srcList
sed -i '/\.sys/d' $srcList
sed -i '/\.flags\.d/d' $srcList
echo ""

#
# Build the TAGS database file
#
echo "Building PCIe Tags database"
cscope -i $srcList -q -R -b -f $lszDevCscope 2> /dev/null
ctags -R --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px  --extra=f -L $srcList -f $lszDevCTags 2> /dev/null
echo ""

#
# Completed the building of LSZDEV Tags file. Cleanup
# all intermediate products.
#
mv $srcList $lszDevSrcList
echo "Completed the building of LSZDEV Tags"
echo ""

#
# Change back to the original directory
#
cd $currDir

