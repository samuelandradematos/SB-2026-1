#include <instrucoes.hpp>
#include <fstream>

unordered_map<string, tuple<bool, int, list<int>>> tabelaDePendencias;

bool IsInstrucao(string);
bool IsDiretiva(string);
string GetOpcodeInstrucao(string);
int GetQuantidadeOperandosInstrucao(string);
int GetQuantidadeOperandosDiretiva(string);
void InsertPendencia(string,  int,  bool);
void UpdateListaPendencias(string label,  int posicaoMemoria);
bool LabelDefinido(string);
void PrintTabelaDePendencias();
tuple<string, int> resolveInstrucao(string, string);
void InsertSimbolo(string, bool, int);
void TraduzOpcode(string, string);
int GetEndereco(string);
bool LabelDefinido(string);
void Parser(string);