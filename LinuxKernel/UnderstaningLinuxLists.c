
The way to understand "list" functions of Linux kernel is
as follows:

The HEAD/TAIL of the list is a constant i.e.
	it represents the plain list_head structure
	it serves only one purpose to enter/exit from list
	it does not encapsulate any user data

This HEAD/TAIL could then be added a bunch of nodes that encapsulate
context data. To enable the addition or removal of node it embeds
inside it a field of type list_head. This field is used to hook the
node to the HEAD/TAIL

For example we want to maintain two lists:
	ALIVE to encapsulate nodes that are alive
	DEAD to encapsulate nodes that are dead
	
One way to implement these two list are as follows:

init_list_head(alive);
init_list_head(dead);

for (idx = 0; idx < 100; idx++) {
	node = new node();
	initialize(node, idx);
	init_list_head(node.list);
	list_add(node.list, alive);
}

Given above one can build various list related apis. To
get the first entry one can give the "HEAD" and specify
the type of structure the next list_head will represent.

	<node><node><node><-LH-><node><node><node><node><node>

Using the LH we can go to next element and cast its type
as "node" based on its member field "list"

This basic setup can be used to do various things like iterate
over list elements, to get prev or next elements, first or last,
etc.

For api's that remove an element or add one in the middle one can
give the two nodes that are at play. Since this is a relative operation
we don't need the HEAD/TAIL.

Another way to think about HEAD/TAIL is that they serve the purpose
of navigating a list only ONCE. If we didn't have this SENTINEL element
one will go round and round.



