#include <iostream>
#include <fstream>
#include "pre_processamento.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: ./montador <arquivo.asm>" << std::endl;
        return 1;
    }

    // ABRIR O ARQUIVO
    std::string nomeArquivo = argv[1];
    std::ifstream arquivo(nomeArquivo);
    std::string rotuloIsolado = "";

    if (!isArquivoASM(nomeArquivo))
    {
        std::cerr << "Erro: o arquivo deve ter extensao .asm" << std::endl;
        return 1;
    }

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return 1;
    }

    std::cout << "Arquivo aberto com sucesso!" << std::endl;

    // LER O ARQUIVO
    std::string linha;
    while (getline(arquivo, linha))
    {

        // 1. Remover comentários
        linha = removeComments(linha);

        // 2. Remover espacos desnecessário
        linha = removeUnnecessarySpaces(linha);

        // se depois de remover comentários e espacos a linha é vazia, só ignora ela
        if (linha.empty())
            continue;

        // 2. Converter tudo para maíusculo
        linha = toUpperCase(linha);

        // 3. Trata rótulos em linhas separadas das instrucoes
        // Se a linha termina com :, é um rótulo isolado
        if (linha.back() == ':'){
            if (!rotuloIsolado.empty()){
                // se já tem rótulo, escreve uma linha com rótulo o anterior
                std::cout << rotuloIsolado << std::endl;
            }
            rotuloIsolado = linha; // guarda o atual
            continue;
        }

        // Se chegou aqui, é instrução
        if (!rotuloIsolado.empty()){
            linha = rotuloIsolado + " " + linha; // junta o útlimo rótulo com a instrucao
            rotuloIsolado.clear();
        }

        std::cout << linha << std::endl;

    }

    arquivo.close();
    return 0;
}