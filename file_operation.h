#ifndef _file_operation_h
#define _file_operation_h

#include <bits/stdc++.h>
using namespace std;

void copyfile(char* filename, char* outfilename );

int removefile(char* filename);

void movefile(char* source, char* destination);

void copy_dir(char *source, char* dest);

void copy_dir_wrapper(char* source, char* dest);

void remove_dir(char* source);

void move_dir(char* source, char* dest);

void search(string dir, vector<pair<string,string> > &fname,string parent);

vector<pair<string,string> > search_util(string pattern, string dir);


#endif
