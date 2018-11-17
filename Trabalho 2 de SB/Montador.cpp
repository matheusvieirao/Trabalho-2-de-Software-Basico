#include "pch.h"
#include "Montador.h"

#include <iostream>

Montador::Montador(){
}


Montador::~Montador(){
}

//Montador::TokensDaLinha::TokensDaLinha(const std::string & label, const std::string & operacao, const std::vector<std::string>& operando, int numeroDaLinha) {}

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
	//char linha_aux[51];
	char token_aux[51];
	int i;

	std::transform(conteudoArquivo.begin(), conteudoArquivo.end(), conteudoArquivo.begin(), ::toupper); //transforma tudo pra upercase

	int tam_arquivo = conteudoArquivo.size();
	int sair = false;
	int index = -1;
	bool tem_label;
	int flag_token = 0;
	int aux;
	int posicao_token_aux;
	char proximo_char;
	while (index != tam_arquivo) {
		i = 0;
		index++;
		int a = conteudoArquivo.size();
		tem_label = false;
		flag_token = 0;
		posicao_token_aux = 0;
		//pega uma linha inteira nesse while
		while (conteudoArquivo[index] != '\n' && index != tam_arquivo) {
			//ja fazer a analise aqui se encontrar o primeiro ' ' ou ':' e colocar no lugar certo de tokens_linha
			//linha_aux[i] = char_atual;
			token_aux[posicao_token_aux] = conteudoArquivo[index];
			proximo_char = conteudoArquivo[index+1];

			//se entrar em qualquer uma dessas condiçoes, é pq achou um token
			if (proximo_char == ':') {
				tem_label = true;
				token_aux[posicao_token_aux+1] = '\0';
				tokens_linha.label = token_aux;
				flag_token++;
				posicao_token_aux = -1;
				index++;
			}
			else if (proximo_char == ' ' || proximo_char == '\n' || index + 1 == tam_arquivo) {
				token_aux[posicao_token_aux+1] = '\0';

				if (tem_label) {
					aux = flag_token - 1;
				}
				else {
					aux = flag_token;
				}
				
				if (aux == 0) {
					tokens_linha.operacao = token_aux;
				}
				else if (aux == 1) {
					tokens_linha.operando1 = token_aux;
				}
				else if (aux == 2) {
					tokens_linha.operando2 = token_aux;
				}
				
				flag_token++;
				posicao_token_aux = -1;
				if (proximo_char == ' ') {
					index++;
				}
			}

			posicao_token_aux++;
			i++;
			index++;
		}
		listaDeTokens.push_back(tokens_linha);
		tokens_linha.label = "";
		tokens_linha.operacao = "";
		tokens_linha.operando1 = "";
		tokens_linha.operando2 = "";
	}
}

