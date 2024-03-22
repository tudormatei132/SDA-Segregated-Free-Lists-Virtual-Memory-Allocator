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
                al_node_t *temp1 = temp->next;
                if (!temp1) {
                    printf("Segmentation fault (core dumped)\n");
                    return;
                }

                if (temp1->address != temp->address + temp->data_size) {
                    printf("Segmentation fault (core dumped)\n");
                    return;
                }

            }
        }
        temp = temp->next;
    }
    printf("Segmentation fault (core dumped)\n");
    return;
}