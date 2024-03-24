al_node_t* stop_writing(unsigned long address, unsigned int no_bytes, int *dif, al_node_t* current)
{
    al_node_t *temp = current;
    while (temp) {
        al_node_t* temp1 = temp->next;
        if (!temp1 || temp1->address != temp->address + temp->data_size) {  
            return NULL;
        }
        *dif += temp1->data_size;
        if (*dif >= 0) {
            return temp1;
        }
        temp = temp1;
    }
    return NULL;
}

void write_address(linked_list_t* list, unsigned long address, unsigned int no_bytes, void* data)
{
    al_node_t *temp = list->head;
    if (!temp) {
        printf("Segmentation fault (core dumped)\n");
        return;
    }
    while (temp) {
        if (temp->address <= address && temp->address + temp->data_size > address) {
            int dif = temp->address + temp->data_size - address - no_bytes;
            char *c = (char *)temp->data; //converting to char* so we can use arithmetic operations 
            if (dif >= 0) {
                memcpy(c + address - temp->address, data, no_bytes);
                return;
            }
            else {
                //CONVERT THIS INTO A WHILE TEMP1 IS TEMP->NEXT AND TEMP CHANGES AT THE END OF THE LOOP!!!! (MUST DO FOR BIGGER BLOCKS FORMED BY SMALLER ONES, E.G. 24 BYTES FORMED BY 3 BLOCKS OF 8!!)
                al_node_t* stop = stop_writing(address, no_bytes, &dif, temp);
                if (!stop) {
                    printf("Segmentation fault (core dumped)\n");
                    return;
                }
                char* d = (char *)data;
                char* c = temp->data;
                memcpy(c + address - temp->address, d, temp->data_size + temp->address - address);
                d+= temp->data_size;
                temp = temp->next;
                while (temp != stop) {
                    char* c = (char *)temp->data;
                    memcpy(c, d, temp->data_size);
                    d += temp->data_size;
                    temp = temp->next;
                }
                c = temp->data;
                memcpy(c, d, temp->data_size - dif);
                return;
            }
        }
        temp = temp->next;
    }
    printf("Segmentation fault (core dumped)\n");
    return;
}


