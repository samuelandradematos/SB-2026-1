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

    std::string nomeArquivoPreGerado = nomeArquivo.substr(0, nomeArquivo.size() - 4) + ".pre";
    std::ofstream arquivoPreGerado(nomeArquivoPreGerado);
    std::string rotuloIsolado = "";
    std::map<std::string, std::string> tabelaEQU; // dicionário com nome da EQU e valor
    bool pularProximaLinha = false;               // controla a parte do IF
    std::vector<std::string> secaoText;
    std::vector<std::string> secaoData;

    bool emData = false;

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
        // 0. verifica se deve pular a linha (caso valor de 0 do IF na linha anterior)
        if (pularProximaLinha)
        {
            pularProximaLinha = false;
            continue;
        }

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
        if (linha.back() == ':')
        {
            if (!rotuloIsolado.empty())
            {
                // se já tem rótulo, escreve uma linha com rótulo o anterior
                if (emData)
                {
                    secaoData.push_back(rotuloIsolado);
                }
                else
                {
                    secaoText.push_back(rotuloIsolado);
                }
            }
            rotuloIsolado = linha; // guarda o atual
            continue;
        }

        // Se chegou aqui, é instrução
        if (!rotuloIsolado.empty())
        {
            linha = rotuloIsolado + " " + linha; // junta o útlimo rótulo com a instrucao
            rotuloIsolado.clear();
        }

        // 4. Trata a diretiva EQU
        // se o armazenamento tiver sucesso, segue, nao escreve a linha no arquivo
        if (storeEQUs(linha, tabelaEQU))
            continue;

        // 5. Trata IFs
        // se for IF, a flag de pular a proxima linha fica true, pq deve ser ignorada
        if (treatIF(linha, tabelaEQU, pularProximaLinha))
            continue;

        // 6. substituir EQU
        linha = substituirEQU(linha, tabelaEQU);

        // 7. Verificar se é secao TEXT ou DATA para manter DATA no fim
        if (linha == "SECTION DATA")
        {
            emData = true;
            continue;
        }

        if (linha == "SECTION TEXT")
        {
            emData = false;
            continue;
        }

        if (emData)
        {
            secaoData.push_back(linha);
        }
        else
        {
            secaoText.push_back(linha);
        }
    }

    // gambi: caso a ultima linha seja um rótulo isolado, salva ele
    if (!rotuloIsolado.empty())
    {
        if (emData)
        {
            secaoData.push_back(rotuloIsolado);
        }
        else
        {
            secaoText.push_back(rotuloIsolado);
        }
    }

    // Escreve toda a secao text primeiro
    arquivoPreGerado << "SECTION TEXT" << std::endl;

    for (const auto &l : secaoText)
    {
        arquivoPreGerado << l << std::endl;
    }

    // escreve toda a secao data dps
    arquivoPreGerado << "SECTION DATA" << std::endl;

    for (const auto &l : secaoData)
    {
        arquivoPreGerado << l << std::endl;
    }

    arquivo.close();
    arquivoPreGerado.close();

    return 0;
}