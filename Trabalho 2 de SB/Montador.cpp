#include "pch.h"
#include "Montador.h"

#include <iostream>

Montador::Montador(){
}


Montador::~Montador(){
}

//Montador::TokensDaLinha::TokensDaLinha(const std::string & label, const std::string & arg0, const std::vector<std::string>& operando, int numeroDaLinha) {}

void Montador::adicionarTokenDaLinha(TokensDaLinha linha){
	Montador::listaDeTokens.push_back(linha);
}

void Montador::PreProcessamento(std::string conteudoArquivo) {

	conteudoArquivo = RemoveComentarios(conteudoArquivo);
	conteudoArquivo = RemoveTabulacoes(conteudoArquivo);
	conteudoArquivo = RemoveEspacosEmBrancoExtras(conteudoArquivo);
	conteudoArquivo = JuntaLabelEOperacao(conteudoArquivo);

	SeparaTokens(conteudoArquivo);

}

std::string Montador::RemoveComentarios(std::string arquivoConteudo) {
	std::string::size_type posicaoComentario = posicaoComentario = arquivoConteudo.find(';');
	while (posicaoComentario != std::string::npos) {

		std::string::size_type posicaoFinal = posicaoComentario;
		while (arquivoConteudo[posicaoFinal] != '\n') {
			posicaoFinal++;
		}

		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoComentario, arquivoConteudo.begin() + posicaoFinal);
		posicaoComentario = arquivoConteudo.find(';');
	}
	return arquivoConteudo;
}

std::string Montador::RemoveTabulacoes(std::string fileString) {
	fileString = std::regex_replace(fileString, std::regex("\t"), " ");
	return fileString;
}

std::string Montador::RemoveEspacosEmBrancoExtras(std::string arquivoConteudo) {

	//remove duplos espaços
	std::string::size_type posicaoEspaco = arquivoConteudo.find("  ");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find("  ");
	}

	//remove duplos pulos de linha
	posicaoEspaco = arquivoConteudo.find("\n\n");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find("\n\n");
	}

	////remove duplos pulos no final do arquivo
	if (arquivoConteudo[arquivoConteudo.size() - 1] == '\n') {
		arquivoConteudo.erase(arquivoConteudo.end()-1);
	}

	//remove duplos pulos no inicio do arquivo
	if (arquivoConteudo[0] == '\n') {
		arquivoConteudo.erase(arquivoConteudo.begin());
	}

	//remove espaços antes de \n
	posicaoEspaco = arquivoConteudo.find(" \n");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find(" \n");
	}

	//remove espaços depois de \n
	posicaoEspaco = arquivoConteudo.find("\n ");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco + 1);
		posicaoEspaco = arquivoConteudo.find("\n ");
	}

	//remove espaços antes de :
	posicaoEspaco = arquivoConteudo.find(" :");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find(" :");
	}

	return arquivoConteudo;
}

std::string Montador::JuntaLabelEOperacao(std::string arquivo) {
	std::string::size_type posicao = arquivo.find(":\n");
	while (posicao != std::string::npos) {
		arquivo[posicao + 1] = ' ';
		posicao = arquivo.find(":\n");
	}
	return arquivo;
}

void Montador::SeparaTokens(std::string conteudoArquivo) {
	TokensDaLinha tokens_linha;
	int tam_arquivo = conteudoArquivo.size();
	char proximo_char;
	int i = -1; //é usada pra percorrer o arquivo char por char
	bool tem_label; //se a primeira instrução da linha atual é uma label
	char token_aux[51];
	int i_token; //é usada pra percorrer o novo token
	int posicao_token = 0; //se for o primeiro token da linha = 0, se for o segundo token da linha = 1...
	int posicao_token_sem_label; //mesma coisa de posicao_token mas desconsiderando a label se existir na linha

	std::transform(conteudoArquivo.begin(), conteudoArquivo.end(), conteudoArquivo.begin(), ::toupper); //transforma tudo pra upercase

	while (i != tam_arquivo) {
		i++;
		tem_label = false;
		posicao_token = 0;
		i_token = 0;
		//trabalha linha por linha esse while
		while (conteudoArquivo[i] != '\n' && i != tam_arquivo) {
			token_aux[i_token] = conteudoArquivo[i];
			proximo_char = conteudoArquivo[i+1];

			//se entrar em qualquer uma dessas condiçoes, é pq achou um token
			if (proximo_char == ':') {
				tem_label = true;
				token_aux[i_token+1] = '\0';
				tokens_linha.label = token_aux;
				posicao_token++;
				i_token = -1;
				i++;
			}
			else if (proximo_char == ' ' || proximo_char == '\n' || i + 1 == tam_arquivo) {
				token_aux[i_token+1] = '\0';

				if (tem_label) {
					posicao_token_sem_label = posicao_token - 1;
				}
				else {
					posicao_token_sem_label = posicao_token;
				}
				
				if (posicao_token_sem_label == 0) {
					tokens_linha.arg0 = token_aux;
				}
				else if (posicao_token_sem_label == 1) {
					tokens_linha.arg1 = token_aux;
				}
				else if (posicao_token_sem_label == 2) {
					tokens_linha.arg2 = token_aux;
				}
				
				posicao_token++;
				i_token = -1;
				if (proximo_char == ' ') {
					i++;
				}
			}

			i_token++;
			i++;
		}
		listaDeTokens.push_back(tokens_linha);
		tokens_linha.label = "";
		tokens_linha.arg0 = "";
		tokens_linha.arg1 = "";
		tokens_linha.arg2 = "";
	}
}

