#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song *SONGQUEUE = NULL;

// print_queue :: outputs the contents of the queue
void print_queue(struct songQ* queue) {
     while (queue) {
          if (!strcmp(queue->name, "PLAYLIST_ORIGIN") {
               printf("%s", queue->name);
          }
          queue = queue->next;
     }
     printf("\n");
}

// remove_song :: removes target song from queue
struct songQ* remove_song(struct songQ* queue, struct songQ* target) {
     if (!queue) {
          return queue;
     }
     if (queue == target) {
          struct song *start = list->next;
          free(queue);
          return start;
     }
     struct song *current = queue;
     while (current->queue) {
          if (current->next == target) {
               current->next = target->next;
               free(target);
               return queue;
          }
          current = current->next;
     }
     return NULL;
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

}
