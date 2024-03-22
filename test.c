#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void* move_right(void** array, int n, int pos, int data_size) {
    char *c = realloc(*array, (data_size) * (n + 1));
    if (!c)
        exit(0);
    *array = c;
    for (int i = n; i > pos; i--) {
        memcpy(c + i * data_size, c + (i - 1)*data_size, data_size);
    }
    return (void *)c;
}

int main()
{
    int *v = malloc(sizeof(int) * 4);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;
    v = (int *)move_right((void *)&v, 4, 2, sizeof(int));
    v[2] = 4;
    for (int i = 0; i < 5; i++)
        printf("%d", v[i]);
    free(v);
}