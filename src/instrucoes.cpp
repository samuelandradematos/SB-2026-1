#include <instrucoes.hpp>

void PopulaInstrucoes(){
    instrucoes["ADD"] = make_tuple(1,"01");
    instrucoes["SUB"] = make_tuple(1,"02");
    instrucoes["MUL"] = make_tuple(1,"03");
    instrucoes["DIV"] = make_tuple(1,"04");
    instrucoes["JMP"] = make_tuple(1,"05");
    instrucoes["JMPN"] = make_tuple(1,"06");
    instrucoes["JMPP"] = make_tuple(1,"07");
    instrucoes["JMPZ"] = make_tuple(1,"08");
    instrucoes["COPY"] = make_tuple(2,"09");
    instrucoes["LOAD"] = make_tuple(1,"10");
    instrucoes["STORE"] = make_tuple(1,"11");
    instrucoes["INPUT"] = make_tuple(1,"12");
    instrucoes["OUTPUT"] = make_tuple(1,"13");
    instrucoes["STOP"] = make_tuple(0, "14");
}

void PopulaDiretivas(){
    diretivas["SPACE"] = 1;
    diretivas["CONST"] = 1;
}
