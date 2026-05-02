#include <passagem_unica.hpp>

bool IsInstrucao(string instrucao) {
    unordered_map<string, tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);
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
    unordered_map<string, tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);

    string opcode = get<1>(it->second);

    return opcode;
}

int GetQuantidadeOperandosInstrucao(string instrucao) {
    unordered_map<string, tuple<int, string>>::const_iterator it = instrucoes.find(instrucao);

    int operandos = get<0>(it->second);

    return operandos;
}

int GetQuantidadeOperandosDiretiva(string diretiva) {
    unordered_map<string, int>::const_iterator it = diretivas.find(diretiva);

    int operandos = it->second;

    return operandos;
}

bool LabelDefinido(string label) {
    unordered_map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
    if (it == tabelaDePendencias.end()) {
        return false;
    }
    else {
        return get<0>(it->second);
    }
}

bool LabelNaTabela(string label) {
    unordered_map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
    if (it == tabelaDePendencias.end())
        return false;
    else
        return true;
}

void InsertPendencia(string label, string posicaoMemoria, bool definido = false) {
    list<string> posicaoPendencia;
    posicaoPendencia.emplace_front(posicaoMemoria);
    tabelaDePendencias.emplace(make_pair(label, make_tuple(definido, posicaoMemoria, posicaoPendencia)));
}

string GetEndereco(string label) {
    unordered_map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
    return to_string(get<1>(it->second));
}

void UpdateListaPendencias(string label, string endereco) {
    get<2>(tabelaDePendencias.find(label)->second).emplace_front(endereco);
}

string ConverteIntEndereco(int posicao) {
    string endereco = to_string(posicao);
    if (endereco.length() < 2)
        endereco.insert(0, 2 - endereco.length(), '0');
    
    return endereco;
}

string ResolveInstrucao(string opcode, string operando, int posicao) {
    string auxRetorno;
    string auxPosicao;
    
    if (opcode == "COPY") {
        string operando1 = operando.substr(0, operando.find(","));
        string operando2 = operando.substr(operando.find(",") + 1, operando.size());
        if (LabelDefinido(operando1)) {
            auxRetorno = GetEndereco(operando1);
        }
        else {
            if (LabelNaTabela(operando1)) {
                UpdateListaPendencias(operando1, ConverteIntEndereco(posicao + 1));
                auxRetorno = ConverteIntEndereco(posicao + 1);
            }
            else {
                InsertPendencia(operando1, ConverteIntEndereco(posicao + 1));
                auxRetorno = ConverteIntEndereco(posicao + 1);
            }
        }

        if (LabelDefinido(operando2)) {
            auxRetorno += " " + GetEndereco(operando2);
        }
        else {
            if (LabelNaTabela(operando2)) {
                UpdateListaPendencias(operando2, ConverteIntEndereco(posicao + 2));
                auxRetorno += " " + ConverteIntEndereco(posicao + 2);
            }
            else {
                InsertPendencia(operando2, ConverteIntEndereco(posicao + 2));
                auxRetorno += " " + ConverteIntEndereco(posicao + 2);
            }
        }
        
    }
    else {
        if (LabelDefinido(operando)) {
            auxRetorno = GetEndereco(operando);
        }
        else {
            if (LabelNaTabela(operando)) {
                UpdateListaPendencias(operando, ConverteIntEndereco(posicao + 1));
                auxRetorno = ConverteIntEndereco(posicao + 1);
            }
            else {
                InsertPendencia(operando,ConverteIntEndereco(posicao + 1));
                auxRetorno = ConverteIntEndereco(posicao + 1);
            }
        }
    }
        return auxRetorno;
}

tuple<string,string,string> getConteudoLinha(string linha) {
    string label,opcode,operando;
    int posProxSubString;

    if (linha.find(":") != string::npos) {
        label = linha.substr(0,linha.find(":"));
        posProxSubString = linha.find(":") + strlen(":");
        linha = linha.substr(posProxSubString,linha.size());
    }

    if (linha.find(" ") != string::npos) {
        opcode = linha.substr(0, linha.find(" "));
        operando = linha.substr(linha.find(" ") + 1, linha.size());
    }
    else {
        opcode = linha;
    }

    return make_tuple(label,opcode,operando);
}

void UpdateLabelDefinido(string label, int posicao) {
    unordered_map<string, tuple<bool, int, list<string>>>::iterator it = tabelaDePendencias.find(label);
    it->second = make_tuple(true, posicao, get<2>(it->second));
}

void CriaArquivoSaida(list<tuple<string,string>> codigo, string nomeArquivo, string extensao) {
    ofstream saida(nomeArquivo + "." + extensao);

    if (extensao == "obj") {
        for (auto& instrucao : codigo) {
            saida << GetOpcodeInstrucao(get<0>(instrucao)) << " " << get<1>(instrucao);
        }
    }

    if (extensao == "pen") {
        for (auto& instrucao : codigo) {
            saida << GetOpcodeInstrucao(get<0>(instrucao)) << " " << get<1>(instrucao);
        }

        saida << endl;

        saida << "Label  " << "      |       " << "  Definido    " << "      |       " << "  Endereço Real   " << "      |       " << "  Pendencias" << endl;
        string auxOutput = "";
        for (auto& it: tabelaDePendencias) {
            auxOutput += it.first + "           |           " + (get<0>(it.second) ? "T" : "F") + "         |           " + to_string(get<1>(it.second)) + "         |           ";
            auxOutput += "[";
            for (auto& k: get<2>(it.second))
                auxOutput += k + ", ";
            auxOutput.substr(0,  auxOutput.size() - 2);
            auxOutput += "]";
            saida << auxOutput << endl;
        }
    }
}

void Parser(string ArquivoIn) {       
    fstream arquivo;
    arquivo.open(ArquivoIn);
    list<tuple<string,string>> codigoMaquina;
    string linha;
    string opcodeCopy = "09";
    tuple<string,string,string> camposDaLinha;
    int contador = 0;

    while (!arquivo.eof()) {
        getline(arquivo,linha);
        camposDaLinha = getConteudoLinha(linha);
        
        // Verifica se tem label na linha
        if (!get<0>(camposDaLinha).empty()) {
            // Verifica se o label não está definido
            if (!LabelDefinido(get<0>(camposDaLinha))) {
                // Caso não esteja, verifica se ele está na tabela
                if (LabelNaTabela(get<0>(camposDaLinha))) {
                    UpdateLabelDefinido(get<0>(camposDaLinha), contador);
                }
                else {
                    // Como não está na tabela, insere na tabela de simbolos
                    InsertPendencia(get<0>(camposDaLinha),ConverteIntEndereco(contador),true);
                    // Verifica se o label é uma instrucao
                    if (IsInstrucao(get<1>(camposDaLinha))) {
                        codigoMaquina.emplace_back(
                                                    make_tuple(
                                                                get<1>(camposDaLinha),
                                                                ResolveInstrucao(
                                                                                    get<1>(camposDaLinha),
                                                                                    get<2>(camposDaLinha),
                                                                                    contador
                                                                                )
                                                            )
                                                );
                        // Verifica se é COPY, para incrementar o contador em 3 caso seja
                        if(get<1>(camposDaLinha) == "COPY") {
                            contador += 3;
                        }
                        // Verifica se é STOP, para incrementar o contador em 1 caso seja
                        else if (get<1>(camposDaLinha) == "STOP") {
                            contador += 1;
                        }
                        // Para todas as outras instrucoes, incrementa o contador em 2
                        else {
                            contador += 2;
                        }
                    }
                    else {
                        if (get<1>(camposDaLinha) == "SPACE") {
                            
                            if (get<1>(camposDaLinha).find('+') != string::npos){
                                string auxSpace = get<1>(camposDaLinha);
                                auxSpace.substr(auxSpace.find('+') + 1, auxSpace.size());
                                for (int i = 0; i < stoi(auxSpace); i++) {
                                    string auxInstrucao = get<1>(camposDaLinha);
                                    codigoMaquina.emplace_back(make_tuple(auxInstrucao,"00"));
                                    contador++;
                                }
                            }
                            else {
                                string auxInstrucao = get<1>(camposDaLinha);
                                codigoMaquina.emplace_back(make_tuple(auxInstrucao,"00"));
                                contador++;
                            }
                        }
                        else {
                            string auxInstrucao = get<1>(camposDaLinha);
                            string auxOperandos = get<2>(camposDaLinha);
                            codigoMaquina.emplace_back(make_tuple(auxInstrucao, auxOperandos));
                            contador++;
                        }
                    }
                }
            }
        }
        else {
            if (IsInstrucao(get<1>(camposDaLinha))) {
                codigoMaquina.emplace_back(
                                            make_tuple(
                                                        
                                                        get<1>(camposDaLinha),
                                                        ResolveInstrucao(
                                                                            get<1>(camposDaLinha),
                                                                            get<2>(camposDaLinha),
                                                                            contador
                                                                        )
                                                    )
                                        );
                // Verifica se é COPY, para incrementar o contador em 3 caso seja
                if(get<1>(camposDaLinha) == "COPY") {
                    contador += 3;
                }
                // Verifica se é STOP, para incrementar o contador em 1 caso seja
                else if (get<1>(camposDaLinha) == "STOP") {
                    contador += 1;
                }
                // Para todas as outras instrucoes, incrementa o contador em 2
                else {
                    contador += 2;
                }
            }
            else {
                if (get<1>(camposDaLinha) == "SPACE") {
                    
                    if (get<2>(camposDaLinha).find('+') != string::npos){
                        string auxSpace = get<2>(camposDaLinha);
                        auxSpace.substr(auxSpace.find('+') + 1, auxSpace.size());
                        for (int i = 0; i < stoi(auxSpace); i++) {
                            codigoMaquina.emplace_back(make_tuple((get<1>(camposDaLinha),"00")));
                            contador++;
                        }
                    }
                    else {
                        codigoMaquina.emplace_back(make_tuple((get<1>(camposDaLinha),"00")));
                        contador++;
                    }
                }
                else {
                    codigoMaquina.emplace_back(make_tuple((get<1>(camposDaLinha),get<2>(camposDaLinha))));
                    contador++;
                }
            }
        }
    }

    for (auto& pendencia : tabelaDePendencias) {
        int enderecoFinal = get<1>(pendencia.second);
        list<string>& listaPendencias = get<2>(pendencia.second);
        
        // For each pending address that needs to be replaced
        for (const string& enderecoPendente : listaPendencias) {
            string enderecoFinal_str = ConverteIntEndereco(enderecoFinal);
            
            // Iterate through codigoMaquina and replace
            for (auto& instrucao : codigoMaquina) {
                string& operandos = get<1>(instrucao);
                
                if (get<0>(instrucao) == "COPY") {
                    string operando1 = operandos.substr(0, operandos.find(" "));
                    string operando2 = operandos.substr(operandos.find(" ") + 1);
                    if (operando1 == enderecoPendente) {
                        operando1 = enderecoFinal_str;
                    }
                    if (operando2 == enderecoPendente) {
                        operando2 = enderecoFinal_str;
                    }
                    operandos = operando1 + " " + operando2;
                }
                else {
                    if (operandos == enderecoPendente) {
                        operandos = enderecoFinal_str;
                    }
                }
            }
        }
    }

    CriaArquivoSaida(codigoMaquina, ArquivoIn, "obj");

    CriaArquivoSaida(codigoMaquina, ArquivoIn, "pen");

}
 