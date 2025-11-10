#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <tokens.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

// Interpretador de comando
//
// mybc -> cmd ( cmdsep cmd ) EOF
void mybc(void) {
	cmd();

	while(lookahead != EOF) {
		
		// cmdsep:
		if(lookahead == ';' || lookahead == '\n') {
			match(lookahead);
		}
		
		cmd();
	}

	match(EOF);
}

void cmd(void) {
	switch (lookahead) {
		case EXIT:
		case QUIT:						
			exit(0);
		// FIRST(E)
		case '+':
		case '-':
		case '(':
		case DEC:
		case FLT:
		case HEX:
		case OCT:
		case ID:
			E();
			printf("%lg\n", acc);
			break;
		default:
			;
	}
}
// cmd  -> E | exit | quit | <epsilon> 
// cmdsep -> ';' | '\n'
double acc;
#define STACKSIZE 1024
double stack[STACKSIZE];
int sp = -1;

// tabela de simbolos como dicionario dos valores armazenados na memoria virtual
#define MAXSTENTRIES 4096
char symtab[MAXSTENTRIES][MAXIDLEN+1];
int symtab_next_entry = 0; // uso: strcpy(symtab[symtab_next_entry], name);
double vmem[MAXSTENTRIES];

int address;
double recall(char const *name){
	for(address = symtab_next_entry -1; address > -1; address--){
			if(strcmp(symtab[address], name) == 0){
				return vmem[address];
			}
	}
	address = symtab_next_entry++;
	strcpy(symtab[address], name);
	return 0.0e0;
}

void store(char const *name){
	recall(name); //vai localizar o endereço da variavel na memoria;
	vmem[address] = acc;
}

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// ominus = ['+', '-']
// oplus = ['+', '-']
void E(void)
{
	/**/char varname[MAXIDLEN+1];/**/
	/**/int isnegate = 0;/**/
	/**/int isotimes = 0;/**/
	/**/int isoplus = 0;/**/

	if(lookahead == '+' || lookahead == '-') {
		if (lookahead == '-') {
			isnegate = lookahead;
		}
		match(lookahead);
	}

	_Tbegin:

	_Fbegin:

	switch(lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case DEC:
			// /**/fprintf(objcode, " %s ", lexeme);/**/
			acc = atoi(lexeme);
			match(DEC); break;
		case FLT:
			// /**/fprintf(objcode, " %s ", lexeme);/**/
			acc = atof(lexeme);
			match(FLT); break;
		default:
			// /**/fprintf(objcode, " %s ", lexeme);/**/
			// F -> ID [ := E]
			/**/ strcpy(varname, lexeme); /**/
			match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				E(); // tras o resultado no acumulador (acc)
				/**/store(varname);/**/ // armazena no endereco associado a (varname)
			}else{
				/**/acc = recall(varname);/**/
			}
	}
	// factor end
	/**/
	if(isotimes) {
		if (isotimes == '*') {
			stack[sp] = stack[sp] * acc;
		} else {
			stack[sp] = stack[sp] / acc;		
		}
		acc = stack[sp--];
		isotimes = 0;
	}
	/**/

	if(lookahead == '*' || lookahead == '/') {
		/**/isotimes = lookahead;/**/
		stack[++sp] = acc;
		match(lookahead); goto _Fbegin;
	}
    // term end
    /**/
    if (isnegate) {
		acc = -acc;
		isnegate = 0;
	}
	/**/

	/**/
	if(isoplus) {
		if (isoplus == '+') {
			stack[sp] = stack[sp] + acc;
		} else {
			stack[sp] = stack[sp] - acc;		
		}
		acc = stack[sp--];
		isoplus = 0;
	}
	/**/

	if(lookahead == '+' || lookahead == '-') {
		isoplus = lookahead;
		stack[++sp] = acc;
		match(lookahead); goto _Tbegin;
	}
	// expression end

}

//////////////////////////// parser components /////////////////////////////////
int lookahead;
void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		
		fprintf(stderr, "token mismatch at line %d, column %d\n", lineno, colno);
		exit(ERRTOKEN);
	}
}