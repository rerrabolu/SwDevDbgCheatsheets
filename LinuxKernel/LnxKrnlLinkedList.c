
Linux kernel provides a generic implementation of linked lists. Users who
need to build and maintain various lists can use this service instead of
implementing it themselves.

The linked list service provided by Linux kernel assumes the list is:

    --- Is Circuluar
    --- Doubly Linked

The standard implementation was added to Linux kernel back in 2.1.45 version
It consists of struct list_head and a bunch of apis/macros to access list
functionality. All of these entities are in header file <linux/list.h>

    struct list_head {
        struct list_head *next, *prev;
    };

Users who wish to use this service can simply build a struct that is a
union of user data plus a member to encode list_head. For example user
wants to maintain a list of a numbers. This can be achieved by following
the two steps below:

    // Define and initialize the head of the list
    // This can be achieved statically or dynamically
     
        // Define and initialize Dynamically
        struct list_head usrDataList;
        INIT_LIST_HEAD(&usrDataList);
        
        // Define and initialize Statically
        // The macro will expand to statements that both
        // define and initialization the list head
        LIST_HEAD(usrDataList);

    // Define the user data structure that would form
    // the elements of the list
    struct usrDataStruct {
        uint32_t data;
        struct list_head list;
    }

Users can begin to build/maintain the linked list without regard to
how the head of the list was constructed - statically or dynamically
For example to add a new element one can do the following:

    // Define a new instance of user data
    struct usrDataStruct *node;
    node = kmalloc(sizeof(node));

    // Initialize object with an appropriate value
    node->data = 0x11231926;

    // Initialize the list member of object
    INIT_LIST_HEAD(&node->list);

    // Add the object to list of user data
    // @note: Assumes usrDataList has been defined
    // earlier to contain elements of usrDataStruct
    list_add(&node.list, &usrDataList);

It is possible for users to define a structure that has a set of lists
For example one can define a struct as follows:

    struct usrLists {
      struct list_head idList;
      struct list_head dataList;
    }

One can then do something like the following:

  // Define and initialize the container of lists
  struct usrLists *usrLists;
  usrLists = kmalloc(sizeof(usrLists));

  // Initialize the members to point to various lists
  // @note: Assumes usrIdList and usrDataList have been
  // defined and initialized before
  usrLists->idList = usrIdList;
  usrLists->dataList = usrDataList;

With above code block one can access user ID Data as follows:
  
  list_add(&newIdNode->list, &usrLists->usrIdList);
  list_add(&newDataNode->list, &usrLists->usrDataList);




