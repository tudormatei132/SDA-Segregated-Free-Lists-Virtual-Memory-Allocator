//Matei Tudor-Andrei 314CA
void dump(dll_t ***list_array, linked_list_t **allocd, int bytes, int n,
		  int alloc_mem, int free_mem, int free_blocks, int alloc_blocks,
		  int malloc_calls, int frag_no, int free_calls, int current_size,
		  int destroy)
{
	printf("+++++DUMP+++++\nTotal memory: %d bytes\n", bytes * n);
	printf("Total allocated memory: %d bytes\n", alloc_mem);
	printf("Total free memory: %d bytes\n", free_mem);
	printf("Free blocks: %d\n", free_blocks);
	printf("Number of allocated blocks: %d\n", alloc_blocks);
	printf("Number of malloc calls: %d\n", malloc_calls);
	printf("Number of fragmentations: %d\n", frag_no);
	printf("Number of free calls: %d\n", free_calls);
	for (int i = 0; i < current_size; i++) {
		if ((*list_array)[i]->size) {
			free_block_t *temp = (*list_array)[i]->head->data;
			printf("Blocks with %d bytes - %d free block(s) :",
				   temp->block_size, (*list_array)[i]->size);
			print_dll_address((*list_array)[i]);
			printf("\n");
		}
	}
	printf("Allocated blocks :");
	print_ll_address(*allocd);
	printf("-----DUMP-----\n");
	if (destroy) {
		for (int i = 0; i < current_size; i++)
			dll_free(&(*list_array)[i]);
		free(*list_array);
		ll_free(allocd);
	}
}

void malloc_mod(int *malloc_calls, int *alloc_blocks, int *free_mem,
				int *alloc_mem, int no_bytes)
{
	//the way memory changes with ever malloc call
	(*malloc_calls)++;
	(*alloc_blocks)++;
	(*free_mem) -= no_bytes;
	*alloc_mem = *alloc_mem + no_bytes;
}

void free_mod(int *free_calls, int *free_blocks, int *alloc_blocks,
			  int *free_mem, int *alloc_mem, int no_bytes)
{
	(*free_calls)++;
	(*free_blocks)++;
	(*alloc_blocks)--;
	(*free_mem) += no_bytes;
	*alloc_mem -= no_bytes;
}

void free_all(dll_t ***list_array, linked_list_t **allocd, char **command,
			  int current_size)
{
	//frees all of the lists and the array
	free(*command);
	for (int i = 0; i < current_size; i++)
		dll_free(&(*list_array)[i]);
	free(*list_array);
	ll_free(allocd);
}
