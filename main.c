//Matei Tudor-Andrei 314CA
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "struct.h"
#include "utils.h"
#include "dump_memory.h"
#include "malloc_func.h"
#include "write_func.h"

int main(void)
{
	unsigned long address;
	int n, bytes, current_size = 0, free_mem = 0, alloc_mem = 0;
	int free_blocks = 0, alloc_blocks = 0, frag_no = 0, free_calls = 0;
	int no_bytes;
	int malloc_calls = 0;
	char *command = malloc(100 * sizeof(*command));
	DIE(!command, "malloc () failed");
	dll_t **list_array;
	linked_list_t *allocd;
	scanf("%s", command);
	while (1) {
		if (!strcmp(command, "INIT_HEAP")) {
			init_heap(&address, &n, &bytes, &current_size, &list_array, &allocd
			, &free_mem, &free_blocks);
		} else if (!strcmp(command, "MALLOC")) {
			scanf("%u", &no_bytes);
			while (!no_bytes)
				scanf("%u", &no_bytes);
			//finds the memory
			dll_t *memory = find_mem(list_array, no_bytes, current_size);
			if (!memory) {
				printf("Out of memory\n");
			} else {
				free_block_t *mem_dt = memory->head->data;
				malloc_mod(&malloc_calls, &alloc_blocks, &free_mem, &alloc_mem,
						   no_bytes);
				int dif = mem_dt->block_size - no_bytes;
				//memory fragmentation
				fragmentation(&list_array, dif, &free_blocks, &current_size,
							  &frag_no, mem_dt, no_bytes);
				alloc(&list_array, allocd, no_bytes, mem_dt, &current_size,
					  &memory);
			}
		} else if (!strcmp(command, "WRITE")) {
			int x = write(&list_array, &allocd, &address, &no_bytes, bytes, n,
			alloc_mem, free_mem, free_blocks, alloc_blocks, malloc_calls,
			frag_no, free_calls, current_size, &command);
			if (x)
				break;
		} else if (!strcmp(command, "READ")) {
			scanf("%lx %u", &address, &no_bytes);
			int res = read_from_address(allocd, address, no_bytes);
			if (!res) {
				printf("\n");
			} else {
				dump(&list_array, &allocd, bytes, n, alloc_mem, free_mem,
					 free_blocks, alloc_blocks, malloc_calls, frag_no,
					 free_calls, current_size, 1);
				free(command);
				break;
			}
		} else if (!strcmp(command, "FREE")) {
			scanf("%lx", &address);
			int i;
			al_node_t *to_be_freed = find_free(allocd, address, &i);
			if (!to_be_freed) {
				printf("Invalid free\n");
			} else {
				al_block_t *dtemp = to_be_freed->data;
				free_mod(&free_calls, &free_blocks, &alloc_blocks, &free_mem,
						 &alloc_mem, dtemp->block_size);
				free_and_add(&list_array, dtemp, &current_size, allocd, i);
				free(to_be_freed);
			}
		} else if (!strcmp(command, "DUMP_MEMORY")) {
			dump(&list_array, &allocd, bytes, n, alloc_mem, free_mem,
				 free_blocks, alloc_blocks, malloc_calls, frag_no,
				  free_calls, current_size, 0);
		} else if (!strcmp(command, "DESTROY_HEAP")) {
			free_all(&list_array, &allocd, &command, current_size);
			break;
		}
		scanf("%s", command);
	}
}
