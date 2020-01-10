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

struct songQ
{
     char name[100];
     struct song *next;
} void print_queue(struct songQ *queue);
struct songQ *add_song(struct songQ *queue, char *name);
struct songQ *play_song(struct songQ *queue);
struct songQ *clear_queue(struct songQ *list);