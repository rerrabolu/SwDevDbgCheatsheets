#
# The list of files to TAG by CSCOPE is collected
# into a file called srcList.txt in the current directory.
#
currDir=${PWD}
KFD_ROOT=${HOME}/RamRock
CODENAV_ROOT="${KFD_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
kfdCTags="${CODENAV_ROOT}/kfdCTags"
kfdCscope="${CODENAV_ROOT}/kfdCscope.out"
kfdSrcList="${CODENAV_ROOT}/kfdSrcList.txt"

#
# Generate the list of source files to TAG.
#
# @note: Syntax is kind of weird, mixing Windows
# and Unix styles.
#
echo ""
mkdir -p ${CODENAV_ROOT}
echo "Generating Tags for KFD Source Code"

rm -f $srcList
rm -f ${kfdCTags}*
rm -f ${kfdCscope}*

#
# Tag source code for KFD
#
cd $KFD_ROOT/ROCK-Kernel-Driver/drivers/gpu/drm/amd
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for TTM
#
cd $KFD_ROOT/ROCK-Kernel-Driver/drivers/gpu/drm/ttm
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Edit the source file list to remove files we are not
# interested in: e.g. build/w7/B_Dbg/...., .vcproj, .sln
#
echo "Removing unwanted files - e.g. obj, sym, map, etc"

# Remove object files from the list

sed -i '/\.obj/d' $srcList
sed -i '/\.lib/d' $srcList
sed -i '/\.sym/d' $srcList
sed -i '/\.map/d' $srcList
sed -i '/\.sys/d' $srcList
sed -i '/\.flags\.d/d' $srcList
sed -i '/build/d' $srcList
echo ""

#
# Build the TAGS database file
#
echo "Building KFD Tags database"
cscope -i $srcList -q -R -b -f $kfdCscope 2> /dev/null
ctags -R --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px  --extra=f -L $srcList -f $kfdCTags 2> /dev/null
echo ""

#
# Completed the building of kFD Tags file. Cleanup
# all intermediate products.
#
mv $srcList $kfdSrcList
echo "Completed the building of KFD Tags"
echo ""

#
# Change back to the original directory
#
cd $currDir

