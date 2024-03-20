node_t* find_mem(dll_t** array, int size, int n)
{
    //points to the address with the lowest value that has at least "size" free bytes 
    for (int i = 0; i < n; i++) {
        if (array[i]->data_size >= size) {
            return array[i]->head;
        }
    }
    return NULL;
}