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


stack<string> STF, STB;

#define debug(x) printf("Checkpoint %d \n", x )

int main()
{
    freopen("/dev/null", "w", stderr);
    int c,nrow, ncol,start, end;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    nrow = w.ws_row;
    ncol = w.ws_col;

    string cur_path = GetCurrentWorkingDir();
    string homedir = cur_path;
    vector<FS> DirList = ls_dir_wrapper(cur_path);
    int position = 0;
    start = 0;    
    end = min((int)DirList.size()-1, nrow-4);
    cursorup(end - start +1);
    
    while (true)    
    {

        c = kbget();
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        nrow = w.ws_row;
        ncol = w.ws_col;
        //cout<<nrow<<" "<<ncol<<endl;
        if(c == KEY_UP and position > 0)
        {
            if(position <= start)
            {
                --start;
                --position;
                --end;
                display_window(DirList, start, end);
                cursorup(end-start+1);
                continue;
            }
            --position;
        	cursorup(1);
            continue;
        }
        if(c == KEY_DOWN and position < DirList.size()-1)
        {
            if(position >= end)
            {
                ++start;
                ++end;
                ++position;
                display_window(DirList, start, end);
                cursorup(1);
                continue;
            }
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
                start = 0;
                end = min((int)DirList.size()-1, nrow-4);
                cursorup(end - start +1 );
                
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
                start = 0;
                end = min((int)DirList.size()-1, nrow-4);
                cursorup(end - start +1 );
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
                start = 0;
                end = min((int)DirList.size()-1, nrow-4);
                cursorup(end - start +1 );   
            }
            continue;
        }
        if(c == 104)
        {   
            string fname = homedir;
            STB.push(GetCurrentWorkingDir());
            while(!STF.empty())
                STF.pop();
            DirList.clear();
            DirList = ls_dir_wrapper(fname);
            cur_path = homedir;
            position = 0;
            start = 0;
            end = min((int)DirList.size()-1, nrow-4);
            cursorup(end - start +1 );
            continue;
        }
        if(c == 127)
        {
            string fname = GetCurrentWorkingDir();
            if(fname == homedir)
                continue;
            STB.push(fname);
            fname += "/..";
            DirList.clear();
            DirList = ls_dir_wrapper(fname);
            cur_path = GetCurrentWorkingDir();
            position = 0;
            start = 0;
            end = min((int)DirList.size()-1, nrow-4);
            cursorup(end - start +1 );
            continue;
        }
        if(c == 58)
        {
            cursordown(nrow);
            cout<<":";
            string command;
            cin>>command;
            string curdir = GetCurrentWorkingDir();
            c = kbget();
            if(command == "snapshot")
            {
                string folder,fname,name;
                cin>>folder;
                c = kbget();
                cin>>fname;
                c = kbget();
                name = folder+"/"+fname;
                fstream file;
                file.open(name.c_str(), ios::out);
                string line;
                streambuf* stream_buffer_cout = cout.rdbuf();
                streambuf* stream_buffer_cin = cin.rdbuf();
                streambuf* stream_buffer_file = file.rdbuf();
                cout.rdbuf(stream_buffer_file);
                ls_subtree(folder);
                cout.rdbuf(stream_buffer_cout);
                file.close();
                sleep(5);
                
            }
            else if(command == "cp")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                copyfile(sname, dname);
                sleep(2);
            }
            else if(command == "cp-r")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                copy_dir_wrapper(sname, dname);
                sleep(2);
            }
            else if(command == "rm-r")
            {
                string source;
                cin>>source;
                c = kbget();
                char name[2048];
                strcpy(name, source.c_str());
                remove_dir(name);
                sleep(2);
            }
            else if(command == "rm")
            {
                string source;
                cin>>source;
                c = kbget();
                char name[2048];
                strcpy(name, source.c_str());
                removefile(name);
                sleep(2);
            }
            else if(command == "mv")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                movefile(sname, dname);
                sleep(2);
            }
            else if(command == "mv-r")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                move_dir(sname, dname);
                sleep(2);
            }
            else if(command == "search")
            {
                //TODO
            }
            else if(command == "quit")
                return 0;

            DirList.clear();
            DirList = ls_dir_wrapper(curdir);
            position = 0;
            start = 0;
            end = min((int)DirList.size()-1, nrow-4);
            cursorup(end - start +1 );
            continue;
        }


    }

    return 0;
}