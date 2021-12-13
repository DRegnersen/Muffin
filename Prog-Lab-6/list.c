#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// ========== Segmentation fault catching ==========

void *catch(void *pointer, int line) {
    if (pointer == NULL) {
        printf("ERROR! Segmentation fault, line: \n", line);
        exit(EXIT_FAILURE);
    }
    return pointer;
}

// ========== Standard List ==========

ArrayList declareList() {
    ArrayList new_list;

    new_list.size = 0;
    new_list.capacity = LISTCAPASITY;
    new_list.bytes = (char *) catch(malloc(new_list.capacity * sizeof(char)), __LINE__);

    return new_list;
}

void pushBack(ArrayList *list, char new_byte) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->bytes = (char *) catch(realloc(list->bytes, list->capacity * sizeof(char)), __LINE__);
    }
    list->bytes[(list->size)++] = new_byte;
}

// ========== String List ==========

s_ArrayList s_declareList() {
    s_ArrayList new_list;

    new_list.size = 0;
    new_list.capacity = SLISTCAPASITY;
    new_list.values = (char **) catch(malloc(new_list.capacity * sizeof(char *)), __LINE__);

    for (int i = 0; i < new_list.capacity; i++) {
        new_list.values[i] = (char *) catch(malloc(LINESIZE * sizeof(char)), __LINE__);
    }

    return new_list;
}

void s_pushBack(s_ArrayList *list, char *new_value) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->values = (char **) catch(realloc(list->values, list->capacity * sizeof(char *)), __LINE__);

        for (int i = list->size; i < list->capacity; i++) {
            list->values[i] = (char *) catch(malloc(LINESIZE * sizeof(char)), __LINE__);
        }
    }

    list->values[(list->size)++] = new_value;
}
