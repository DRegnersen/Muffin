#include <stdio.h>
#include <string.h>
#include "list.h"
#include "baker.h"

ArrayList bake(s_ArrayList filenames) {
    ArrayList list = declareList();

    for (int i = 0; i < filenames.size; i++) {
        int length = strlen(filenames.values[i]);

        // filename size writing
        for (int j = 0; j < 4; j++) {
            pushBack(&list, length % 256);
            length /= 256;
        }

        //filename writing
        for (int j = 0; j < length; j++) {
            pushBack(&list, filenames.values[i][j]);
        }

        //allocating memory for file size
        int start_sz_idx = list.size;
        for (int j = 0; j < 4; j++) {
            pushBack(&list, 0);
        }

        int size = 0;

        //file reading
        FILE *in = fopen(filenames.values[i], "rb");

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

ArrayList bakeEncoder(ArrayList encoder, ArrayList data) {
    ArrayList muffin = declareList();

    int encoder_sz = encoder.size;

    for (int i = 0; i < 4; i++) {
        pushBack(&muffin, encoder_sz % 256);
        encoder_sz /= 256;
    }

    for (int i = 0; i < encoder.size; i++) {
        pushBack(&muffin, encoder.bytes[i]);
    }

    int data_sz = data.size;

    for (int i = 0; i < 4; i++) {
        pushBack(&muffin, data_sz % 256);
        data_sz /= 256;
    }

    for (int i = 0; i < data.size; i++) {
        pushBack(&muffin, data.bytes[i]);
    }

    return muffin;
}
