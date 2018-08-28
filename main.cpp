#include "file_operation.h"
#include "screen_man.h"
#include "dir_traverse.h"
#include <stdio.h>
#include <bits/stdc++.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
using namespace std;

std::string GetCurrentWorkingDir() {
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

stack<string> STF, STB;

#define debug(x) printf("Checkpoint %d \n", x )

int main()
{
    int c,nrow, ncol;
    string cur_path = GetCurrentWorkingDir() ;
    string homedir = cur_path;
    vector<FS> DirList = ls_dir_wrapper(cur_path);
    int position = 0;    
    cursorup(DirList.size());
    
    while (true)    
    {

        c = kbget();
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        nrow = w.ws_row;
        ncol = w.ws_col;

        if(c == KEY_UP and position > 0)
        {
            --position;
        	cursorup(1);
            continue;
        }
        if(c == KEY_DOWN and position < DirList.size()-1)
        {
            ++position;
        	cursordown(1);
            continue;
        }
        if(c == KEY_ENTER)
        {
            struct stat statbuf;
            FS T = DirList[position];
            lstat(T.FName.c_str(),&statbuf);
            if(S_ISDIR(statbuf.st_mode))
            {
                STB.push(cur_path);
                DirList.clear();
                string fname = GetCurrentWorkingDir();
                fname += "/";
                fname += T.FName;
                cur_path = fname;
                DirList = ls_dir_wrapper(fname);
                position = 0;
                cursorup(DirList.size());
            }
            else
            {
                string fname = GetCurrentWorkingDir();
                fname += "/";
                fname += T.FName;
                int pid = fork();
                if (pid == 0)
                {
                    execl("/usr/bin/xdg-open", "xdg-open", fname.c_str(), (char *)0);
                    exit(1);
                }
            }
            continue;
            
        }
        if(c == KEY_LEFT)
        {
            if(!STB.empty())
            {
                string fname = STB.top();
                STB.pop();
                STF.push(GetCurrentWorkingDir());
                cur_path = fname;
                DirList.clear();
                DirList = ls_dir_wrapper(fname);
                position = 0;
                cursorup(DirList.size());
            }
            continue;
        }
        if(c == KEY_RIGHT)
        {
            if(!STF.empty())
            {
                string fname = STF.top();
                STF.pop();
                STB.push(GetCurrentWorkingDir());
                cur_path = fname;
                DirList.clear();
                DirList = ls_dir_wrapper(fname);
                position = 0;
                cursorup(DirList.size());    
            }
            continue;
        }
        if(c == 127)
        {   
            string fname = homedir;
            STB.push(GetCurrentWorkingDir());
            while(!STF.empty())
                STF.pop();
                
            DirList.clear();
            DirList = ls_dir_wrapper(fname);
            cur_path = homedir;
            cursorup(DirList.size());
            continue;
        }

    }

    return 0;
}