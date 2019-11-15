 /******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "import.h"
#include "cmds.h"

int main()
{
//    printf("Hello World");

//    filedata *f = f_read("test.txt");
    cmdstatus *cs = c_create();
    
    c_key_init(cs);
    while(1) {
        char *cmd = c_cmd_get(40);
        printf("\n");
        if (!strncmp(cmd, "quit", strlen("quit"))) {
            printf("bye!\n");
            exit(0);
        }
            
        c_execute(cs, cmd);
    }
/*
    c_execute(cs, strdup("read test.txt"));
    c_execute(cs, strdup("tmake test.dat"));
    c_execute(cs, strdup("tshow 1"));
    */
//    printf("\n%s\n", cmd);

    return 0;  
}

