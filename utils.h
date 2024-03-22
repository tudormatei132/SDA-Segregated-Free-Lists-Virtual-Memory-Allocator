#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)


dll_t* create_list(int data_size)
{
    dll_t *result = malloc(sizeof(*result));
    result->size = 0;
    result->data_size = data_size;
    return result;
}

void move_right(void** array, int n, int pos, int data_size) {
    //assuming that the array has already been realloc'd
    char *c = *array;
    if (!c)
        exit(0);
    for (int i = n; i > pos; i--) {
        memcpy(c + i * data_size, c + (i - 1)*data_size, data_size);
    }
}

void move_left(void** array, int n, int pos, int data_size) {
    char *c = (char *) *array;
    for (int i = pos; i < n - 1; i++) {
        memcpy(c + i * data_size, c + (i + 1)*data_size, data_size);
    }
    c = realloc(*array, (data_size) * (n - 1));
    if (!c)
        exit(0);
    *array = c;
}


int add_new_dim(dll_t*** array, int *n, unsigned int new_size)
{
    dll_t* new = create_list(new_size);
    for (int i = 0; i < *n; i++) {
        if (new_size < (*array)[i]->data_size) {
            for (int j = *n; j > i; j--) {
            (*array)[j] = (*array)[j - 1];
            }
            *n = *n + 1;
            (*array)[i] = new;
            return i; 
        }
    }
}




void
ll_add_nth_node(linked_list_t* list, unsigned int n, unsigned int data_size, unsigned long address)
{
    if (n < 0)
        return;
    if (list->size == 0) {
        list->head = malloc(sizeof(al_node_t));
        list->head->address = address;
        list->head->data_size = data_size;
        DIE(!list->head, "error");
        list->head->data = malloc(data_size);
        DIE(!list->head->data, "error");
        list->head->next = NULL;
        list->size++;
        return;
    }
    al_node_t* new_node = malloc(sizeof(al_node_t));
    DIE(!new_node, "error");
    new_node->data = malloc(data_size);
    DIE(!new_node->data, "error");
    new_node->data_size = data_size;
    new_node->address = address;    
    if (n == 0) {
        new_node->next = list->head;
        list->head = new_node;
        list->size++;
        return;
    }
    
    if (n >= list->size) {
        al_node_t* temp = list->head;
        while (temp->next) {
        temp = temp->next;
        }
        temp->next = new_node;
        new_node->next = NULL;
        list->size++;
        return;
    }

    n--;
    al_node_t* temp = list->head;
    while (n--) {
        temp = temp->next;
    }
    new_node->next = temp->next;
    temp->next = new_node;
    list->size++;
}

al_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n, unsigned int data_size)
{
    /* TODO */
    if (n < 0 || list->size == 0)
    {
        al_node_t* ans = malloc(sizeof(*ans));
        ans->data = malloc(sizeof(data_size));
        return ans;
    }

    al_node_t* temp = list->head;
     if (n == 0) {
        list->head = list->head->next;
        temp->next = NULL;
        return temp;
    }

    if (n >= list->size - 1) {
        al_node_t* next;
        next = temp->next;
        while (next->next) {
            temp = temp->next;
            next = temp->next;
        }
        temp->next = NULL;
        return next;
    }
    n--;
    while (n--) {
        temp = temp->next;
    }
    al_node_t *ans = temp->next;
    temp->next = ans->next;
    return ans;
}


node_t*
dll_get_nth_node(dll_t* list, unsigned int n)
{
	/* TODO */
	if (list->size == 0)
		return NULL;
	n = n % list->size;
	node_t* node = list->head;
	while (n--) {
		node = node->next;
	}
	return node;
}

void
dll_add_nth_node(dll_t* list, unsigned int n, unsigned long address)
{
	/* TODO */
	if (n < 0)
		return;
	node_t *node = malloc(sizeof(node_t));
	DIE(!node, "ERROR");
	node->address = address;
	if (list->size == 0) {
		node->next = NULL;
		node->prev = NULL;
		list->head = node;
		list->size++;
		return;
	}
	if (n == 0) {
		node_t *head = list->head;
		node->next = head;
		node->prev = NULL;
		head->prev = node;
		list->head = node;
		list->size++;

		return;
	}
	if (n >= list->size) {
		n = list->size - 1;	
		node_t *tail = dll_get_nth_node(list, n);
		node->next = NULL;
		node->prev = tail;
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
dll_remove_nth_node(dll_t *list, unsigned int n)
{
    if (!list->size)
        return NULL;
	if (n == 0) {
		node_t *ans = list->head;
		node_t *new_h = list->head->next;
		list->head = new_h;
        if (new_h)
		    new_h->prev = NULL;
		list->size--;
		return ans;
	}
	if (n >= list->size) {
        n = list->size - 1;
		node_t *ans = dll_get_nth_node(list, n);
		node_t *new = ans->prev;
		new->next = NULL;
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


void init_lists(dll_t** array, int n, unsigned long address)
{
    int i = n;
    while (i) {
        int no_elements = pow(2, i - 1);
        array[n - i]->size = 0;
        int c = 0;
        while (no_elements--) {
            dll_add_nth_node(array[n - i], c, address);
            address += array[n - i]->data_size;
            c++;
        }
        i--;
    }
}

int dll_fragment_placer(dll_t *list, unsigned long address)
{
    node_t *temp = list->head;
    int c = 0;
    while (temp) {
        if (address < temp->address) {
            return c; 
        }
        temp = temp->next;
        c++;
    }
    return c;
}

int add_allocd(linked_list_t *list, unsigned long address) {
    if (!list->size)
        return 0;
    al_node_t *temp = list->head;
    int n = 0;
    while (temp) {
        if (address < temp->address)
            return n;
        n++;
        temp = temp->next;
    }
    return n;
}

void ll_print(linked_list_t* list)
{
    al_node_t* temp = list->head;
    while (temp) {
        printf("%lu", temp->address);
        temp = temp->next;
    }
}