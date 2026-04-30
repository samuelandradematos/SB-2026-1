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
        // std::cout << linha << std::endl;
    }

    arquivo.close();
    return 0;
}