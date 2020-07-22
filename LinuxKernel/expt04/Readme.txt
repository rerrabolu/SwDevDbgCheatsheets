Create a simple module that does the following
  The name of the module will be Sairam-104
  All it does is print when loaded and unloaded
  The module will create and register a device object
  The module will create nodes in SysFs with 4 attributes
    Two attributes will be used to read some parameter
      The parameters will be of different types
    The remaining two attributes will be used to write some parameter
      The parameters will be of different types
  The module will be built outside of the Linux Kernel Code Tree

Validation:
  See the printouts in dmesg log
  Insmod the module
  Rmmod the module
  Modinfo the module
  See the device object show up on /dev hierarchy
  See the SysFS hierarchy has the object being surfaced
  See the SysFs objects show up on /sys hierarchy
  Perform the read and write operations



