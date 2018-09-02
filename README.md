# Xplorer
Linux File Explorer

This is assignment submission for the OS(PG) Course in session 2018-2019.
User is expected to follow the syntax commands from the manual.

To enter in command mode, you need to press ':' , without ''.  After the execution of command, it return back to normal mode. So, to enter another command again, press : again.

For the instance of examples, lets consider the executable is present in /home/username . Thus, the root is /home/username. 
In every path, user is expected NOT to add ```/``` at the end of path.

List of commands are as follow:
 
1) ```snapshot {relative path of directory} {filenametodump}``` : This command is used to take snapshot of the subtree of the specified directory.

      ```console
           snapshot /Downloads snapshotDump.txt 
      ```

2) ```search {pattern}``` : This command is used to search for a pattern in the subtree where user is present It lets user open files and traverse directory from there.

      ```console
           search /Downloads 
      ```
 
 3) ```rename {oldfilename} {newfilename}``` : It renames the file if it is present in the current working directory.
   
      ```console
           raname foo.txt bar.txt 
      ```

4) ```goto {directory_path}``` : Goto lets you jump to any directory you want to visit. User need to specify directory path which should be absolute.
  
      ```console
           goto Downloads/assignments 
      ```
      
5) ```create_file {file_name} {destination}``` : Enables user to create file in specified directory. if path is ```~/Desktop ``` , then ```~``` is expected to specify the address relative to home. Else, relative path from root is expected. Use of ```.``` shows that user wants to create file in the same directory. NOTE: File name cannot contain spaces.
  
      ```console
           create_file foo.txt ~/foobar
           create_file foo.txt .
      ```

6) ```create_dir {dir_name} {destination}``` : Enables user to create directory in specified directory. if path is ```~/Desktop ``` , then ```~``` is expected to specify the address relative to home. Else, relative path from root is expected. Use of ```.``` shows that user wants to create directory in the same directory. NOTE: Directory name cannot contain spaces.
  
      ```console
           create_dir foo ~/foobar
           create_dir foo .
      ```

7) ```copy_file {list of source files} {destination directory}``` : Copies files. Destination address syntax is similar to specified in ```create_dir``` command. List of source files contains list of source files in the current working directory seperated by space.
  
      ```console
           copy_file foo.txt bar.txt baz.mp4 ~/foobar
      ```

8) ```copy_dir {source dir} {destination directory}``` : Copy a single directory. Destination address takes only absolute path wrt root . Source directory should be present in the current working directory. Copies single directory at a time.
  
      ```console
           copy_dir foo ~/foobar
      ```

9) ```delete_file {filename}``` :Delete a file. Filename should be present with absoulte path wrt root.

      ```console
           delete_file /Downloads/foo.txt
      ```

10) ```delete_dir {dirname}``` : Delete a directory. Directory name should be present with absoulte path wrt root.

      ```console
           delete_dir /Downloads/foo
      ```

11) ```move_file {list of source files} {destination directory}``` : Moves listed files. Destination address syntax is similar to specified in ```create_dir``` command. List of source files contains list of source files in the current directory.
  
      ```console
           move_file foo.txt bar.txt baz.mp4 ~/foobar
      ```

12) ```move_dir {source dir} {destination directory}``` : Moves a single directory. Destination address syntax is similar to specified in ```copy_dir``` command. Source directory should be present in the current working directory. Moves single directory at a time.
  
      ```console
           move_dir foo ~/foobar
      ```

13) ```quit``` : Quits the application.



  

