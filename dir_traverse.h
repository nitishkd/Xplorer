#ifndef _dir_traverse_h
#define _dir_traverse_h

#include <string>
#include <vector>

struct FS
{
    std::string permission;
    std::string u_name;
    std::string dateStr;
    long FileSize;
    std::string FName;
};

void ls_subtree(char *dir, int depth);

void permission_str(struct stat filestat, char* perm);

std::vector<FS> ls_dir(std::string dir);

std::vector<FS> ls_dir_wrapper(std::string source);

#endif