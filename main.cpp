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

vector<string> split(std::string &txt, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    vector<string>strs;
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs;
}


string filename(string path)
{
    vector<string> V = split(path, '/');
    return V[V.size()-1];
}

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
    vector<FS> DirList = ls_dir_wrapper(cur_path,homedir);
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
                if(T.FName == ".." and GetCurrentWorkingDir() == homedir)
                    continue;
                STB.push(cur_path);
                DirList.clear();
                string fname = GetCurrentWorkingDir();
                fname += "/";
                fname += T.FName;
                cur_path = fname;
                DirList = ls_dir_wrapper(fname,homedir);
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
                DirList = ls_dir_wrapper(fname,homedir);
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
                DirList = ls_dir_wrapper(fname,homedir);
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
            DirList = ls_dir_wrapper(fname,homedir);
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
            DirList = ls_dir_wrapper(fname,homedir);
            cur_path = GetCurrentWorkingDir();
            position = 0;
            start = 0;
            end = min((int)DirList.size()-1, nrow-4);
            cursorup(end - start +1 );
            continue;
        }
        if(c == 58)
        {
            cursordown(nrow-2);
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
                if(folder[0] == '/')
                    folder = homedir + folder;
                else 
                    folder = homedir + "/" + folder;
                
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
            else if(command == "rename")
            {
                string source,dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                source = GetCurrentWorkingDir() + "/" + source;
                dest = GetCurrentWorkingDir() + "/" + dest;
                int status = rename(source.c_str(), dest.c_str());    
                sleep(5);
            }
            else if(command == "goto")
            {
                string source;
                cin>>source;
                c = kbget();
                STB.push(GetCurrentWorkingDir());
                if(source == "/")
                    source = homedir;
                else
                {
                    if(source[0] == '/')
                        source = homedir + source;
                    else
                        source = homedir + "/" + source;
                }
                    
                curdir = source;
            }
            else if(command == "create_file")
            {
                //TODO
                string filename,dest;
                cin>>filename;
                c = kbget();
                cin>>dest;
                c = kbget();
                if(dest == ".")
                    dest = GetCurrentWorkingDir();
                else if(dest[0] == '~')
                {
                    string tmp = homedir;
                    for(int i =1; i < dest.length(); ++i)
                        tmp += dest[i];
                    dest = tmp;
                    dest += "/";
                }
                dest += filename;
                char name[2018];
                strcpy(name, dest.c_str());
                createfile(name);
                sleep(2);
            }
            else if(command == "create_dir")
            {
                //TODO
                string filename,dest;
                cin>>filename;
                c = kbget();
                cin>>dest;
                c = kbget();

                if(dest == ".")
                    dest = GetCurrentWorkingDir();
                else if(dest[0] == '~')
                {
                    string tmp = homedir;
                    for(int i =1; i < dest.length(); ++i)
                        tmp += dest[i];
                    dest = tmp;
                    dest += "/";
                }
                dest += filename;
                mkdir(dest.c_str(), 0777);
                sleep(2);
            }
            else if(command == "copy")
            {
                string source,text, dest;
                getline(cin,text);
                //c = kbget();

                std::istringstream iss(text);
                std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                 std::istream_iterator<std::string>());
                dest = results[results.size()-1];
                if(dest[0] == '~')
                {
                    string tmp = homedir;
                    for(int i =1; i < dest.length(); ++i)
                        tmp += dest[i];
                    dest = tmp;
                }
                dest += "/";
                for(int i = 0; i < results.size()-1; ++i)
                {
                    string path = GetCurrentWorkingDir();
                    path += "/"; 
                    source = results[i];
                    string fname = filename(source);
                    source = path + source;
                    string tdest = dest + fname;
                    char sname[2048], dname[2048];
                    strcpy(sname, source.c_str());
                    strcpy(dname, tdest.c_str());
                    copyfile(sname, dname);
                    
                }
                sleep(2);
            }
            else if(command == "copy_dir")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                if(source[0] == '/')
                    source = homedir + source;
                else
                    source = homedir + "/" + source;
                if(dest[0] == '/')
                    dest = homedir + dest;
                else
                    dest = homedir + "/" + dest;
                    
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                copy_dir_wrapper(sname, dname);
                sleep(2);
            }
            else if(command == "delete_dir")
            {
                string source;
                cin>>source;
                c = kbget();
                if(source[0] == '/')
                    source = homedir + source;
                else
                    source = homedir + "/" + source;
                
                char name[2048];
                strcpy(name, source.c_str());
                remove_dir(name);
                sleep(2);
            }
            else if(command == "delete_file")
            {
                string source;
                cin>>source;
                c = kbget();
                if(source[0] == '/')
                    source = homedir + source;
                else
                    source = homedir + "/" + source;
                char name[2048];
                strcpy(name, source.c_str());
                removefile(name);
                sleep(2);
            }
            else if(command == "move_file")
            {
                string source,text, dest;
                getline(cin,text);
                //c = kbget();

                std::istringstream iss(text);
                std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                 std::istream_iterator<std::string>());
                dest = results[results.size()-1];
                if(dest[0] == '~')
                {
                    string tmp = homedir;
                    for(int i =1; i < dest.length(); ++i)
                        tmp += dest[i];
                    dest = tmp;
                }
                dest += "/";
                for(int i = 0; i < results.size()-1; ++i)
                {
                    string path = GetCurrentWorkingDir();
                    path += "/"; 
                    source = results[i];
                    string fname = filename(source);
                    source = path + source;
                    string tdest = dest + fname;
                    char sname[2048], dname[2048];
                    strcpy(sname, source.c_str());
                    strcpy(dname, tdest.c_str());
                    movefile(sname, dname);
                    
                }
                sleep(2);
                
            }
            else if(command == "move_dir")
            {
                string source, dest;
                cin>>source;
                c = kbget();
                cin>>dest;
                c = kbget();
                if(source[0] == '/')
                    source = homedir + source;
                else
                    source = homedir + "/" + source;
                
                if(dest[0] == '/')
                    dest = homedir + dest;
                else
                    dest = homedir + "/" + dest;
                 
                char sname[2048], dname[2048];
                strcpy(sname, source.c_str());
                strcpy(dname, dest.c_str());
                move_dir(sname, dname);
                sleep(2);
            }
            else if(command == "search")
            {
                string pattern;
                cin>>pattern;
                c = kbget();
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                nrow = w.ws_row;
                ncol = w.ws_col;
                vector<pair<string,string> > Vec = search_util(pattern, cur_path);
                display_search_result(pattern, Vec, 0, min((int)Vec.size()-1, nrow-4));
                int tposition=0,tstart=0, tend=min((int)Vec.size()-1, nrow-4);
                cursorup(tend - tstart +1);
                while(true)
                {
                    c = kbget();
                    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                    nrow = w.ws_row;
                    ncol = w.ws_col;
                    //cout<<nrow<<" "<<ncol<<endl;
                    if(c == KEY_UP and tposition > 0)
                    {
                        if(tposition <= tstart)
                        {
                            --tstart;
                            --tposition;
                            --tend;
                            display_search_result(pattern, Vec, tstart, tend);
                            cursorup(tend-tstart+1);
                            continue;
                        }
                        --tposition;
                        cursorup(1);
                        continue;
                    }
                    if(c == KEY_DOWN and tposition < Vec.size()-1)
                    {
                        if(tposition >= tend)
                        {
                            ++tstart;
                            ++tend;
                            ++tposition;
                            display_search_result(pattern, Vec, tstart, tend);
                            cursorup(1);
                            continue;
                        }
                        ++tposition;
                        cursordown(1);
                        continue;
                    }
                    if(c == KEY_ENTER)
                    {
                        struct stat statbuf;
                        pair<string,string> T = Vec[tposition];
                        string filename = T.second + "/" + T.first;
                        lstat(filename.c_str(),&statbuf);
                        if(S_ISDIR(statbuf.st_mode))
                        {
                            STB.push(curdir);
                            DirList.clear();
                            string fname = filename;
                            cur_path = filename;
                            curdir = filename;
                            break;
                        }
                        else
                        {
                            int pid = fork();
                            if (pid == 0)
                            {
                                execl("/usr/bin/xdg-open", "xdg-open", filename.c_str(), (char *)0);
                                exit(1);
                            }
                        }
                        continue;
                        
                    }
                    if(c == 127 or c == KEY_LEFT)
                        break;
                    
                }

            }
            else if(command == "quit")
                return 0;

            DirList.clear();
            DirList = ls_dir_wrapper(curdir,homedir);
            position = 0;
            start = 0;
            end = min((int)DirList.size()-1, nrow-4);
            cursorup(end - start +1 );
            continue;
        }


    }

    return 0;
}