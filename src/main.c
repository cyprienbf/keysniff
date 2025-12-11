#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Welcome to keysniff!\n");
    
    char flags[] = "la";
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "ls -%s", flags);
    system(cmd);

    return 0;
}


