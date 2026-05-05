#include <instrucoes.hpp>

Instrucoes* Instrucoes::instance = nullptr;
Instrucoes::~Instrucoes(){};

Instrucoes& Instrucoes::GetInstance(){
    if(instance == nullptr){
        instance = new Instrucoes();
    }
    return *instance;
}

Instrucoes::Instrucoes(){
    instrucao["ADD"] = make_tuple(1,"01");
    instrucao["SUB"] = make_tuple(1,"02");
    instrucao["MUL"] = make_tuple(1,"03");
    instrucao["DIV"] = make_tuple(1,"04");
    instrucao["JMP"] = make_tuple(1,"05");
    instrucao["JMPN"] = make_tuple(1,"06");
    instrucao["JMPP"] = make_tuple(1,"07");
    instrucao["JMPZ"] = make_tuple(1,"08");
    instrucao["COPY"] = make_tuple(2,"09");
    instrucao["LOAD"] = make_tuple(1,"10");
    instrucao["STORE"] = make_tuple(1,"11");
    instrucao["INPUT"] = make_tuple(1,"12");
    instrucao["OUTPUT"] = make_tuple(1,"13");
    instrucao["STOP"] = make_tuple(0, "14");
}

bool Instrucoes::IsInstrucao(string instrucao) {
    unordered_map<string, tuple<int, string>>::const_iterator it = this->instrucao.find(instrucao);
    if (it != this->instrucao.end())
        return true;
    else 
        return false;
}

bool Instrucoes::IsCopy(string instrucao) {
    if (instrucao == "COPY")
        return true;
    else
        return false;
}

bool Instrucoes::IsStop(string instrucao) {
    if (instrucao == "STOP")
        return true;
    else
        return false;
}

unordered_map<string, tuple<int, string>>::const_iterator Instrucoes::GetInstrucao(string instrucao) {
    return this->instrucao.find(instrucao);
}

Diretivas* Diretivas::instance = nullptr;
Diretivas::~Diretivas(){};

Diretivas& Diretivas::GetInstance(){
    if(instance == nullptr){
        instance = new Diretivas();
    }
    return *instance;
}

Diretivas::Diretivas(){
    diretiva["SPACE"] = 1;
    diretiva["CONST"] = 1;
}

bool Diretivas::IsDiretiva(string diretiva) {
    if (this->diretiva.find(diretiva) != this->diretiva.end())
        return true;
    else
        return false;
}

unordered_map<string,int>::const_iterator Diretivas::GetDiretiva(string diretiva) {
    return this->diretiva.find(diretiva);
}
