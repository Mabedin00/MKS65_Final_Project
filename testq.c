#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

int main() {
     struct songQ *queue = NULL;

     print_queue(queue);

     enqueue(queue, "americanpie");
     enqueue(queue, "heartless");
     enqueue(queue, "kidsseeghosts");
     enqueue(queue, "allofthelights");

     print_queue(queue);
}
