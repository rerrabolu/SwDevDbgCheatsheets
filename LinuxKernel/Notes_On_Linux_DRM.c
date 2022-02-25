
"Introduction":
	Notes in this document is taken from the following Linux kerneldocs link: 
		https://www.kernel.org/doc/html/v5.15/gpu/drm-internals.html


	The Linux DRM layer contains code intended to support complex graphics devices
	which usually contain programmable pipelines that are well suited to 3D graphics
	acceleration.

	DRM layer provides several services to graphics drivers, many of them driven by
	the application interfaces it provides through libdrm, the library that wraps
	most of the DRM ioctls. These include memory management, command submission &
	fencing, etc.

	Graphics drivers in the kernel may make use of DRM functions to make tasks like
	memory management, interrupt handling and DMA easier, and provide a uniform
	interface to applications.

	@note: A graphic driver that engages services from DRM layer is also called as
	DRM Driver.

"Driver_Initialization":

	At the core of every DRM driver is an instance of type struct drm_driver. Drivers
	typically initialize this instance statically and pass it to devm_drm_dev_alloc()
	to allocate a device instance. After the device instance is fully initialized it
	can be registered using drm_dev_register() to make it accessible from userspace.
	The struct drm_driver contains static information that describes the driver and
	features it supports, and pointers to methods that the DRM core can call to
	implement the DRM API.

		// Features supported by DRM Driver 
		// @note: Focuses on elements of interest
		u32 driver_features;

		DRIVER_GEM
			Driver use the GEM memory manager.
		DRIVER_MODESET
			Driver supports mode setting interfaces (KMS).
		DRIVER_PRIME
			Driver implements DRM PRIME buffer sharing.

	@note: Consistent with other device drivers, Graphic/DRM drivers are identified
	by Major, Minor and Patch IDs. This info is made available to userspace via the
	IOCtl DRM_IOCTL_VERSION.

"Managing_Ownership_Of_The_Framebuffer_Aperture":

	A graphics device might be supported by different drivers. However only one
	driver can be active at any given time. For example, many systems engage a
	generic graphics driver such as VESA during early stages of boot process only
	to replace it with a dedicated, hardware-specific driver for later stages of
	the boot process. This taking over of device by dedicated driver is achieved
	by following procedure:
		- Remove the generic driver
		- Invoke DRM aperture api to gain ownership

	DRM drivers should call drm_aperture_remove_conflicting_framebuffers() at the
	top of their probe function. The function removes any generic driver that is
	currently associated with the given framebuffer memory.

"Device_Instance_And_Driver_Handling":

	A device instance for a drm driver is represented by struct drm_device. This is
	allocated with drm_dev_alloc(), usually from bus-specific ->:c:func:probe()
	callbacks implemented by the driver. The driver then needs to initialize all o
	the subsystems of drm device like memory management, modesetting support, etc.
	An important part of this is also calling drm_dev_set_unique() to set the
	userspace-visible unique name of this device instance. Finally when everything
	is up and running and ready for userspace the device instance can be published
	using drm_dev_register().

	When cleaning up a device instance everything needs to be done in reverse:
		First unpublish the device instance with drm_dev_unregister()
		Then clean up any resources allocated at device initialization
		Drop the drivers reference to drm_device using drm_dev_unref()

	@note: It is recommended that drivers embed struct drm_device into their own
	device structure, which is supported through drm_dev_init(). Implementation of
	this method internally will invoke drm_dev_alloc() i.e. this call will be in lieu
	of that call.

"Memory_Manager_Initialization":

Every DRM driver requires a memory manager which must be initialized at load time.
DRM currently contains two memory managers, the Translation Table Manager (TTM)
and the Graphics Execution Manager (GEM).

TTM was the first DRM memory manager to be developed and tried to be a one-size
fits-them all solution. It provides a single userspace API to accommodate the need
of all hardware, supporting both Unified Memory Architecture (UMA) devices and
devices with dedicated video RAM (i.e. most discrete video cards). This resulted
in a large, complex piece of code that turned out to be hard to use for driver
development.

GEM started as an Intel-sponsored project in reaction to TTMs complexity. Its
design philosophy is completely different: instead of providing a solution to
every graphics memory-related problems, GEM identified common code between drivers
and created a support library to share it. GEM has simpler initialization and
execution requirements than TTM, but has no video RAM management capabilities
and is thus limited to UMA devices.



