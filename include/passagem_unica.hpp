#ifndef PASSAGEM_UNICA_HPP
#define PASSAGEM_UNICA_HPP
#include <instrucoes.hpp>
#include <fstream>
#include <cstring>
#include <regex>

class TabelaDeSimbolos
{
    private:
        TabelaDeSimbolos();
        ~TabelaDeSimbolos();
        static TabelaDeSimbolos* instance;   
    public:
        map<string, tuple<bool, int, list<string>>> tabelaDePendencias;
        static TabelaDeSimbolos& GetInstance();
        bool LabelDefinido(string label);
        bool LabelNaTabela(string label);
        void UpdateListaPendencias(string label, string endereco);
        string GetEndereco(string label);
        void InsertSimbolo(string label, string posicaoMemoria, bool definido);
        void UpdateLabelDefinido(string label, int endereco);
        void PrintTabelaDePendencias();
};

//unordered_map<string, tuple<bool, int, list<string>>> tabelaDePendencias;

bool IsInstrucao(string);
bool IsDiretiva(string);
string GetOpcodeInstrucao(string);
int GetQuantidadeOperandosInstrucao(string);
int GetQuantidadeOperandosDiretiva(string);
void InsertPendencia(string,  string,  bool);
void UpdateListaPendencias(string,  string);
bool LabelDefinido(string);
void PrintTabelaDePendencias();
string ResolveInstrucao(string, string, int);
void InsertSimbolo(string, bool, int);
void TraduzOpcode(string, string);
string GetEndereco(string);
string ConverteIntEndereco(int);
void UpdateLabelDefinido(string);
bool LabelNaTabela(string);
bool LabelDefinido(string);
tuple<string,string,string> getConteudoLinha(string);
void Parser(string);
void CriaArquivoSaida(list<tuple<int,tuple<string,string>>>, string);

#endif