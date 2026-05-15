#include <simualdor.hpp>
#include <passagem_unica.hpp>

Opcodes* Opcodes::instance = nullptr;
Simulador* Simulador::instance = nullptr;

Opcodes& Opcodes::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Opcodes();
	}
	return *instance;
}

Opcodes::Opcodes()
{
	opcode["01"] = "ADD";
	opcode["02"] = "SUB";
	opcode["03"] = "MUL";
	opcode["04"] = "DIV";
	opcode["05"] = "JMP";
	opcode["06"] = "JMPN";
	opcode["07"] = "JMPP";
	opcode["08"] = "JMPZ";
	opcode["09"] = "COPY";
	opcode["10"] = "LOAD";
	opcode["11"] = "STORE";
	opcode["12"] = "INPUT";
	opcode["13"] = "OUTPUT";
	opcode["14"] = "STOP";
}

Opcodes::~Opcodes()
{
	delete instance;
}

bool Opcodes::IsOpcode(string instrucao)
{
	if (opcode.find(instrucao) != opcode.end())
		return true;
	else
		return false;
}

string Opcodes::GetOpcode(string instrucao) {
	if (opcode.find(instrucao) != opcode.end())
		return opcode.find(instrucao)->second;
	else
		return "";
}

Simulador& Simulador::GetInstance() {
	if (instance == nullptr){
		instance = new Simulador();
	}
	return *instance;
}

Simulador::Simulador(){
	acumulador = 0;
	programCounter = "";
	for (int i = 0; i < TAM_ENDERECO; i++) {
		programCounter += "0";
	}
	memoria = unordered_map<string,string>();
	programa = map<string,tuple<string,string>>();
	tabelaOperacaoOperandos = list<tuple<string,string,string>>();
}

Simulador::~Simulador()
{
	delete instance;
}

string Simulador::GetConteudoMemoria(string endereco)
{
	if (memoria.find(endereco) != memoria.end())
	{
		return memoria.find(endereco)->second;
	}
	else 
		return "";
}

void Simulador::SetConteudoMemoria(string endereco, string valor) {
	if (memoria.find(endereco) != memoria.end())
		memoria.find(endereco)->second = valor;
}



void Simulador::Start(string codigo) {
	string opcode, operando1, operando2, aux;
	int posicao = 0;
	cout << "Lendo codigo..." << endl;
	while (codigo.find(" ") != string::npos) {
		cout << "Codigo inicial: " << codigo << endl;
		opcode = codigo.substr(0, codigo.find(" "));
		codigo.erase(0, codigo.find(" ") + 1);
		if (opcode == "14") {
			posicao++;
			Simulador::GetInstance().programa.emplace(ConverteIntEndereco(posicao), make_tuple(opcode,""));
			cout << "Construcao da memoria" << endl;
			while (!codigo.empty()) {
				if (codigo != "00") {
					aux = codigo.substr(0,codigo.find(" "));
					cout << "Posicaçao: " << ConverteIntEndereco(posicao) << " | Valor: " << aux << endl;
					memoria.emplace(ConverteIntEndereco(posicao),aux);
					codigo.erase(0, codigo.find(" ") + 1);
					posicao++;
				}
				else {
					memoria.emplace(ConverteIntEndereco(posicao),codigo);
					cout << "Posicaçao: " << ConverteIntEndereco(posicao) << " | Valor: " << aux << endl;
					posicao++;
					codigo = "";
				}
			}
		}
		else if (opcode == "09") {
			operando1 = codigo.substr(0, codigo.find(" "));
			codigo.erase(0, codigo.find(" ") + 1);
			operando2 = codigo.substr(0, codigo.find(" "));
			codigo.erase(0, codigo.find(" ") + 1);
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode, operando1, operando2));
			Simulador::GetInstance().programa.emplace(ConverteIntEndereco(posicao),make_tuple(opcode,operando1 + " " + operando2));
			posicao += 3;
		}
		else {
			operando1 = codigo.substr(0, codigo.find(" "));
			codigo.erase(0, codigo.find(" ") + 1);
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode,operando1,""));
			Simulador::GetInstance().programa.emplace(ConverteIntEndereco(posicao),make_tuple(opcode,operando1));
			posicao += 2;
		}
		cout << "Codigo restante: " << codigo << endl;
	}
}

void Simulador::Run(string arquivoOriginal) {
	fstream arquivo;
	arquivo.open(arquivoOriginal);
	string linha, auxRun, auxOpcode, auxOperando1, auxOperando2, auxCopy, auxInput, auxOutput;
	int auxOpcodeNum, auxOperacao;
	unordered_map<string,string>::iterator it = memoria.begin();
	list<string> operandos = list<string>();
	getline(arquivo,linha);
	if (!linha.empty()) {
		Simulador::GetInstance().Start(linha);
	}
	
	cout << "---------------------------------------------------" << endl;
	cout << "Conteudo de programa" << endl;
	for (auto& it: programa) {
		cout << "Posição memória: " << it.first << " | Operação: " << get<0>(it.second) << " | Memória: " << get<1>(it.second) << endl;
	}
	cout << "---------------------------------------------------" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Conteudo da memoria" << endl;
	for (auto& it: memoria) {
		cout << "Posicao memoria: " << it.first << " | Conteudo: " << it.second << endl;
	}
	cout << "---------------------------------------------------" << endl;

	auxRun = linha;
	while (programCounter != "STOP") {
		cout << "PC: " << programCounter << endl;
		cout << "Opcode: " << get<0>(programa.find(programCounter)->second) << endl;
		auxOpcodeNum = stoi(get<0>(programa.find(programCounter)->second));
		cout << "Opcode NUM: " << auxOpcodeNum << endl;


		switch (auxOpcodeNum) {
			case 1:
				// ADD
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				cout << "ADD|  Valor do acumulador: " << acumulador << " | Valor do conteudo de memória: " << auxOperacao << endl;
				acumulador = acumulador + auxOperacao;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 2:
				// SUB
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				cout << "auxOperando1: " << auxOperando1 << endl;
				auxOperacao = stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				cout << "SUB|  Valor do acumulador: " << acumulador << " | Valor do conteudo de memória: " << auxOperacao << endl;
				acumulador = acumulador -auxOperacao;
				cout << "Inc PC" << endl;				
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				cout << "Apos inc PC" << endl;
				break;
			case 3:
				// MUL
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				cout << "MUL|  Valor do acumulador: " << acumulador << " | Valor do conteudo de memória: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
				acumulador = acumulador * stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 4:
				// DIV
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				cout << "DIV|  Valor do acumulador: " << acumulador << " | Valor do conteudo de memória: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
				acumulador = acumulador / stoi(GetConteudoMemoria(ConverteStringEndereco(auxOperando1)));
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 5:
				// JMP
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				cout << "JMP|  Para o PC: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
				programCounter = ConverteStringEndereco(auxOperando1);
			case 6:
				// JMPN
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador < 0) {
					cout << "JMPN|  Para o PC: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
					programCounter = ConverteStringEndereco(auxOperando1);
				}
				else {
					programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				}
				break;
			case 7:
				// JMPP
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador > 0) {
					cout << "JMPP|  Para o PC: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
					programCounter = ConverteStringEndereco(auxOperando1);
				}
				else {
					programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				}
				break;
			case 8:
				// JMPZ
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador == 0) {
					cout << "JMPZ|  Para o PC: " << GetConteudoMemoria(ConverteStringEndereco(auxOperando1)) << endl;
					programCounter = ConverteStringEndereco(auxOperando1);
				}
				else {
					programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				}
				break;
			case 9:
				// COPY
				auxCopy = get<1>(programa.find(programCounter)->second);
				auxOperando1 = auxCopy.substr(0, auxCopy.find(" "));
				auxOperando2 = auxCopy.substr(auxCopy.find(" ") + 1 , auxCopy.size());
				auxCopy = GetConteudoMemoria(ConverteStringEndereco(auxOperando1));
				cout << "COPY do endereço: " << auxOperando1 << " para o endereço: " << auxOperando2 << " o valor: " << auxCopy << endl;
				it = memoria.find(ConverteStringEndereco(auxOperando2));
				it->second = auxCopy;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 3);
				break;
			case 10:
				// LOAD
				auxOperando1 = GetConteudoMemoria(get<1>(programa.find(programCounter)->second));
				acumulador = stoi(auxOperando1);
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 11:
				// STORE
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				cout << "STORE em: " << auxOperando1 << endl;
				it = memoria.find(ConverteStringEndereco(auxOperando1));
				it->second = acumulador;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 12:
				// INPUT
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				cin >> auxInput;
				cout << "INPUT valor digitado: " << auxInput << endl;
				SetConteudoMemoria(auxOperando1,auxInput);
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 13:
				// OUTPUT
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOutput = GetConteudoMemoria(auxOperando1);
				cout << auxOutput;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 14:
				// STOP
				programCounter = "STOP";
				break;
			default:
				programCounter = "STOP";
				break;
		}
	}
}