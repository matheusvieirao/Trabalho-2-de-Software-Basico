#pragma once

#include <string>
#include <vector>
#include <regex> //pra usar regex_replace
#include <algorithm> //pra usar transform

class Montador {
public:
	struct TokensDaLinha {
		std::string label;
		std::string operacao;
		std::string operando1;
		std::string operando2;

		//TokensDaLinha(const std::string &label, const std::string &operacao, const std::vector<std::string> &operando, int numeroDaLinha);
	};

	std::vector<TokensDaLinha> listaDeTokens;

	void adicionarTokenDaLinha(TokensDaLinha linha);

	void PreProcessamento(std::string conteudoArquivo); //recebe o conteudo do arquivo em uma string e salva os tokens na listaDeTokensDoArquivo

	std::string RemoveComentarios(std::string arquivoConteudo); //remove tudo que tiver entre ; e \n

	std::string RemoveTabulacoes(std::string fileString); //substitui tabula��es por espa�os em branco

	std::string RemoveEspacosEmBrancoExtras(std::string arquivoConteudo); //remove v�rios casos de espa�os em branco como duplos espa�os ou espa�os antes de \n

	std::string JuntaLabelEOperacao(std::string arquivo); //quando se tem uma label sozinha em uma linha, coloca a linha com tokens abaixo dela na mesma linha da linha da label

	void SeparaTokens(std::string conteudoArquivo); //preenche o vetor listaDeTokens com as instru��es separadas por tokens

	Montador();
	~Montador();
};

