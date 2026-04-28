#include <passagem_unica.hpp>

bool IsInstrucao(string instrucao) {
    unordered_map<string, tuple<int,string>>::const_iterator it = instrucoes.find(instrucao);
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
    unordered_map<string, tuple<int,string>>::const_iterator it = instrucoes.find(instrucao);

    string opcode = get<1>(it->second);

    return opcode;
}

int GetQuantidadeOperandosInstrucao(string instrucao) {
    unordered_map<string, tuple<int,string>>::const_iterator it = instrucoes.find(instrucao);

    int operandos = get<0>(it->second);

    return operandos;
}

int GetQuantidadeOperandosDiretiva(string diretiva) {
    unordered_map<string, int>::const_iterator it = diretivas.find(diretiva);

    int operandos = it->second;

    return operandos;
}

bool IsDefinido(string label) {
    unordered_map<string,tuple<bool,list<int>>>::const_iterator it = tabelaDePendencias.find(label);
    if (it == tabelaDePendencias.end()) {
        return false;
    }
    else {
        return get<0>(it->second);
    }
}

void InserePendencia(string label, int posicaoMemoria, bool definido) {
    list<int> posicaoPendencia;
    posicaoPendencia.emplace_front(posicaoMemoria);
    tabelaDePendencias.emplace(label, tuple(definido, posicaoPendencia));
}

void InserePosicaoEmPendencia(string label, int posicaoMemoria) {
    get<1>(tabelaDePendencias.find(label)->second).emplace_front(posicaoMemoria);
}

void PrintTabelaDePendencias() {
    cout << "Label  " << "      |       " << "  Definido    " << "      |       " << "Pendencias" << endl;
    for (auto& it: tabelaDePendencias) {
        cout << it.first << "           |           " << (get<0>(it.second) ? "T" : "F") << "         |           ";
        for (auto& k: get<1>(it.second))
            cout << k;
        cout << endl;
    }
}