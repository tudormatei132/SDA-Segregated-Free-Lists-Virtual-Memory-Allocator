//Matei Tudor-Andrei 314CA
al_node_t *stop_writing(int *dif, al_node_t *current)
{
	//it basically checks for any memory that isn't alloc'd
	//returns NULL if the reading/writing operation can't be done
	//returns a pointer to a block, which is the last one of a
	//contiguous memory area that starts at the given address and
	//is "no_bytes" bytes long
	al_node_t *temp = current;
	while (temp) {
		al_block_t *dtemp = temp->data;
		al_node_t *temp1 = temp->next;
		if (!temp1)
			return NULL;
		al_block_t *dtemp1 = temp1->data;
		if (dtemp1->address != dtemp->address + dtemp->block_size)
			return NULL;
		*dif += dtemp1->block_size;
		if (*dif >= 0)
			return temp1;
		temp = temp1;
	}
	return NULL;
}

int write_address(linked_list_t *list, unsigned long address,
				  int no_bytes, void *data)
{
	al_node_t *temp = list->head;
	if (!temp) {
		printf("Segmentation fault (core dumped)\n");
		return -1;
	}
	while (temp) {
		al_block_t *dtemp = temp->data;
		if (dtemp->address <= address &&
			(dtemp->address + dtemp->block_size > address)) {
			int dif = dtemp->address + dtemp->block_size - address - no_bytes;
			char *c = (char *)dtemp->data;
			//converting to char* so we can use arithmetic operations
			if (dif >= 0) {
				memcpy(c + address - dtemp->address, data, no_bytes);
				return 0;
			}
			al_node_t *stop = stop_writing(&dif, temp);
			if (!stop) {
				printf("Segmentation fault (core dumped)\n");
				return -1;
			}
			char *d = (char *)data;
			c = dtemp->data;
			//copy the first segment in the first block
			memcpy(c + address - dtemp->address, d,
				   dtemp->block_size + dtemp->address - address);
			d += dtemp->block_size + dtemp->address - address;
			temp = temp->next;
			while (temp != stop) {
				al_block_t *dtemp = temp->data;
				c = (char *)dtemp->data;
				//write the entire block since it's in the middle
				memcpy(c, d, dtemp->block_size);
				d += dtemp->block_size;
				temp = temp->next;
			}
			al_block_t *dtemp = temp->data;
			c = dtemp->data;
			//write the remaining data
			memcpy(c, d, dtemp->block_size - dif);
			return 0;
		}
		temp = temp->next;
	}
	printf("Segmentation fault (core dumped)\n");
	return -1;
}

int read_from_address(linked_list_t *list, unsigned long address,
					  int no_bytes)
{
	//works the same way as write_from_address
	al_node_t *temp = list->head;
	if (!temp) {
		printf("Segmentation fault (core dumped)\n");
		return -1;
	}
	while (temp) {
		al_block_t *dtemp = temp->data;
		if (dtemp->address <= address &&
			dtemp->address + dtemp->block_size > address) {
			int dif = dtemp->address + dtemp->block_size - address - no_bytes;
			char *c = (char *)dtemp->data;
			if (dif >= 0) {
				for (int i = 0; i < no_bytes; i++)
					printf("%c", *(c + address - dtemp->address + i));
				return 0;
			}
			al_node_t *stop = stop_writing(&dif, temp);
			if (!stop) {
				printf("Segmentation fault (core dumped)\n");
				return -1;
			}
			c = dtemp->data;
			for (int i = address - dtemp->address; i < dtemp->block_size;
				 i++) {
				printf("%c", *(c + i));
			}
			temp = temp->next;
			while (temp != stop) {
				dtemp = temp->data;
				c = (char *)dtemp->data;
				for (int i = 0; i < dtemp->block_size; i++)
					printf("%c", *(c + i));
				temp = temp->next;
			}
			dtemp = temp->data;
			c = dtemp->data;
			for (int i = 0; i < dtemp->block_size - dif; i++)
				printf("%c", *(c + i));
			return 0;
		}
		temp = temp->next;
	}
	printf("Segmentation fault (core dumped)\n");
	return -1;
}

char *get_data(char *data, int *bytes)
{
	char *result = malloc(strlen(data));
	DIE(!result, "malloc () failed");
	char *p = strtok(data + 1, "\"");
	//data + 1 is needed to ignore the blank space between the address and
	//the actual data
	strcpy(result, p);
	p = strtok(NULL, "\"");
	while (p) {
		if (atoi(p)) {
			*bytes = atoi(p);
			break;
		}
		strcat(result, "\"");
		strcat(result, p);
		strcat(result, "\"");
		p = strtok(NULL, "\"");
	}

	return result;
}

int find_first_quote(char *s)
{
	int i = 0;
	while (s[i] != '"')
		i++;
	return i;
}

int find_last_quote(char *s)
{
	int i = 0, res = -1;
	while (s[i] != '\0') {
		if (s[i] == '"')
			res = i;
		i++;
	}
	return res;
}

char *get_string(char *data, int *bytes)
{
	//get the string between '"'
	char *result = malloc(strlen(data) + 1);
	DIE(!result, "malloc () failed");
	strncpy(result, data + find_first_quote(data) + 1,
			find_last_quote(data) - find_first_quote(data) - 1);
	result[find_last_quote(data) - find_first_quote(data) - 1] = 0;
	*bytes = atoi(data + find_last_quote(data) + 2);
	return result;
}

int write(dll_t ***list_array, linked_list_t **allocd, unsigned long *address,
		  int *no_bytes, int bytes, int n, int alloc_mem, int free_mem,
		  int free_blocks, int alloc_blocks, int malloc_calls, int frag_no,
		  int free_calls, int current_size, char **command)
{
	//implementation of the write command
	char *data = malloc(600);
	DIE(!data, "malloc () failed");
	scanf("%lx", address);
	if (fgets(data, 600, stdin)) {
		char *to_write = get_string(data, no_bytes);
		if ((int)strlen(to_write) < *no_bytes)
			*no_bytes = strlen(to_write);
		int res = write_address(*allocd, *address, *no_bytes, to_write);
		if (res) { //if res != 0, then it failed
			dump(list_array, allocd, bytes, n, alloc_mem, free_mem,
				 free_blocks, alloc_blocks, malloc_calls, frag_no,
				free_calls, current_size, 1);
			free(data);
			free(*command);
			free(to_write);
			return -1; //returns -1 so it knows when to break in main
		}
		free(data);
		free(to_write);
		return 0;
	}
	return -1;
}
