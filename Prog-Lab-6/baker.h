#ifndef PROG_LAB_6_BAKER_H
#define PROG_LAB_6_BAKER_H

ArrayList bake(s_ArrayList filenames);

ArrayList bakeEncoder(ArrayList encoder, ArrayList data);

void splitEncoder(ArrayList *encoder, ArrayList *data, char *filename);

void splitAndCreate(ArrayList data, char *directory);

s_ArrayList getFilenames(ArrayList data);

#endif //PROG_LAB_6_BAKER_H
