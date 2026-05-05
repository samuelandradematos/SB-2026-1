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
                   [](unsigned char c)
                   { return std::toupper(c); });
    return linha;
}

std::string removeComments(std::string linha)
{
    size_t position = linha.find(";");

    if (position != std::string::npos)
    {
        return linha.substr(0, position);
    }

    return linha;
}

// TODO: Essa func tbm precisa remover espacos entre LABEL e :
std::string removeUnnecessarySpaces(const std::string &str)
{
    std::string resultado;
    bool dentroDeEspaco = false;

    for (char c : str)
    {
        if (std::isspace((unsigned char)c))
        {
            // se já estamos em espaço, ignora
            if (!dentroDeEspaco)
            {
                resultado += ' '; // coloca só um espaço
                dentroDeEspaco = true;
            }
        }
        else
        {
            // remove espaços antes de ":"
            if (c == ':')
            {
                while (!resultado.empty() && resultado.back() == ' ')
                {
                    resultado.pop_back();
                }
            }
            resultado += c;
            dentroDeEspaco = false;
        }
    }

    // remove espaço do início
    if (!resultado.empty() && resultado.front() == ' ')
    {
        resultado.erase(0, 1);
    }

    // remove espaço do fim
    if (!resultado.empty() && resultado.back() == ' ')
    {
        resultado.pop_back();
    }

    return resultado;
}

bool storeEQUs(const std::string &linha, std::map<std::string, std::string> &tabelaEQU)
{
    std::vector<std::string> tokens;
    std::stringstream ss(linha);
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() >= 3 && tokens[1] == "EQU")
    {
        std::string nome = tokens[0];
        std::string valor = tokens[2];

        if (!nome.empty() && nome.back() == ':')
        {
            nome.pop_back();
        }

        tabelaEQU[nome] = valor;
        return true;
    }

    return false;
}

std::string substituirEQU(const std::string &linha,
                          const std::map<std::string, std::string> &tabelaEQU)
{

    std::stringstream ss(linha);
    std::string token;
    std::vector<std::string> tokens;

    // tokeniza
    while (ss >> token)
    {
        // se token está no map, substitui
        if (tabelaEQU.count(token))
        {
            token = tabelaEQU.at(token);
        }
        tokens.push_back(token);
    }

    std::string resultado;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        resultado += tokens[i];
        if (i != tokens.size() - 1)
        {
            resultado += " ";
        }
    }

    return resultado;
}

bool treatIF(const std::string &linha,
             const std::map<std::string, std::string> &tabelaEQU,
             bool &pularProximaLinha)
{

    std::stringstream ss(linha);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() >= 2 && tokens[0] == "IF")
    {
        std::string arg = tokens[1];
        int valor = 0;

        if (tabelaEQU.count(arg))
        {
            valor = std::stoi(tabelaEQU.at(arg));
        }
        else
        {
            valor = std::stoi(arg);
        }

        if (valor == 0)
        {
            pularProximaLinha = true;
        }

        // se retorna true é pq tinha IF
        return true;
    }

    return false;
}

void preProcessar(const std::string &nomeArquivo)
{
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
        return;
    }

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return;
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
}