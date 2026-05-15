#ifndef SIMUALDOR_HPP
#define SIMUALDOR_HPP
#include <instrucoes.hpp>
#include <passagem_unica.hpp>
#include <fstream>
#include <list>

class Opcodes
{
	private:
		Opcodes();
		~Opcodes();
		static Opcodes* instance;
		unordered_map<string, string> opcode;
	public:
		static Opcodes& GetInstance();
		string GetOpcode(string instrucao);
		bool IsOpcode(string instrucao);
};

class Simulador
{
	private:
		Simulador();
		~Simulador();
		static Simulador* instance;
		int acumulador;
		string programCounter;
		unordered_map<string,string> memoria;
		list<tuple<string,string,string>> tabelaOperacaoOperandos;
		map<string,tuple<string,string>> programa;
	public:
		static Simulador& GetInstance();
		string GetConteudoMemoria(string endereco);
		void SetConteudoMemoria(string endereco, string valor);
		void Start(string codigo);
		void Run(string arquivo);
};

#endif