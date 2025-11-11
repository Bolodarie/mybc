#!/bin/bash
# Script de teste para mensagens de erro do mybc

echo "=== Testando detecção de erros ==="
echo ""

echo "Teste 1: Token inválido no meio de expressão (2_2)"
echo "2_2" | ./mybc
echo ""

echo "Teste 2: Operador inválido (@)"
echo "2 @ 3" | ./mybc
echo ""

echo "Teste 3: Token inválido no início (_)"
echo "_ 5" | ./mybc
echo ""

echo "Teste 4: Dois números sem operador"
echo "5 5" | ./mybc
echo ""

echo "Teste 5: Operador duplo"
echo "2 - - 2" | ./mybc
echo ""

echo "Teste 6: Operador esperando operando"
echo "2 + * 3" | ./mybc
echo ""

echo "Teste 7: Parêntese não fechado"
echo "(2 + 3" | ./mybc
echo ""

echo "=== Testando casos válidos ==="
echo ""

echo "Teste 8: Expressões válidas"
echo -e "2+3\n5*4\n10/2" | ./mybc
echo ""

echo "Teste 9: Parênteses"
echo "(2+3)*4" | ./mybc
echo ""

echo "Teste 10: Atribuição e uso de variável"
echo -e "x := 5\nx + 3" | ./mybc
echo ""

echo "Teste 11: Número float"
echo "3.14 * 2" | ./mybc
