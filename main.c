#include <stdio.h>
#include <stdlib.h>
#include <main.h>

FILE *source, *objcode;

extern double acc;
extern void mybc(void);
int main(void)
{
	lookahead = gettoken(source = stdin);
	objcode = stdout;

	mybc();

	return 0;
}
