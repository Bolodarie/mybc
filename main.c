#include <stdio.h>
#include <stdlib.h>
#include <main.h>

FILE *source;

extern double acc;
extern void mybc(void);
int main(void)
{
	lookahead = gettoken(source = stdin);

	mybc();

	return 0;
}
