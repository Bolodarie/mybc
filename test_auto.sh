#!/bin/bash
# Testes automatizados com PASS/FAIL

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASS_COUNT=0
FAIL_COUNT=0
TOTAL_TESTS=0

# Função de teste automatizado
run_test() {
    local num=$1
    local desc=$2
    local input=$3
    local expected=$4
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Executa o teste
    actual=$(echo -e "$input" | ./mybc 2>&1)
    
    # Compara resultado
    if [ "$actual" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC} | Teste $num: $desc"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}✗ FAIL${NC} | Teste $num: $desc"
        echo "  ${YELLOW}Esperado:${NC}"
        echo "$expected" | sed 's/^/    /'
        echo "  ${YELLOW}Obtido:${NC}"
        echo "$actual" | sed 's/^/    /'
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

echo "╔════════════════════════════════════════════════════════════╗"
echo "║         TESTES AUTOMATIZADOS - MYBC INTERPRETER            ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# TESTE 1: Cálculo básico
run_test "01" "Cálculo simples: 2+3" \
    "2+3" \
    "5"

# TESTE 2: Multiplicação
run_test "02" "Multiplicação: 5*4" \
    "5*4" \
    "20"

# TESTE 3: Parênteses
run_test "03" "Parênteses: (2+3)*4" \
    "(2+3)*4" \
    "20"

# TESTE 4: Número negativo
run_test "04" "Negativo: -5+10" \
    "-5+10" \
    "5"

# TESTE 5: Float
run_test "05" "Float: 3.5*2" \
    "3.5*2" \
    "7"

# TESTE 6: Variável
run_test "06" "Atribuição: x := 10" \
    "x := 10" \
    "10"

# TESTE 7: Uso de variável
run_test "07" "Uso de variável: x := 5; x + 3" \
    "x := 5
x + 3" \
    "5
8"

# TESTE 8: Múltiplos comandos
run_test "08" "Múltiplos comandos válidos" \
    "1+1
2+2
3+3" \
    "2
4
6"

# TESTE 9: Erro - operador duplo
run_test "09" "ERRO: operador duplo (2 + + 3)" \
    "2 + + 3" \
    "
Syntax Error at line 1, column 5:
  Expected: ID
  Found:    + ('+')"

# TESTE 10: Erro - token inválido
run_test "10" "ERRO: token inválido (_)" \
    "_ 5" \
    "
Syntax Error at line 1, column 2:
  Invalid token: '_' ('_')
  Expected: expression, 'exit', or 'quit'"

# TESTE 11: Recuperação de erro
run_test "11" "RECUPERAÇÃO: continua após erro" \
    "2+3
2_2
5+5" \
    "5
2

Syntax Error at line 4, column 2:
  Unexpected token: '_' ('_')
  Expected: ; or newline
10"

# TESTE 12: Contagem de linha com output
run_test "12" "LINHA: contagem após output" \
    "1+1
2 + * 2" \
    "2

Syntax Error at line 3, column 5:
  Expected: ID
  Found:    * ('*')"

# TESTE 13: Variáveis persistem
run_test "13" "VARIÁVEL: persistência entre comandos" \
    "x := 100
x
x + 50" \
    "100
100
150"

# TESTE 14: Expressão complexa
run_test "14" "EXPRESSÃO: complexa com parênteses" \
    "(1+2)*(3+4)" \
    "21"

# TESTE 15: Recuperação múltipla
run_test "15" "RECUPERAÇÃO: múltiplos erros" \
    "1+1
@@
2+2" \
    "2

Syntax Error at line 3, column 2:
  Invalid token: '@' ('@')
  Expected: expression, 'exit', or 'quit'
4"

# ==================== TESTES DE DETECÇÃO DE ERROS ====================

# TESTE 16: Token inválido no meio de expressão
run_test "16" "ERRO: token inválido no meio (2_2)" \
    "2_2" \
    "2

Syntax Error at line 2, column 2:
  Unexpected token: '_' ('_')
  Expected: ; or newline"

# TESTE 17: Operador inválido @
run_test "17" "ERRO: operador inválido (@)" \
    "2 @ 3" \
    "2

Syntax Error at line 2, column 3:
  Unexpected token: '@' ('@')
  Expected: ; or newline"

# TESTE 18: Dois números sem operador
run_test "18" "ERRO: números sem operador (5 5)" \
    "5 5" \
    "5

Syntax Error at line 2, column 2:
  Unexpected token: DEC ('5')
  Expected: ; or newline"

# TESTE 19: Parênteses não balanceados
run_test "19" "ERRO: parênteses não fechado" \
    "(2+3" \
    "
Syntax Error at line 2, column 1:
  Expected: )
  Found:    newline"

# TESTE 20: Operador esperando operando
run_test "20" "ERRO: operador sem operando (2 + * 3)" \
    "2 + * 3" \
    "
Syntax Error at line 1, column 5:
  Expected: ID
  Found:    * ('*')"

# ==================== TESTES DE RECUPERAÇÃO ====================

# TESTE 21: Erro intercalado com sucesso
run_test "21" "RECUPERAÇÃO: erro no meio de comandos válidos" \
    "2+3
2 + * 3
5*4" \
    "5

Syntax Error at line 3, column 5:
  Expected: ID
  Found:    * ('*')
20"

# TESTE 22: Múltiplos erros seguidos
run_test "22" "RECUPERAÇÃO: múltiplos erros consecutivos" \
    "@@
##
1+1" \
    "
Syntax Error at line 1, column 2:
  Invalid token: '@' ('@')
  Expected: expression, 'exit', or 'quit'

Syntax Error at line 2, column 2:
  Invalid token: '#' ('#')
  Expected: expression, 'exit', or 'quit'
2"

# TESTE 23: Recuperação com variáveis
run_test "23" "RECUPERAÇÃO: variáveis com erro no meio" \
    "x := 5
x + 3
@ invalid
y := 10
y * 2" \
    "5
8

Syntax Error at line 5, column 2:
  Invalid token: '@' ('@')
  Expected: expression, 'exit', or 'quit'
10
20"

# TESTE 24: Parênteses com recuperação
run_test "24" "RECUPERAÇÃO: erro em parênteses e continua" \
    "1+1
(2+3
4+4" \
    "2

Syntax Error at line 4, column 1:
  Expected: )
  Found:    newline
8"

# ==================== TESTES DE CONTAGEM DE LINHA ====================

# TESTE 25: Linha após 1 output
run_test "25" "LINHA: erro na linha 3 (após 1 output)" \
    "2+3
2 + * 3" \
    "5

Syntax Error at line 3, column 5:
  Expected: ID
  Found:    * ('*')"

# TESTE 26: Linha após 3 outputs
run_test "26" "LINHA: erro na linha 7 (após 3 outputs)" \
    "1+1
2+2
3+3
4 + * 5" \
    "2
4
6

Syntax Error at line 7, column 5:
  Expected: ID
  Found:    * ('*')"

# TESTE 27: Linha com erro no início
run_test "27" "LINHA: erro na linha 1 (sem output antes)" \
    "_ bad" \
    "
Syntax Error at line 1, column 2:
  Invalid token: '_' ('_')
  Expected: expression, 'exit', or 'quit'"

# TESTE 28: Linha após múltiplos comandos
run_test "28" "LINHA: erro após 5 outputs" \
    "1+1
2+2
3+3
4+4
5+5
6_6" \
    "2
4
6
8
10
6

Syntax Error at line 12, column 2:
  Unexpected token: '_' ('_')
  Expected: ; or newline"

# ==================== TESTES ADICIONAIS ====================

# TESTE 29: Divisão
run_test "29" "Operação: divisão (10/2)" \
    "10/2" \
    "5"

# TESTE 30: Expressão complexa mista
run_test "30" "EXPRESSÃO: mista 10/2+3*4" \
    "10/2+3*4" \
    "17"

# TESTE 31: Múltiplas variáveis
run_test "31" "VARIÁVEL: múltiplas variáveis" \
    "a := 10
b := 20
c := a + b
c" \
    "10
20
30
30"

# TESTE 32: Linha vazia (epsilon)
run_test "32" "EPSILON: linha vazia" \
    "
2+2" \
    "4"

echo ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║                    RESUMO DOS TESTES                       ║"
echo "╠════════════════════════════════════════════════════════════╣"
printf "║  Total de testes: %-2d                                      ║\n" "$TOTAL_TESTS"
printf "║  ${GREEN}✓ PASSOU: %-2d${NC}                                             ║\n" "$PASS_COUNT"
printf "║  ${RED}✗ FALHOU: %-2d${NC}                                             ║\n" "$FAIL_COUNT"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Exit code baseado no resultado
if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}🎉 TODOS OS TESTES PASSARAM!${NC}"
    exit 0
else
    echo -e "${RED}❌ ALGUNS TESTES FALHARAM!${NC}"
    exit 1
fi
