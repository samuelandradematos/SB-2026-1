#include <simualdor.hpp>

Opcodes* Opcodes::instance = nullptr;

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

Simulador& Simulador::GetInstance() {
	if (instance == nullptr){
		instance = new Simulador();
	}
	return *instance;
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
}

void Simulador::SetConteudoMemoria(string endereco, int valor) {
	if (memoria.find(endereco) != memoria.end())
		memoria.find(endereco)->second = valor;
}



void Simulador::Start(string codigo) {
	string opcode, operando1, operando2, aux;
	int posicao = 0;
	while (codigo.find(" ") != string::npos) {
		opcode = codigo.substr(0, codigo.find(" "));
		codigo = codigo.substr(codigo.find(" ") + 1, codigo.size());
		if (opcode == "14") {
			posicao++;
			while (codigo.find(" ") != string::npos) {
				aux = codigo.substr(0,codigo.find(" "));
				memoria.emplace(ConverteIntEndereco(posicao),aux);
				codigo.substr(codigo.find(" ") + 1, codigo.size());
				posicao++;
			}
		}
		else if (opcode == "09") {
			operando1 = codigo.substr(0, codigo.find(" "));
			codigo.substr(codigo.find(" ") + 1, codigo.size());
			operando2 = codigo.substr(0, codigo.find(" "));
			codigo.substr(codigo.find(" ") + 1, codigo.size());
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode, operando1, operando2));
			posicao += 3;
		}
		else {
			operando1 = codigo.substr(0, codigo.find(" "));
			codigo.substr(codigo.find(" ") + 1, codigo.size());
			tabelaOperacaoOperandos.emplace_back(make_tuple(opcode,operando1));
			posicao += 2;
		}
	}
}