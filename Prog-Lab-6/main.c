#include <stdio.h>
#include <string.h>
#include "list.h"
#include "baker.h"
#include "compressor.h"

void createMuffin(char *muf_name, char *directory, s_ArrayList filenames) {
    ArrayList data = bake(filenames);
    ArrayList encoder = declareList();
    ArrayList compressed_data = compress(data, &encoder);
    ArrayList muffin = bakeEncoder(encoder, compressed_data);

    char filename[LINESIZE];
    sprintf(filename, "%s\\%s.muf", directory, muf_name);

    FILE *out = fopen(filename, "wb");

    for (int i = 0; i < muffin.size; i++) {
        fputc(muffin.bytes[i], out);
    }

    fclose(out);
}

int main(int argc, char **argv) {

    for (int i = 1; i < argc; i++) {
        char *command = argv[i];

        if (!strcmp(command, "--create")) {
            s_ArrayList filenames = s_declareList();
            char directory[LINESIZE];
            char muf_name[LINESIZE];

            for (int j = i + 1; j < argc; j++) {
                if (argv[j][0] == '-') {
                    break;
                }
                s_pushBack(&filenames, argv[j]);
                i++;
            }

            printf("Enter directory:");
            // gets(directory);
            printf("Enter Muffin's name:");
            //gets(muf_name);

            createMuffin("new_arch", "C:\\Users\\deofr\\C\\Prog-Lab-6\\archive", filenames);

        } else if (!strcmp(command, "--extract")) {
            ArrayList encoder = declareList();
            ArrayList data = declareList();
            splitEncoder(&encoder, &data, "C:\\Users\\deofr\\C\\Prog-Lab-6\\archive\\new_arch.muf");
            ArrayList decoded = extract(data, encoder);
        } else {
            printf("Command '%s' is unacceptable\n", command);
        }
    }

    return 0;
}
