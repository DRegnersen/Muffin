#include <stdio.h>
#include "baker.h"
#include "compressor.h"

int main() {
    Baked list = declareList();

    pushBack(&list, 'a');
    pushBack(&list, 'a');
    pushBack(&list, 'b');
    pushBack(&list, 'b');
    pushBack(&list, 'b');
    pushBack(&list, 'c');

    char **encoder = createEncoder(list.size, list.bytes);

    printf("a - %s\n", encoder[(unsigned char) 'a']);
    printf("b - %s\n", encoder[(unsigned char) 'b']);
    printf("c - %s\n", encoder[(unsigned char) 'c']);

    return 0;
}
