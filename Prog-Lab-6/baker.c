#include <stdio.h>
#include <string.h>
#include "list.h"
#include "baker.h"

ArrayList bake(s_ArrayList filenames) {
    ArrayList list = declareList();

    for (int i = 0; i < filenames.size; i++) {
        char local[LINESIZE];
        sscanf(strrchr(filenames.values[i], '\\'), "\\%s", local);

        int length = strlen(local);

        // Filename size writing
        for (int j = 0; j < 4; j++) {
            pushBack(&list, length % 256);
            length /= 256;
        }

        length = strlen(local);

        // Filename writing
        for (int j = 0; j < length; j++) {
            pushBack(&list, local[j]);
        }

        // Allocating memory for file size
        int start_sz_idx = list.size;
        for (int j = 0; j < 4; j++) {
            pushBack(&list, 0);
        }

        int size = 0;

        // File reading
        FILE *in = fopen(filenames.values[i], "rb");

        if (in == NULL) {
            printf("ERROR! File '%s' does not exist.\n", filenames.values[i]);
        }

        char byte = fgetc(in);
        while (!feof(in)) {
            pushBack(&list, byte);
            byte = fgetc(in);
            size++;
        }

        fclose(in);

        // Size writing
        for (int j = start_sz_idx; j < start_sz_idx + 4; j++) {
            list.bytes[j] = (unsigned char) size % 256;
            size /= 256;
        }
    }

    printf("  o==\n");
    return list;
}

ArrayList bakeEncoder(ArrayList encoder, ArrayList data) {
    ArrayList packed_archive = declareList();

    // Encoder size writing
    int e_size = encoder.size;

    for (int i = 0; i < 4; i++) {
        pushBack(&packed_archive, e_size % 256);
        e_size /= 256;
    }

    // Encoder writing
    for (int i = 0; i < encoder.size; i++) {
        pushBack(&packed_archive, encoder.bytes[i]);
    }

    // Data size writing
    int d_size = data.size;

    for (int i = 0; i < 4; i++) {
        pushBack(&packed_archive, d_size % 256);
        d_size /= 256;
    }

    // Data writing
    for (int i = 0; i < data.size; i++) {
        pushBack(&packed_archive, data.bytes[i]);
    }

    printf(" \\___/\n");
    return packed_archive;
}

void splitEncoder(ArrayList *encoder, ArrayList *data, char *filename) {
    int power;

    // Archive reading
    FILE *in = fopen(filename, "rb");

    if (in == NULL) {
        printf("ERROR! File '%s' does not exist.\n", filename);
        return;
    }

    // Encoder size reading
    int e_size = 0;
    power = 1;

    for (int i = 0; i < 4; i++) {
        e_size += fgetc(in) * power;
        power *= 256;
    }

    // Encoding reading
    for (int i = 0; i < e_size; i++) {
        pushBack(encoder, fgetc(in));
    }

    // Data size reading
    int d_size = 0;
    power = 1;

    for (int i = 0; i < 4; i++) {
        d_size += fgetc(in) * power;
        power *= 256;
    }

    // Data reading
    for (int i = 0; i < d_size; i++) {
        pushBack(data, fgetc(in));
    }

    printf("  o(\n");
    fclose(in);
}

void splitAndCreate(ArrayList data, char *directory) {
    int power;
    int idx = 0;

    while (idx < data.size - 4) {
        char local[LINESIZE];
        int n_size = 0;
        power = 1;

        // Filename size reading
        for (int i = 0; i < 4; i++) {
            n_size += data.bytes[idx++] * power;
            power *= 256;
        }

        // Filename reading
        for (int i = 0; i < n_size; i++) {
            local[i] = data.bytes[idx++];
        }
        local[n_size] = 0;

        // File size reading
        int f_size = 0;
        power = 1;

        for (int i = 0; i < 4; i++) {
            f_size += ((unsigned char) data.bytes[idx++]) * power;
            power *= 256;
        }

        // File reading and creating
        char filename[LINESIZE];
        sprintf(filename, "%s\\%s", directory, local);

        FILE *out = fopen(filename, "wb");

        if (out == NULL) {
            printf("ERROR! File writing failed for '%s'.\n", filename);
            return;
        }

        for (int i = 0; i < f_size; i++) {
            fputc(data.bytes[idx++], out);
        }

        fclose(out);
    }
    printf(" \\___/\n");
}

s_ArrayList getFilenames(ArrayList data) {
    s_ArrayList local_names = s_declareList();

    int power;
    int idx = 0;

    while (idx < data.size - 4) {
        char local[LINESIZE];
        int n_size = 0;
        power = 1;

        // Filename size reading
        for (int i = 0; i < 4; i++) {
            n_size += data.bytes[idx++] * power;
            power *= 256;
        }

        // Filename reading
        for (int i = 0; i < n_size; i++) {
            local[i] = data.bytes[idx++];
        }
        local[n_size] = 0;

        // Adding a new filename
        s_pushBack(&local_names, local);

        // File size reading
        int f_size = 0;
        power = 1;

        for (int i = 0; i < 4; i++) {
            f_size += ((unsigned char) data.bytes[idx++]) * power;
            power *= 256;
        }

        // Extra bytes skipping
        idx += f_size;
    }

    printf(" \\___/\n");
    return local_names;
}