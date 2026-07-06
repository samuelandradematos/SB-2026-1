# SB-2026-1
Trabalhos de SB 2026-1, professor Bruno

# Alunos:
170155943 - Samuel Andrade de Matos

222006445 - Suyanne Sara Miranda Silva

# SO utilizado: 
Linux

## Como compilar o código
```bash
g++ src/*.cpp -Iinclude -o montador # usando g++ (preferivelmente)
gcc src/*.cpp -Iinclude -lstdc++ -o montador # usando gcc puramente
```

## Como rodar as etapas:
```bash
./montador nomeArquivo.asm   # executa pré-processamento → gera .pre
./montador nomeArquivo.pre   # executa montagem → gera .obj e .pen
./montador nomeArquivo.obj   # executa simulador
```
