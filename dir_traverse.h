#ifndef _dir_traverse_h
#define _dir_traverse_h

void ls_subtree(char *dir, int depth);

void permission_str(struct stat filestat, char* perm);

void ls_dir(char* dir);

#endif