#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "baker.h"

Baked declareList() {
    Baked new_list;

    new_list.size = 0;
    new_list.capacity = MERGEDCAPASITY;
    new_list.bytes = (char *) malloc(new_list.capacity * sizeof(char));

    if (new_list.bytes == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    return new_list;
}

void pushBack(Baked *list, char new_byte) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->bytes = (char *) realloc(list->bytes, list->capacity * sizeof(char));

        if (list->bytes == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }
    }
    list->bytes[(list->size)++] = new_byte;
}

Baked bake(int f_num, char **filenames) {
    Baked list = declareList();

    for (int i = 0; i < f_num; i++) {
        int length = strlen(filenames[i]);

        // filename size writing
        for (int j = 0; j < 4; j++) {
            pushBack(&list, length % 256);
            length /= 256;
        }

        //filename writing
        for (int j = 0; j < length; j++) {
            pushBack(&list, filenames[i][j]);
        }

        //allocating memory for file size bytes
        int start_sz_idx = list.size;
        for (int j = 0; j < 4; j++) {
            pushBack(&list, 0);
        }

        int size = 0;

        //file reading
        FILE *in = fopen(filenames[i], "rb");

        char new_byte;
        while ((new_byte = fgetc(in)) != EOF) {
            pushBack(&list, new_byte);
            size++;
        }

        fclose(in);

        //size writing
        for (int j = start_sz_idx; j < start_sz_idx + 4; j++) {
            list.bytes[j] = size % 256;
            size /= 256;
        }
    }

    return list;
}