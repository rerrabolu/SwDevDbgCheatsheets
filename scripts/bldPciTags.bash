#
# The list of files to TAG by CSCOPE is collected
# into a file called srcList.txt in the current directory.
#
currDir=${PWD}
PCI_ROOT=${HOME}/Work_Repos
CODENAV_ROOT="${PCI_ROOT}/codeNav"
srcList="${CODENAV_ROOT}/tmpSrcList.txt"
pciCTags="${CODENAV_ROOT}/pciCTags"
pciCscope="${CODENAV_ROOT}/pciCscope.out"
pciSrcList="${CODENAV_ROOT}/pciSrcList.txt"

# Documentation of various aspects of s390 systems
# is located at the following location. The one on
# PCIe is called pci.rst
# cd $PCI_ROOT/linux/Documentation/arch/s390/


#
# Generate the list of source files to TAG.
#
# @note: Syntax is kind of weird, mixing Windows
# and Unix styles.
#
echo ""
mkdir -p ${CODENAV_ROOT}
echo "Generating Tags for PCIe Source Code"

rm -f $srcList
rm -f ${pciCTags}*
rm -f ${pciCscope}*

#
# Tag source code for general PCIe support
#
cd $PCI_ROOT/linux/drivers/pci
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for general PCIe support
#
# cd $PCI_ROOT/linux/drivers/pci/pcie
# find `pwd` -iname '*.c*' >> $srcList
# find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for general PCIe Endpoint support
#
# cd $PCI_ROOT/linux/drivers/pci/endpoint
# find `pwd` -iname '*.c*' >> $srcList
# find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for PCIe
#
cd $PCI_ROOT/linux/arch/s390/pci
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for s390 PCIe headers
#
cd $PCI_ROOT/linux/arch/s390/include/asm
find `pwd` -iname '*.c*' >> $srcList
find `pwd` -iname '*.h*' >> $srcList

#
# Tag source code for s390 Hotplug support
#
echo "$PCI_ROOT/linux/drivers/pci/hotplug/s390_pci_hpc.c" >> $srcList

#
# Edit the source file list to remove files we are not
# interested in
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
echo "Building PCIe Tags database"
cscope -i $srcList -q -R -b -f $pciCscope 2> /dev/null
ctags -R --fields=+l --langmap=c:.c.h --languages=c --c-kinds=+px  --extra=f -L $srcList -f $pciCTags 2> /dev/null
echo ""

#
# Completed the building of PCIe Tags file. Cleanup
# all intermediate products.
#
mv $srcList $pciSrcList
echo "Completed the building of PCIe Tags"
echo ""

#
# Change back to the original directory
#
cd $currDir

