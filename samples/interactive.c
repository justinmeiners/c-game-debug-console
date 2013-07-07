/*
 By: Justin Meiners

 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Console.h"
#include "ConsoleStdLib.h"

int main(int argc, const char * argv[])
{

    ConsoleRef console = Console_Create(stdout);
    ConsoleStdLib_Register(console);
    

    FILE* readFile = fopen("./console_save.txt", "r");
    if (readFile)
    {
        Console_Load(console, readFile);
        fclose(readFile);
    }
    
    printf("ready\n");
    printf("type quit to exit\n");

    size_t size = 1024;
    char* line = malloc(size);
    
    int shouldContinue = 1;

    while (shouldContinue)
    {
        getline(&line, &size, stdin);
        
        if (strstr(line, "quit") != NULL)
        {
            shouldContinue = 0;
        }
        else
        {
            Console_Execute(console, line);
        }
        
    }
    
    FILE* writeFile = fopen("./console_save.txt", "w");
    if (writeFile)
    {
        Console_Save(console, writeFile);
        fclose(writeFile);
    }

    free(line);
    Console_Destroy(console);
    
    printf("done\n");
    
    return 0;
}

