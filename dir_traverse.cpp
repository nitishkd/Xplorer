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
#include <bits/stdc++.h>
using namespace std;


bool comparator(FS a, FS b)
{
    return (strcmp(a.FName ,b.FName) < 0 );
}


void ls_subtree(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) 
    {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    
    chdir(dir);
    while((entry = readdir(dp)) != NULL) 
    {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) 
        {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            ls_subtree(entry->d_name,depth+4);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
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


vector<FS> ls_dir(char* dir)
{
    vector<FS> Dirlist;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) 
    {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return Dirlist;
    }
    
    chdir(dir);
    while((entry = readdir(dp)) != NULL) 
    {
        char date[10];
        lstat(entry->d_name,&statbuf);
        strftime(date, 10, "%d-%m-%y", localtime(&(statbuf.st_ctime)));
        char permission[11];
        permission_str(statbuf, permission);
        struct passwd *pwd;
        pwd = getpwuid(statbuf.st_uid);
        FS dir_file;

        dir_file.dateStr = (char*)malloc(strlen(date)+1);
        strcpy(dir_file.dateStr, date);
        
        dir_file.permission = (char*)malloc(strlen(permission)+1);
        strcpy(dir_file.permission, permission);

        dir_file.u_name = (char*)malloc(strlen(pwd->pw_name)+1);
        strcpy(dir_file.u_name, pwd->pw_name);

        dir_file.FileSize = statbuf.st_size;
        
        dir_file.FName = (char*)malloc(strlen(entry->d_name)+1);
        strcpy(dir_file.FName, entry->d_name);

        Dirlist.push_back(dir_file);
        
    }
    chdir("..");
    closedir(dp);
    return Dirlist;
}

void ls_dir_wrapper(char* source)
{
    vector<FS> Listdir = ls_dir(source);
    sort(Listdir.begin(), Listdir.end(), comparator);

    for(int i = 0; i < Listdir.size(); ++i)
        printf(" %s     %s      %s     %ld Bytes       %.30s\n",Listdir[i].permission, Listdir[i].u_name ,Listdir[i].dateStr, Listdir[i].FileSize, Listdir[i].FName);
    
}
