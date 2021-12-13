#ifndef PROG_LAB_6_BAKER_H
#define PROG_LAB_6_BAKER_H

ArrayList bake(s_ArrayList filenames);

ArrayList bakeEncoder(ArrayList encoder, ArrayList data);

void split(ArrayList data, char *directory);

void splitEncoder(ArrayList *encoder, ArrayList *data, char *filename);

#endif //PROG_LAB_6_BAKER_H
