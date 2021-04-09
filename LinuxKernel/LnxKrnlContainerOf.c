
Linux kernel provides a service called "container_of". Users can use this service
to discover the base address of an instance given the address of a member field.
The concept and usefulness of this service is best understood via an illustration.

Assume the following:

    struct someDataStruct {
      uint32_t foo;
      bool boo;
      list_head idList;
      uint64_t moo;
      char *coo;
      struct randData rand;
      list_head dataList;
    };

Assume user has the address of a string object that is guaranteed to represent
the member "coo". User wants to know the value of boolean flag "boo" that is
part of the same instance of someDataStruct. One can use the container_of macro
provided by Linux kernel to accomplish this task.

  // Obtain the base address of struct someDataStruct instance given the
  // address of member filed "coo". The address of member field "coo" is
  // represented by the object "cooAddr" in code block below
  struct someDataStruct *someDataAddr;
  someDataAddr = container_of(cooAddr, struct someDataStruct, coo);

  // Simply read the value of "boo" field
  bool booValue = someDataAddr->boo;



