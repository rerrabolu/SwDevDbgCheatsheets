
The module extends the simple device by adding code to allow some
file operation. For example it supports Read/Write file operations
while it does not support other operations such as "mmap", "ioctl",
etc.  However it differs from the other rw_device's in that the
file operations are defined in another file and are referenced
via extern

It does the device registration as part of module init and
tears it down as part of moodule exit.

All that a user can do is the following:
	Load the module
		Prints a diagnostic image
	Check device registration - /dev/re_device_8
		This should be present
	Write to the device
		sudo echo "Om Sairam, Jai Sairam!" > /dev/re_device_8
	Read from the device
		sudo cat /dev/rw_device_8
	Unload the module
		Prints a diagnostic image
	Check device registration - /dev/re_device_8
		This should be gone

 
