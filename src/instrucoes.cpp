#include <instrucoes.hpp>

void PopulaInstrucoes(){
    instrucoes.emplace("ADD", (1,"01"));
    instrucoes.emplace("SUB", (1,"02"));
    instrucoes.emplace("MUL", (1,"03"));
    instrucoes.emplace("DIV", (1,"04"));
    instrucoes.emplace("JMP", (1,"05"));
    instrucoes.emplace("JMPN", (1,"06"));
    instrucoes.emplace("JMPP", (1,"07"));
    instrucoes.emplace("JMPZ", (1,"08"));
    instrucoes.emplace("COPY", (2,"09"));
    instrucoes.emplace("LOAD", (1,"10"));
    instrucoes.emplace("STORE", (1,"11"));
    instrucoes.emplace("INPUT", (1,"12"));
    instrucoes.emplace("OUTPUT", (1,"13"));
    instrucoes.emplace("STOP", (0, "14"));
}

void PopulaDiretivas(){
    diretivas.emplace(diretivas.begin(),"SPACE", 1);
    diretivas.emplace(diretivas.begin(),"CONST", 1);
}
