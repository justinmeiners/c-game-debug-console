C Game Debug Console
====================

A Quake style debug console for games.
* Portable - ANSI C
* Easy Integration - A single header and source file for the console, and one for the default library.
* Clean - Well designed opaque data structures.
* Simple - Easily add new commands and variables.
* Persitent - Save and load and console states.

### Integration: ###

```C 

#include "Console.h"
#include "ConsoleStdLib.h"

/* create console with output file */
ConsoleRef console = Console_Create(stdout);

/* register standard library (optional) */
ConsoleStdLib_Register(console);

/* execute functions */
Console_Execute(...)
Console_Execute(...)
....

/* shutdown */
Console_Destroy(console);

```

### Usage: ###

Commands take the simple form of:

```
command_name arg1 arg2 etc

Example:
set my_var 40

or 

set my_var other_var

```
Standard Library:
TRUE - bool 
FALSE - bool
echo - prints the value of a variable
inspect - prints the type of a variable
set - assigns the value of the second argument to the first 

Style (optional):
Constants - Caps, underscore seperating words.
Commands - Lowercase, underscore seperating words.
Variables  - Lowercase, underscore seperating words.

### Custom Vars: ###

```C

ConsoleVarRef myVar = Console_RegisterVar(console,
                                          "my_var", /* var name */
                                          kConsoleVarTypeInt, /* int type */
                                          kConsoleVarFlagReadonly); /* make this readonly by the console */
										  


```


### Custom Commands: ###
```C 

int calculateAverage(ConsoleRef console, ConsoleArgRef args)
{
	ConsoleArgRef arg = args;
	
	double total = 0.0;
	int count = 0;
	
	/* args is linked list */
	while (arg)
	{
		total += ConsoleVar_DoubleValue(arg->var);
		count++;
		arg = arg->next;
	}
		
	fprintf(Console_Log(console), "%lf\n", total / (double)count);
	
	/* return success */
	return 1;
}

/* register a command */
Console_RegisterCommand(console,
                        "avg", /* command name */
                        calculateAverage, /* function pointer */
                        -1); /* how many arguments (-1 means whatever) ? */
						
```
