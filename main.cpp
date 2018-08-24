#include "file_operation.h"
#include "screen_man.h"
#include "dir_traverse.h"
#include <stdio.h>
#include <bits/stdc++.h>
#include <string.h>
using namespace std;

int main()
{
    clear_util();
    
    int c;
    ls_dir_wrapper("/home/nitish/code/college/OS/");

    while (true) 
    {
    
        c = kbget();
        // if (c == KEY_ENTER || c == KEY_ESCAPE || c == KEY_UP || c == KEY_DOWN) {
        //     break;
        // }
        
        if (c == KEY_RIGHT) 
            cursorbackward(1);
        else if (c == KEY_LEFT)
            cursorforward(1);
        else if(c == KEY_UP)
        	cursorup(1);
        else if(c == KEY_DOWN)
        	cursordown(1);

    }

    return 0;
}