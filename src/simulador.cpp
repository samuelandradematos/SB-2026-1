#include <simualdor.hpp>

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
	memoria = unordered_map<string,string>();
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

void Simulador::SetConteudoMemoria(string endereco, int valor) {
	if (memoria.find(endereco) != memoria.end())
		memoria.find(endereco)->second = to_string(valor);
}



void Simulador::Start(string codigo) {
	string opcode, operando1, operando2, aux;
	int posicao = 0;
	cout << "Codigo antes de processar: " << codigo << endl;
	while (codigo.find(" ") != string::npos) {
		cout << "Codigo não vazio...." << endl;
		opcode = codigo.substr(0, codigo.find(" "));
		cout << "Opcode: " << opcode << " | ";
		codigo.erase(0, codigo.find(" ") + 1);
		if (opcode == "14") {
			posicao++;
			cout << "Section data: " << endl;
			while (codigo.find(" ") != string::npos) {
				aux = codigo.substr(0,codigo.find(" "));
				memoria.emplace(ConverteIntEndereco(posicao),aux);
				cout << "Posição memoria: " << ConverteIntEndereco(posicao) << " | Conteudo: " << aux << endl;
				codigo.erase(0, codigo.find(" ") + 1);
				posicao++;
			}
		}
		else if (opcode == "09") {
			operando1 = codigo.substr(0, codigo.find(" "));
			codigo.erase(0, codigo.find(" ") + 1);
			operando2 = codigo.substr(0, codigo.find(" "));
			cout << "Endereco memoria: " << operando1 << " e " << operando2 << endl;
			codigo.erase(0, codigo.find(" ") + 1);
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode, operando1, operando2));
			posicao += 3;
		}
		else {
			operando1 = codigo.substr(0, codigo.find(" "));
			cout << "Endereco memoria: " << operando1 << endl;
			codigo.erase(0, codigo.find(" ") + 1);
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode,operando1,""));
			posicao += 2;
		}
	}
}

void Simulador::Run(string arquivoOriginal) {
	fstream arquivo;
	arquivo.open(arquivoOriginal);
	string linha, auxRun, auxOpcode, auxOperando1, auxOperando2;
	int auxOpcodeNum;
	list<string> operandos = list<string>();
	getline(arquivo,linha);
	if (!linha.empty()) {
		Simulador::GetInstance().Start(linha);
	}

	for (auto& it: tabelaOperacaoOperandos) {
		if (get<0>(it) != "09") {
			cout << "Operação: " << Opcodes::GetInstance().GetOpcode(get<0>(it)) << " | Memória: " << get<1>(it) << endl;
		}
		else {
			cout << "Operação: " << Opcodes::GetInstance().GetOpcode(get<0>(it)) << " | Memória: " << get<1>(it) << " e " << get<2>(it) << endl;
		}
	}
	auxRun = linha;
	while (Simulador::GetInstance().programCounter != "STOP") {
		auxOpcodeNum = stoi(auxRun.substr(0, auxRun.find(" ")));
		auxOpcode = Opcodes::GetInstance().GetOpcode(auxRun.substr(0, auxRun.find(" ")));
		auxRun.erase(0, auxRun.find(" ") + 1);

		switch (auxOpcodeNum) {
			case 1:
				// ADD
				auxOperando1 = auxRun.substr(0, auxRun.find(" "));
				Simulador::GetInstance().acumulador = Simulador::GetInstance().acumulador + stoi(Simulador::GetInstance().GetConteudoMemoria(auxOperando1));
				programCounter = to_string(stoi(programCounter) + 2);
				auxRun.erase(0, auxRun.find(" ") + 1);
				break;
			case 2:
				// SUB
				auxOperando1 = auxRun.substr(0, auxRun.find(" "));
				Simulador::GetInstance().acumulador = Simulador::GetInstance().acumulador - stoi(Simulador::GetInstance().GetConteudoMemoria(auxOperando1));
				programCounter = to_string(stoi(programCounter) + 2);
				auxRun.erase(0, auxRun.find(" ") + 1);
				break;
			case 3:
				// MUL
				auxOperando1 = auxRun.substr(0, auxRun.find(" "));
				Simulador::GetInstance().acumulador = Simulador::GetInstance().acumulador * stoi(Simulador::GetInstance().GetConteudoMemoria(auxOperando1));
				programCounter = to_string(stoi(programCounter) + 2);
				auxRun.erase(0, auxRun.find(" ") + 1);
				break;
			case 4:
				// DIV
				auxOperando1 = auxRun.substr(0, auxRun.find(" "));
				Simulador::GetInstance().acumulador = Simulador::GetInstance().acumulador / stoi(Simulador::GetInstance().GetConteudoMemoria(auxOperando1));
				programCounter = to_string(stoi(programCounter) + 2);
				auxRun.erase(0, auxRun.find(" ") + 1);
				break;
			case 5:
				// JMP
				
				break;
			case 6:
				// JMPN
				break;
			case 7:
				// JMPP
				break;
			case 8:
				// JMPZ
				break;
			case 9:
				// COPY
				break;
			case 10:
				// LOAD
				break;
			case 11:
				// STORE
				break;
			case 12:
				// INPUT
				break;
			case 13:
				// OUTPUT
				break;
			case 14:
				// STOP
				break;
			default:
				break;
		}
	}
}