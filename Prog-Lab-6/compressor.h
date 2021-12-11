#ifndef PROG_LAB_6_COMPRESSOR_H
#define PROG_LAB_6_COMPRESSOR_H

#define EMPTY -1

struct str_Node {
    char value;
    int rank;
    struct str_Node *left;
    struct str_Node *right;
};

typedef struct str_Node Node;

char **createEncoder(int b_size, char *bytes);

#endif //PROG_LAB_6_COMPRESSOR_H
