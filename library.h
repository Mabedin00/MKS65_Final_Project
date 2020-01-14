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
struct songQ* add_song(struct songQ* queue, char* name);
struct songQ* remove_song(struct songQ* queue, char* name);
struct songQ* clear_queue(struct songQ* list);

#endif
