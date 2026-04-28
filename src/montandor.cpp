#include <montador.hpp>
#include <ostream>
#include <regex>
#include <passagem_unica.hpp>

int main(int argc, char* argv[]){
    tuple<bool,string> validaChamada = ChamadaValida(argc, argv);

    if (get<0>(validaChamada)) {
        if (get<1>(validaChamada) == "pre"){
            //Chamadas para o pré-processamento
        }
        if (get<1>(validaChamada) == "asm"){
            //Chamadas para o montandor
        }
        if (get<1>(validaChamada) == "pen"){
            //Chamadas para o algoritmo de passagem única
        }

    }

}

tuple<bool,string> ChamadaValida(int argc, char* argv[]){
    if (argc != 2){
        cout << "Chamada incorreta: excesso de paremetros, uso correto: './montandor <arquivo.asm|.pre|.pen>'";
        return tuple(false, "error");
    }
    else if (regex_match(argv[1],regex("(.pre)"))){
        return tuple(true, "pre");
    }
    else if (regex_match(argv[1],regex("(.asm)"))){
        return tuple(true, "asm");
    }
    else if (regex_match(argv[1],regex("(.pen)"))){
        return tuple(true, "pen");
    }
    else{
        cout << "Erro no formato de arquivo: formatos permitidos - .asm | .pre | .pen";
        return tuple(false,"error");
    }
}