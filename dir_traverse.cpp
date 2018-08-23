#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>

#include "dir_traverse.h"

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


void ls_dir(char* dir)
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
        char date[10];

        lstat(entry->d_name,&statbuf);
        strftime(date, 10, "%d-%m-%y", localtime(&(statbuf.st_ctime)));
        char permission[10];
        permission_str(statbuf, permission);
        struct passwd *pwd;
        pwd = getpwuid(statbuf.st_uid);
        printf(" %s     %s      %s     %ld Bytes       %.20s\n",permission, pwd->pw_name ,date, statbuf.st_size, entry->d_name);
    }
    chdir("..");
    closedir(dp);
}

