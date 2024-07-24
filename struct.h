//Matei Tudor-Andrei 314CA
typedef struct node_t {
	struct node_t *next;
	struct node_t *prev;
	void *data;//address + data_size
} node_t;

typedef struct {
	node_t *head;
	int data_size;//size of free_block
	int size;
} dll_t;

typedef struct al_node_t {
	struct al_node_t *next;
	unsigned long address;
	void *data; //address + size of the block + string
} al_node_t;

typedef struct {
	al_node_t *head;
	int size;
	int data_size;
} linked_list_t;

typedef struct {
	unsigned long address;
	int block_size;
} free_block_t;

typedef struct {
	void *data;
	unsigned long address;
	int block_size;
} al_block_t;
