
Linux kernel provides a generic implementation of "For Loop" to process
lists. Users who wish to process elements of a list can use this service
to obtain each element and take appropriate actions

The list being traversed is assumed to have been built using Kernel's list
service i.e. it employs a linked list that is:

    --- Circuluar
    --- Doubly Linked

Assuming one has built a linked list using kernel's list service, it can be
used as follows:

To Access Each Element Of a List, involves two steps:

    1.  Get each element of list as an instance of type "struct list_head"
    2.  Cast the list_head instance into its encapsulating data type

    For example assume we have built a list of nodes whose type and contents
    are as given below. The list of nodes is anchored at symbol "usrDataList",
    which is an instance of list_head. To print the payload of each node one
    can do the following:
        
        // Used to chain a list of nodes of type "struct usrDataStruct"
        LIST_HEAD(usrDataList);

        // Defines the payload of each instance of this type
        struct usrDataStruct {
            uint32_t data;
            struct list_head list;
        }


        // Build a list of nodes which is accomplished in two
        // phases: Instantiating a node and then adding it to
        // the list
        struct usrDataStruct* node;
        for(uint32_t idx = 0; idx < 23; idx++) {

            // Allocate an instance and initialize it
            node = malloc(sizeof(struct usrDataStruct));
            node->data = idx * 23;
            INIT_LIST_HEAD(&node->list);

            // Add this node to the list of other nodes being
            // maintained by the list symbol "usrDataList"
            list_add(&(node->list), &usrDataList);
        }


        // Prints out all the nodes currently in the list
        struct list_head* currElem;
        struct usrDataStruct* currNode;
        list_for_each(currElem, &usrDataList){
            currNode = list_entry(currElem, struct usrDataStruct, list);
            printf("Data = %d\n", currNode->data);
        }

        // Alternatively one can use the macro for each entry as well
        // @note: It is critical tha the third parameter refer to the
        // field's name from struct type referred to by first parameter
        // In this example type of first param is "struct usrDataStruct"
        // which has a field by the name "list" which is what is passed
        // in as third parameter
        struct usrDataStruct* currNode;
        list_for_each_entry(currNode, &usrDataList, list);
            printf("Data = %d\n", currNode->data);


