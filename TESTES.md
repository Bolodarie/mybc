# 🧪 Guia de Testes - MYBC Interpreter

## 📋 Scripts de Teste Disponíveis

### 1. **test_auto.sh** - Testes Automatizados (RECOMENDADO) ⭐
**Para quem:** Qualquer pessoa  
**Dificuldade:** ⭐ Fácil  
**Tempo:** ~5 segundos

```bash
./test_auto.sh
```

**O que faz:**
- Executa **32 testes automaticamente** cobrindo todas as funcionalidades
- Compara resultado esperado vs obtido
- Mostra **✓ PASS** ou **✗ FAIL** para cada teste
- No final mostra: `🎉 TODOS OS TESTES PASSARAM!` ou `❌ ALGUNS FALHARAM`

**Categorias de testes:**
- ✅ Operações básicas (8 testes)
- 🐛 Detecção de erros (6 testes)
- 🔄 Recuperação de erros (5 testes)
- 🔢 Contagem de linhas (4 testes)
- 📊 Variáveis e expressões (9 testes)

**Exemplo de saída:**
```
✓ PASS | Teste 01: Cálculo simples: 2+3
✓ PASS | Teste 02: Multiplicação: 5*4
✗ FAIL | Teste 03: Bug encontrado
  Esperado: 20
  Obtido: 10

Total: 32 testes
✓ PASSOU: 31
✗ FALHOU: 1
```

---

### 2. **test_checklist.sh** - Checklist Manual
**Para quem:** Quem quer validar visualmente  
**Dificuldade:** ⭐⭐ Médio  
**Tempo:** ~2 minutos

```bash
./test_checklist.sh
```

**O que faz:**
- Mostra 10 testes com comandos para executar
- Você executa e marca [ ] com [x] se passou
- Pode executar todos automaticamente (digitar 's' quando perguntar)

**Exemplo:**
```
[ ] 1. TESTE BÁSICO
    Comando: echo "2+3" | ./mybc
    Deve mostrar: 5
```

---

### 3. **test_all.sh** - Suite Completa de Demonstração
**Para quem:** Apresentações e demonstrações  
**Tempo:** ~10 segundos

```bash
./test_all.sh
```

**O que faz:**
- Executa exemplos visuais de cada funcionalidade
- Mostra o interpretador em ação
- Ótimo para demonstrações

---


## 🚀 Como Usar

### Teste Rápido (5 segundos)
```bash
./test_auto.sh
```
Se mostrar `🎉 TODOS OS TESTES PASSARAM!` → Está tudo OK!

### Teste Completo (1 minuto)
```bash
./test_auto.sh        # Automatizado (32 testes)
./test_checklist.sh   # Manual (10 verificações)
```

### Para Apresentação
```bash
./test_all.sh         # Demonstração visual
```

---

## 📊 O Que Cada Teste Valida

| Aspecto | test_auto | test_checklist | test_all |
|---------|-----------|----------------|----------|
| Cálculos básicos | ✅ (8) | ✅ | ✅ |
| Detecção de erros | ✅ (6) | ✅ | ✅ |
| Recuperação (REPL) | ✅ (5) | ✅ | ✅ |
| Contagem de linhas | ✅ (4) | ✅ | ✅ |
| Variáveis | ✅ (9) | ✅ | ✅ |
| **Ctrl+C (SIGINT)** | ❌ | ❌ | ✅ (manual) |
| **Total de testes** | **32** | **10** | **~15** |
| **Automatizado?** | ✅ | ⚠️ | ⚠️ |
| **PASS/FAIL claro?** | ✅ | ⚠️ | ❌ |
| **Visual/Demo?** | ❌ | ✅ | ✅ |

**Nota:** O teste de Ctrl+C no `test_all.sh` inclui instruções para teste manual.

---

## 🎯 Recomendação

**Para validação rápida:**
```bash
./test_auto.sh
```

**Para apresentar para alguém:**
```bash
./test_all.sh         # Demonstração visual
# Ou
./test_checklist.sh   # Checklist interativo
```

**Para debug de problema específico:**
Use `./test_auto.sh` e veja qual teste falhou. Os testes estão organizados por categoria:
- Testes 01-08: Operações básicas
- Testes 09-10, 16-20: Detecção de erros
- Testes 11, 15, 21-24: Recuperação
- Testes 12, 25-28: Contagem de linhas
- Testes 13, 31: Variáveis

---

## ✅ Critérios de Sucesso

O interpretador está **correto** se:

1. `./test_auto.sh` mostra: **32/32 testes PASSARAM** ✅
2. Detecta erros de sintaxe corretamente ✅
3. Continua executando após erros (não trava) ✅
4. Números de linha refletem modo interativo ✅
5. Mensagens de erro são claras e informativas ✅
6. **Ctrl+C** imprime quebra de linha e continua (não encerra) ✅

---

## 🐛 Se Algo Falhar

```bash
# Execute o teste automatizado
./test_auto.sh

# Se algum teste FALHOU, ele mostra:
✗ FAIL | Teste XX: descrição
  Esperado: ...
  Obtido: ...

# Use isso para identificar o bug
```

---

## 📝 Adicionando Novos Testes

Edite `test_auto.sh` e adicione:

```bash
run_test "33" "Descrição do teste" \
    "input aqui" \
    "output esperado aqui"
```

---

## 📦 Resumo dos Arquivos

| Arquivo | Propósito | Testes | Tempo |
|---------|-----------|--------|-------|
| `test_auto.sh` | ✅ Validação automatizada | 32 | 5s |
| `test_checklist.sh` | 📋 Checklist manual | 10 | 2min |
| `test_all.sh` | 🎬 Demonstração visual + Ctrl+C | ~15 | 10s |

**Total:** 3 scripts cobrindo todas as necessidades de teste! 🎉

**Observação:** O teste de Ctrl+C está integrado no `test_all.sh` (seção 7) com instruções para teste manual.
