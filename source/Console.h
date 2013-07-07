/*
 By: Justin Meiners

 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#define CONSOLE_VERSION_1_0 0
#define CONSOLE_VERSION_1_1 1

#define CONSOLE_VERSION CONSOLE_VERSION_1_1

/*
 v1.0:
 - Initial release
 
 v1.1:
 - Custom Memory alloactor support
 
 */

typedef enum
{
    kConsoleVarTypeDouble = 0,
    kConsoleVarTypeString,
    kConsoleVarTypeInt,
    kConsoleVarTypeBool
    
} ConsoleVarType_t;

typedef enum
{
    /* can commands modify this? */
    kConsoleVarFlagReadonly = 1 << 0,

} ConsoleVarFlag_t;


typedef struct ConsoleVar* ConsoleVarRef;
typedef struct ConsoleArg* ConsoleArgRef;
typedef struct ConsoleCommand* ConsoleCommandRef;
typedef struct Console* ConsoleRef;

struct ConsoleArg
{
    /* public */
    struct ConsoleVar* var;
    struct ConsoleArg* next;
};

typedef int (*ConsoleFunc_t)(ConsoleRef console, ConsoleArgRef arguments);

/* for custom allocators */
extern void Console_InstallAllocators(void *(*mallocFunc)(size_t sz), void (*freeFunc)(void *ptr));

    
/* ConsoleVar */
extern ConsoleVarType_t ConsoleVar_Type(ConsoleVarRef var);
extern int ConsoleVar_Readonly(ConsoleVarRef var);

extern void ConsoleVar_SetDoubleValue(ConsoleVarRef var, double value);
extern double ConsoleVar_DoubleValue(ConsoleVarRef var);

extern void ConsoleVar_SetIntValue(ConsoleVarRef var, int value);
extern int ConsoleVar_IntValue(ConsoleVarRef var);

/*
 assigning bools and ints are equivlent here
 this exists for consistency 
 */
extern void ConsoleVar_SetBoolValue(ConsoleVarRef var, int value);
extern int ConsoleVar_BoolValue(ConsoleVarRef var);

extern void ConsoleVar_SetStringValue(ConsoleVarRef var, const char* string);
extern const char* ConsoleVar_StringValue(ConsoleVarRef var);

/* Console */
extern ConsoleRef Console_Create(FILE* logfile);
extern void Console_Destroy(ConsoleRef console);

/* save current settings to file */
extern void Console_Save(ConsoleRef console, FILE* outFile);
/* load settings from file - returns success */
extern int Console_Load(ConsoleRef console, FILE* inFile);
/* the file commands should log to (fprintf) */

extern ConsoleVarRef Console_FindVar(ConsoleRef console, const char* name);

extern FILE* Console_Log(ConsoleRef console);

/* register a new command */
extern ConsoleCommandRef Console_RegisterCommand(ConsoleRef console,
                                                 const char* name,
                                                 ConsoleFunc_t consoleFunc,
                                                 /* if argCount -1 any number of arguments are valid */
                                                 int argCount);

/* register a new variable */
extern ConsoleVarRef Console_RegisterVar(ConsoleRef console,
                                         const char* name,
                                         ConsoleVarType_t type,
                                         ConsoleVarFlag_t flags);

/* exectue a command string, returns success */
extern int Console_Execute(ConsoleRef console, const char* command);

#ifdef __cplusplus
}
#endif
    
#endif
