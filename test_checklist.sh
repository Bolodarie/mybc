#!/bin/bash
# Checklist simples de validação manual

echo "════════════════════════════════════════════════════════════"
echo "        CHECKLIST DE VALIDAÇÃO MANUAL - MYBC"
echo "════════════════════════════════════════════════════════════"
echo ""
echo "Instruções: Execute cada comando e marque [ ] com [x] se OK"
echo ""

cat << 'EOF'
[ ] 1. TESTE BÁSICO
    Comando: echo "2+3" | ./mybc
    Deve mostrar: 5

[ ] 2. ERRO DETECTADO
    Comando: echo "2 + + 3" | ./mybc
    Deve mostrar: Syntax Error (e mencionar '+')

[ ] 3. CONTINUA APÓS ERRO
    Comando: echo -e "2+3\n2_2\n5+5" | ./mybc
    Deve mostrar: 5, erro, 10 (três saídas!)

[ ] 4. CONTAGEM DE LINHA
    Comando: echo -e "1+1\n2 + * 2" | ./mybc
    O erro deve dizer: "line 3" (não line 2!)
    
    Explicação visual:
    Linha 1: 1+1     (você digita)
    Linha 2: 2       (programa imprime)
    Linha 3: 2 + * 2 (você digita - ERRO aqui)

[ ] 5. VARIÁVEIS FUNCIONAM
    Comando: echo -e "x := 10\nx + 5" | ./mybc
    Deve mostrar: 10, 15

[ ] 6. PARÊNTESES
    Comando: echo "(2+3)*4" | ./mybc
    Deve mostrar: 20

[ ] 7. MENSAGEM CLARA
    Comando: echo "2 @ 3" | ./mybc
    Deve mostrar: linha, coluna, token esperado e encontrado

[ ] 8. MÚLTIPLAS LINHAS VÁLIDAS
    Comando: echo -e "1+1\n2+2\n3+3" | ./mybc
    Deve mostrar: 2, 4, 6 (três resultados)

[ ] 9. ERRO NO INÍCIO
    Comando: echo "_ 5" | ./mybc
    Deve detectar '_' como inválido

[ ] 10. FLOAT
    Comando: echo "3.14 * 2" | ./mybc
    Deve mostrar: 6.28

═══════════════════════════════════════════════════════════════
RESUMO: Se todos os 10 itens estão marcados [x], está OK!
═══════════════════════════════════════════════════════════════

EOF

echo ""
echo "Quer executar todos automaticamente? (s/n)"
read -r response

if [[ "$response" == "s" || "$response" == "S" ]]; then
    echo ""
    echo "Executando testes..."
    echo ""
    
    echo "→ Teste 1: 2+3"
    echo "2+3" | ./mybc
    echo ""
    
    echo "→ Teste 2: 2 + + 3"
    echo "2 + + 3" | ./mybc 2>&1
    echo ""
    
    echo "→ Teste 3: Recuperação"
    echo -e "2+3\n2_2\n5+5" | ./mybc 2>&1
    echo ""
    
    echo "→ Teste 4: Contagem de linha"
    echo -e "1+1\n2 + * 2" | ./mybc 2>&1
    echo ""
    
    echo "→ Teste 5: Variáveis"
    echo -e "x := 10\nx + 5" | ./mybc
    echo ""
    
    echo "→ Teste 6: Parênteses"
    echo "(2+3)*4" | ./mybc
    echo ""
    
    echo "→ Teste 7: Mensagem clara"
    echo "2 @ 3" | ./mybc 2>&1
    echo ""
    
    echo "→ Teste 8: Múltiplas linhas"
    echo -e "1+1\n2+2\n3+3" | ./mybc
    echo ""
    
    echo "→ Teste 9: Erro no início"
    echo "_ 5" | ./mybc 2>&1
    echo ""
    
    echo "→ Teste 10: Float"
    echo "3.14 * 2" | ./mybc
    echo ""
fi
