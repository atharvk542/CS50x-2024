#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
i*ptr = list;
    while (ptr != NULL)
    {
        printf("%i\n", ptr->number);
        // Points to the next value in the list
        ptr = ptr->next;
    }
}
