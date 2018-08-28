#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <vector>
#include <string>
#include "dir_traverse.h"
#include "screen_man.h"
#include <bits/stdc++.h>
using namespace std;


bool comparator(FS a, FS b)
{
    a.FName < b.FName;
}


void ls_subtree(string dir)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir.c_str())) == NULL) 
    {
        //fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    vector<string> fname;   
    chdir(dir.c_str());
    while((entry = readdir(dp)) != NULL) 
    {
        lstat(entry->d_name,&statbuf);
        string name = entry->d_name;
        if(S_ISDIR(statbuf.st_mode)) 
        {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            fname.push_back(name);
            ls_subtree(name);
        }
        else
            fname.push_back(name);
    }
    cout<<dir<<endl;
    for(int i =0; i < fname.size(); ++i)
        printf("%10s ", fname[i].c_str());
    printf("\n");
    printf("\n");
    chdir("..");
    closedir(dp);
}

void permission_str(struct stat filestat, char* perm)
{
    
    perm[0] = (S_ISDIR(filestat.st_mode)) ? 'd' :  '-';
    perm[1] = (filestat.st_mode & S_IRUSR) ? 'r' : '-';
    perm[2] = (filestat.st_mode & S_IWUSR) ? 'w' : '-';
    perm[3] = (filestat.st_mode & S_IXUSR) ? 'x' : '-';
    perm[4] = (filestat.st_mode & S_IRGRP) ? 'r' : '-';
    perm[5] = (filestat.st_mode & S_IWGRP) ? 'w' : '-';
    perm[6] = (filestat.st_mode & S_IXGRP) ? 'x' : '-';
    perm[7] = (filestat.st_mode & S_IROTH) ? 'r' : '-';
    perm[8] = (filestat.st_mode & S_IWOTH) ? 'w' : '-';
    perm[9] = (filestat.st_mode & S_IXOTH) ? 'x' : '-';

}

vector<FS> ls_dir(string dir)
{
    vector<FS> Dirlist;

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir.c_str())) == NULL) 
    {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return Dirlist;
    }
    cout<<dir<<endl;
    chdir(dir.c_str());

    while((entry = readdir(dp)) != NULL) 
    {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if((strcmp(".",entry->d_name) == 0) or (strcmp("..",entry->d_name) == 0))
                continue;
        }

        char date[15];
        strftime(date, 10, "%d-%m-%y", localtime(&(statbuf.st_ctime)));
        char permission[15];
        permission_str(statbuf, permission);
        struct passwd *pwd;
        pwd = getpwuid(statbuf.st_uid);
        FS dir_file;
        dir_file.dateStr = date;
        dir_file.permission = permission;
        dir_file.u_name = pwd->pw_name;
        dir_file.FileSize = statbuf.st_size;
        
        dir_file.FName = entry->d_name;

        Dirlist.push_back(dir_file);

    }
    closedir(dp);
    
    return Dirlist;
}

vector<FS> ls_dir_wrapper(string source)
{
    clear_util();
    vector<FS> Listdir = ls_dir(source);
    sort(Listdir.begin(), Listdir.end(), comparator);
    for(int i = 0; i < Listdir.size(); ++i)
        printf(" %12s     %10s      %10s     %10ld Bytes       %15s\n",Listdir[i].permission.c_str(), Listdir[i].u_name.c_str() ,Listdir[i].dateStr.c_str(), Listdir[i].FileSize, Listdir[i].FName.c_str());
    
    return Listdir;
}
