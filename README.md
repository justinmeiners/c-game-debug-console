C Game Debug Console
====================

A Quake style debug console for games.
* **Portable** - ANSI C + Standard Library
* **Easy Integration** - A single header and source file for the console, and one for the default library.
* **Clean** - Well designed interface with opaque data structures.
* **Simple** - Simple design and featureset. Easily add new commands and variables. If more complexity is needed (arrays, returns, complex expressions etc.) a complete scripting language may be more appropriate. Check out [lua](http://lua.org) instead.
* **Persitent** - Save and load and console states.

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
command_name arg1 arg2 argN...

Example:
set my_var 40

or 

set my_var other_var

```

Standard Library:

* **TRUE** - bool 1
* **FALSE** - bool 0
* **echo** - prints the value of a variable
* **inspect** - prints the type of a variable
* **set** - assigns the value of the second argument to the first 

Style (optional):

* **Constants** - Caps, underscore seperating words.
* **Commands** - Lowercase, underscore seperating words.
* **Variables**  - Lowercase, underscore seperating words.

### Custom Variables: ###

```C

/* registration */
ConsoleVarRef myVar = Console_RegisterVar(console,
                                          "my_var", /* variable name */
                                          kConsoleVarTypeInt, /* int type */
                                          0); /* flags - readonly, etc (none used) */

										   
...
...
...

/* access outside of console in C code */

/* find handle or use cached */
ConsoleVarRef myVar = Console_FindVar("my_var");

/* get value */
int val = ConsoleVar_IntValue(myVar);

/* assignment */
ConsoleVar_SetIntValue(myVar, 100);

```


### Custom Commands: ###
```C 

/* sample command function */
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

/* registration */
Console_RegisterCommand(console,
                        "avg", /* command name */
                        calculateAverage, /* function pointer */
                        -1); /* how many arguments (-1 indicates a variable number) ? */
						
```
