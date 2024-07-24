//Matei Tudor-Andrei 314CA
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

int ll_get_size(linked_list_t *list);

linked_list_t *ll_create(int data_size)
{
	/* TODO */
	linked_list_t *result = malloc(sizeof(*result));
	DIE(!result, "error");
	result->head = NULL;
	result->data_size = data_size;
	result->size = 0;
	return result;
}

void ll_add_nth_node(linked_list_t *list, int n, const void *new_data)
{
	/* TODO */
	if (n < 0)
		return;
	if (list->size == 0) {
		list->head = malloc(sizeof(al_node_t));
		DIE(!list->head, "error");
		list->head->data = malloc(list->data_size);
		DIE(!list->head->data, "error");
		memcpy(list->head->data, new_data, list->data_size);
		list->head->next = NULL;
		list->size++;
		return;
	}
	if (n == 0) {
		al_node_t *new_node = malloc(sizeof(al_node_t));
		DIE(!new_node, "error");
		new_node->data = malloc(list->data_size);
		DIE(!new_node->data, "error");
		memcpy(new_node->data, new_data, list->data_size);
		new_node->next = list->head;
		list->head = new_node;
		list->size++;
		return;
	}

	if (n >= list->size) {
		al_node_t *new_node = malloc(sizeof(al_node_t));
		DIE(!new_node, "error");
		new_node->data = malloc(list->data_size);
		DIE(!new_node->data, "error");
		memcpy(new_node->data, new_data, list->data_size);
		al_node_t *temp = list->head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
		new_node->next = NULL;
		list->size++;
		return;
	}

	n--;
	al_node_t *new_node = malloc(sizeof(al_node_t));
	DIE(!new_node, "error");
	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "error");
	memcpy(new_node->data, new_data, list->data_size);

	al_node_t *temp = list->head;
	while (n--)
		temp = temp->next;
	new_node->next = temp->next;
	temp->next = new_node;
	list->size++;
}

al_node_t *ll_remove_nth_node(linked_list_t *list, int n)
{
	/* TODO */
	list->size = ll_get_size(list);
	if (n < 0 || list->size == 0)
		return NULL;

	al_node_t *temp = list->head;
	if (n == 0) {
		list->head = list->head->next;
		temp->next = NULL;
		return temp;
	}

	if (n >= list->size - 1) {
		al_node_t *next;
		next = temp->next;
		while (next->next) {
			temp = temp->next;
			next = temp->next;
		}
		temp->next = NULL;
		return next;
	}
	n--;
	while (n--)
		temp = temp->next;
	al_node_t *ans = temp->next;
	temp->next = ans->next;
	return ans;
}

int ll_get_size(linked_list_t *list)
{
	/* TODO */
	int size = 0;
	al_node_t *current = list->head;
	if (!current)
		return 0;
	while (current) {
		size++;
		current = current->next;
	}
	return size;
}

dll_t *dll_create(int data_size)
{
	/* TODO */
	dll_t *ans = malloc(sizeof(*ans));
	DIE(!ans, "malloc() failed");
	ans->head = NULL;
	ans->data_size = data_size;
	ans->size = 0;
	return ans;
}

node_t *dll_get_nth_node(dll_t *list, int n)
{
	/* TODO */
	if (list->size == 0)
		return NULL;
	n = n % list->size;
	node_t *node = list->head;
	while (n--)
		node = node->next;
	return node;
}

void
dll_add_nth_node(dll_t *list, int n, const void *new_data)
{
	/* TODO */
	if (n < 0)
		return;
	node_t *node = malloc(sizeof(node_t));
	DIE(!node, "ERROR");
	node->data = malloc(list->data_size);
	DIE(!node->data, "ERROR");
	memcpy(node->data, new_data, list->data_size);
	if (list->size == 0) {
		node->next = node;
		node->prev = node;
		list->head = node;
		list->size++;
		return;
	}
	if (n == 0) {
		node_t *current = list->head->prev;
		node_t *nxt = list->head;
		node->next = nxt;
		node->prev = current;
		current->next = node;
		nxt->prev = node;
		list->head = node;
		list->size++;

		return;
	}
	if (n >= list->size) {
		n = list->size - 1;
		node_t *head = list->head;
		node_t *tail = list->head->prev;
		node->next = head;
		node->prev = tail;
		head->prev = node;
		tail->next = node;
		list->size++;

		return;
	}
	n--;
	node_t *current = dll_get_nth_node(list, n);
	node_t *nxt = current->next;
	node->next = current->next;
	node->prev = nxt->prev;
	current->next = node;
	nxt->prev = node;
	list->size++;
}

node_t*
dll_remove_nth_node(dll_t *list, int n)
{
	/* TODO */
	if (n == 0) {
		node_t *ans = list->head;
		node_t *tail = list->head->prev;
		node_t *new_h = list->head->next;
		list->head = new_h;
		tail->next = new_h;
		new_h->prev = tail;
		list->size--;
		return ans;
	}
	if (n >= list->size) {
		node_t *ans = list->head->prev;
		node_t *new_t = ans->prev;
		list->head->prev = new_t;
		new_t->next = list->head;
		list->size--;
		return ans;
	}
	node_t *ans = dll_get_nth_node(list, n);
	node_t *next = ans->next;
	node_t *prev = ans->prev;
	next->prev = prev;
	prev->next = next;
	list->size--;
	return ans;
}

int
dll_get_size(dll_t *list)
{
	/* TODO */
	int c = 0;
	node_t *temp = list->head;
	if (temp)
		c++;
	temp = temp->next;
	while (temp != list->head) {
		c++;
		temp = temp->next;
	}
	return c;
}

int add_new_dim(dll_t ***array, int *n, int new_size)
{
	dll_t *new = dll_create(sizeof(free_block_t));
	int i = 0;
	for (i = 0; i < *n; i++) {
		if ((*array)[i]->size) {
			free_block_t *temp = (*array)[i]->head->data;
		if (new_size < temp->block_size) {
			for (int j = *n; j > i; j--)
				(*array)[j] = (*array)[j - 1];
			*n = *n + 1;
			(*array)[i] = new;
			return i;
		}
		}
	}
	(*array)[*n] = new;
	*n = *n + 1;
	return i;
}

void print_dll_address(dll_t *list)
{
	node_t *temp = list->head;
	free_block_t *dtemp = temp->data;
		printf(" 0x%lx", dtemp->address);
		temp = temp->next;
	while (temp != list->head) {
		free_block_t *dtemp = temp->data;
		printf(" 0x%lx", dtemp->address);
		temp = temp->next;
	}
}

void print_ll_address(linked_list_t *list)
{
	al_node_t *temp = list->head;
	while (temp) {
		al_block_t *dtemp = temp->data;
		printf(" (0x%lx - %d)", dtemp->address, dtemp->block_size);
		temp = temp->next;
	}
	printf("\n");
}

void init_lists(dll_t **array, int n, unsigned long address, int bytes)
{
	int i = n;
	free_block_t *temp = malloc(sizeof(*temp));
	while (i) {
		int no_elements = bytes / pow(2, n - i + 3);
		array[n - i]->size = 0;
		int c = 0;

		temp->block_size = pow(2, n - i + 3);
		while (no_elements--) {
			temp->address = address;
			dll_add_nth_node(array[n - i], c, temp);
			address += pow(2, n - i + 3);
			c++;
		}

		i--;
	}
	free(temp);
}

int dll_fragment_placer(dll_t *list, unsigned long address)
{
	//finds the spot for the free block in a doubly linked list
	node_t *temp = list->head;
	int c = 0;
	free_block_t *dtemp = (free_block_t *)temp->data;
		if (address < dtemp->address)
			return c;
		temp = temp->next;
		c++;
	while (temp != list->head) {
		free_block_t *dtemp = (free_block_t *)temp->data;
		if (address < dtemp->address)
			return c;
		temp = temp->next;
		c++;
	}
	return c;
}

int add_allocd(linked_list_t *list, unsigned long address)
{
	//finds the spot for the new allocated block
	if (!list->size)
		return 0;
	al_node_t *temp = list->head;
	int n = 0;
	while (temp) {
		al_block_t *dtemp = temp->data;
		if (address < dtemp->address)
			return n;
		n++;
		temp = temp->next;
	}
	return n;
}

void dll_free(dll_t **list)
{
	node_t *temp = (*list)->head;
	while ((*list)->size--) {
		node_t *next = temp->next;
		free(temp->data);
		free(temp);
		temp = next;
	}
	free(*list);
}

void ll_free(linked_list_t **list)
{
	al_node_t *temp = (*list)->head;
	while (temp) {
		al_node_t *next = temp->next;
		al_block_t *dtemp = temp->data;
		free(dtemp->data);
		free(temp->data);
		free(temp);
		temp = next;
	}
	free(*list);
}

void init_heap(unsigned long *address, int *n, int *bytes,
			   int *current_size, dll_t ***list_array, linked_list_t **allocd,
			   int *free_mem, int *free_blocks)
{
	scanf("%lx", address);
			scanf("%d", n);
			*current_size = *n;
			*list_array = malloc(*n * sizeof(**list_array));
			DIE(!(*list_array), "malloc () failed");
			*allocd = ll_create(sizeof(al_block_t));
			DIE(!(*allocd), "malloc () failed");
			scanf("%d", bytes);
			int bonus = 0;
			scanf("%d", &bonus);
			*free_mem = *bytes;
			for (int i = 0; i < *n; i++)
				(*list_array)[i] = dll_create(sizeof(free_block_t));
			*free_mem = *n * (*bytes);
			//initializing the segregated free lists
			init_lists(*list_array, *n, *address, *bytes);
			for (int i = 0; i < *n; i++)
				(*free_blocks) += (*list_array)[i]->size;
}
