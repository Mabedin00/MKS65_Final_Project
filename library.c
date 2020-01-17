#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

// print_queue :: outputs the contents of the queue
void print_queue(struct songQ* queue) {
    song* current = queue->first
    printf("Up next...\n");
    int i = 1;
     while (current) {
          printf("\t%d) %s", i, first->name);
          current = current->next;
          i++;
     }
     printf("\n");
}

// remove_song :: removes target song from queue
void dequeue(struct songQ* queue, char* buffer) {
    if (!queue->first) {
        return queue;
    }
    song* to_del = queue->first;
    strcpy(buffer, to_del->name);
    queue->first = to_del->next;
    if (!queue->first) {
        queue->last = NULL;
    }
    free(to_del);
}

// clear_queue :: clears entire queue, save for buffer 'PLAYLIST_ORIGIN'
void clear_queue(struct songQ* queue) {
    char* trash_can[100];
    while (queue->first) {
        dequeue(queue, trash_can);
    }
}

// add_song :: adds new song to back of queue
void enqueue(struct songQ* queue, char* name) {
    struct song* new_node = malloc(sizeof(struct song));
    strncpy(new_node->name, name);
    new_node->next = NULL;
    if (queue->last) {
        queue->last->next = new_node;
    } else {
        queue->first = new_node;
    }
    queue->last = new_node;
}
