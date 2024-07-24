**Nume:Matei Tudor-Andrei**
**Grupă:314CA**

## Segregated Free Lists (Tema 1)

### Description:

* The program represents a virtual memory allocator.
* In order to build it, the free memory was stored into an array of doubly 
linked lists and the allocated blocks were kept as the nodes of a linked list.
The data given to the 2 type of nodes was stored in 2 structures, both of them 
having the block_size and address fields. The difference between them was a
void* field, which is used to store the actual information written at a given
address in a allocated block.
* The memory allocator can malloc, free, write, read and give information about
the virtual memory that's being used.

* The commands that can be given within the terminal are the following:

#### INIT_HEAP

* receives the start address of the heap, the number of lists that are going
to be used, the number of bytes for each list

* initializes the doubly linked lists and the linked list

* add the nodes, that represents the free blocks, to the array for each list
and also assign them a starting address and a size.

#### MALLOC

* receives the number of bytes of the block that will be allocated
* checks if there is enough memory by comparing the given amount of bytes
to the block size of every head of the lists in the array
* if there isn't enough memory, then an error message will be printed
* if enough memory is found, then a new node will be created in the
list for the allocated blocks and the head that's big enough with the lowest 
value of the starting address will be removed from the its list.


#### FREE
* receives an address
* checks if there is any allocated block that starts at that address
* if there is no such block, an error message in printed
* if a block is found, then its corresponding node will be removed
from the linked list and added into the doubly linked lists from the
array
* if the block has a size that already exists in the array, then
it will be placed in an already existing list (the list will be sorted
by their nodes' addresses at any time)
* if the array doesn't have any lists with needed block_size, then a new list
is created and added into the array (the array is sorted by the lists'
heads' sizes)

#### WRITE
* receives an address, a string and a number of bytes that will be written
* we will iterate between the nodes of the linked list to search for an
allocated block that contains the given address
* if the block is found, then the search continues until there is a gap
between the addresses of the blocks or there are enough bytes to write
the given string
* if the string can't be written, then an error message and information about
the used memory will be printed and the program will end after freeing all the 
used resources
* if there is enough contiguous virtual memory, then the field for the string
in the data of the node will be store a part of the string or the entire one
(depending on the case)

#### READ
* receives an address and a number of bytes that will be written
* we will iterate between the nodes of the linked list to search for an
allocated block that contains the given address
* if the block is found, then the search continues until there is a gap
between the addresses of the blocks or there have been found enough bytes
to read from
* if the information from the given amount of bytes can't be read, 
then an error message and information about the used memory will be printed
and the program will end after freeing all the used resources
* if there is enough contiguous virtual memory, then the reading process will
happen byte by byte, using the information stored in the allocated node

#### DUMP_MEMORY
* prints detailed info about the virtual memory: total memory, allocated
memory, free memory, number of free blocks, of allocated blocks, of malloc
calls, of fragmentations, of free calls, of free blocks and block size for each
list from the list array and the address of every block (free and allocated)

#### DESTROY_HEAP
* frees all of the used resources and exits the program 

