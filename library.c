#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_queue(struct songQ* queue) {
     while (queue) {
          printf("%s", queue->name);
          queue = queue->next;
     }
     printf("\n");
}
