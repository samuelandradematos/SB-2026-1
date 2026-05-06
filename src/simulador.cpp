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

list<tuple<string, string>> GetListOpcodeOperandos(string codigo) {
	list<tuple<string, string>> codigoMaquina;
	string auxCodigo = codigo;
	string auxOpcode, auxOperando1, auxOperando2;

	//Exemplo de loop
	// 12	01	10	03	04	05	11	07	03	09	11	11	10	13	02	15	11	17	13	19	09	21 22	10	24	07	4	14	2	00	00	00	
	// 0:	auxOpcode




	while (auxCodigo.find("	") != string::npos) {
		auxOpcode = auxCodigo.substr(0, auxCodigo.find("	"));
		if (Opcodes::GetInstance().IsOpcode(auxOpcode)) {
			auxCodigo = auxCodigo.substr(auxCodigo.find("	") + 1, auxCodigo.size());
			if (auxOpcode == "09") {
				auxOperando1 = auxCodigo.substr(0,auxCodigo.find("	"));
				auxCodigo = auxCodigo.substr(auxCodigo.find("	") + 1, auxCodigo.size());
				auxOperando2 = auxCodigo.substr(0,auxCodigo.find("	"));


			}
		}
			
		
	}
	return codigoMaquina;
}

string GetConteudoMemoria(string endereco, list<tuple<string, string>> codigo) {
	for (const auto& it : codigo) {
		if (get<1>(it) == endereco) {
			return get<0>(it);
		}
	}
	return "00";
}

string Opcodes::GetOpcode(string instrucao)
{
	return opcode[instrucao];
}

void Run(string arquivo) {
	// fstream arquivoEntrada;
	// arquivoEntrada.open(arquivo, ios::in);
	// string linha;
	// list<tuple<string, string>> operacaoOperando = list<tuple<string, string>>();
	// string memoria;
	// if (arquivoEntrada.is_open()) {
	// while (getline(arquivoEntrada, linha)) {
	// 		if (!linha.empty()) {
	// 			operacaoOperando = GetListOpcodeOperandos(linha);
	// 		}
	// 	}
	// }

	// for (auto& it : operacaoOperando) {
	// 	string opcode = Opcodes::GetInstance().GetOpcode(get<0>(it));
	// 	if (opcode == "14") {
	// 		break;
	// 		// STOP
	// 	}
	// 	else {
	// 		if (opcode == "01") {
				
	// 		}
	// 	}
	// }

}