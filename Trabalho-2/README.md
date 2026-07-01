# Calculadora IA-32 — Software Básico

## Sistema Operacional
Linux (Ubuntu 22.04 / Debian ou compatível, x86 ou com suporte a 32 bits)
Linux (CachyOS / ArchLinux)

## Dependências
- **NASM** (Netwide Assembler)
- **LD** (GNU Linker)

Instalar no Ubuntu/Debian:
```bash
sudo apt update
sudo apt install nasm binutils gcc-multilib make
```
Instalar no ArchLinux:
```bash
sudo pacman -Syu
sudo pacman -S nasm binutils gcc-multilib make
```

---

## Compilar e Ligar

### Passo a passo

Para apenas compilar e ligar
```bash
make
```
Para compilar, ligar e rodar
```bash
make run
```
---

## Executar

```bash
./calculadora
```

---

## Arquivos

| Arquivo           | Conteúdo                                              |
|-------------------|-------------------------------------------------------|
| CALCULADORA.ASM   | Programa principal (`_start`), funções de I/O         |
| SOMA.ASM          | Operação de soma (16 e 32 bits)                       |
| SUBTRACAO.ASM     | Operação de subtração (16 e 32 bits)                  |
| MUTIPLICACAO.ASM  | Operação de multiplicação (16 e 32 bits)              |
| DIVISAO.ASM       | Operação de divisao (16 e 32 bits)                    |
| README.md         | Este arquivo                                          |

---

## Estrutura do Programa

- **Variáveis globais**: apenas strings de mensagem, `var_nome`, `var_precisao` e `var_opcao`
- **Todas as demais variáveis**: locais na pilha (`sub esp, N`)
- **Passagem de parâmetros**: exclusivamente pela pilha (convenção cdecl)
- **Retorno de funções**: sempre em `EAX`
- **Saída de texto**: única função `print_string`, chamada com ponteiro + tamanho na pilha
- **Programa principal**: chama apenas funções, sem I/O direto e sem processar dados
- **Biblioteca IO.MAC**: **NÃO utilizada**

---

## Funções exportadas em CALCULADORA.ASM

| Função          | Descrição                                            |
|-----------------|------------------------------------------------------|
| `print_string`  | Imprime string (ponteiro + tamanho na pilha)         |
| `read_string`   | Lê string do teclado (buffer + max_bytes na pilha)   |
| `read_num16`    | Lê inteiro de 16 bits; retorna em EAX                |
| `read_num32`    | Lê inteiro de 32 bits; retorna em EAX                |
| `print_int`     | Imprime inteiro (valor na pilha)                     |

---

## Observações

- Funciona com números **positivos e negativos**
- Números de 16 bits são tratados com `movsx` (sign-extension) para garantir a precisão correta
- A operação 7 (SAIR) encerra o programa via `sys_exit`
