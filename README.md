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

```

### Customization: ###

```C 

int customCommandFunc(ConsoleRef console, ConsoleArgRef args)
{
	ConsoleArgRef arg = args;
	
	/* args is linked list */
	while (arg)
	{
		...
		arg = arg->next;
	}
	
	/* return success */
	return 1;
}

/* register a command */
Console_RegisterCommand(console,
                        "customcommand", /* command name */
                        customCommandFunc, /* function pointer */
                        2); /* how many arguments ? */
						
```
