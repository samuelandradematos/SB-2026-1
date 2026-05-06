#include <passagem_unica.hpp>

#define COLUNA_GRANDE 50
#define COLUNA_PEQUENA 20

TabelaDeSimbolos* TabelaDeSimbolos::instance = nullptr;
TabelaDeSimbolos::~TabelaDeSimbolos(){};

TabelaDeSimbolos& TabelaDeSimbolos::GetInstance() {
	if (instance == nullptr) {
		instance = new TabelaDeSimbolos();
	}
	return *instance;
}

TabelaDeSimbolos::TabelaDeSimbolos() {
	tabelaDePendencias = map<string, tuple<bool, int, list<string>>>();
}

void TabelaDeSimbolos::InsertSimbolo(string label, string posicaoMemoria, bool definido = false) {
	list<string> posicaoPendencia = list<string>();
	posicaoPendencia.emplace_front(posicaoMemoria);
	tabelaDePendencias.emplace(make_pair(label, make_tuple(definido, stoi(posicaoMemoria), posicaoPendencia)));
}

void TabelaDeSimbolos::UpdateLabelDefinido(string label, int endereco) {
	tabelaDePendencias.find(label)->second = make_tuple(true, endereco, get<2>(tabelaDePendencias.find(label)->second));
}

bool IsInstrucao(string instrucao) {
	if (Instrucoes::GetInstance().IsInstrucao(instrucao))
		return true;
	else 
		return false;
}

bool IsDiretiva(string diretiva) {
	if (Diretivas::GetInstance().IsDiretiva(diretiva))
		return true;
	else
		return false;

}

string GetOpcodeInstrucao(string instrucao) {
	unordered_map<string, tuple<int, string>>::const_iterator it = Instrucoes::GetInstance().GetInstrucao(instrucao);

	string opcode = get<1>(it->second);

	return opcode;
}

int GetQuantidadeOperandosInstrucao(string instrucao) {
	unordered_map<string, tuple<int, string>>::const_iterator it = Instrucoes::GetInstance().GetInstrucao(instrucao);

	int operandos = get<0>(it->second);

	return operandos;
}

int GetQuantidadeOperandosDiretiva(string diretiva) {
	unordered_map<string, int>::const_iterator it = Diretivas::GetInstance().GetDiretiva(diretiva);

	int operandos = it->second;

	return operandos;
}

bool TabelaDeSimbolos::LabelDefinido(string label) {
	map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
	if (it == tabelaDePendencias.end()) {
		return false;
	}
	else {
		return get<0>(it->second);
	}
}

bool TabelaDeSimbolos::LabelNaTabela(string label) {
	map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
	if (it == tabelaDePendencias.end())
		return false;
	else
		return true;
}

string TabelaDeSimbolos::GetEndereco(string label) {
	map<string, tuple<bool, int, list<string>>>::const_iterator it = tabelaDePendencias.find(label);
	return to_string(get<1>(it->second));
}

void TabelaDeSimbolos::UpdateListaPendencias(string label, string endereco) {
	get<2>(tabelaDePendencias.find(label)->second).emplace_front(endereco);
}

string ConverteIntEndereco(int posicao) {
	string endereco = to_string(posicao);
	if (endereco.length() < 2)
		endereco.insert(0, 2 - endereco.length(), '0');
	
	return endereco;
}

string CriaStringCentralizadaComPreenchimento(string texto, char preenchimento, int tamanho) {
	if (texto.length() % 2 != 0) {
		texto += preenchimento;
	}
	int tamanhoPreenchimento = (tamanho - texto.length()) / 2;
	if (texto.length() < tamanho) {
		texto.insert(0, tamanhoPreenchimento, preenchimento);
		texto.insert(texto.length(), tamanhoPreenchimento, preenchimento);
	}
	return texto;
}

string ResolveInstrucao(string opcode, string operando, int posicao) {
	string auxRetorno;
	string auxPosicao;
	
	if (Instrucoes::GetInstance().IsInstrucao(opcode)) {
		if (Instrucoes::GetInstance().IsCopy(opcode)) {
			string operando1 = operando.substr(0, operando.find(","));
			string operando2 = operando.substr(operando.find(",") + 1, operando.size());
			if (TabelaDeSimbolos::GetInstance().LabelDefinido(operando1)) {
				auxRetorno = TabelaDeSimbolos::GetInstance().GetEndereco(operando1);
			}
			else {
				if (TabelaDeSimbolos::GetInstance().LabelNaTabela(operando1)) {
					TabelaDeSimbolos::GetInstance().UpdateListaPendencias(operando1, ConverteIntEndereco(posicao + 1));
					auxRetorno = ConverteIntEndereco(posicao + 1);
				}
				else {
					TabelaDeSimbolos::GetInstance().InsertSimbolo(operando1, ConverteIntEndereco(posicao + 1));
					auxRetorno = ConverteIntEndereco(posicao + 1);
				}
			}

			if (TabelaDeSimbolos::GetInstance().LabelDefinido(operando2)) {
				auxRetorno += " " + TabelaDeSimbolos::GetInstance().GetEndereco(operando2);
			}
			else {
				if (TabelaDeSimbolos::GetInstance().LabelNaTabela(operando2)) {
					TabelaDeSimbolos::GetInstance().UpdateListaPendencias(operando2, ConverteIntEndereco(posicao + 2));
					auxRetorno += " " + ConverteIntEndereco(posicao + 2);
				}
				else {
					TabelaDeSimbolos::GetInstance().InsertSimbolo(operando2, ConverteIntEndereco(posicao + 2));
					auxRetorno += " " + ConverteIntEndereco(posicao + 2);
				}
			}
			
		}
		else if (Instrucoes::GetInstance().IsStop(opcode)) {
			auxRetorno = "";
		}
		else {
			// Alterar para considerar o + para o caso de LABEL+N
			if (TabelaDeSimbolos::GetInstance().LabelDefinido(operando)) {
				auxRetorno = TabelaDeSimbolos::GetInstance().GetEndereco(operando);
			}
			else {
				if (TabelaDeSimbolos::GetInstance().LabelNaTabela(operando)) {
					TabelaDeSimbolos::GetInstance().UpdateListaPendencias(operando, ConverteIntEndereco(posicao + 1));
					auxRetorno = ConverteIntEndereco(posicao + 1);
				}
				else {
					TabelaDeSimbolos::GetInstance().InsertSimbolo(operando, ConverteIntEndereco(posicao + 1));
					auxRetorno = ConverteIntEndereco(posicao + 1);
				}
			}
		}
			return auxRetorno;
	}
	else {
		if (Diretivas::GetInstance().IsDiretiva(opcode)) {
			if (opcode == "SPACE") {
				string auxOperando = "";
				if (operando != "") {
					for (int i = 0; i < stoi(operando); i++) {
						auxOperando += "00 ";
					}
					auxOperando = auxOperando.substr(0, auxOperando.size() - 1);
				}
				else {
					auxOperando = "00";
				}
				if (TabelaDeSimbolos::GetInstance().LabelNaTabela(operando)) {
					TabelaDeSimbolos::GetInstance().UpdateLabelDefinido(operando, posicao);
				}
				else {
					TabelaDeSimbolos::GetInstance().InsertSimbolo(operando, ConverteIntEndereco(posicao), true);
				}
				return auxOperando;
			}
			else {
				if (TabelaDeSimbolos::GetInstance().LabelNaTabela(operando)) {
					TabelaDeSimbolos::GetInstance().UpdateLabelDefinido(operando, posicao);
				}
				else {
					TabelaDeSimbolos::GetInstance().InsertSimbolo(operando, ConverteIntEndereco(posicao), true);
				}
				return operando;
			}
		}
		else
			return "";
	}
}

tuple<string,string,string> getConteudoLinha(string linha) {
	string label,opcode,operando;
	int posProxSubString;

	if (linha.find(": ") != string::npos) {
		label = linha.substr(0,linha.find(": "));
		posProxSubString = linha.find(": ") + strlen(": ");
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

void ResolvePendencias(list<tuple<string,string>>& codigo) {
	TabelaDeSimbolos& tabela = TabelaDeSimbolos::GetInstance();
	int memoria;
	cout << "Lista antes..." << endl;
	for (auto& it : codigo) {
		cout << "Opcode: " << get<0>(it) << " Memoria: " << get<1>(it) << endl;
	}
	
	for (auto& it: tabela.tabelaDePendencias) {
		for (auto& memoria : get<2>(tabela.tabelaDePendencias.find(it.first)->second)) {
			for (auto& instrucao : codigo) {
				if (Instrucoes::GetInstance().IsCopy(get<0>(instrucao))){
					string auxOperandos = get<1>(instrucao);
					string Operando1 = auxOperandos.substr(0, auxOperandos.find(" "));
					string Operando2 = auxOperandos.substr(auxOperandos.find(" ") + 1, auxOperandos.size());
					if (Operando1 == memoria) {
						Operando1 = ConverteIntEndereco(get<1>(tabela.tabelaDePendencias.find(it.first)->second));
						instrucao = make_tuple(get<0>(instrucao), Operando1 + " " + Operando2);
					}
					if (Operando2 == memoria) {
						Operando2 = ConverteIntEndereco(get<1>(tabela.tabelaDePendencias.find(it.first)->second));
						instrucao = make_tuple(get<0>(instrucao), Operando1 + " " + Operando2);
					}
				}
				else {
					if(get<1>(instrucao) == memoria) {
						instrucao = make_tuple(get<0>(instrucao), ConverteIntEndereco(get<1>(tabela.tabelaDePendencias.find(it.first)->second)));
					}
				}
			}
		}
	}

	cout << "Lista depois...." << endl;

	for (auto& it : codigo) {
		cout << "Opcode: " << get<0>(it) << " Memoria: " << get<1>(it) << endl;
	}

}

void CriaArquivoSaida(list<tuple<string,string>> codigo, string nomeArquivo, string extensao) {
	ofstream saida(nomeArquivo + "." + extensao);
	TabelaDeSimbolos& tabela = TabelaDeSimbolos::GetInstance();

	if (extensao == "obj") {
		for (auto& instrucao : codigo) {
			if (Instrucoes::GetInstance().IsInstrucao(get<0>(instrucao))) {
				if (Instrucoes::GetInstance().IsStop(get<0>(instrucao))) {
					saida << GetOpcodeInstrucao(get<0>(instrucao)) << " ";
					continue;
				}
				else {
				saida << GetOpcodeInstrucao(get<0>(instrucao)) << " " << get<1>(instrucao) << " ";
				}
			}
			else {
				saida << get<1>(instrucao) << " ";
			}
		}
		saida << endl;
	}

	if (extensao == "pen") {
		string auxOutput = "";
		for (auto& instrucao : codigo) {
			if (Instrucoes::GetInstance().IsInstrucao(get<0>(instrucao))) {
				if (Instrucoes::GetInstance().IsStop(get<0>(instrucao))) {
					auxOutput += GetOpcodeInstrucao(get<0>(instrucao)) + " ";
					continue;
				}
				else {
					auxOutput += GetOpcodeInstrucao(get<0>(instrucao)) + " " + get<1>(instrucao) + " ";
				}
			}
			else {
				auxOutput += get<1>(instrucao) + " ";
			}
		}
		auxOutput = auxOutput.substr(0, auxOutput.size() - 1);
		saida << auxOutput;

		saida << endl << endl << endl;

		saida << CriaStringCentralizadaComPreenchimento("Tabela de Símbolos", ' ', (COLUNA_PEQUENA * 3) + COLUNA_GRANDE) << endl;
		
		auxOutput = "";
		string label = "Label";
		string definido = "Definido";
		string enderecoReal = "Endereço Real";
		string pendencias = "Pendências";
		saida << CriaStringCentralizadaComPreenchimento(label,' ', COLUNA_PEQUENA) << "|";
		saida << CriaStringCentralizadaComPreenchimento(definido, ' ', COLUNA_PEQUENA) << "|";
		saida << CriaStringCentralizadaComPreenchimento(enderecoReal, ' ', COLUNA_PEQUENA) << "|";
		saida << CriaStringCentralizadaComPreenchimento(pendencias, ' ', COLUNA_GRANDE) << endl;
		string auxListaPendencias = "";
		for (auto& it: tabela.tabelaDePendencias) {
			if (regex_match(it.first, regex("^[A-Z]{1}[A-Z0-9_]*$"))) {
				auxOutput += 
							CriaStringCentralizadaComPreenchimento(it.first, ' ', COLUNA_PEQUENA) + "|" 
							+ CriaStringCentralizadaComPreenchimento((get<0>(it.second) ? "T" : "F"), ' ', COLUNA_PEQUENA) + "|"
							+ CriaStringCentralizadaComPreenchimento(ConverteIntEndereco(get<1>(it.second)), ' ', COLUNA_PEQUENA) + "|";
				auxListaPendencias += "[";
				if (get<2>(it.second).empty()) {
					auxListaPendencias += "]";
					auxOutput += CriaStringCentralizadaComPreenchimento(auxListaPendencias, ' ', COLUNA_GRANDE);
					saida << auxOutput << endl;
				}
				else {
					for (auto& k: get<2>(it.second))
						auxListaPendencias += k + ", ";
					
					auxListaPendencias = auxListaPendencias.substr(0, auxListaPendencias.size() - 2);
					auxListaPendencias += "]";
					auxOutput += CriaStringCentralizadaComPreenchimento(auxListaPendencias, ' ', COLUNA_GRANDE);
					saida << auxOutput << endl;
				}
				auxOutput = "";
				auxListaPendencias = "";
			}
		}
		saida << endl;
	}
}

void Parser(string ArquivoIn) {       
	fstream arquivo;;
	arquivo.open(ArquivoIn, ios::in);
	string auxArquivoOut = ArquivoIn.substr(0, ArquivoIn.find("."));
	list<tuple<string,string>> codigoMaquina = list<tuple<string,string>>();
	string linha;
	tuple<string,string,string> camposDaLinha = tuple<string,string,string>();
	int contador = 0;

	if (arquivo.is_open()) {
		while (getline(arquivo,linha)) {
			if (linha == "SECTION TEXT" || linha == "SECTION DATA"){
				continue;
			}
			camposDaLinha = getConteudoLinha(linha);
			// Verifica se tem label na linha
			if (!get<0>(camposDaLinha).empty()) {
				// Verifica se o label não está definido
				if (!TabelaDeSimbolos::GetInstance().LabelDefinido(get<0>(camposDaLinha))) {
					// Caso não esteja, verifica se ele está na tabela
					if (TabelaDeSimbolos::GetInstance().LabelNaTabela(get<0>(camposDaLinha))) {
						TabelaDeSimbolos::GetInstance().UpdateLabelDefinido(get<0>(camposDaLinha), contador);
						
						// Verifica se é COPY, para incrementar o contador em 3 caso seja
						if (Instrucoes::GetInstance().IsInstrucao(get<1>(camposDaLinha))) {
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
							if(Instrucoes::GetInstance().IsCopy(get<1>(camposDaLinha))) {
								contador += 3;
							}
							// Verifica se é STOP, para incrementar o contador em 1 caso seja
							else if (Instrucoes::GetInstance().IsStop(get<1>(camposDaLinha))) {
								contador++;
							}
							// Para todas as outras instrucoes, incrementa o contador em 2
							else {
								contador += 2;
							}
						}
						else {
							// Se não for uma instrucao, verifica se é uma diretiva. Caso seja, verifica se é SPACE, para tratar a quantidade de espaços a serem alocados.
							if (get<1>(camposDaLinha) == "SPACE") {
								string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);							
								codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
								contador++;
							}
							// Caso não seja SPACE é CONST e apenas insere a diretiva e seus operandos normalmente.
							else {
								string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);
								codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
								contador++;
							}
						}

					}
					else {
						// Como não está na tabela, insere na tabela de simbolos, com definido = true.
						TabelaDeSimbolos::GetInstance().InsertSimbolo(get<0>(camposDaLinha),ConverteIntEndereco(contador),true);
						// Verifica se o label é uma instrucao
						if (Instrucoes::GetInstance().IsInstrucao(get<1>(camposDaLinha))) {
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
							if(Instrucoes::GetInstance().IsCopy(get<1>(camposDaLinha))) {
								contador += 3;
							}
							// Verifica se é STOP, para incrementar o contador em 1 caso seja
							else if (Instrucoes::GetInstance().IsStop(get<1>(camposDaLinha))) {
								contador++;
							}
							// Para todas as outras instrucoes, incrementa o contador em 2
							else {
								contador += 2;
							}
						}
						else {
							// Se não for uma instrucao, verifica se é uma diretiva. Caso seja, verifica se é SPACE, para tratar a quantidade de espaços a serem alocados.
							if (get<1>(camposDaLinha) == "SPACE") {
								string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);							
								codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
								contador++;
							}
							// Caso não seja SPACE é CONST e apenas insere a diretiva e seus operandos normalmente.
							else {
								string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);
								codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
								contador++;
							}
						}
					}
				}
				else {
					
				}
			}
			// Caso não tenha label, verifica se é uma instrucao. Se for, resolve os operandos e insere na lista de codigo maquina.
			else {
				// Verifica se é uma instrucao, caso seja, resolve os operandos e insere na lista de codigo maquina.
				if (Instrucoes::GetInstance().IsInstrucao(get<1>(camposDaLinha))) {
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
					if(Instrucoes::GetInstance().IsCopy(get<1>(camposDaLinha))) {
						contador += 3;
					}
					// Verifica se é STOP, para incrementar o contador em 1 caso seja
					else if (Instrucoes::GetInstance().IsStop(get<1>(camposDaLinha))) {
						contador++;
					}
					// Para todas as outras instrucoes, incrementa o contador em 2
					else {
						contador += 2;
					}
				}
				// Caso não seja uma instrucao, é uma diretiva. Verifica se é SPACE, para tratar a quantidade de espaços a serem alocados.
				else {
					if (get<1>(camposDaLinha) == "SPACE") {
						string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);							
						codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
						contador++;
					}
					// Caso não seja SPACE é CONST e apenas insere a diretiva e seus operandos normalmente.
					else {
						string auxDiretiva = ResolveInstrucao(get<1>(camposDaLinha), get<2>(camposDaLinha), contador);
						codigoMaquina.emplace_back(make_tuple(get<1>(camposDaLinha), auxDiretiva));
						contador++;
					}
				}
			}
		}
	}

	CriaArquivoSaida(codigoMaquina, auxArquivoOut, "pen");
	ResolvePendencias(codigoMaquina);
	CriaArquivoSaida(codigoMaquina, auxArquivoOut, "obj");
}
 