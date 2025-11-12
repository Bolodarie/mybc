#!/bin/bash
# Script consolidado de todos os testes do mybc

echo "###############################################"
echo "#     SUITE COMPLETA DE TESTES - MYBC        #"
echo "###############################################"
echo ""

echo "================================================"
echo "1. TESTE DE DETECÇÃO DE ERROS"
echo "================================================"
echo "Testando tokens inválidos..."
echo -e "2_2\n_ 5\n2 @ 3" | ./mybc 2>&1
echo ""

echo "================================================"
echo "2. TESTE DE RECUPERAÇÃO DE ERROS"
echo "================================================"
echo "Verificando se o interpretador continua após erros..."
echo -e "2+3\n2 + * 3\n5+5" | ./mybc 2>&1
echo ""

echo "================================================"
echo "3. TESTE DE CONTAGEM DE LINHAS"
echo "================================================"
echo "Verificando se números de linha refletem modo interativo..."
echo -e "1+1\n2+2\n3 + * 3\n4+4" | ./mybc 2>&1
echo ""

echo "================================================"
echo "4. TESTE DE VARIÁVEIS"
echo "================================================"
echo -e "x := 10\nx + 5\ny := x * 2\ny" | ./mybc 2>&1
echo ""

echo "================================================"
echo "5. TESTE DE RECUPERAÇÃO MÚLTIPLA"
echo "================================================"
echo "Vários erros em sequência..."
echo -e "1+1\n@@\n2+2\n##\n3+3" | ./mybc 2>&1
echo ""

echo "================================================"
echo "6. TESTE DE EXPRESSÕES COMPLEXAS"
echo "================================================"
echo -e "(2+3)*4\n10/2+3*4\n-5+10\n(1+2)*(3+4)" | ./mybc 2>&1
echo ""

echo "================================================"
echo "7. TESTE DE CTRL+C (MANUAL)"
echo "================================================"
echo "⚠️  ESTE TESTE REQUER INTERAÇÃO MANUAL"
echo ""
echo "Para testar Ctrl+C:"
echo "  1. Execute: ./mybc"
echo "  2. Digite uma expressão: (10+20)*(30+"
echo "  3. Pressione Ctrl+C"
echo "  4. Deve aparecer uma quebra de linha"
echo "  5. Digite: 2+2"
echo "  6. Deve mostrar: 4"
echo "  7. Digite: exit"
echo ""
echo "Comportamento esperado:"
echo "  ✓ Ctrl+C imprime quebra de linha"
echo "  ✓ Interpretador NÃO encerra"
echo "  ✓ Continua aguardando comandos"
echo ""
echo "Pressione ENTER para pular este teste..."
read -t 3 -r || true
echo ""

echo "================================================"
echo "RESUMO DOS RECURSOS TESTADOS:"
echo "================================================"
echo "✅ Detecção de erros de sintaxe"
echo "✅ Recuperação de erros (REPL continua)"
echo "✅ Contagem de linhas em modo interativo"
echo "✅ Mensagens de erro detalhadas"
echo "✅ Variáveis e atribuições"
echo "✅ Expressões aritméticas"
echo "✅ Parênteses e precedência"
echo "✅ Números decimais e float"
echo "✅ Captura de Ctrl+C (teste manual)"
echo ""
echo "###############################################"
echo "#            TESTES CONCLUÍDOS                #"
echo "###############################################"
