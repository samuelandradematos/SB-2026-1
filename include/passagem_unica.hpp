#include <instrucoes.hpp>

unordered_map<string,tuple<bool,list<int>>> tabelaDePendencias;

bool IsInstrucao(string);
bool IsDiretiva(string);
string GetOpcodeInstrucao(string);
int GetQuantidadeOperandosInstrucao(string);
int GetQuantidadeOperandosDiretiva(string);
void InserePendencia(string, int);
bool IsDefinido(string);
void PrintTabelaDePendencias();

