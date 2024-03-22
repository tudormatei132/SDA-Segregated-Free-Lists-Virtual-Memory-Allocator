typedef struct node_t{
    struct node_t* next;
    struct node_t* prev;
    unsigned long address;
} node_t;

typedef struct {
    node_t *head;
    int data_size;//the memory of the blocks
    int size;
} dll_t;

typedef struct al_node_t{
    struct al_node_t* next;
    unsigned long address;
    unsigned int data_size;
    void* data;
} al_node_t;

typedef struct linked_list_t {
    al_node_t* head;
    int size;
} linked_list_t;
