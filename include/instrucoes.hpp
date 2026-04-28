#include <string>
#include <map>
#include <tuple>
#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;

unordered_map<string,tuple<int,string>> instrucoes;
unordered_map<string,int> diretivas;

void PopulaInstrucoes();
void PopulaDiretivas();