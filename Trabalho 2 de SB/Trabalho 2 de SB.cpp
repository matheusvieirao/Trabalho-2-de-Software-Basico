// Trabalho 2 de SB.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
	std::string nomeArquivo;
	std::ifstream file;
	std::stringstream conteudoArquivo;

	nomeArquivo = argv[1];


	file.open(nomeArquivo);
	if (file.is_open() && file.good()) {
		conteudoArquivo << file1.rdbuf();
		file1.close();
	}
	else {
		std::cout << "Erro. Nao foi possivel ler o arquivo." << std::endl;
		getchar();
		return -1;
	}


    std::cout << nomeArquivo <<std::endl;

}