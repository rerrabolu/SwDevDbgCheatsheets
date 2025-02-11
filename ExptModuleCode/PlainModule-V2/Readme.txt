
The module extends the simple module by adding code to register
itself a "char" device. This version of char device does not
support any file operations such as 'open", "close", "mmap",
etc. It does the device registration as part of module init
and tears down the same as part of moodule exit.

All that a user can do is the following:
	Load the module
		Prints a diagnostic image
	Check device registration - /dev/re_device_2
		This should be present
	Unload the module
		Prints a diagnostic image
	Check device registration - /dev/re_device_2
		This should be gone

 
