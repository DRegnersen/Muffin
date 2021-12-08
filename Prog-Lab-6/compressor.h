#ifndef PROG_LAB_6_COMPRESSOR_H
#define PROG_LAB_6_COMPRESSOR_H

#define QUEUESIZE 256
#define ARRAYSIZE 256

typedef struct {
    char array[QUEUESIZE];
    int tail;
    int head;
} Queue;

typedef struct {
    char byte;
    long times;
} Frequency;

#endif //PROG_LAB_6_COMPRESSOR_H
