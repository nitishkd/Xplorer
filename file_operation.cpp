#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "file_operation.h"

void copyfile(char* filename, char* outfilename )
{
	char block[1024];
	struct stat statistics;
	stat(filename, &statistics);
	int in, out,nread;
	in = open(filename, O_RDONLY);
	out = open(outfilename, statistics.st_mode);
	while((nread = read(in, block,sizeof(block))) > 0)
		write(out, block,nread);
		
}
void createfile(char* filename)
{
	fstream fs;
	fs.open(filename, ios::out);
	fs.close();
}
int removefile(char* filename)
{
	int status = remove(filename);
	return status;	
}

void movefile(char* source, char* destination)
{
	copyfile(source, destination);
	removefile(source);
}

void copy_dir(char *source, char* dest)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(source)) == NULL) 
        return;
	
	lstat(source,&statbuf);
	mkdir(dest, statbuf.st_mode);
	chdir(source);
	while((entry = readdir(dp)) != NULL) 
    {
		lstat(entry->d_name,&statbuf);
		char *new_source, *new_dest;
		new_source = (char*)malloc(strlen(source) + 1 + 1 + strlen(entry->d_name));
		strcpy(new_source, source);
		strcat(new_source, "/");
		strcat(new_source, entry->d_name);
		new_dest = (char*)malloc(strlen(dest) + 1 + 1 + strlen(entry->d_name));
		strcpy(new_dest, dest);
		strcat(new_dest, "/");
		strcat(new_dest, entry->d_name);
		
        if(S_ISDIR(statbuf.st_mode)) 
        {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            copy_dir(new_source,new_dest);
        }
        else
		{
			copyfile(new_source, new_dest);
		}
    }
    chdir("..");
    closedir(dp);
}

void copy_dir_wrapper(char* source, char* dest)
{
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(source)) == NULL) 
        return;
	
	lstat(source,&statbuf);
	int n = strlen(source);
	int i;
	for(i =n-1; i >= 0; --i)
	{
		if(source[i] == '/')
			break;
	}
	char dirname[n-i];
	for(int j = i+1; j <= n; ++j)
		dirname[j-i-1] = source[j];
	char mdir[2048];
	strcpy(mdir, dest);
	strcat(mdir, "/");
	strcat(mdir, dirname);

	mkdir(mdir, statbuf.st_mode);
	copy_dir(source, mdir);
	closedir(dp);
}

void remove_dir(char* source)
{
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(source)) == NULL) 
        return;
	
	chdir(source);
	while((entry = readdir(dp)) != NULL) 
    {
		lstat(entry->d_name,&statbuf);
		char *new_source;
		new_source = (char*)malloc(strlen(source) + 1 + 1 + strlen(entry->d_name));
		strcpy(new_source, source);
		strcat(new_source, "/");
		strcat(new_source, entry->d_name);

        if(S_ISDIR(statbuf.st_mode)) 
        {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            remove_dir(new_source);
        }
        else
		{
			removefile(new_source);
		}
    }
	rmdir(source);
    chdir("..");
    closedir(dp);

}

void move_dir(char* source, char* dest)
{
	copy_dir_wrapper(source, dest);
	remove_dir(source);
}

void search(string dir, vector<pair<string,string> > &fname,string parent)
{
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir.c_str())) == NULL) 
        return;
    
    chdir(dir.c_str());
    while((entry = readdir(dp)) != NULL) 
    {
        lstat(entry->d_name,&statbuf);
        string name = entry->d_name;
        if(S_ISDIR(statbuf.st_mode)) 
        {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            fname.push_back({name,parent});
            search(name,fname,parent+"/"+name);
        }
        else
            fname.push_back({name,parent});
    }
	chdir("..");
    closedir(dp);

}

vector<pair<string,string> > search_util(string pattern, string dir)
{
	vector<pair<string,string> > fname,resList;
	search(dir, fname, dir);
	for(int i =0; i < fname.size(); ++i)
	{
		if(fname[i].first.find(pattern) != std::string::npos)
			resList.push_back(fname[i]);
	}
	return resList;
}
