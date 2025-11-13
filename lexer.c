#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <lexer.h>
#include <tokens.h>

char lexeme[MAXIDLEN + 1]; // Buffer global para armazenar o lexema do token lido

/* Versão extendida de identificador Pascal
 * ID = [A-Za-z][A-Za-z0-9]*
 */
// Reconhece um ID ([A-Za-z][A-Za-z0-9]*).
//
// Esta função também atua como a tabela de palavras-chave.
// Se o 'lexeme' lido for "exit" ou "quit", retorna o token
// correspondente em vez de ID.
int isID(FILE *tape)
{
	if ( isalpha(lexeme[0] = getc(tape)) ) {
		int i = 1;
		while ( isalnum( lexeme[i] = getc(tape) ) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		if(strcmp(lexeme, "exit") == 0)
			return EXIT;	
		if(strcmp(lexeme, "quit") == 0) 
			return QUIT;
		
		return ID;
	}

	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

// Tenta processar um número Decimal.
// Trata o caso especial de "0" (que não é seguido por outros dígitos)
// e números de "1-9" (que podem ser seguidos por outros).
/*
 * DEC = [1-9][0-9]* | '0'
 * ------------------------------------------
 * |                      digit               |
 * |                    --------              |
 * |                   |        |             |
 * digit      |     not zero      V        |  epsilon    V
 * -->(is DEC)--------->(is ZERO)---------->(isdigit)-------------->((DEC))
 * |
 * | epsilon
 * |
 * V
 * ((0))
 */
int isDEC(FILE *tape)
{
	if ( isdigit(lexeme[0] = getc(tape)) ) {
		if (lexeme[0] == '0') {
			return DEC;
		}
		int i = 1;
		while ( isdigit(lexeme[i] = getc(tape)) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return DEC;
	}

	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

// fpoint = DEC\.[0-9]* | \.[0-9][0-9]*
// flt = fpoint EE? | DEC EE
// EE = [eE]['+''-']?[0-9][0-9]*
// entrada de teste: 3e+
//                   012

// Tenta processar a parte de notação científica (ex: 'e+10', 'E-5').
//
// É chamada por 'isNUM' após um número ser lido.
// 'hassign' controla o sinal opcional (+/-).
// Se a sintaxe estiver errada (ex: "3e+"), faz 'ungetc'
// dos caracteres lidos para que não sejam consumidos (backtracking).
int isEE(FILE *tape)
{
	int i = strlen(lexeme);

	if ( toupper(lexeme[i] = getc(tape)) == 'E' ) {
		i++;
		
		// verifica sinal opcional
		int hassign;
		if ( (lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-' ) {
			hassign = i++;
		} else {
			hassign = 0;
			ungetc(lexeme[i], tape);
		}
		
		// verifica dígito obrigatório seguinte
		if ( isdigit(lexeme[i] = getc(tape)) ) {
			i++;
			while( isdigit(lexeme[i] = getc(tape)) ) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return FLT;
		}
		// Backtracking: falhou em encontrar dígito após 'e' ou 'e+'
		ungetc(lexeme[i], tape);
		i--;
		if (hassign) {
			ungetc(lexeme[i], tape);
			i--;
		}
	}

	ungetc(lexeme[i], tape);
	lexeme[i] = 0;	
	return 0;
}

// Tenta processar qualquer formato numérico (DEC, FLT).
//
// Esta é uma função "orquestradora" que tenta, em ordem:
// 1. Um decimal (isDEC).
// 2. Se for decimal, tenta um ponto flutuante (ex: "123.").
// 3. Se não for decimal, tenta um flutuante (ex: ".123").
// 4. No final, tenta ler notação científica (isEE).
// A ordem importa para tratar casos como ".5" vs "5.".
int isNUM(FILE *tape)
{
	int token = isDEC(tape);
	if (token == DEC) {
		int i = strlen(lexeme);
		if ( (lexeme[i] = getc(tape)) == '.' ) {
			i++;
			while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			token = FLT;
		} else {
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
	} else {
		if ( (lexeme[0] = getc(tape)) == '.' ) {
			if ( isdigit( lexeme[1] = getc(tape) ) ) {
				token = FLT;
				int i = 2;
				while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
			} else {
				ungetc(lexeme[1], tape);
				ungetc(lexeme[0], tape);
				lexeme[0] = 0;
				return 0; // não é um número
			}
		} else {
			ungetc(lexeme[0], tape);
			lexeme[0] = 0;
			return 0; // não é um número
		}
	}
	
	if (isEE(tape)) {
		token = FLT;
	}

	return token;
}

// Tenta processar o token de atribuição ':='.
//
// Se ler um ':' mas o próximo não for '=', devolve ambos
// os caracteres para o 'tape' (ungetc).
int isASGN(FILE *tape){
	lexeme[0] = getc(tape);
	if(lexeme[0] == ':'){
		lexeme[1] = getc(tape);
		if(lexeme[1] == '='){
			lexeme[2] = '0';
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	return lexeme[0] = 0;
}

// Tenta processar um número Octal (prefixo '0', seguido de [0-7]).
//
// Se ler '0' mas o próximo não for [0-7], devolve os
// caracteres para o 'tape' para ser lido como um DEC ("0")
// pela função 'isNUM'.
/*
 * OCT = '0'[0-7]+
 */
int isOCT(FILE *tape)
{
	if ( (lexeme[0] = getc(tape)) == '0') {
		int i = 1;
		if ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7') {
			i = 2;
			while ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7') i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return OCT;
		}
		ungetc(lexeme[1], tape);
		ungetc(lexeme[0], tape);
	} else {
		ungetc(lexeme[0], tape);
	}
	return 0;
}

// Tenta processar um número Hexadecimal (prefixo '0[xX]').
//
// Requer "0x" ou "0X" seguido de pelo menos um dígito hexadecimal.
// Se a regra falhar (ex: "0x" sem nada), devolve tudo
// para o 'tape' para ser lido como um DEC ("0") por 'isNUM'.
/*
 * HEX = '0'[Xx][0-9A-Fa-f]+
 *
 * isxdigit == [0-9A-Fa-f]
 */
int isHEX(FILE *tape)
{
	// Para ter um hexa, é necessário que venha o prefixo "0[xX]" seguido de um hexa digito
	if ( (lexeme[0] = getc(tape)) == '0' ) {
		if ( toupper(lexeme[1] = getc(tape)) == 'X' ) {
			if ( isxdigit(lexeme[2] = getc(tape)) ) {
				int i = 3;
				while ( isxdigit(lexeme[i] = getc(tape)) ) i++;
				ungetc(lexeme[i], tape);
				lexeme[i] = 0;
				return HEX;
			}
			ungetc(lexeme[2], tape);
			ungetc(lexeme[1], tape);
			ungetc(lexeme[0], tape);
			lexeme[0] = 0;
			return 0;
		}
		ungetc(lexeme[1], tape);
		ungetc(lexeme[0], tape);
		lexeme[0] = 0;
		return 0;
	}
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

int lineno = 1;
int colno = 1;

// Consome espaços em branco, exceto '\n' (que é um token).
//
// Também ignora sequências de escape ANSI (ex: '\x1B[A' - seta para cima)
void skipspaces(FILE *tape)
{
	int head, aux_head;
	while(1){
		head = getc(tape);
		if ( isspace(head) && head != '\n' ) {
			colno++;
			continue;
		}
		if ( head == '\x1B' ) { // \x1B é o código ASCII para 'ESC'
			if((aux_head = getc(tape)) == '['){
				//getc(tape); // Consome o '['
				getc(tape); // Consome o 'A', 'B', 'C' ou 'D'
				continue; // Volta ao início do while
			}
			else{
				ungetc(aux_head,tape);
			}
		}
		ungetc(head, tape);
		break;
	}
}

// Função principal do analisador léxico.
//
// 1. Pula espaços e lixo (skipspaces).
// 2. Tenta reconhecer tokens na ordem de prioridade.
//
// A ORDEM DAS CHAMADAS É CRUCIAL:
// - isID() vem antes de isNUM() pois "exit" é um ID, não um número.
// - isHEX() e isOCT() vêm antes de isNUM() para que "0x10" ou "010"
//   não sejam lidos como o decimal "0".
//
// Se nada for reconhecido, retorna o próprio caractere (ex: '+', '(', etc).
// Também atualiza 'lineno' e 'colno' para o parser.
int gettoken(FILE *source)
{
	int token;

	skipspaces(source);

	if ( (token = isID(source)) ) ;
	else if ( (token = isHEX(source)) ) ;
	else if ( (token = isOCT(source)) ) ;
	else if ( (token = isNUM(source)) ) ;
	else if ( (token = isASGN(source)) ) ;
	else {
		lexeme[0] = token = getc(source);
		lexeme[1] = 0;

		// Atualiza caso de '\n'
		if (token == '\n') {
			lineno++;
			colno = 1;
		} else {
			colno++;
		}
		return token;
	}
    colno += (strlen(lexeme)-1);
    return token;
}