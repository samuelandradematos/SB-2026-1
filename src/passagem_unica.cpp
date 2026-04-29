#include <passagem_unica.hpp>

bool IsInstrucao(string instrucao) {
    unordered_map<string,  tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);
    if (it != instrucoes.end())
        return true;
    else 
        return false;
}

bool IsDiretiva(string diretiva) {
    if (diretivas.find(diretiva) != diretivas.end()) 
        return true;
    else
        return false;

}

string GetOpcodeInstrucao(string instrucao) {
    unordered_map<string,  tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);

    string opcode = get<1>(it->second);

    return opcode;
}

int GetQuantidadeOperandosInstrucao(string instrucao) {
    unordered_map<string,  tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);

    int operandos = get<0>(it->second);

    return operandos;
}

int GetQuantidadeOperandosDiretiva(string diretiva) {
    unordered_map<string,  int>::const_iterator it = diretivas.find(diretiva);

    int operandos = it->second;

    return operandos;
}

bool LabelDefinido(string label) {
    unordered_map<string, tuple<bool, int, list<int>>>::const_iterator it = tabelaDePendencias.find(label);
    if (it == tabelaDePendencias.end()) {
        return false;
    }
    else {
        return get<0>(it->second);
    }
}

void InsertPendencia(string label, int posicaoMemoria, bool definido) {
    list<int> posicaoPendencia;
    posicaoPendencia.emplace_front(posicaoMemoria);
    tabelaDePendencias.emplace(label, tuple(definido, posicaoMemoria, posicaoPendencia));
}

void PrintTabelaDePendencias() {
    cout << "Label  " << "      |       " << "  Definido    " << "      |       " << "  Endereço Real   " << "      |       " << "  Pendencias" << endl;
    string auxOutput = "";
    for (auto& it: tabelaDePendencias) {
        auxOutput += it.first + "           |           " + (get<0>(it.second) ? "T" : "F") + "         |           " + to_string(get<1>(it.second)) + "         |           ";
        for (auto& k: get<2>(it.second))
            auxOutput += "[" + to_string(k) + ", ";
        auxOutput.substr(0,  auxOutput.size() - 2);
        auxOutput += "]";
        cout << auxOutput << endl;
    }
}

void InsertSimbolo(string label, bool definido, int endereco) {
    list<string> auxListEndereco;
    auxListEndereco.emplace_front(endereco);
    tabelaDePendencias.emplace(label,  tuple(definido,  endereco,  auxListEndereco));
}

int GetEndereco(string label) {
    unordered_map<string, tuple<bool, int, list<int>>>::const_iterator it = tabelaDePendencias.find(label);
    return get<1>(it->second);
}

void UpdateListaPendencias(string label, int endereco) {
    get<2>(tabelaDePendencias.find(label)->second).emplace_front(endereco);
}

tuple<string, int> resolveInstrucao(string opcode, string operando, int posicao) {
    if (LabelDefinido(operando)) {
        return tuple(opcode, GetEndereco(operando));
    }
    else {
        unordered_map<string, tuple<bool, int, list<int>>>::const_iterator it = tabelaDePendencias.find(operando);
        if (it != tabelaDePendencias.end()) {
            UpdateListaPendencias(operando, posicao + 1);
        }

    }
    
}

void Parser(string path_arquivo) {
    fstream arquivo;
    arquivo.open(path_arquivo);
    
    // TODO:
    //      lógica de leitura de cada linha(icluir contador de posição)
    //      para cada linha - verificar se contém definição de label e se tiver inserir na tabelaDePendencias como label definido
    //                        verificar se o operando já está na tabelaDePendencias:
    //                                                                              se estiver e já foi definido, retornar endereço
    //                                                                              se estiver e não foi definido, adicinar posição na lista de pendências
    //                                                                              se não estiver, adicionar na tabelaDePendencias
}