#ifndef INSTRUCOES_HPP
#define INSTRUCOES_HPP
#include <string>
#include <map>
#include <tuple>
#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;

class Instrucoes
{
    private:
        Instrucoes();
        ~Instrucoes();
        static Instrucoes* instance;
        unordered_map<string,tuple<int,string>> instrucao;
    public:
        static Instrucoes& GetInstance();
        bool IsInstrucao(string instrucao);
        bool IsCopy(string instrucao);
        bool IsStop(string instrucao);
        unordered_map<string, tuple<int, string>>::const_iterator GetInstrucao(string instrucao);
};

class Diretivas
{
    private:
        Diretivas();
        ~Diretivas();
        static Diretivas* instance;
        unordered_map<string,int> diretiva;
    public:
        static Diretivas& GetInstance();
        bool IsDiretiva(string diretiva);
        unordered_map<string,int>::const_iterator GetDiretiva(string diretiva);
};
#endif