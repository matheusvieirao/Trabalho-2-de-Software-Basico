#pragma once

#include <string>
#include <vector>
#include <regex> //pra usar regex_replace
#include <algorithm> //pra usar transform
#include <unordered_map>

#include <iostream> //usada pra testar

class Tradutor {
public:
	struct TokensDaLinha {
		std::string label; //label da instru�ao (o que vem antes de ":")
		std::string arg0; //primeiro argumento depois da label. Geralmente � a opera��o
		std::string arg1; //segundo argumento depois da label. Geralmente � o primeiro operando
		std::string arg2; //terceiro argumento depois da label. Geralmente � o segundo operando
	};

	std::vector<TokensDaLinha> listaDeTokens;

	void adicionarTokenDaLinha(TokensDaLinha linha);

	void PreProcessamento(std::string conteudoArquivo); //recebe o conteudo do arquivo em uma string e salva os tokens na listaDeTokensDoArquivo

	std::string RemoveComentarios(std::string arquivoConteudo); //remove tudo que tiver entre ; e \n

	std::string RemoveTabulacoes(std::string fileString); //substitui tabula��es por espa�os em branco

	std::string RemoveEspacosEmBrancoExtras(std::string arquivoConteudo); //remove v�rios casos de espa�os em branco como duplos espa�os ou espa�os antes de \n

	std::string JuntaLabelEOperacao(std::string arquivo); //quando se tem uma label sozinha em uma linha, coloca a linha com tokens abaixo dela na mesma linha da linha da label

	void SeparaTokens(std::string conteudoArquivo); //preenche o vetor listaDeTokens com as instru��es separadas por tokens.

	std::string TraducaoParaIA32(); // Para funcionar corretamente listaDeTokens deve j� estar preenchida corretamente (depois do m�todo PreProcessamento)

	std:: string adicionaCodigoPadrao();

	Tradutor();
	~Tradutor();
};

