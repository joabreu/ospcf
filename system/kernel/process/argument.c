/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\process\argument.c
    @brief Argument manipulation
*/
#include <errno.h>

int CountArguments(char **args)
{
    int maximum = 1024; // Maximum number of args

	if(args == 0)
		 return 0;
	do {
		 if(args[1024 - maximum] == 0)
			  return (1024 - maximum);
	} while(maximum--);
	// Too much args presents
	return -E2BIG;
}
