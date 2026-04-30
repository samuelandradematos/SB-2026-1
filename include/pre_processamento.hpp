#ifndef PRE_PROCESSAMENTO_HPP
#define PRE_PROCESSAMENTO_HPP

#include <algorithm>
#include <string>
#include <cctype>
#include <vector>


bool isArquivoASM(const std::string& nome);
std::string toUpperCase(std::string linha);
std::string removeComments(std::string linha);
std::string removeUnnecessarySpaces(const std::string& str);
#endif