#include "pre_processamento.hpp"

bool isArquivoASM(const std::string& nome) {
    if (nome.size() < 4) return false;
    return nome.substr(nome.size() - 4) == ".asm";
}