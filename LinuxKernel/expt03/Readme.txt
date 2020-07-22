Create a simple module that does the following
  The name of the module will be Sairam-103
  All it does is print when loaded and unloaded
  The module will create and register a device object
  The module will create nodes in SysFs with no attributes
  The module will be built outside of the Linux Kernel Code Tree

Validation:
  See the printouts in dmesg log
  Insmod the module
  Rmmod the module
  Modinfo the module
  See the device object show up on /dev hierarchy
  See the SysFS hierarchy has the object being surfaced


