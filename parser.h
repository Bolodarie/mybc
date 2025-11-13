#pragma once

#define ERRTOKEN -0x10000000

extern int lookahead;
extern double acc;

extern char lexeme[];

extern int lineno;
extern int colno;

extern FILE* objcode;
extern FILE *source;

extern void match(int);

extern int gettoken(FILE *);

extern void mybc(void);
extern void cmd(void);
extern void E(void);
extern void T(void);
extern void F(void);

