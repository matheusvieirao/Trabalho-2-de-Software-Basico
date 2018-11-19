// Trabalho 2 de SB.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Montador.h"

int main(int argc, char* argv[]) {
	std::string nomeArquivo;
	std::ifstream fileAsm;
	std::ifstream fileS;
	std::stringstream conteudoArquivo;
	Montador montador;

	nomeArquivo = argv[1];


	fileAsm.open(nomeArquivo);
	if (fileAsm.is_open() && fileAsm.good()) {
		conteudoArquivo << fileAsm.rdbuf();
		fileAsm.close();

		montador.PreProcessamento(conteudoArquivo.str());
		montador.TraducaoParaIA32();
		fileS.open("arquivo.s", std::ifstream::out);
	}
	else {
		std::cout << "Erro. Nao foi possivel ler o arquivo." << std::endl;
		getchar();
		return -1;
	}

}