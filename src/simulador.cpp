#include <simualdor.hpp>
#include <passagem_unica.hpp>

Opcodes *Opcodes::instance = nullptr;
Simulador *Simulador::instance = nullptr;

Opcodes &Opcodes::GetInstance()
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

string Opcodes::GetOpcode(string instrucao)
{
	if (opcode.find(instrucao) != opcode.end())
		return opcode.find(instrucao)->second;
	else
		return "";
}

Simulador &Simulador::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Simulador();
	}
	return *instance;
}

Simulador::Simulador()
{
	acumulador = 0;
	programCounter = "";
	for (int i = 0; i < TAM_ENDERECO; i++)
	{
		programCounter += "0";
	}
	memoria = unordered_map<string, string>();
	programa = map<string, tuple<string, string>>();
	tabelaOperacaoOperandos = list<tuple<string, string, string>>();
}

Simulador::~Simulador()
{
	delete instance;
}

string Simulador::GetConteudoMemoria(string endereco)
{
	unordered_map<string, string>::const_iterator it = memoria.find(endereco);
	string aux;
	if (it != memoria.end())
	{
		aux = it->second;
		return aux;
	}
	else
	{
		return "";
	}
}

void Simulador::SetConteudoMemoria(string endereco, string valor)
{
	if (memoria.find(endereco) != memoria.end())
		memoria.find(endereco)->second = valor;
}

void Simulador::Start(string codigo)
{
    string token;
    vector<string> tokens;

    std::stringstream ss(codigo);

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    size_t i = 0;
    int posicao = 0;

    while (i < tokens.size())
    {
        string opcode = tokens[i];

        if (opcode == "14")
        {
            programa.emplace(
                ConverteIntEndereco(posicao),
                make_tuple(opcode, "")
            );

            posicao++;
            i++;

            while (i < tokens.size())
            {
                memoria.emplace(
                    ConverteIntEndereco(posicao),
                    tokens[i]
                );

                posicao++;
                i++;
            }
        }
        else if (opcode == "09")
        {
            string operando1 = tokens[i + 1];
            string operando2 = tokens[i + 2];

            programa.emplace(
                ConverteIntEndereco(posicao),
                make_tuple(opcode, operando1 + " " + operando2)
            );

            posicao += 3;
            i += 3;
        }
        else
        {
            string operando1 = tokens[i + 1];

            programa.emplace(
                ConverteIntEndereco(posicao),
                make_tuple(opcode, operando1)
            );

            posicao += 2;
            i += 2;
        }
    }
}

void Simulador::Run(string arquivoOriginal)
{
	fstream arquivo;
	arquivo.open(arquivoOriginal);
	string linha, auxRun, auxOpcode, auxOperando1, auxOperando2, auxCopy, auxInput, auxOutput;
	int auxOpcodeNum, auxOperacao;
	list<string> operandos = list<string>();
	getline(arquivo, linha);
	if (!linha.empty())
	{
		Start(linha);
	}

	auxRun = linha;
	while (programCounter != "STOP")
	{
		auxOpcodeNum = stoi(get<0>(programa.find(programCounter)->second));

		switch (auxOpcodeNum) {
			case 1:
				// ADD
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(auxOperando1));
				acumulador = acumulador + auxOperacao;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 2:
				// SUB
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(auxOperando1));
				acumulador = acumulador - auxOperacao;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 3:
				// MUL
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(auxOperando1));
				acumulador = acumulador * auxOperacao;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 4:
				// DIV
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOperacao = stoi(GetConteudoMemoria(auxOperando1));
				acumulador = acumulador / auxOperacao;
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 5:
				// JMP
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				programCounter = auxOperando1;
				break;
			case 6:
				// JMPN
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador < 0) {
					programCounter = auxOperando1;
				}
				else {
					programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				}
				break;
			case 7:
				// JMPP
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador > 0) {
					programCounter = auxOperando1;
				}
				else {
					programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				}
				break;
			case 8:
				// JMPZ
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				if (acumulador == 0) {
					programCounter = auxOperando1;
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
				auxCopy = GetConteudoMemoria(auxOperando1);
				SetConteudoMemoria(auxOperando2, auxCopy);
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
				SetConteudoMemoria(auxOperando1,to_string(acumulador));
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 12:
				// INPUT
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				cin >> auxInput;
				SetConteudoMemoria(auxOperando1,auxInput);
				programCounter = ConverteIntEndereco(stoi(programCounter) + 2);
				break;
			case 13:
				// OUTPUT
				auxOperando1 = get<1>(programa.find(programCounter)->second);
				auxOutput = GetConteudoMemoria(auxOperando1);
				cout << auxOutput << "\n";
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