#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "struct.h"
#include "utils.h"
#include "malloc_func.h"

int main(void)
{
    unsigned long address;
    int n, bytes, current_size = 0;
    char *command = malloc(10 * sizeof(*command)); 
    dll_t **list_array;
    while (1) {
        scanf("%s", command);
        if (!strcmp(command, "INIT_HEAP")) {
            scanf("%lx", &address);
            scanf("%d", &n);
            current_size = n;
            list_array = malloc(n * sizeof(*list_array));
            scanf("%d", &bytes);
            for (int i = 0; i < n; i++) {
                list_array[i] = create_list(bytes / pow(2, n - i - 1));
                printf("%d\n", (list_array[i]->data_size));
            }
            init_lists(list_array, n, address); //initializing the segregated free lists
        }
        else if (!strcmp(command, "MALLOC")) {
            int no_bytes;
            scanf("%d", &no_bytes);
            //finds the memory
            if (find_mem(list_array, no_bytes, current_size) == NULL)
                printf("Out of memory\n");
            else {
                printf("%lx", find_mem(list_array, no_bytes, current_size));
            
            //memory fragmentation
            
            //remove the block from its list, then add the remaining free part of it to a new one, if
            //that's the case   

            //if the list already exists

            //if a new list needs to be created

            //maybe check if a list is free, then remove it if that's the case
            }

            
        }
    }
}