#include "file_operation.h"
#include <stdio.h>

int main()
{
    //copy_dir_wrapper("/home/nitish/code/college", "/home/nitish");
    //remove_dir("/home/nitish/Desktop/project");
    move_dir("/home/nitish/Desktop/college", "/home/nitish");
    return 0;
}