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
		std::string label; //label da instruçao (o que vem antes de ":")
		std::string arg0; //primeiro argumento depois da label. Geralmente é a operação
		std::string arg1; //segundo argumento depois da label. Geralmente é o primeiro operando
		std::string arg2; //terceiro argumento depois da label. Geralmente é o segundo operando
	};

	std::vector<TokensDaLinha> listaDeTokens;

	void adicionarTokenDaLinha(TokensDaLinha linha);

	void PreProcessamento(std::string conteudoArquivo); //recebe o conteudo do arquivo em uma string e salva os tokens na listaDeTokensDoArquivo

	std::string RemoveComentarios(std::string arquivoConteudo); //remove tudo que tiver entre ; e \n

	std::string RemoveTabulacoes(std::string fileString); //substitui tabulações por espaços em branco

	std::string RemoveEspacosEmBrancoExtras(std::string arquivoConteudo); //remove vários casos de espaços em branco como duplos espaços ou espaços antes de \n

	std::string JuntaLabelEOperacao(std::string arquivo); //quando se tem uma label sozinha em uma linha, coloca a linha com tokens abaixo dela na mesma linha da linha da label

	void SeparaTokens(std::string conteudoArquivo); //preenche o vetor listaDeTokens com as instruções separadas por tokens.

	std::string TraducaoParaIA32(); // Para funcionar corretamente listaDeTokens deve já estar preenchida corretamente (depois do método PreProcessamento)

	std:: string adicionaCodigoPadrao();

	Tradutor();
	~Tradutor();
};

