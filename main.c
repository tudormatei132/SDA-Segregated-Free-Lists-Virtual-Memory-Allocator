#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "struct.h"
#include "utils.h"
#include "malloc_func.h"
#include "write_func.h"

int main(void)
{
    unsigned long address;
    int n, bytes, current_size = 0;
    char *command = malloc(10 * sizeof(*command)); 
    dll_t **list_array;
    linked_list_t *allocd; 
    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "INIT_HEAP")) {
            scanf("%lx", &address);
            scanf("%d", &n);
            current_size = n;
            list_array = malloc(n * sizeof(*list_array));
            allocd = malloc(sizeof(*allocd));
            allocd->size = 0;
            scanf("%d", &bytes);
            for (int i = 0; i < n; i++) {
                list_array[i] = create_list(bytes / pow(2, n - i - 1));
                printf("%d\n", (list_array[i]->data_size));
            }
            init_lists(list_array, n, address); //initializing the segregated free lists
        }
        else if (!strcmp(command, "MALLOC")) {
            unsigned int no_bytes;
            scanf("%u", &no_bytes);
            while (!no_bytes) {
                scanf("%u", &no_bytes);
            }
            //finds the memory
            dll_t *memory = find_mem(list_array, no_bytes, current_size);
            if (memory == NULL)
                printf("Out of memory\n");
            else {
                printf("-----%d-----", memory->data_size);
                unsigned int dif = memory->data_size - no_bytes;
                
            //memory fragmentation
                if (dif) {
                    int found = 0;
                    for (int i = 0; i < current_size && !found; i++) {
                        if (dif == list_array[i]->data_size) {
                            printf("%d\n\n\n", dll_fragment_placer(list_array[i], memory->head->address + no_bytes));
                            dll_add_nth_node(list_array[i], dll_fragment_placer(list_array[i], memory->head->address + no_bytes), memory->head->address + no_bytes);
                            found = 1;
                        }
                    }
                    if (!found) {
                        list_array = realloc(list_array, (current_size + 1) * sizeof(dll_t *));
                        int pos = add_new_dim(&list_array, &current_size, dif);
                        
                        printf("%d ", current_size);
                        printf("%d\n", pos);
                        dll_add_nth_node(list_array[pos], 0, memory->head->address + no_bytes);
                    }
                }
                    
                    

            //check if a list is free, then remove it if that's the case(actually, not sure if it's necessary)

            //move the removed block into the ll_t
            ll_add_nth_node(allocd, add_allocd(allocd, memory->head->address), no_bytes, memory->head->address);
            node_t *removed = dll_remove_nth_node(memory, 0);
            }
        }
        else if (!strcmp(command, "PRINT")) {
            ll_print(allocd);
        }
        else if (!strcmp(command, "WRITE")) {
            char *data = malloc(50);
            unsigned int no_bytes;
            scanf("%lx %s %u", &address, data, &no_bytes);
            write_address(allocd, address, no_bytes, data);
        }
        else if (!strcmp(command, "READ")) {

        }
    }
}