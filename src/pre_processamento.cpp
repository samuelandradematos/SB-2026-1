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
            if (c == ':') {
                while (!resultado.empty() && resultado.back() == ' ') {
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

bool treatIF(const std::string& linha,
              const std::map<std::string, std::string>& tabelaEQU,
              bool& pularProximaLinha) {
    
    std::stringstream ss(linha);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 2 && tokens[0] == "IF") {
        std::string arg = tokens[1];
        int valor = 0;

        if (tabelaEQU.count(arg)) {
            valor = std::stoi(tabelaEQU.at(arg));
        } else {
            valor = std::stoi(arg);
        }

        if (valor == 0) {
            pularProximaLinha = true;
        }

        // se retorna true é pq tinha IF
        return true;
    }

    return false;
}
