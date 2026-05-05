# SB-2026-1
Trabalhos de SB 2026-1, professor Bruno

## Compilar o código
```
g++ src/*.cpp -Iinclude -o montador
```
## Rodar as etapas:
```bash
./montador nomeArquivo.asm   # executa pré-processamento → gera .pre
./montador nomeArquivo.pre   # executa montagem → gera .obj e .pen
./montador nomeArquivo.obj   # executa simulador
```