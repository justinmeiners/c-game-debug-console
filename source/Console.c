
/*
 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "Console.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


#define CONSOLE_MAX_COMMANDS 128
#define CONSOLE_MAX_VARS 1024
#define CONSOLE_VAR_NAME_MAX 256
#define CONSOLE_VAR_STRING_MAX 1024

#define CONSOLE_MAX_TOKENS 128

struct ConsoleVar
{
    char name[CONSOLE_VAR_NAME_MAX];
    struct ConsoleVal;
    ConsoleVarType_t type;
    int intValue;
    double doubleValue;
    char stringValue[CONSOLE_VAR_STRING_MAX];
    ConsoleVarFlag_t flags;
    int temp;
};

struct ConsoleCommand
{
    char name[CONSOLE_VAR_NAME_MAX];
    int argCount;
    ConsoleFunc_t func;
};

struct Console
{
    ConsoleCommandRef commands[CONSOLE_MAX_COMMANDS];
    int commandCount;
    
    ConsoleVarRef vars[CONSOLE_MAX_VARS];
    int varCount;
    
    FILE* logFile;
};

static ConsoleVarRef _ConsoleVar_Create(ConsoleVarType_t type, int temporary)
{
    ConsoleVarRef var = malloc(sizeof(struct ConsoleVar));
    
    if (var)
    {
        var->intValue = 0;
        var->doubleValue = 0.0;
        var->type = type;
        var->flags = 0;
        var->temp = temporary;
    }
    
    return var;
}

ConsoleVarType_t ConsoleVar_Type(ConsoleVarRef var)
{
    assert(var);
    return var->type;
}

extern int ConsoleVar_Readonly(ConsoleVarRef var)
{
    assert(var);
    return var->flags & kConsoleVarFlagReadonly;
}

extern void ConsoleVar_SetDoubleValue(ConsoleVarRef var, double value)
{
    assert(var);
    assert(var->type == kConsoleVarTypeDouble);
    var->doubleValue = value;
    var->intValue = (int)value;
}

double ConsoleVar_DoubleValue(ConsoleVarRef var)
{
    assert(var);
    
    switch (var->type)
    {
        case kConsoleVarTypeDouble:
            return var->doubleValue;
        case kConsoleVarTypeInt:
            return var->doubleValue;
        case kConsoleVarTypeBool:
            return var->doubleValue;
        case kConsoleVarTypeString:
        {
            double val = 0.0;
            sscanf(var->stringValue, "%lf", &val);
            return val;
        }
        default:
            break;
    }
    
    return var->doubleValue;
}

void ConsoleVar_SetIntValue(ConsoleVarRef var, int value)
{
    assert(var);
    assert(var->type == kConsoleVarTypeInt ||
           var->type == kConsoleVarTypeBool);
    
    var->intValue = value;
    var->doubleValue = (double)value;
}

int ConsoleVar_IntValue(ConsoleVarRef var)
{
    assert(var);
    
    switch (var->type)
    {
        case kConsoleVarTypeDouble:
            return var->intValue;
        case kConsoleVarTypeInt:
            return var->intValue;
        case kConsoleVarTypeBool:
            return var->intValue;
        case kConsoleVarTypeString:
        {
            int val = 0.0;
            sscanf(var->stringValue, "%d", &val);
            return val;
        }
        default:
            break;
    }
}

void ConsoleVar_SetBoolValue(ConsoleVarRef var, int value)
{
    ConsoleVar_SetIntValue(var, value);
}

int ConsoleVar_BoolValue(ConsoleVarRef var)
{
    return ConsoleVar_IntValue(var);
}


void ConsoleVar_SetStringValue(ConsoleVarRef var, const char* string)
{
    assert(var);
    strcpy(var->stringValue, string);
}

const char* ConsoleVar_StringValue(ConsoleVarRef var)
{
    assert(var);
    return var->stringValue;
}

static ConsoleCommandRef _ConsoleCommand_Create()
{
    ConsoleCommandRef command = malloc(sizeof(struct ConsoleCommand));
    
    if (command)
    {
        command->func = NULL;
        command->argCount = -1;
    }
    
    return command;
}

/* lists all available commands */
static int _Console_Help(ConsoleRef console, ConsoleArgRef args)
{
    fprintf(Console_Log(console), "%i commands available\n", console->commandCount);
    
    int i;
    for (i = 0; i < console->commandCount; i ++)
    {
        fprintf(Console_Log(console), "%s ", console->commands[i]->name);
    }
    
    fprintf(Console_Log(console), "\n");
    
    return 1;
}

ConsoleRef Console_Create(FILE* logfile)
{
    if (!logfile)
    {
        return NULL;
    }
    
    ConsoleRef console = malloc(sizeof(struct Console));
    
    if (console)
    {
        console->commandCount = 0;
        console->varCount = 0;
        console->logFile = logfile;
        
        /* only built-in command */
        Console_RegisterCommand(console,
                                "help",
                                _Console_Help,
                                0);
    }
    
    return console;
}

void Console_Destroy(ConsoleRef console)
{
    if (console)
    {
        free(console);
    }
}

ConsoleVarRef Console_FindVar(ConsoleRef console, const char* name)
{
    assert(console);
    assert(name);
    
    int i;
    for (i = 0; i < console->varCount; i ++)
    {
        if (strcmp(console->vars[i]->name, name) == 0)
        {
            return console->vars[i];
        }
    }
    
    return NULL;
}

static ConsoleCommandRef _Console_FindCommand(ConsoleRef console, const char* name)
{
    for (int i = 0; i < console->commandCount; i ++)
    {
        if (strcmp(console->commands[i]->name, name) == 0)
        {
            return console->commands[i];
        }
    }
    
    return NULL;
}

void Console_Save(ConsoleRef console, FILE* outFile)
{
    assert(console);
    assert(outFile);
    
    for (int i = 0; i < console->varCount; i ++)
    {
        /* don't save readonly variables */
        if (ConsoleVar_Readonly(console->vars[i]))
        {
            continue;
        }
        
        fprintf(outFile, "%s : ", console->vars[i]->name);
        
        switch (console->vars[i]->type)
        {
            case kConsoleVarTypeString:
                fprintf(outFile, "%s\n", console->vars[i]->stringValue);
                break;
            case kConsoleVarTypeInt:
                fprintf(outFile, "%i\n", console->vars[i]->intValue);
                break;
            case kConsoleVarTypeDouble:
                fprintf(outFile, "%lf\n", console->vars[i]->doubleValue);
                break;
            case kConsoleVarTypeBool:
                fprintf(outFile, "%i\n", console->vars[i]->intValue);
                break;
            default:
                break;
        }
    }
}

int Console_Load(ConsoleRef console, FILE* inFile)
{
    assert(console);
    assert(inFile);
    
    char varName[128];
    char varValue[1024];
    
    while (!feof(inFile))
    {
        if (fscanf(inFile, "%s : ", varName) != 0)
        {
            ConsoleVarRef var = Console_FindVar(console, varName);
            
            if (!var)
            {
                return 0;
            }
            
            switch (var->type)
            {
                case kConsoleVarTypeString:
                {
                    fscanf(inFile, "%s\n", varValue);
                    ConsoleVar_SetStringValue(var, varValue);
                    break;
                }
                case kConsoleVarTypeInt:
                {
                    int intVar;
                    fscanf(inFile, "%i\n", &intVar);
                    ConsoleVar_SetIntValue(var, intVar);
                    break;
                }
                case kConsoleVarTypeBool:
                {
                    int intVar;
                    fscanf(inFile, "%i\n", &intVar);
                    ConsoleVar_SetIntValue(var, intVar);
                    break;
                }
                case kConsoleVarTypeDouble:
                {
                    double doubleVar;
                    fscanf(inFile, "%lf\n", &doubleVar);
                    ConsoleVar_SetDoubleValue(var, doubleVar);
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

FILE* Console_Log(ConsoleRef console)
{
    assert(console);
    return console->logFile;
}

ConsoleCommandRef Console_RegisterCommand(ConsoleRef console,
                                          const char* name,
                                          ConsoleFunc_t consoleFunc,
                                          int argCount)
{
    assert(console);
    assert(name);
    
    ConsoleCommandRef newCommand = _ConsoleCommand_Create();
    newCommand->func = consoleFunc;
    newCommand->argCount = argCount;
    strcpy(newCommand->name, name);
    
    console->commands[console->commandCount] = newCommand;
    console->commandCount++;
    
    return newCommand;
}

ConsoleVarRef Console_RegisterVar(ConsoleRef console,
                                  const char* name,
                                  ConsoleVarType_t type,
                                  ConsoleVarFlag_t flags)
{
    assert(console);
    assert(name);
    
    ConsoleVarRef newVar = _ConsoleVar_Create(type, 0);
    strcpy(newVar->name, name);
    
    newVar->flags = flags;
    
    console->vars[console->varCount] = newVar;
    console->varCount++;
    
    return newVar;
}

static ConsoleArgRef _ArgCreate(ConsoleVarRef var)
{
    ConsoleArgRef arg = malloc(sizeof(struct ConsoleArg));
    arg->var = var;
    arg->next = NULL;
    return arg;
}

static int _TokenIsFloat (const char *s)
{
    char* ep = NULL;
    strtol(s, &ep, 10);
    
    if (!*ep)
    {
        return 0;
    }
    
    if (*ep == 'e'  ||
        *ep == 'E'  ||
        *ep == '.')
    {
        return 1;
    }
    
    return 0;
}


static int _Console_ParseTokens(ConsoleRef console,
                                char* variableCommand,
                                char** outTokens)
{
    int tokenCounter = 0;
    
    int expectStatment = 1;
    int parsingString = 0;
    
    char* it = variableCommand;
    
    while (*it != '\0')
    {
        if (!parsingString)
        {
            /* treat new lines as termination */
            if (*it == '\n')
            {
                *it = '\0';
            }
        }
        
        if (expectStatment)
        {
            /* start string */
            if (*it == '\"')
            {
                parsingString = 1;
            }
            
            /* new token */
            outTokens[tokenCounter] = it;
            tokenCounter++;
            expectStatment = 0;
        }
        else if (parsingString)
        {
            /* begin string */
            if (*it == '\"')
            {
                *it = '\0';
                parsingString = 0;
            }
        }
        else
        {
            /* convert white space to termination */
            if (isspace(*it))
            {
                *it = '\0';
                expectStatment = 1;
            }
        }
        
        it ++;
    }
    
    if (parsingString)
    {
        fprintf(Console_Log(console), "trailing quote\n");
    }
    
    return tokenCounter;
}

static void _Console_FreeArgChain(ConsoleArgRef arg)
{
    ConsoleArgRef it = arg;
    
    while (it)
    {
        ConsoleArgRef next = it->next;
        
        if (it->var->temp)
        {
            free(it->var);
        }
        free(it);
        
        it = next;
    }
}

int Console_Execute(ConsoleRef console, const char* staticCommandString)
{
    assert(console);
    assert(staticCommandString);
    
    char commandString[CONSOLE_VAR_NAME_MAX];
    strcpy(commandString, staticCommandString);
    
    
    char* tokens[CONSOLE_MAX_TOKENS];
    int tokenCount = _Console_ParseTokens(console, commandString, tokens);
    
    if (tokenCount < 1)
    {
        fprintf(Console_Log(console), "nothing to execute\n");
        return 0;
    }
    
    ConsoleArgRef argChain = NULL;
    int argCount = 0;
    
    ConsoleCommandRef command = _Console_FindCommand(console, tokens[0]);
    
    if (!command)
    {
        printf("unknown command: %s\n", tokens[0]);
        return 0;
    }
    
    
    int i;
    for (i = 1; i < tokenCount; i++)
    {
        const char* argToken = tokens[i];
        
        ConsoleArgRef newArg = NULL;
        
        /* string */
        if (*argToken == '\"' || *argToken == '-')
        {
            newArg = _ArgCreate(_ConsoleVar_Create(kConsoleVarTypeString, 1));            
            strcpy(newArg->var->stringValue, argToken + 1);
            newArg->var->flags = kConsoleVarFlagReadonly;
        }
        else
        {
            ConsoleVarRef var = Console_FindVar(console, argToken);
            
            /* variable */
            if (var)
            {
                newArg = _ArgCreate(var);
            }
            else
            {
                int found = 0;

                if (_TokenIsFloat(argToken))
                {
                    /* double */
                    double doubleValue;
                    if (sscanf(argToken, "%lf", &doubleValue) != 0)
                    {
                        newArg = _ArgCreate(_ConsoleVar_Create(kConsoleVarTypeDouble, 1));
                        newArg->var->doubleValue = doubleValue;
                        newArg->var->flags = kConsoleVarFlagReadonly;
                        found = 1;
                    }
                }
                else
                {
                    /* int */
                    int intValue;
                    if (sscanf(argToken, "%d", &intValue) != 0)
                    {
                        newArg = _ArgCreate(_ConsoleVar_Create(kConsoleVarTypeInt, 1));
                        newArg->var->intValue = intValue;
                        newArg->var->flags = kConsoleVarFlagReadonly;
                        found = 1;
                    }
                }
                
                if (!found)
                {
                    fprintf(Console_Log(console), "unknown symbol: \"%s\"\n", argToken);
                    _Console_FreeArgChain(argChain);
                    return 0;
                }
            }
        }
        
        if (newArg)
        {
            if (argChain)
            {
                argChain->next = newArg;
            }
            else
            {
                argChain = newArg;
            }
            argCount++;
        }
    }
    
    int fail = 0;
    if (command->argCount >= 0)
    {
        if (command->argCount != argCount)
        {
            fprintf(Console_Log(console), "%s: expected %i arguments\n", command->name, command->argCount);
            fail = 1;
        }
    }
    
    if (!fail)
    {
        fail = !command->func(console, argChain);
    }
    
    if (fail)
    {
        fprintf(Console_Log(console), "%s failed\n", command->name);
    }
    
    _Console_FreeArgChain(argChain);
    
    return 1;
}
