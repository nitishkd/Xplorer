#ifndef _dir_traverse_h
#define _dir_traverse_h

#include <string>
#include <vector>
using namespace std;
struct FS
{
    std::string permission;
    std::string u_name;
    std::string dateStr;
    long FileSize;
    std::string FName;
};

void ls_subtree(std::string dir);

std::string GetCurrentWorkingDir();

void permission_str(struct stat filestat, char* perm);

std::vector<FS> ls_dir(std::string dir);

std::vector<FS> ls_dir_wrapper(std::string source);

void display_window(std::vector<FS> Listdir, int start, int end);

void display_search_result(string pattern, vector<pair<string,string> > Listdir, int start, int end);

#endif