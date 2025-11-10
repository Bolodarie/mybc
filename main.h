#pragma once
// Declarações:
extern int lookahead; // a ser definido no parser
extern int gettoken(FILE *); // a ser definido no lexer

// E() emulates the initial symbol E

extern void E(void);
extern void mybc(void);

