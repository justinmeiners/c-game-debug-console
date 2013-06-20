c-game-debug-console
====================

A Quake style debug console for games.
* Portable - ANSI C
* Easy Integration - A single header and source file for the console, and one for the default library.
* Clean - Well designed opaque data structures.
* Simple - Easily add new commands and variables.
* Persitent - Save and load and console states.

## Integration: ##

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

Commands take the form of:
command_name arg1 arg2 etc

