#ifndef _dir_traverse_h
#define _dir_traverse_h

#include <string>
#include <vector>

struct FS
{
    char* permission;
    char* u_name;
    char* dateStr;
    long FileSize;
    char* FName;
};

void ls_subtree(char *dir, int depth);

void permission_str(struct stat filestat, char* perm);

std::vector<FS> ls_dir(char* dir);

void print_dir(std::vector<FS> Listdir);

#endif