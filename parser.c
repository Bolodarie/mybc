#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <lexer.h>
#include <tokens.h>
#include <parser.h>
// Declaração antecipada (forward declaration)
const char* token_name(int token);

int lookahead; // este é o olho do compilador
jmp_buf error_recovery; // Ponto de retorno para recuperação de erros

// Handler para Ctrl+C (SIGINT)
void sigint_handler(int sig) {
	(void)sig; // Evita warning de parâmetro não usado
	printf("\n"); // Imprime quebra de linha
	fflush(stdout);
	siglongjmp(error_recovery, 1); // Volta para o loop principal
}

// Ponto de entrada e loop principal (REPL) do interpretador.
//
// 1. Configura o 'sigint_handler' para capturar Ctrl+C.
// 2. Configura 'sigsetjmp' como ponto de recuperação de erro.
// 3. Processa um 'cmd()' por vez.
// 4. Em caso de erro (via 'siglongjmp'), descarta tokens até
//    a próxima linha (';' ou '\n') e continua o loop.
void mybc(void) {
	// Registra handler para Ctrl+C
	struct sigaction sa;
	sa.sa_handler = sigint_handler; // Nossa função de tratamento
	sigemptyset(&sa.sa_mask);       // Não bloquear outros sinais durante a execução
	sa.sa_flags = 0;                // Nenhuma flag especial
	
	// Aplica a configuração para o sinal SIGINT (Ctrl+C)
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("Erro ao configurar sigaction"); // Boa prática
		exit(1);
	}
	// Loop principal: processa comandos até EOF
	while(lookahead != EOF) {
		// Marca ponto de recuperação de erro
		if (sigsetjmp(error_recovery, 1) != 0) {
			// RECUPERAÇÃO DE ERRO: chegou aqui via longjmp
			// Descarta tokens até encontrar um separador válido
			while(lookahead != '\n' && lookahead != ';' && lookahead != EOF) {
				lookahead = gettoken(source);
			}
			// Consome o separador se houver
			if (lookahead == '\n' || lookahead == ';') {
				lookahead = gettoken(source);
			}
			// Volta ao início do loop para processar próximo comando
			continue;
		}
		
		// Processa um comando
		cmd();
		
		// Após o comando, deve vir um separador (ou EOF)
		if(lookahead == ';' || lookahead == '\n') {
			match(lookahead);
		} else if (lookahead != EOF) {
			// Token inesperado após comando
			fprintf(stderr, "\nSyntax Error at line %d, column %d:\n", lineno, colno);
			fprintf(stderr, "  Unexpected token: %s", token_name(lookahead));
			if (lexeme[0] != '\0' && lookahead != '\n' && lookahead != EOF) {
				fprintf(stderr, " ('%s')", lexeme);
			}
			fprintf(stderr, "\n  Expected: ; or newline\n");
			siglongjmp(error_recovery, 1);
		}
	}

	match(EOF);
}

// Processa um único comando ou linha.
//
// Esta função decide o que fazer com base no primeiro token (lookahead):
// - 'exit'/'quit': Encerra o programa.
// - FIRST(E): Se for o início de uma expressão (número, ID, '+', etc.),
//             chama E() para calcular e depois imprime o resultado.
// - Epsilon (;, \n, EOF): Se for uma linha vazia, não faz nada.
// - default: Se for um token inválido, dispara um erro.
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
			fflush(stdout);
			lineno++; // Incrementa linha pois o output cria uma nova linha visual
			break;
		// produção épsilon - permite comando vazio (ex: linha em branco)
		case ';':
		case '\n':
		case EOF:
			// Comando vazio válido seguido por separador ou EOF
			break;
		default:
			// Token inválido no início do comando
			fprintf(stderr, "\nSyntax Error at line %d, column %d:\n", lineno, colno);
			fprintf(stderr, "  Invalid token: %s", token_name(lookahead));
			if (lexeme[0] != '\0' && lookahead != '\n' && lookahead != EOF) {
				fprintf(stderr, " ('%s')", lexeme);
			}
			fprintf(stderr, "\n  Expected: expression, 'exit', or 'quit'\n");
			siglongjmp(error_recovery, 1);
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
// Busca ou cria uma variável na tabela de símbolos.
//
// Procura o 'name' de trás para frente (do mais recente para o mais antigo).
// - Se achar, retorna o valor de 'vmem[address]'.
// - Se não achar, cria uma nova entrada no fim da tabela com valor 0.0.
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

// Armazena o valor do acumulador 'acc' em uma variável.
//
// Usa 'recall(name)' para encontrar o endereço da variável (ou criar um)
// e então salva o valor de 'acc' nesse endereço em 'vmem'.
void store(char const *name){
	recall(name); //vai localizar o endereço da variavel na memoria;
	vmem[address] = acc;
}

// Processa uma expressão (Fator -> Termo -> Expressão)
//
// Esta função NÃO é recursiva. Ela usa 'goto' e uma pilha
// para implementar a precedência de operadores de forma iterativa.
//
// 1. Trata Fatores (números, '()', IDs) e laço _Fbegin para ( *, / )
// 2. Trata Termos e laço _Tbegin para ( +, - )
// 3. 'acc' (acumulador) guarda o resultado final ou intermediário.
void E(void)
{
	char varname[MAXIDLEN+1];
	int isnegate = 0;
	int isotimes = 0;
	int isoplus = 0;

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
			acc = atoi(lexeme);
			match(DEC); break;
		case FLT:
			acc = atof(lexeme);
			match(FLT); break;
		default:
			// F -> ID [ := E]
			strcpy(varname, lexeme);
			match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				E(); // tras o resultado no acumulador (acc)
				store(varname); // armazena no endereco associado a (varname)
			}else{
				acc = recall(varname);
			}
	}
	// fim do fator
	
	if(isotimes) {
		if (isotimes == '*') {
			stack[sp] = stack[sp] * acc;
		} else {
			stack[sp] = stack[sp] / acc;		
		}
		acc = stack[sp--];
		isotimes = 0;
	}
	

	if(lookahead == '*' || lookahead == '/') {
		isotimes = lookahead;
		stack[++sp] = acc;
		match(lookahead); goto _Fbegin;
	}
    // fim do termo
    
    if (isnegate) {
		acc = -acc;
		isnegate = 0;
	}
	

	
	if(isoplus) {
		if (isoplus == '+') {
			stack[sp] = stack[sp] + acc;
		} else {
			stack[sp] = stack[sp] - acc;		
		}
		acc = stack[sp--];
		isoplus = 0;
	}
	

	if(lookahead == '+' || lookahead == '-') {
		isoplus = lookahead;
		stack[++sp] = acc;
		match(lookahead); goto _Tbegin;
	}
	// fim da expressão

}

//////////////////////////// parser components /////////////////////////////////

// Função auxiliar para converter token em string legível
const char* token_name(int token) {
	switch(token) {
		case ID: return "ID";
		case DEC: return "DEC";
		case OCT: return "OCT";
		case HEX: return "HEX";
		case FLT: return "FLT";
		case ASGN: return ":=";
		case EXIT: return "exit";
		case QUIT: return "quit";
		case EOF: return "EOF";
		case '\n': return "newline";
		case ';': return ";";
		case '+': return "+";
		case '-': return "-";
		case '*': return "*";
		case '/': return "/";
		case '(': return "(";
		case ')': return ")";
		default:
			if (token >= 32 && token <= 126) {
				static char buf[4];
				snprintf(buf, sizeof(buf), "'%c'", token);
				return buf;
			}
			return "UNKNOWN";
	}
}

int lookahead;
// Consome o token esperado ou dispara um erro.
//
// Se 'lookahead' for o 'expected', avança para o próximo token.
// Se não for, imprime o erro e usa 'siglongjmp' para
// pular de volta para o loop principal (recuperação de erro).
void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "\nSyntax Error at line %d, column %d:\n", lineno, colno);
		fprintf(stderr, "  Expected: %s\n", token_name(expected));
		fprintf(stderr, "  Found:    %s", token_name(lookahead));
		
		// Mostra o lexema se ele existir e for significativo
		if (lexeme[0] != '\0' && lookahead != '\n' && lookahead != EOF) {
			fprintf(stderr, " ('%s')", lexeme);
		}
		fprintf(stderr, "\n");
		
		siglongjmp(error_recovery, 1);
	}
}