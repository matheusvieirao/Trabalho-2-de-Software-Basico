#include "pch.h"
#include "Tradutor.h"


Tradutor::Tradutor(){
}


Tradutor::~Tradutor(){
}

//Tradutor::TokensDaLinha::TokensDaLinha(const std::string & label, const std::string & arg0, const std::vector<std::string>& operando, int numeroDaLinha) {}

void Tradutor::adicionarTokenDaLinha(TokensDaLinha linha){
	Tradutor::listaDeTokens.push_back(linha);
}

void Tradutor::PreProcessamento(std::string conteudoArquivo) {

	conteudoArquivo = RemoveComentarios(conteudoArquivo);
	conteudoArquivo = RemoveTabulacoes(conteudoArquivo);
	conteudoArquivo = RemoveEspacosEmBrancoExtras(conteudoArquivo);
	conteudoArquivo = JuntaLabelEOperacao(conteudoArquivo);

	SeparaTokens(conteudoArquivo);

}

std::string Tradutor::RemoveComentarios(std::string arquivoConteudo) {
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

std::string Tradutor::RemoveTabulacoes(std::string fileString) {
	fileString = std::regex_replace(fileString, std::regex("\t"), " ");
	return fileString;
}

std::string Tradutor::RemoveEspacosEmBrancoExtras(std::string arquivoConteudo) {

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

std::string Tradutor::JuntaLabelEOperacao(std::string arquivo) {
	std::string::size_type posicao = arquivo.find(":\n");
	while (posicao != std::string::npos) {
		arquivo[posicao + 1] = ' ';
		posicao = arquivo.find(":\n");
	}
	return arquivo;
}

void Tradutor::SeparaTokens(std::string conteudoArquivo) {
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

std::string Tradutor::TraducaoParaIA32() {
	std::string conteudoSaida;
	std::unordered_map<std::string, std::string> simbolos;
	std::unordered_map<std::string, std::string>::const_iterator simbolo_especifico;
	std::string label, arg0, arg1, arg2;
	//funcoes usadas:
	bool output_n = false;
	bool swap_b = false;
	bool multiplic = false;

	//int tamanhoLista = listaDeTokens.size();
	//for (int i = 0; i < tamanhoLista; i++) {

	//A primeira passagem é para salvar os valores associados com EQU para que IF e CONST funcione
	for (auto linha = listaDeTokens.begin(); linha != listaDeTokens.end(); linha++) {
		if (linha->arg0 == "EQU") {
			simbolos.emplace(linha->label, linha->arg1);
		}
	}

	conteudoSaida.append("global _start\n\n");

	for (auto linha = listaDeTokens.begin(); linha != listaDeTokens.end(); linha++) {
		label = linha->label;
		arg0 = linha->arg0;
		arg1 = linha->arg1;
		arg2 = linha->arg2;

		//apenas retira o ":"
		if (arg0 == "EQU") {
			conteudoSaida.append(label + ' ' + arg0 + ' ' + arg1 + '\n');
		}
		//se for "IF 1" le a linha de baixo
		else if (arg0 == "IF") {
			simbolo_especifico = simbolos.find(arg1);
			//entra se encontrou o simbolo na tabela de simbolos
			if (simbolo_especifico != simbolos.end()) {
				//nao incluir a linha abaixo
				if (std::stoi(simbolo_especifico->second) != 1) {
					linha++;
				}
			}
			else {
				//nao incluir a linha abaixo
				if (std::stoi(arg1) != 1) {
					linha++;
				}
			}
		}
		// TODO do jeito que esta, "VALOR: CONST "hello world"" é convertido para "VALOR DD "hello world"". Ver se esta certo ou se deveria ser DB 
		else if (arg0 == "CONST") {
			simbolo_especifico = simbolos.find(arg1);
			//entra se encontrou o simbolo na tabela de simbolos
			if (simbolo_especifico != simbolos.end()) {
				conteudoSaida.append(label + " DD " + simbolo_especifico->second + '\n');
			}
			else {
				conteudoSaida.append(label + " DD " + arg1 + '\n');
			}
		}
		// "PTR: SPACE 3" fica "PTR RESB 3"
		else if (arg0 == "SPACE") {
			if (arg1 == "")
				arg1 = '1';
			conteudoSaida.append(label + " RESD " + arg1 + '\n');
		}
		//colocamos essa condição aqui pq EQU, CONST e SPACE usam label de uma meneira diferente das outras instruções
		else if (label != "") {
			conteudoSaida.append(label + ": ");
		}
		
		
		if (arg0 == "SECTION") {
			if (arg1 == "TEXT") {
				conteudoSaida.append("section .text\n_start:\n");
			}
			else if (arg1 == "BSS") {
				conteudoSaida.append("section .bss\n");
			}
			else if (arg1 == "DATA") {
				conteudoSaida.append("section .data\n");
			}
		}
		else if (arg0 == "STOP") {
			conteudoSaida.append("mov eax, 1\nmov ebx, 0\nint 80h\n");
		}
		// le numero inteiro com sinal de 32 bits
		else if (arg0 == "INPUT") {
			conteudoSaida.append("push eax\n");
			conteudoSaida.append("mov eax, 3\n"); // 3 é a instrução de read
			conteudoSaida.append("mov ebx, 0\n");
			conteudoSaida.append("mov ecx, " + arg1 + "\n");
			conteudoSaida.append("mov edx, 4\n"); //inteiro tem 4 bytes
			conteudoSaida.append("int 80h\n");
			conteudoSaida.append("pop eax\n");
		}
		// um char de 8 bits
		else if (arg0 == "C_INPUT") {
			conteudoSaida.append("push eax\n");
			conteudoSaida.append("mov eax, 3\n");
			conteudoSaida.append("mov ebx, 0\n");
			conteudoSaida.append("mov ecx, " + arg1 + "\n");
			conteudoSaida.append("mov edx, 1\n"); //char tem 1 byte
			conteudoSaida.append("int 80h\n");
			conteudoSaida.append("pop eax\n");
		}
		// input de string com o tamanho determinado pelo segundo operando
		else if (arg0 == "S_INPUT") {
			conteudoSaida.append("push eax\n");
			conteudoSaida.append("mov eax, 3\n");
			conteudoSaida.append("mov ebx, 0\n");
			conteudoSaida.append("mov ecx, " + arg1 + "\n");
			conteudoSaida.append("mov edx, " + arg2 + "\n"); //tamanho da string
			conteudoSaida.append("int 80h\n");
			conteudoSaida.append("pop eax\n");
		}
		// le numero inteiro com sinal de 32 bits
		else if (arg0 == "OUTPUT") {
			conteudoSaida.append("mov ecx, [" + arg1 + "]\n");
			conteudoSaida.append("call output_n\n");
			output_n = true;
		}
		// um char de 8 bits
		else if (arg0 == "C_OUTPUT") {
			conteudoSaida.append("push eax\n");
			conteudoSaida.append("mov eax, 4\n");
			conteudoSaida.append("mov ebx, 1\n");
			conteudoSaida.append("mov ecx, " + arg1 + "\n");
			conteudoSaida.append("mov edx, 1\n"); //char tem 1 byte
			conteudoSaida.append("int 80h\n");
			conteudoSaida.append("pop eax\n");
		}
		// output de string com o tamanho determinado pelo segundo operando
		else if (arg0 == "S_OUTPUT") {
			conteudoSaida.append("push eax\n");
			conteudoSaida.append("mov eax, 3\n");
			conteudoSaida.append("mov ebx, 0\n");
			conteudoSaida.append("mov ecx, " + arg1 + "\n");
			conteudoSaida.append("mov edx, " + arg2 + "\n"); // tamanho da string
			conteudoSaida.append("int 80h\n");
			conteudoSaida.append("pop eax\n");
		}
		// carrega no acumulador (EAX) o valor (assume-se que DWORD) que esta em um endereco de memoria
		else if (arg0 == "LOAD") {
			conteudoSaida.append("mov DWORD eax, " + arg1 + "\n");
		}
		else if (arg0 == "STORE") {
			conteudoSaida.append("mov DWORD [" + arg1 + "], eax\n");
		}
		else if (arg0 == "COPY") {
			conteudoSaida.append("mov DWORD " + arg2 + ", " + arg1 + "\n");
		}
		//pulo incondicional
		else if (arg0 == "JMP") {
			conteudoSaida.append("jmp " + arg1 + "\n");
		}
		// pula se acumulador < 0
		else if (arg0 == "JMPN") {
			conteudoSaida.append("cmp eax, 0");
			conteudoSaida.append("jg " + arg1 + "\n");
		}
		// pula se acumulador > 0
		else if (arg0 == "JMPP") {
			conteudoSaida.append("cmp eax, 0");
			conteudoSaida.append("jl " + arg1 + "\n");
		}
		// pula se acumulador == 0
		else if (arg0 == "JMPZ") {
			conteudoSaida.append("cmp eax, 0");
			conteudoSaida.append("jz " + arg1 + "\n");
		}
		else if (arg0 == "ADD") {
			conteudoSaida.append("add eax, [" + arg1 + "]\n");
		}
		else if (arg0 == "SUB") {
			conteudoSaida.append("sub eax, [" + arg1 + "]\n");
		}
		// imul multiplica (com sinal) EAX pelo conteudo na memoria da variável e salva em EDX:EAX
		else if (arg0 == "MULT") {
		conteudoSaida.append("imul [" + arg1 + "]\n");
		conteudoSaida.append("cmp edx FFFFh\n"); //TODO ver se da certo isso
		conteudoSaida.append("jz sem_overflow\n");
		conteudoSaida.append("cmp edx 0\n"); //TODO ver se da certo isso
		conteudoSaida.append("jz sem_overflow\n");

		//mensagem de overflow
		conteudoSaida.append("push eax\n");
		conteudoSaida.append("mov eax, 3\n");
		conteudoSaida.append("mov ebx, 0\n");
		conteudoSaida.append("mov ecx, msg_overflow\n");
		conteudoSaida.append("mov edx, size_msg_overflow\n"); // tamanho da string
		conteudoSaida.append("int 80h\n");
		conteudoSaida.append("pop eax\n");

		conteudoSaida.append("sem_overflow: mov eax, eax\n"); //nao faz nada essa instrucao. só pra ser usada no pulo
		}
		// divide EDX:EAX pelo conteudo na memória da variavel e salva em EAX e o resto em EDX .acc / []
		else if (arg0 == "DIV") {
			//extender o sinal de edx de acordo com eax
			conteudoSaida.append("idiv [" + arg1 + "]\n");
		}
	}

	
	if (output_n) {
		swap_b = true;
		conteudoSaida.append("\n\nsection .bss\n");
		conteudoSaida.append("	OUT_N RESB 12\n");
		conteudoSaida.append("\nsection .text\n");
		conteudoSaida.append("output_n:\n	push eax\n	; mover de ecx para dx : ax(dividendo)\n	mov edx, ecx\n	shr edx, 16; ver se  right mesmo\n	mov eax, ecx\n\n	xor esi, esi; contador de loop / casa decimal\n	mov bx, 10; divisor\n\n	div_10 : \n	idiv bx\n\n	; Colocar o resto(dx) no vetor.\n	; ver se o quociente(ax) j  0\n	; se nao for, dividir de novo ax por 10.\n	; (provavelmente isso nao funciona pra numeros muito grandes ja que vamos perder parte do quociente na divisao nesses casos.)\n	add dx, '0'; soma com o valor de 0 em ASCII\n	mov BYTE[OUT_N + esi], dl; salva na string\n			xor dx, dx\n	inc esi\n	cmp ax, 0\n	jnz div_10\n\n	mov BYTE[OUT_N + esi], 0dh\n	inc esi\n	mov BYTE[OUT_N + esi], 0ah\n	inc esi\n\n	mov ebx, OUT_N\n	mov ecx, esi\n	sub ecx, 3\n	call swap_b; ebx contem o endereço da string e acx quantos bytes vao ser invertidos\n\n	mov eax, 4\n	mov ebx, 1\n	mov ecx, OUT_N\n	mov edx, esi\n	int 80h\n	pop eax\n	ret\n");		
	}
	if (swap_b) {
		conteudoSaida.append("\nswap_b :\n	push esi\n	xor esi, esi; esi vai crescendo do inicio ao fim e ecx do fim ao inicio\n\n	denovo :\n	mov BYTE dh, [ebx + esi]\n	mov BYTE dl, [ebx + ecx]\n	mov[ebx + ecx], dh\n	mov[ebx + esi], dl\n\n	inc esi\n	sub ecx, 1\n\n	cmp ecx, esi\n	jg denovo\n\n	pop esi\n	ret\n");
	}
	if (multiplic) {
		conteudoSaida.append("msg_overflow DB 'Overflow na multiplicacao (o resultado deve ter ate 32 bits)', 0dH, 0aH\n");
		conteudoSaida.append("size_msg_overflow EQU $-msg_overflow\n\n");
	}



	return(conteudoSaida);
}

