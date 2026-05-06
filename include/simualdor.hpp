#ifndef SIMUALDOR_HPP
#define SIMUALDOR_HPP
#include <instrucoes.hpp>
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


#endif