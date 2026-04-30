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

std::string removeComments(std::string linha){
    size_t position = linha.find(";");
    
    if (position != std::string::npos) {
        return linha.substr(0, position);
    }

    return linha;
}

std::string removeUnnecessarySpaces(const std::string& str) {
    std::string resultado;
    bool dentroDeEspaco = false;

    for (char c : str) {
        if (std::isspace((unsigned char)c)) {
            // se já estamos em espaço, ignora
            if (!dentroDeEspaco) {
                resultado += ' '; // coloca só um espaço
                dentroDeEspaco = true;
            }
        } else {
            resultado += c;
            dentroDeEspaco = false;
        }
    }

    // remove espaço do início
    if (!resultado.empty() && resultado.front() == ' ') {
        resultado.erase(0, 1);
    }

    // remove espaço do fim
    if (!resultado.empty() && resultado.back() == ' ') {
        resultado.pop_back();
    }

    return resultado;
}