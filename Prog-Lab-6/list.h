#ifndef PROG_LAB_6_LIST_H
#define PROG_LAB_6_LIST_H

#define LISTCAPASITY 1000
#define SLISTCAPASITY 50
#define LINESIZE 300

typedef struct {
    int size;
    int capacity;
    char *bytes;
} ArrayList;

typedef struct {
    int size;
    int capacity;
    char **values;
} s_ArrayList;

void *catch(void *pointer, int line);

ArrayList declareList();

void pushBack(ArrayList *list, char new_byte);

s_ArrayList s_declareList();

void s_pushBack(s_ArrayList *list, char *new_value);

#endif //PROG_LAB_6_LIST_H
