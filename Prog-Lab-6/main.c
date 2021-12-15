#include <stdio.h>
#include <string.h>
#include "list.h"
#include "baker.h"
#include "compressor.h"

// ----------------------------
// ---------- MUFFIN ----------
// ----------------------------

void createArchive(char *arch_name, char *directory, s_ArrayList filenames) {
    ArrayList data = bake(filenames);
    ArrayList encoder = declareList();
    ArrayList compressed_data = compress(data, &encoder);
    ArrayList packed_archive = bakeEncoder(encoder, compressed_data);

    char filename[LINESIZE];
    sprintf(filename, "%s\\%s.muf", directory, arch_name);

    FILE *out = fopen(filename, "wb");

    if (out == NULL) {
        printf("ERROR! File writing failed for '%s'.\n", filename);
        return;
    }

    for (int i = 0; i < packed_archive.size; i++) {
        fputc(packed_archive.bytes[i], out);
    }

    fclose(out);
    printf("Archive has been successfully created. Path: %s\n", filename);
}

void extractFromArchive(char *arch_name, char *directory) {
    ArrayList encoder = declareList();
    ArrayList data = declareList();
    splitEncoder(&encoder, &data, arch_name);
    ArrayList decoded = extract(data, encoder);

    splitAndCreate(decoded, directory);
    printf("Files have been successfully extracted.\n");
}

void printLocalFilenames(char *arch_name) {
    ArrayList encoder = declareList();
    ArrayList data = declareList();
    splitEncoder(&encoder, &data, arch_name);
    ArrayList decoded = extract(data, encoder);

    s_ArrayList local_names = getFilenames(decoded);

    char local_arch_name[LINESIZE];
    sscanf(strrchr(arch_name, '\\'), "\\%s", local_arch_name);

    printf("Files from %s:\n", local_arch_name);
    for (int i = 0; i < local_names.size; i++) {
        printf("%d) %s\n", i + 1, local_names.values[i]);
    }
}

int main(int argc, char **argv) {

    for (int i = 1; i < argc; i++) {
        char *command = argv[i];

        if (!strcmp(command, "--create")) {
            s_ArrayList filenames = s_declareList();
            char directory[LINESIZE];
            char arch_name[LINESIZE];

            for (int j = i + 1; j < argc; j++) {
                if (argv[j][0] == '-') {
                    break;
                }
                s_pushBack(&filenames, argv[j]);
                i++;
            }

            printf("Archive directory:");
            gets(directory);
            printf("Name:");
            gets(arch_name);

            createArchive(arch_name, directory, filenames);

        } else if (!strcmp(command, "--extract")) {
            char directory[LINESIZE];

            printf("Extracted files directory:");
            gets(directory);

            extractFromArchive(argv[++i], directory);
        } else if (!strcmp(command, "--list")) {
            printLocalFilenames(argv[++i]);
        } else {
            printf("Command '%s' is unacceptable\n", command);
        }
    }

    return 0;
}
