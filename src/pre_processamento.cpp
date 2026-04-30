#include "pre_processamento.hpp"

bool isArquivoASM(const std::string &nome)
{
    if (nome.size() < 4)
        return false;
    return nome.substr(nome.size() - 4) == ".asm";
}

std::string toUpperCase(std::string linha)
{
    std::transform(linha.begin(), linha.end(), linha.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return linha;
}