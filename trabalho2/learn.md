           ↑ parâmetros

[ebp+12] segundo parâmetro
[ebp+8 ] primeiro parâmetro
[ebp+4 ] endereço de retorno
[ebp   ] EBP antigo
--------------------------
[ebp-4 ] variável local 1
[ebp-8 ] variável local 2
[ebp-12] variável local 3

           ↓ pilha cresce


           Sim. E eu vou te dar tarefas na ordem que eu faria se estivesse estudando para a prova e implementando o trabalho ao mesmo tempo.

A tentação é começar pelo menu, mas isso é um erro. O menu depende de entrada e saída, então você precisa construir as fundações primeiro.

# Tarefa 1: Criar um programa que só imprime uma mensagem

Objetivo:

```text
Olá mundo
```

e termina.

Mas sem funções ainda.

Você precisa aprender:

* `section .data`
* `section .text`
* `_start`
* `sys_write`
* `sys_exit`

Quando funcionar, você terá certeza de que:

* NASM está instalado;
* LD está funcionando;
* Você sabe compilar;
* Você sabe executar.

---

# Tarefa 2: Transformar a impressão em uma função

Agora o programa principal não imprime.

Ele apenas chama:

```text
print_string()
```

A função faz o `write`.

Aqui você aprende:

* `call`
* `ret`
* passagem de parâmetros pela pilha

O objetivo é conseguir fazer:

```assembly
push tamanho
push endereco
call print_string
```

e a função recuperar esses parâmetros usando:

```assembly
push ebp
mov ebp, esp
```

---

# Tarefa 3: Ler uma string

Faça uma função:

```text
read_string()
```

que use:

```text
sys_read
```

Objetivo:

```text
Digite seu nome:
```

usuário digita:

```text
Suyanne
```

e o programa termina.

Ainda sem imprimir o nome de volta.

Você aprende:

* stdin
* buffers
* `.bss`
* `read`

---

# Tarefa 4: Ecoar o nome

Agora:

```text
Digite seu nome:
Suyanne

Você digitou:
Suyanne
```

Aqui você começa a entender melhor ponteiros e buffers.

---

# Tarefa 5: Criar a tela inicial completa

Objetivo:

```text
Bem-vindo. Digite seu nome:
```

lê nome

depois:

```text
Hola, Suyanne, bem-vindo ao programa de CALC IA-32
```

---

# Tarefa 6: Criar a função do menu

Faça uma função:

```text
mostrar_menu()
```

que imprime todas as linhas.

Lembre:

O professor quer que todas as mensagens passem pela mesma função de saída.

Então dentro de `mostrar_menu()` você chamará várias vezes `print_string()`.

---

# Tarefa 7: Ler uma opção

Por enquanto aceite apenas:

```text
1
7
```

como caracteres.

Nem precisa converter para inteiro ainda.

Só verificar:

```text
se digitou '1'
```

ou

```text
se digitou '7'
```

---

# Tarefa 8: Criar o loop principal

Objetivo:

```text
menu

opcao

menu

opcao

menu

opcao
```

até escolher sair.

Aqui você aprende os saltos:

```assembly
jmp
je
jne
cmp
```

---

# O que eu faria HOJE

Somente as tarefas 1 e 2.

Meta final de hoje:

```text
_start

    push tamanho
    push msg
    call print_string

    sair
```

e a função:

```text
print_string(ptr, tamanho)
```

funcionando.

Quando você terminar isso, me manda o código inteiro (mesmo que dê erro) e eu vou te explicar exatamente como os parâmetros estão chegando na pilha, porque essa parte é o coração do trabalho.
