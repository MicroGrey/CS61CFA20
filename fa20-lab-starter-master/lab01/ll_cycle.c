#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    while (head->next != NULL)
    {
        if (head->value == 1)
        {
            return 1;
        }
        head->value=1;
        head=head->next;
    }
    return 0;
}