#ifndef LIBRARY_H
#define LIBRARY_H

struct songQ {
    struct song* first;
    struct song* last;
};

struct song {
    char name[100];
    struct song* next;
};

void print_queue(struct songQ* queue);
void enqueue(struct songQ* queue, char* name);
void dequeue(struct songQ* queue, char* name);
void clear_queue(struct songQ* list);

#endif
