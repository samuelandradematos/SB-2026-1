#ifndef PRE_PROCESSAMENTO_HPP
#define PRE_PROCESSAMENTO_HPP

#include <string>
#include <algorithm>
#include <cctype>

bool isArquivoASM(const std::string& nome);
std::string toUpperCase(std::string linha);
#endif