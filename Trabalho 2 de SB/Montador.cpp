#include "pch.h"
#include "Montador.h"


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

void Montador::TraducaoParaIA32() {
	std::string conteudoSaida;
	std::unordered_map<std::string, std::string> simbolos;
	std::unordered_map<std::string, std::string>::const_iterator simbolo_especifico;
	std::string label_aux, arg0_aux, arg1_aux, arg2_aux;

	//int tamanhoLista = listaDeTokens.size();
	//for (int i = 0; i < tamanhoLista; i++) {

	//A primeira passagem é para salvar os valores associados com EQU para que IF funcione independente da posição que for colocado
	for (auto linha = listaDeTokens.begin(); linha != listaDeTokens.end(); linha++) {
		if (linha->arg0 == "EQU") {
			simbolos.emplace(linha->label, linha->arg1);
		}
	}

	for (auto linha = listaDeTokens.begin(); linha != listaDeTokens.end(); linha++) {
		label_aux = linha->label;
		arg0_aux = linha->arg0;
		arg1_aux = linha->arg1;
		arg2_aux = linha->arg2;
		//apenas retira o ":"
		if (arg0_aux == "EQU") {
			conteudoSaida.append(label_aux + ' ' + arg0_aux + ' ' + arg1_aux + '\n');
		}
		//se for "IF 1" le a linha de baixo
		else if (arg0_aux == "IF") {
			simbolo_especifico = simbolos.find(arg1_aux);
			//entra se encontrou o simbolo na tabela de simbolos
			if (simbolo_especifico != simbolos.end()) {
				//nao incluir a linha abaixo
				if (std::stoi(simbolo_especifico->second) != 1) {
					linha++;
				}
			}
			else {
				//nao incluir a linha abaixo
				if (std::stoi(arg1_aux) != 1) {
					linha++;
				}
			}
		}
		// "VALOR: CONST PI" fica "%assign VALOR PI"
		else if (arg0_aux == "CONST") {
			// TODO ver qual opcao é a certa
			//conteudoSaida.append("%assign " + label_aux + ' ' + arg1_aux + '\n');
			conteudoSaida.append(label_aux + " DW " + arg1_aux + '\n');
		}
		// "PTR: SPACE 3" fica "PTR RESB 3"
		else if (arg0_aux == "SPACE") {
			if (arg1_aux == "")
				arg1_aux = '1';
			conteudoSaida.append(label_aux + " RESB " + arg1_aux + '\n');
		}
		else if (arg0_aux == "SECTION") {
			if (label_aux != "") {
				conteudoSaida.append(label_aux + ": ");
			}
			if (arg1_aux == "TEXT") {
				conteudoSaida.append("section .text\n_start:\n");
			}
			else if (arg1_aux == "BSS") {
				conteudoSaida.append("section .bss\n");
			}
			else if (arg1_aux == "DATA") {
				conteudoSaida.append("section .data\n");
			}
		}
	}
	printf("a");
}

