/*
 By: Justin Meiners

 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#include "Console.h"
#include "ConsoleStdLib.h"

int main(int argc, const char * argv[])
{

    ConsoleRef console = Console_Create(stdout);
    ConsoleStdLib_Register(console);

    Console_RegisterVar(console, "test_string", kConsoleVarTypeString, 0);
    Console_RegisterVar(console, "test_int", kConsoleVarTypeInt, 0);
    Console_RegisterVar(console, "test_double", kConsoleVarTypeDouble, 0);

    /* var identification */
    Console_Execute(console, "inspect TRUE");
    /* int literal identification */
    Console_Execute(console, "inspect 1");
    /* double literal identification */
    Console_Execute(console, "inspect 1.5");
    /* string literals */
    Console_Execute(console, "echo \"how are you today\"");
    
    /* string assignment */
    Console_Execute(console, "set test_string \"This console is cool!\"");
    /* string vars */
    Console_Execute(console, "echo test_string");
    Console_Execute(console, "inspect test_string");

    /* int assignment */
    Console_Execute(console, "set test_int 100");
    /* int vars */
    Console_Execute(console, "echo test_int");
    Console_Execute(console, "inspect test_int");

    /* double assignment */
    Console_Execute(console, "set test_double 50.0");
    /* double vars */
    Console_Execute(console, "echo test_double");
    Console_Execute(console, "inspect test_double");
    
    /* int to double conversion */
    Console_Execute(console, "set test_double test_int");
    Console_Execute(console, "echo test_double");

    /* double to int conversion */
    Console_Execute(console, "set test_int test_double");
    Console_Execute(console, "echo test_int");
    
    Console_Destroy(console);
    
    return 0;
}

