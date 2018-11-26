/* Trabalho 2 de SB.cpp : Este arquivo contém a função 'main'. 
A execução do programa começa e termina ali.

*/
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Tradutor.h"

int main(int argc, char* argv[]) {
	std::string nomeArquivo = argv[1]; //nome do arquivo de entrada .asm
	std::ifstream fileAsm(nomeArquivo); //ptr pro arquivo de entrada .asm
	std::ofstream fileS ("arquivo.s"); //ptr pro arquivo de saida .s
	std::stringstream  conteudoArquivoAsm; //conteudo arquivo .asm no assembly inventado
	std::string conteudoArquivoIA32; //conteudo arquivo .s traduzido pra IA32
	Tradutor tradutor; //classe que contem as funções que fazem a tradução do assembly inventado pro IA-32

	if (fileAsm.is_open() && fileAsm.good()) {
		conteudoArquivoAsm << fileAsm.rdbuf();
		fileAsm.close();

		tradutor.PreProcessamento(conteudoArquivoAsm.str()); //trata o arquivo de entrada .asm e separa os tokens na listaDeTokens
		conteudoArquivoIA32 = tradutor.TraducaoParaIA32(); //usa listaDeTokens feita pra traduzir da linguagem do assembly inventado pra IA-32.
		fileS << conteudoArquivoIA32;
		fileS.close();

	}
	else {
		std::cout << "Erro. Nao foi possivel ler o arquivo." << std::endl;
		//getchar();
		return -1;
	}

}