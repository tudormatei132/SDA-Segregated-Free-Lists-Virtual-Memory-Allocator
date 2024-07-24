//Matei Tudor-Andrei 314CA
dll_t *find_mem(dll_t **array, int size, int n)
{
	//points to the address with the lowest value that has at
	//least "size" free bytes

	for (int i = 0; i < n; i++) {
		if (array[i]->head) {
			node_t *temp = array[i]->head;
			free_block_t *dtemp = temp->data;
		if (dtemp->block_size >= size)
			return array[i];
		}
	}
	return NULL;
}

al_node_t *find_free(linked_list_t *list, unsigned long address, int *i)
{
	//checks if the given address is allocated
	//returns a pointer to the block if that's the case
	if (!list->head)
		return NULL;
	al_node_t *temp = list->head;
	*i = 0;
	while (temp) {
		al_block_t *dtemp = temp->data;
		if (address == dtemp->address)
			return temp;
		else if (address < dtemp->address)
			break;
		temp = temp->next;
		(*i)++;
	}
	return NULL;
}

void fragmentation(dll_t ***list_array, int dif, int *free_blocks,
				   int *current_size, int *frag_no,
				   free_block_t *mem_dt, int no_bytes)
{
	//breaks the bigger in block in two smaller ones:
	//into the allocated one and the remaining free one
	//adds the remaining part of the free block into another list
	//from the array
	if (dif) {
		(*frag_no)++;
		int found = 0;
		free_block_t *new_data = malloc(sizeof(*new_data));
		DIE(!new_data, "malloc () failed");
		new_data->block_size = dif;
		new_data->address = mem_dt->address + no_bytes;
		for (int i = 0; i < *current_size && !found; i++) {
			free_block_t *temp = (*list_array)[i]->head->data;
			if (dif == temp->block_size) {
				dll_add_nth_node((*list_array)[i], dll_fragment_placer
				((*list_array)[i], new_data->address), new_data);
				found = 1;
		}
		}
		if (!found) {
			*list_array = realloc(*list_array,
								  (*current_size + 1) * sizeof(dll_t *));
			DIE(!(*list_array), "realloc () failed");
			int pos = add_new_dim(list_array, current_size, dif);
			dll_add_nth_node((*list_array)[pos], 0, new_data);
		}
		free(new_data);
	} else {
		(*free_blocks)--;
	}
}

void alloc(dll_t ***list_array, linked_list_t *allocd, int no_bytes,
		   free_block_t *mem_dt, int *current_size, dll_t **memory)
{
	al_block_t *new_data = malloc(sizeof(*new_data));
	DIE(!new_data, "malloc () failed");
	new_data->block_size = no_bytes;
	new_data->address = mem_dt->address;
	new_data->data = malloc(no_bytes);
	DIE(!new_data->data, "malloc () failed");
	//move the removed block into the ll_t
	ll_add_nth_node(allocd, add_allocd(allocd, mem_dt->address), new_data);
	free(new_data);
	node_t *removed = dll_remove_nth_node(*memory, 0);
	free(removed->data);
	free(removed);
	//if the list from the array is empty, remove it
	if ((*memory)->size == 0) {
		for (int i = 0; i < *current_size; i++) {
			if (*memory == (*list_array)[i]) {
				for (int j = i; j < *current_size - 1; j++)
					(*list_array)[j] = (*list_array)[j + 1];
				}
			}
		(*current_size)--;
		if (*current_size)
			*list_array = realloc(*list_array, *current_size *
													sizeof(**list_array));
		DIE(!(*list_array), "realloc () failed");

		free(*memory);
	}
}

void free_and_add(dll_t ***list_array, al_block_t *dtemp, int *current_size,
				  linked_list_t *allocd, int i)
{
	//frees a block, so it removes it from the linked list allocd
	//and adds it back to one of the lists from the array
	int found = 0;
	free_block_t *dtemp2 = malloc(sizeof(*dtemp2));
	DIE(!dtemp2, "malloc() failed");
	dtemp2->address = dtemp->address;
	dtemp2->block_size = dtemp->block_size;
	for (int j = 0; j < *current_size && !found; j++) {
		if ((*list_array)[j]->size) {
			free_block_t *dtemp1 = (*list_array)[j]->head->data;
			if (dtemp2->block_size == dtemp1->block_size) {
				//the lists will be sorted by the nodes' addresses
				int pos = dll_fragment_placer((*list_array)[j],
														dtemp2->address);
				dll_add_nth_node((*list_array)[j], pos, dtemp2);
				found = 1;
			}
		}
	}
	if (!found) {
		// create a new entry in the array
		// the array will be sorted by the heads' block size
		*list_array = realloc(*list_array, (*current_size + 1) *
													sizeof(dll_t *));
		DIE(!(*list_array), "realloc () failed");
		int pos = add_new_dim(list_array, current_size,
												dtemp2->block_size);
		dll_add_nth_node((*list_array)[pos], 0, dtemp2);
	}
	ll_remove_nth_node(allocd, i);
	free(dtemp2);
	free(dtemp->data);
	if (dtemp)
		free(dtemp);
}
