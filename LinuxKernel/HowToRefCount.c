

https://www.kernel.org/doc/html/latest/core-api/kref.html

To reference count an instance of a structure simply add as member field an
instance of struct ref. Following is illustrated via code:

	// Structure without support for reference counting
	struct foo {
		int x;
		int y;
		int z;
	};

	// Structure with support for reference counting
	struct foo {
		int x;
		int y;
		int z;
		struct kref ref;
	};

Steps:

1.	The instance of kref can be located anywhere i.e. there is nothing
	gained or lost by having it as the first or last member field

2.	Referene count object must be initialized once an instance of the
	encapsulating is create. This sets the reference count to 1

3.	Kernel provides api's to get and put reference count
		kref_get(struct kref *ref);
		kref_put(struct kref *ref, releaseFunction);


Extend struct kgd_mem to be reference counted

1.	Add an instance of struct kref;
2.	Initialize kref in alloc function;
3.	IOCTL calls:
	3A.	Increment reference count upon entry into kernel
	3B.	Decrement reference count upon leaving from kernel
4.  Free IOCTL
	4A. Check reference count
	4B. If more than ONE return
	4B. If ONE proceed to free kfd bo

