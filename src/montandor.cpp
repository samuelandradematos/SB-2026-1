#include <montador.hpp>
#include <ostream>
#include <passagem_unica.hpp>

int main(int argc, char* argv[]){
    tuple<bool,string> validaChamada = ChamadaValida(argc, argv);

    if (get<0>(validaChamada)) {
        if (get<1>(validaChamada) == "pre"){
            string nomeArquivoSaida = argv[1];
            nomeArquivoSaida.replace(nomeArquivoSaida.find(".pre"), strlen(".pre"), "");
            Parser(nomeArquivoSaida, "pre");
            //Chamadas para passagem única e criação de arquivos de saída .obj e .pen
        }
        if (get<1>(validaChamada) == "asm"){
            //Chamadas para pre-processamento
        }
        if (get<1>(validaChamada) == "obj"){
            //Chamadas para o simulador
        }

    }

    return 0;
}

tuple<bool,string> ChamadaValida(int argc, char* argv[]){
    if (argc != 2){
        cout << "Chamada incorreta: excesso de paremetros, uso correto: './montandor <arquivo.asm|.pre|.pen>'";
        return tuple(false, "error");
    }
    else if (regex_search(argv[1],regex("(.pre)"))){
        return tuple(true, "pre");
    }
    else if (regex_search(argv[1],regex("(.asm)"))){
        return tuple(true, "asm");
    }
    else if (regex_search(argv[1],regex("(.pen)"))){
        return tuple(true, "pen");
    }
    else{
        cout << "Erro no formato de arquivo: formatos permitidos - .asm | .pre | .pen" << endl;
        return tuple(false,"error");
    }
}