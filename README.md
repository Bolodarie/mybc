# mybc - Interpretador de Comandos Aritméticos

Interpretador interativo (REPL) de expressões aritméticas em C, desenvolvido como projeto de compiladores.

## 🚀 Compilação e Execução

```bash
make          # Compila o projeto
./mybc        # Executa o interpretador
```

## 📖 Uso

```bash
$ ./mybc
2+3
5
x := 10
10
x * 2
20
(5+3)*4
32
exit
$
```

## ✨ Funcionalidades

- ✅ Operações aritméticas: `+`, `-`, `*`, `/`
- ✅ Parênteses e precedência de operadores
- ✅ Números inteiros, decimais, hexadecimais e octais
- ✅ Variáveis (atribuição com `:=`)
- ✅ Detecção e recuperação de erros de sintaxe
- ✅ Mensagens de erro detalhadas (linha e coluna)
- ✅ Captura de Ctrl+C (não encerra o interpretador)
- ✅ REPL completo (continua após erros)

## 🧪 Testes

### Validação Automatizada (32 testes)
```bash
./test_auto.sh
```

### Suite Completa de Demonstração
```bash
./test_all.sh
```

Ver documentação completa em **[TESTES.md](TESTES.md)**

## 📚 Documentação

- **[TESTES.md](TESTES.md)** - Guia completo de testes

## 🏗️ Arquitetura

- `lexer.c` / `lexer.h` - Análise léxica (tokens)
- `parser.c` / `parser.h` - Análise sintática e interpretação
- `tokens.h` - Definições de tokens
- `main.c` - Ponto de entrada

## 🎯 Características Implementadas

### Recuperação de Erros
O interpretador **não encerra** após erros de sintaxe:
```bash
$ ./mybc
2+3
5
2 + * 3
Syntax Error at line 3, column 5:
  Expected: ID
  Found: * ('*')
4+4
8
```

### Contagem de Linhas Interativa
Números de linha refletem o que você vê no terminal (incluindo outputs):
```bash
Linha 1: 2+3      (input)
Linha 2: 5        (output)
Linha 3: 2 + * 3  (input - erro aqui)
         ↑
Error at line 3
```

### Captura de Ctrl+C
Pressionar Ctrl+C imprime uma quebra de linha e **continua** o interpretador.

## 📦 Estrutura do Projeto

```
mybc/
├── lexer.c, lexer.h      # Analisador léxico
├── parser.c, parser.h    # Parser e interpretador
├── tokens.h              # Definições de tokens
├── main.c, main.h        # Entrada do programa
├── Makefile              # Compilação
├── test_auto.sh          # 32 testes automatizados
├── test_all.sh           # Demonstração completa
├── test_checklist.sh     # Checklist manual
├── TESTES.md             # Documentação de testes
└── README.md             # Este arquivo
```

## ✅ Status

**32/32 testes automatizados passando** ✅

```bash
$ ./test_auto.sh
🎉 TODOS OS TESTES PASSARAM!
```