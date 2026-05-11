#ifndef PRE_PROCESSAMENTO_HPP
#define PRE_PROCESSAMENTO_HPP

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>

bool isArquivoASM(const std::string &nome); 
std::string toUpperCase(std::string linha);
std::string removeComments(std::string linha);
std::string removeUnnecessarySpaces(const std::string &str);
std::string treatCopy(std::string linha);
bool storeEQUs(const std::string &linha, std::map<std::string, std::string> &tabelaEQU);
std::string substituirEQU(const std::string &linha,
                          const std::map<std::string, std::string> &tabelaEQU);
bool treatIF(const std::string &linha,
             const std::map<std::string, std::string> &tabelaEQU,
             bool &pularProximaLinha);
void preProcessar(const std::string &nomeArquivo);


#endif