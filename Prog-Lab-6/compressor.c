#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressor.h"

void sort(int size, Node ***array) {
    for (int i = 1; i < size; i++) {
        int j = i - 1;
        while (j >= 0 && ((*array)[j] == NULL ||
                          ((*array)[j + 1] != NULL && (*array)[j]->rank > (*array)[j + 1]->rank))) {
            Node *tmp = (*array)[j];
            (*array)[j] = (*array)[j + 1];
            (*array)[j + 1] = tmp;
            j--;
        }
    }
}

Node *merge(int size, Node ***array) {
    if ((*array)[1] == NULL) {
        return NULL;
    }

    Node *new_node = NULL;
    new_node = (Node *) malloc(sizeof(Node));

    if (new_node == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    new_node->rank = (*array)[0]->rank + (*array)[1]->rank;
    new_node->left = (*array)[0];
    new_node->right = (*array)[1];

    (*array)[0] = new_node;
    (*array)[1] = NULL;

    sort(size, array);

    return new_node;
}

void encode(char ***encoder, Node *root, char *cur_code) {
    if (root == NULL) {
        printf("WARNING! Tree is empty.\n");
        return;
    }

    if (root->left == NULL && root->right == NULL) {
        strcpy((*encoder)[(unsigned char) root->value], cur_code);
        return;
    }

    if (root->left != NULL) {
        char new_code[strlen(cur_code) + 1];
        sprintf(new_code, "%s0", cur_code);
        encode(encoder, root->left, new_code);
    }

    if (root->right != NULL) {
        char new_code[strlen(cur_code) + 1];
        sprintf(new_code, "%s1", cur_code);
        encode(encoder, root->right, new_code);
    }
}

char **createEncoder(int b_size, char *bytes) {
    Node **freq_table = NULL;
    freq_table = (Node **) malloc(256 * sizeof(Node *));

    if (freq_table == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    for (int i = 0; i < 256; i++) {
        freq_table[i] = (Node *) malloc(sizeof(Node));

        if (freq_table[i] == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }

        freq_table[i]->value = i;
        freq_table[i]->rank = EMPTY;
        freq_table[i]->left = NULL;
        freq_table[i]->right = NULL;
    }

    int q_size = 0;

    for (int i = 0; i < b_size; i++) {
        if (freq_table[(unsigned char) bytes[i]]->rank == EMPTY) {
            freq_table[(unsigned char) bytes[i]]->rank = 1;
            q_size++;
        } else {
            freq_table[(unsigned char) bytes[i]]->rank++;
        }
    }

    Node **queue = NULL;
    queue = (Node **) malloc(q_size * sizeof(Node *));

    if (queue == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    int count = 0;

    for (int i = 0; i < 256; i++) {
        if (freq_table[i]->rank != EMPTY) {
            queue[count] = freq_table[i];
            count++;
        } else {
            free(freq_table[i]);
            freq_table[i] = NULL;
        }
    }

    free(freq_table);
    freq_table = NULL;

    sort(q_size, &queue);
    while (merge(q_size, &queue) != NULL);

    char **encoder = NULL;
    encoder = (char **) malloc(256 * sizeof(char *));

    if (encoder == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    encode(&encoder, queue[0], "");

    for (int i = 0; i < q_size; i++) {
        free(queue[i]);
        queue[i] = NULL;
    }
    free(queue);
    queue = NULL;

    return encoder;
}
