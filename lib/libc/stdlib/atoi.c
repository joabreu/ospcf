/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdlib\atoi.c
    @brief Standard library functions
*/

int atoi(const char *str)
{
    long int v=0;
	int sign=1;
	
	while((*str == ' ')  ||  ((unsigned int)(*str - 9) < 5u))
         str++; // ignore non numeric chars
	
	switch(*str) {
		 case '-':
              sign=-1;
		 case '+':
              ++str;
	}
	while((unsigned int)(*str - '0') < 10u) {
		 v=v*10+*str-'0';
         ++str;
	}
	return ((sign==-1)?-v:v);
}
