dll_t* create_list(int data_size)
{
    dll_t *result = malloc(sizeof(*result));
    result->data_size = data_size;
    return result;
}

void move_right(void** array, int n, int pos, int data_size) {
    char *c = realloc(*array, (data_size) * (n + 1));
    if (!c)
        exit(0);
    *array = c;
    for (int i = n; i > pos; i--) {
        memcpy(c + i * data_size, c + (i - 1)*data_size, data_size);
    }
}

void add_new_dim(dll_t** array, int *n, int new_size)
{
    dll_t* new = create_list(new_size);
    for (int i = 0; i < *n; i++) {
        if (new_size < array[i]->data_size) {
            move_right((void **)array, *n, i, sizeof(dll_t));
            *n++;
        }
        array[i] = new;
    }
}



void init_lists(dll_t** array, int n, unsigned long address)
{
    int i = n;
    while (i) {
        int no_elements = pow(2, i - 1) - 1;
        array[n - i]->head = malloc(sizeof(node_t));
        array[n - i]->head->next = NULL;
        array[n - i]->head->prev = NULL;
        array[n - i]->head->address = address;
        address += array[n - i]->data_size;
        while (no_elements--) {
            
            node_t* new = malloc(sizeof(*new));
            new->prev = NULL;
            new->next = array[n - i]->head;
            array[n - i]->head->prev = new;
            new->address = array[n - i]->head->address;
            array[n - i]->head->address = address;
            array[n - i]->head = new;
            address += array[n - i]->data_size;
        }
        i--;
    }
}

