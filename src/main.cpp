#include <iostream>
#include <fstream>
#include "pre_processamento.hpp"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./montador <arquivo.asm>" << std::endl;
        return 1;
    }

    // ABRIR O ARQUIVO
    std::string nomeArquivo = argv[1];
    std::ifstream arquivo(nomeArquivo);

    if (!isArquivoASM(nomeArquivo)) {
        std::cerr << "Erro: o arquivo deve ter extensao .asm" << std::endl;
        return 1;
    }

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return 1;
    }

    std::cout << "Arquivo aberto com sucesso!" << std::endl;

    // LER O ARQUIVO
    std::string linha;
    while (getline(arquivo, linha)) {
        // 1. Remover comentários
        linha = removeComments(linha);

        // 2. Remover espacos desnecessário
        linha = removeUnnecessarySpaces(linha);

        // se depois de remover comentários e espacos a linha é vazia, só ignora ela
        if (linha.empty()) continue;
        
        // 2. Converter tudo para maíusculo
        linha = toUpperCase(linha);
        std::cout << linha << std::endl;

    }

    arquivo.close();
    return 0;
}