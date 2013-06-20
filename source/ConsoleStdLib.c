
/*
 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "ConsoleStdLib.h"
#include <math.h>

static int _Console_Inspect(ConsoleRef console, ConsoleArgRef arg)
{
    const char* typeString = "unknown";
    
    switch (ConsoleVar_Type(arg->var))
    {
        case kConsoleVarTypeString:
            typeString = "string";
            break;
        case kConsoleVarTypeInt:
            typeString = "int";
            break;
        case kConsoleVarTypeDouble:
            typeString = "double";
            break;
        case kConsoleVarTypeBool:
            typeString = "bool";
            break;
        default:
            break;
    }
    
    fprintf(Console_Log(console), "var type: %s\n", typeString);
    
    return 1;
}

static int _Console_Echo(ConsoleRef console, ConsoleArgRef arg)
{
    switch (ConsoleVar_Type(arg->var))
    {
        case kConsoleVarTypeString:
            fprintf(Console_Log(console), "%s\n", ConsoleVar_StringValue(arg->var));
            break;
        case kConsoleVarTypeInt:
            fprintf(Console_Log(console), "%i\n", ConsoleVar_IntValue(arg->var));
            break;
        case kConsoleVarTypeDouble:
            fprintf(Console_Log(console), "%lf\n", ConsoleVar_DoubleValue(arg->var));
            break;
        case kConsoleVarTypeBool:
            
            if (ConsoleVar_IntValue(arg->var))
            {
                fprintf(Console_Log(console), "TRUE\n");
            }
            else
            {
                fprintf(Console_Log(console), "FALSE\n");
            }
            
            break;
        default:
            break;
    }
    
    return 1;
}



static int _Console_Set(ConsoleRef console, ConsoleArgRef arg)
{
    ConsoleArgRef first = arg;
    ConsoleArgRef second = arg->next;
    
    if (!first || !second)
    {
        return 0;
    }
    
    if (ConsoleVar_Readonly(first->var))
    {
        return 0;
    }
    
    switch (ConsoleVar_Type(first->var))
    {
        case kConsoleVarTypeString:
            ConsoleVar_SetStringValue(first->var, ConsoleVar_StringValue(second->var));
            break;
        case kConsoleVarTypeDouble:
            ConsoleVar_SetDoubleValue(first->var, ConsoleVar_DoubleValue(second->var));
            break;
        case kConsoleVarTypeInt:
            ConsoleVar_SetIntValue(first->var, ConsoleVar_IntValue(second->var));
            break;
        case kConsoleVarTypeBool:
            ConsoleVar_SetIntValue(first->var, ConsoleVar_IntValue(second->var));
            break;
            
        default:
            break;
    }
    
    
    return 1;
}

void ConsoleStdLib_Register(ConsoleRef console)
{
    ConsoleVarRef vTrue = Console_RegisterVar(console,
                                              "TRUE",
                                              kConsoleVarTypeBool,
                                              kConsoleVarFlagReadonly);
    ConsoleVar_SetIntValue(vTrue, 1);
    
    ConsoleVarRef vFalse = Console_RegisterVar(console,
                                               "FALSE",
                                               kConsoleVarTypeBool,
                                               kConsoleVarFlagReadonly);
    ConsoleVar_SetIntValue(vFalse, 0);
    
    Console_RegisterCommand(console,
                            "inspect",
                            _Console_Inspect,
                            1);
    
    Console_RegisterCommand(console,
                            "echo",
                            _Console_Echo,
                            1);
    
    Console_RegisterCommand(console,
                            "set",
                            _Console_Set,
                            2);
    
}
