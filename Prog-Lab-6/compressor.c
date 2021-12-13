#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
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

char **createEncoder(ArrayList toCompress) {
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

    for (int i = 0; i < toCompress.size; i++) {
        if (freq_table[(unsigned char) toCompress.bytes[i]]->rank == EMPTY) {
            freq_table[(unsigned char) toCompress.bytes[i]]->rank = 1;
            q_size++;
        } else {
            freq_table[(unsigned char) toCompress.bytes[i]]->rank++;
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

    for (int i = 0; i < 256; i++) {
        encoder[i] = (char *) malloc(CODESIZE * sizeof(char));
        if (encoder[i] == NULL) {
            printf("ERROR! Segmentation fault, line: \n", __LINE__);
        }

    }

    encode(&encoder, queue[0], "");

    //  for (int i = 0; i < q_size; i++) {
    //    free(queue[i]);
    //     queue[i] = NULL;
    //  }
    free(queue);
    queue = NULL;

    return encoder;
}

ArrayList compress(ArrayList data, ArrayList *compressed_encoder) {
    char **encoder = createEncoder(data);
    ArrayList compressed = declareList();

    short *contains_table = NULL;
    contains_table = (short *) malloc(256 * sizeof(short));

    if (contains_table == NULL) {
        printf("ERROR! Segmentation fault, line: \n", __LINE__);
    }

    for (int i = 0; i < 256; i++) {
        contains_table[i] = 0;
    }

    char new_byte = 0;
    int power = 1;

    for (int i = 0; i < data.size; i++) {
        contains_table[(unsigned char) data.bytes[i]] = 1;

        char *code = encoder[(unsigned char) data.bytes[i]];
        int length = strlen(code);

        for (int j = 0; j < length; j++) {
            if (power >= 256) {
                pushBack(&compressed, new_byte);
                new_byte = 0;
                power = 1;
            }
            new_byte += power * (code[j] - '0');
            power *= 2;
        }
    }

    if (power != 1) {
        pushBack(&compressed, new_byte);
    }

    for (int i = 0; i < 256; i++) {
        if (contains_table[i]) {
            pushBack(compressed_encoder, (char) i);

            int length = strlen(encoder[i]);
            pushBack(compressed_encoder, length);

            new_byte = 0;
            power = 1;

            for (int j = 0; j < length; j++) {
                if (power >= 256) {
                    pushBack(compressed_encoder, new_byte);
                    new_byte = 0;
                    power = 1;
                }
                new_byte += power * (encoder[i][j] - '0');
                power *= 2;
            }

            if (power != 1) {
                pushBack(compressed_encoder, new_byte);
            }
        }
    }

    free(contains_table);
    contains_table = NULL;

    // for (int i = 0; i < 256; i++) {
    //     free(encoder[i]);
    //     encoder[i] = NULL;
    // }

    free(encoder);
    encoder = NULL;

    return compressed;
}

void insertNode(Node **root, char byte, int c_size, int code) {
    if ((*root) == NULL) {
        (*root) = (Node *) malloc(sizeof(Node));

        (*root)->left = NULL;
        (*root)->right = NULL;
    }

    if (c_size == 0) {
        (*root)->value = byte;
        return;
    }

    if (code % 2 == 0) {
        insertNode(&((*root)->left), byte, c_size - 1, code / 2);
    } else {
        insertNode(&((*root)->right), byte, c_size - 1, code / 2);
    }
}

Node *createDecodingTree(ArrayList compressed_encoder) {
    Node *root = NULL;

    for (int i = 0; i < compressed_encoder.size; i++) {
        char byte = compressed_encoder.bytes[i++];
        int c_size = (unsigned char) compressed_encoder.bytes[i++];
        int code = 0;

        int bytes_count = (c_size % 8 == 0) ? c_size / 8 : c_size / 8 + 1;
        int power = 1;

        for (int j = i; j < i + bytes_count; j++) {
            code += ((unsigned char) compressed_encoder.bytes[j]) * power;
            power *= 256;
        }
        i += bytes_count - 1;

        insertNode(&root, byte, c_size, code);
    }

    return root;
}

ArrayList extract(ArrayList data, ArrayList compressed_encoder) {
    ArrayList extracted = declareList();

    Node *root = createDecodingTree(compressed_encoder);
    Node *cur_node = root;

    int idx = 0;
    int bit_count = 8;
    short isEnd = 0;

    while (idx < data.size) {
        while (cur_node->left != NULL && cur_node->right != NULL) {
            if (idx >= data.size) {
                isEnd = 1;
                break;
            }

            unsigned char byte = data.bytes[idx];
            unsigned char bit = byte % 2;

            if (bit == 0) {
                cur_node = cur_node->left;
            } else {
                cur_node = cur_node->right;
            }
            bit_count--;
            data.bytes[idx] = byte / 2;

            if (bit_count == 0) {
                idx++;
                bit_count = 8;
            }
        }

        if (!isEnd) {
            pushBack(&extracted, cur_node->value);
            cur_node = root;
        }
    }

    return extracted;
}