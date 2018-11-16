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
	std::ifstream file;
	std::stringstream conteudoArquivo;
	Montador montador;

	nomeArquivo = argv[1];


	file.open(nomeArquivo);
	if (file.is_open() && file.good()) {
		conteudoArquivo << file.rdbuf();
		file.close();

		montador.PreProcessamento(conteudoArquivo.str());
	}
	else {
		std::cout << "Erro. Nao foi possivel ler o arquivo." << std::endl;
		getchar();
		return -1;
	}

}