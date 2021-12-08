#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "baker.h"

Baked declare_list() {
    Baked new_list;

    new_list.size = 0;
    new_list.capacity = MERGEDCAPASITY;
    new_list.bytes = (char *) malloc(new_list.capacity * sizeof(char));

    if (new_list.bytes == NULL) {
        printf("Segmentation fault\n");
    }

    return new_list;
}

void push_back(Baked *list, char new_byte) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->bytes = (char *) realloc(list->bytes, list->capacity * sizeof(char));

        if (list->bytes == NULL) {
            printf("Segmentation fault\n");
        }
    }
    list->bytes[(list->size)++] = new_byte;
}

Baked bake(int f_num, char **filenames) {
    Baked list = declare_list();

    for (int i = 0; i < f_num; i++) {
        int length = strlen(filenames[i]);

        // filename size writing
        for (int j = 0; j < 4; j++) {
            push_back(&list, length % 256);
            length /= 256;
        }

        //filename writing
        for (int j = 0; j < length; j++) {
            push_back(&list, filenames[i][j]);
        }

        //allocating memory for file size bytes
        int start_sz_idx = list.size;
        for (int j = 0; j < 4; j++) {
            push_back(&list, 0);
        }

        int size = 0;

        //file reading
        FILE *in = fopen(filenames[i], "rb");

        char new_byte;
        while ((new_byte = fgetc(in)) != EOF) {
            push_back(&list, new_byte);
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