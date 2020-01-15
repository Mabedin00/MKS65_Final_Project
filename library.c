#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// print_queue :: outputs the contents of the queue
void print_queue(struct songQ* queue) {
    printf("Up next...\n");
    int i = 1;
     while (queue) {
          printf("\t%d) %s", i, queue->name);
          queue = queue->next;
          i++;
     }
     printf("\n");
}

// remove_song :: removes target song from queue
void** pop(struct songQ* queue) {
     if (!queue->first) {
          return queue;
     }
     song* ans = queue->first;
     queue->first = ans->next;
     return {queue, ans};
}

// clear_queue :: clears entire queue, save for buffer 'PLAYLIST_ORIGIN'
struct songQ* clear_queue(struct songQ* queue) {
     while (queue->next != NULL) {
          if (strcmp(queue->name, "PLAYLIST_ORIGIN")) {
               queue = queue->next;
          }
          else {
               queue = remove_song(queue, queue);
          }
     }
     return queue;
}

// add_song :: adds new song to back of queue
struct songQ *add_song(struct songQ* queue, char* name) {
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
