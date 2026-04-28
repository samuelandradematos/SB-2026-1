#include <instrucoes.hpp>

//using namespace std;

void PopulaInstrucoes(){
    instrucoes.emplace(instrucoes.begin(),"ADD",1,"01");
    instrucoes.emplace(instrucoes.begin(),"SUB",1,"02");
    instrucoes.emplace(instrucoes.begin(),"MUL",1,"03");
    instrucoes.emplace(instrucoes.begin(),"DIV",1,"04");
    instrucoes.emplace(instrucoes.begin(),"JMP",1,"05");
    instrucoes.emplace(instrucoes.begin(),"JMPN",1,"06");
    instrucoes.emplace(instrucoes.begin(),"JMPP",1,"07");
    instrucoes.emplace(instrucoes.begin(),"JMPZ",1,"08");
    instrucoes.emplace(instrucoes.begin(),"COPY",2,"09");
    instrucoes.emplace(instrucoes.begin(),"LOAD",1,"10");
    instrucoes.emplace(instrucoes.begin(),"STORE",1,"11");
    instrucoes.emplace(instrucoes.begin(),"INPUT",1,"12");
    instrucoes.emplace(instrucoes.begin(),"OUTPUT",1,"13");
    instrucoes.emplace(instrucoes.begin(),"STOP",0, "14");
}

void PopulaDiretivas(){
    diretivas.emplace(diretivas.begin(),"SPACE", 1);
    diretivas.emplace(diretivas.begin(),"CONST", 1);
}
