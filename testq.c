#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

int main() {
     struct songQ queue;

     queue.first = NULL;
     queue.last = NULL;

     print_queue(&queue);

     enqueue(&queue, "americanpie");
     enqueue(&queue, "heartless");
     enqueue(&queue, "kidsseeghosts");
     enqueue(&queue, "allofthelights");

     print_queue(&queue);

     char buffer[100];

     dequeue(&queue, buffer);
     printf("Removed %s...\n", buffer);
     dequeue(&queue, buffer);
     printf("Removed %s...\n", buffer);

     print_queue(&queue);

     clear_queue(&queue);

     print_queue(&queue);
}
