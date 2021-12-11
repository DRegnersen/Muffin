#ifndef PROG_LAB_6_BAKER_H
#define PROG_LAB_6_BAKER_H

#define MERGEDCAPASITY 1000

typedef struct {
    int size;
    int capacity;
    char *bytes;
} Baked;

Baked declareList();

void pushBack(Baked *list, char new_byte);

Baked bake(int f_num, char **filenames);

#endif //PROG_LAB_6_BAKER_H
