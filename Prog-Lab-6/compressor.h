#ifndef PROG_LAB_6_COMPRESSOR_H
#define PROG_LAB_6_COMPRESSOR_H

#define EMPTY -1
#define CODESIZE 8

struct str_Node {
    char value;
    int rank;
    struct str_Node *left;
    struct str_Node *right;
};

typedef struct str_Node Node;

char **createEncoder(ArrayList toCompress);

ArrayList compress(ArrayList data, ArrayList *compressed_encoder);

ArrayList extract(ArrayList data, ArrayList compressed_encoder);

#endif //PROG_LAB_6_COMPRESSOR_H
