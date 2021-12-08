#include <stdio.h>
#include <stdlib.h>
#include "compressor.h"

Queue q_declare() {
    Queue new_queue;
    new_queue.tail = 0;
    new_queue.head = 0;
    return new_queue;
}

void q_push(Queue *queue, char n) {
    queue->array[queue->tail] = n;
    queue->tail = (queue->tail + 1) % QUEUESIZE;
}

char q_pop(Queue *queue) {
    char extracted = queue->array[queue->head];
    queue->head = (queue->head + 1) % QUEUESIZE;
    return extracted;
}

void sort(Frequency **array) {
    for (int i = 1; i < ARRAYSIZE; i++) {
        int j = i - 1;
        while (j >= 0 && (*array)[j].times < (*array)[j + 1].times) {
            Frequency tmp = (*array)[j];
            (*array)[j] = (*array)[j + 1];
            (*array)[j + 1] = tmp;
            j--;
        }
    }
}

Queue makeQueue(int b_size, char *bytes) {
    Frequency *freq_table = NULL;
    freq_table = malloc(256 * sizeof(Frequency));
    for (int i = 0; i < 256; i++) {
        freq_table[i].byte = i;
        freq_table[i].times = -1;
    }

    Queue queue = q_declare();

    for (int i = 0; i < b_size; i++) {
        if (freq_table[(unsigned char) bytes[i]].times < 0) {
            freq_table[(unsigned char) bytes[i]].times = 1;
        } else {
            freq_table[(unsigned char) bytes[i]].times++;
        }
    }

    sort(&freq_table);

    int idx = 0;
    while (freq_table[idx].times != -1 && idx < 256) {
        q_push(&queue, freq_table[idx].byte);
        idx++;
    }

    free(freq_table);
    freq_table = NULL;

    return queue;
}
