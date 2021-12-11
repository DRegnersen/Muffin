#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// ========== Standard List ==========

ArrayList declareList() {
    ArrayList new_list;

    new_list.size = 0;
    new_list.capacity = LISTCAPASITY;
    new_list.bytes = (char *) malloc(new_list.capacity * sizeof(char));

    if (new_list.bytes == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    return new_list;
}

void pushBack(ArrayList *list, char new_byte) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->bytes = (char *) realloc(list->bytes, list->capacity * sizeof(char));

        if (list->bytes == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }
    }
    list->bytes[(list->size)++] = new_byte;
}

// ========== String List ==========

s_ArrayList s_declareList() {
    s_ArrayList new_list;

    new_list.size = 0;
    new_list.capacity = SLISTCAPASITY;
    new_list.values = (char **) malloc(new_list.capacity * sizeof(char *));

    if (new_list.values == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    for (int i = 0; i < new_list.capacity; i++) {
        new_list.values[i] = (char *) malloc(LINESIZE * sizeof(char));

        if (new_list.values[i] == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }
    }

    return new_list;
}

void s_pushBack(s_ArrayList *list, char *new_value) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->values = (char **) realloc(list->values, list->capacity * sizeof(char *));

        if (list->values == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }

        for (int i = list->size; i < list->capacity; i++) {
            list->values[i] = (char *) malloc(LINESIZE * sizeof(char));

            if (list->values[i] == NULL) {
                printf("ERROR! Segmentation fault, line: \n", __LINE__);
            }
        }
    }

    list->values[(list->size)++] = new_value;
}