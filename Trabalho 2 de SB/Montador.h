#pragma once

#include <string>
#include <vector>
#include <regex>

class Montador {
public:
	struct TokensDaLinha {
		std::string label;
		std::string operacao;
		std::vector<std::string> operando;

		TokensDaLinha(const std::string &label, const std::string &operacao, const std::vector<std::string> &operando,
			int numeroDaLinha);
	};

	std::vector<TokensDaLinha> listaDeTokensDoArquivo;

	void adicionarTokenDaLinha(TokensDaLinha linha);

	void PreProcessamento(std::string conteudoArquivo); //recebe o conteudo do arquivo em uma string e salva os tokens na listaDeTokensDoArquivo

	std::string RemoveComentarios(std::string arquivoConteudo); //remove tudo que tiver entre ; e \n

	std::string RemoveTabulacoes(std::string fileString); //substitui tabula��es por espa�os em branco

	std::string RemoveEspacosEmBrancoExtras(std::string arquivoConteudo); //remove v�rios casos de espa�os em branco como duplos espa�os ou espa�os antes de \n

	std::string JuntaLabelEOperacao(std::string arquivo); //quando se tem uma label sozinha em uma linha, coloca a linha com tokens abaixo dela na mesma linha da linha da label

	Montador();
	~Montador();
};

