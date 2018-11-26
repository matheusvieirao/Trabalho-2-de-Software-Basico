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
	//Adicionan funcoes e macros padrao
	conteudoSaida.append(adicionaCodigoPadrao());

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
			conteudoSaida.append("stop\n");
		}
		// le numero inteiro com sinal de 32 bits
		else if (arg0 == "INPUT") {
			conteudoSaida.append("push " + arg1 + "\n");
			conteudoSaida.append("call INPUT\n");
		}
		// um char de 8 bits
		else if (arg0 == "C_INPUT") {
			conteudoSaida.append("push " + arg1 + "\n");
			conteudoSaida.append("call C_INPUT\n");
		}
		// input de string com o tamanho determinado pelo segundo operando
		else if (arg0 == "S_INPUT") {
			conteudoSaida.append("push " + arg1 + "\n");
			conteudoSaida.append("push " + arg2 + "\n");
			conteudoSaida.append("call S_INPUT\n");
		}
		// le numero inteiro com sinal de 32 bits
		else if (arg0 == "OUTPUT") {
			conteudoSaida.append("push AUX1212\n");
			conteudoSaida.append("push DWORD [" + arg1 + "]\n");
			conteudoSaida.append("call OUTPUT\n");
			output_n = true;
		}
		// um char de 8 bits
		else if (arg0 == "C_OUTPUT") {
			conteudoSaida.append("push " + arg1 + "\n");
			conteudoSaida.append("call C_OUTPUT\n");
		}
		// output de string com o tamanho determinado pelo segundo operando
		else if (arg0 == "S_OUTPUT") {
			conteudoSaida.append("push " + arg1 + "\n");
			conteudoSaida.append("push " + arg2 + "\n");
			conteudoSaida.append("call S_OUTPUT\n");
		}
		// carrega no acumulador (EAX) o valor (assume-se que DWORD) que esta em um endereco de memoria
		else if (arg0 == "LOAD") {
			conteudoSaida.append("mov DWORD eax, [" + arg1 + "]\n");
		}
		else if (arg0 == "STORE") {
			conteudoSaida.append("mov DWORD [" + arg1 + "], eax\n");
		}
		else if (arg0 == "COPY") {
			conteudoSaida.append("mov DWORD ebx, [" + arg1 + "]\n");
			conteudoSaida.append("mov DWORD [" + arg2 + "], ebx\n");
		}
		//pulo incondicional
		else if (arg0 == "JMP") {
			conteudoSaida.append("jmp " + arg1 + "\n");
		}
		// pula se acumulador < 0
		else if (arg0 == "JMPN") {
			conteudoSaida.append("cmp eax, 0");
			conteudoSaida.append("jl " + arg1 + "\n");
		}
		// pula se acumulador > 0
		else if (arg0 == "JMPP") {
			conteudoSaida.append("cmp eax, 0");
			conteudoSaida.append("jg " + arg1 + "\n");
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
		conteudoSaida.append("push " + arg1 + "\n");
		conteudoSaida.append("call mult\n");
		}
		// divide EDX:EAX pelo conteudo na memória da variavel e salva em EAX e o resto em EDX .acc / []
		else if (arg0 == "DIV") {
			//extender o sinal de edx de acordo com eax
			conteudoSaida.append("cdq\n");
			conteudoSaida.append("idiv DWORD [" + arg1 + "]\n");
		}
	}

	/*
	if (output_n) {
		swap_b = true;
		conteudoSaida.append("\n\nsection .bss\n");
		conteudoSaida.append("	OUT_N RESB 12\n");
		conteudoSaida.append("\nsection .text\n");
		conteudoSaida.append("\n;ecx contem o endereço do numero a ser impresso\noutput_n:\n    push eax\n    xor esi, esi ; contador de loop / casa decimal\n\n    ;;;;pega o modulo;;;;\n    mov eax, [ecx]    \n    cmp eax, 0\n    jge positivo\n    imul eax, -1\n    mov BYTE [OUT_N], '-'\n    inc esi\n    \n    ;;;;divisao;;;;\n    positivo:\n    mov edx, eax\n    shr edx, 16 ;mover divisor para dx:ax (dividendo)\n    mov bx, 10 ;divisor\n    div_10:\n    idiv bx\n    add dx, '0' ; soma com o valor de 0 em ASCII\n    mov BYTE [OUT_N + esi], dl  ;salva na string o resto da divisao por 10\n    inc esi\n    xor dx, dx\n    cmp ax, 0 ;ver se o quociente (ax) é zero\n    jnz div_10 ;se nao for, dividir de novo ax por 10. \n	\n    ;;;;marcando fim da string;;;;\n    mov BYTE [OUT_N + esi], 0dh \n    inc esi\n    mov BYTE [OUT_N + esi], 0ah\n    inc esi\n    \n    ;;;;invertendo posicao dos numeros na string;;;;\n    mov ecx, esi\n    sub ecx, 3 ;ultimo char da string\n    xor ebx, ebx ;ebx vai crescendo do inicio ao fim e ecx do fim ao inicio\n    mov BYTE dl, [OUT_N] ;para ver se é negativo\n    cmp dl, '-'\n    jne denovo\n    inc ebx\n    denovo: \n    mov BYTE dh, [OUT_N + ebx]\n    mov BYTE dl, [OUT_N + ecx]\n    mov [OUT_N + ecx], dh\n    mov [OUT_N + ebx], dl\n    inc ebx\n    sub ecx, 1\n    cmp ecx, ebx\n    jg denovo   \n    \n    ;;;;imprime;;;;\n    mov eax, 4\n    mov ebx, 1\n    mov ecx, OUT_N\n    mov edx, esi\n    int 80h\n    pop eax\nret \n");		
	}
	if (multiplic) {
		conteudoSaida.append("msg_overflow DB 'Overflow na multiplicacao (o resultado deve ter ate 32 bits)', 0dH, 0aH\n");
		conteudoSaida.append("size_msg_overflow EQU $-msg_overflow\n\n");
	}

	*/

	return(conteudoSaida);
}

std::string Tradutor::adicionaCodigoPadrao(){
	std::string conteudoSaida="";
	conteudoSaida.append("section .text\n");

	conteudoSaida.append("\n");
	conteudoSaida.append("%macro endl 0\n");
	conteudoSaida.append("	mov EAX, 4\n");
	conteudoSaida.append("	mov EBX, 1\n");
	conteudoSaida.append("	mov ECX, endlineeee\n");
	conteudoSaida.append("	mov EDX, 1\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("%endmacro\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("%macro return 1\n");
	conteudoSaida.append("	mov EAX, 1\n");
	conteudoSaida.append("	mov EBX, %1 \n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("%endmacro\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("%macro monitor 2\n");
	conteudoSaida.append("	mov EAX, 4\n");
	conteudoSaida.append("	mov EBX, 1\n");
	conteudoSaida.append("	mov ECX, %1\n");
	conteudoSaida.append("	mov EDX, %2\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("%endmacro\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("mult:\n");
	conteudoSaida.append("	enter 0,0\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	mov EBX, [ebp+8]\n");
	conteudoSaida.append("	mov EBX, [EBX]\n");
	conteudoSaida.append("	mov EDX, EAX\n");
	conteudoSaida.append("	imul EBX\n");
	conteudoSaida.append("	cmp EAX,0\n");
	conteudoSaida.append("	jl low\n");
	conteudoSaida.append("gre:	 \n");
	conteudoSaida.append("	cmp EDX, 0x00000000\n");
	conteudoSaida.append("	je contmul\n");
	conteudoSaida.append("	jmp over\n");
	conteudoSaida.append("low:\n");
	conteudoSaida.append("	cmp EDX, 0xffffffff\n");
	conteudoSaida.append("	je contmul\n");
	conteudoSaida.append("over:\n");
	conteudoSaida.append("	monitor dword overflow101010, TAMoverflow101010\n");
	conteudoSaida.append("	return 1\n");
	conteudoSaida.append("contmul:\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 4\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("%macro stop 0\n");
	conteudoSaida.append("	mov EAX, 1\n");
	conteudoSaida.append("	mov EBX, 0 \n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("%endmacro\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("swap: ; swap(**char, **char)\n");
	conteudoSaida.append("	 enter 0, 0\n");
	conteudoSaida.append("	 pusha\n");
	conteudoSaida.append("	mov EAX, [EBP+12]\n");
	conteudoSaida.append("	mov ECX, [EBP+8]\n");
	conteudoSaida.append("	add ECX, EAX\n");
	conteudoSaida.append("l1:	cmp EAX, ECX\n");
	conteudoSaida.append("	jae co\n");
	conteudoSaida.append("	mov BL, [EAX]\n");
	conteudoSaida.append("	mov DL, [ECX]\n");
	conteudoSaida.append("	mov [EAX], DL\n");
	conteudoSaida.append("	mov [ECX], BL\n");
	conteudoSaida.append("	inc EAX\n");
	conteudoSaida.append("	dec ECX\n");
	conteudoSaida.append("	jmp l1\n");
	conteudoSaida.append("co:\n");
	conteudoSaida.append("	popa\n");
	conteudoSaida.append("	leave	\n");
	conteudoSaida.append("ret 8\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("S_INPUT: ; INPUT(** char, int) \n");
	conteudoSaida.append("	enter 0,0\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	mov ECX, [EBP+8]\n");
	conteudoSaida.append("	mov EBX, [EBP+12] ; inicio do vetor\n");
	conteudoSaida.append("	add ECX, EBX	; fim do vetor\n");
	conteudoSaida.append("loopsinput:\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	call C_INPUT\n");
	conteudoSaida.append("	cmp byte [EBX], 0xa   ;confere se eh fim de linha\n");
	conteudoSaida.append("	je fimsinput\n");
	conteudoSaida.append("	inc EBX\n");
	conteudoSaida.append("	cmp EBX, ECX\n");
	conteudoSaida.append("	jb loopsinput\n");
	conteudoSaida.append("	mov byte [EBX], 0xa \n");
	conteudoSaida.append("fimsinput:\n");
	conteudoSaida.append("	mov EAX, EBX\n");
	conteudoSaida.append("	sub EAX,[EBP+12]\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 8\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("S_OUTPUT: ; INPUT(** char, int) \n");
	conteudoSaida.append("	enter 0,0\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	mov ECX, [EBP+8]\n");
	conteudoSaida.append("	mov EBX, [EBP+12] ; inicio do vetor\n");
	conteudoSaida.append("	add ECX, EBX	; fim do vetor\n");
	conteudoSaida.append("loopsoutput:\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	call C_OUTPUT\n");
	conteudoSaida.append("	cmp byte [EBX], 0xa   ;confere se eh fim de linha\n");
	conteudoSaida.append("	je fimsinput\n");
	conteudoSaida.append("	inc EBX\n");
	conteudoSaida.append("	cmp EBX, ECX\n");
	conteudoSaida.append("	jb loopsoutput\n");
	conteudoSaida.append("	endl\n");
	conteudoSaida.append("fimsoutput:\n");
	conteudoSaida.append("	mov EAX, EBX\n");
	conteudoSaida.append("	sub EAX,[EBP+12]\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 8\n");
	conteudoSaida.append("\n");
	conteudoSaida.append("\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("C_INPUT: ; INPUT(* char) \n");
	conteudoSaida.append("	enter 0,0\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	mov EAX, 3\n");
	conteudoSaida.append("	mov EBX, 0\n");
	conteudoSaida.append("	mov ECX, [ebp+8]\n");
	conteudoSaida.append("	mov EDX, 1\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("	;endl\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 4\n");
	conteudoSaida.append("	\n");

	conteudoSaida.append("C_OUTPUT: ; OUTPUT(* char) \n");
	conteudoSaida.append("	enter 0,0\n");
	conteudoSaida.append("	push EAX\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	monitor dword [EBP+8], 1 \n");
	conteudoSaida.append("	;endl\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	pop EAX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 4\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("INPUT: ; INPUT(* int) \n");
	conteudoSaida.append("	enter 6,0\n");
	conteudoSaida.append("	push EAX\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	push EDX\n");
	conteudoSaida.append("	mov dword [EBP-6], 0\n");
	conteudoSaida.append("	mov byte [EBP-2], 0\n");
	conteudoSaida.append("	mov EAX, 3\n");
	conteudoSaida.append("	mov EBX, 0\n");
	conteudoSaida.append("	mov ECX, EBP\n");
	conteudoSaida.append("	sub ECX, 1\n");
	conteudoSaida.append("	mov EDX, 1\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("	cmp byte [EBP-1], '-'\n");
	conteudoSaida.append("	jne pli\n");
	conteudoSaida.append("	mov byte [EBP-2], '-'\n");
	conteudoSaida.append("loopinput:\n");
	conteudoSaida.append("	mov EAX, 3\n");
	conteudoSaida.append("	mov EBX, 0\n");
	conteudoSaida.append("	mov ECX, EBP\n");
	conteudoSaida.append("	sub ECX, 1\n");
	conteudoSaida.append("	mov EDX, 1\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("pli:\n");
	conteudoSaida.append("	mov ESI, [EBP-6]\n");
	conteudoSaida.append("	cmp byte [EBP-1], 0xa   ;confere se eh fim de linha\n");
	conteudoSaida.append("	je fiminput\n");
	conteudoSaida.append("	cmp byte [EBP-1], 0x30  ;verifica se eh um numero valido\n");
	conteudoSaida.append("	jb inv\n");
	conteudoSaida.append("	cmp byte [EBP-1], 0x39  ;verifica se eh um numero valido\n");
	conteudoSaida.append("	ja inv\n");
	conteudoSaida.append("	mov EAX, 10\n");
	conteudoSaida.append("	mul dword [EBP-6]\n");
	conteudoSaida.append("	add byte AL, [EBP-1]\n");
	conteudoSaida.append("	adc AH, 0\n");
	conteudoSaida.append("	sub EAX, 0x30\n");
	conteudoSaida.append("	mov dword [EBP-6], EAX\n");
	conteudoSaida.append("	jmp loopinput\n");
	conteudoSaida.append("inv: \n");
	conteudoSaida.append("	monitor dword numInvalido101010, TAMnumInvalido101010\n");
	conteudoSaida.append("fiminput:\n");
	conteudoSaida.append("	cmp byte [EBP-2], '-'\n");
	conteudoSaida.append("	jne fimfiminput\n");
	conteudoSaida.append("	neg dword [EBP-6]\n");
	conteudoSaida.append("fimfiminput:\n");
	conteudoSaida.append("	mov EAX, [EBP-6]\n");
	conteudoSaida.append("	mov EBX, [EBP+8]\n");
	conteudoSaida.append("	mov dword [EBX], EAX\n");
	conteudoSaida.append("	pop EDX\n");
	conteudoSaida.append("	pop ECX\n");
	conteudoSaida.append("	pop EBX\n");
	conteudoSaida.append("	pop EAX\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 4\n");
	conteudoSaida.append("\n");
	conteudoSaida.append("\n");

	conteudoSaida.append("OUTPUT: ; OUTPUT(**char, int) \n");
	conteudoSaida.append("	enter 4,0\n");
	conteudoSaida.append("	pusha\n");
	conteudoSaida.append("	mov ECX, 0\n");
	conteudoSaida.append("	mov EAX, [EBP+8]\n");
	conteudoSaida.append("	mov EBX, [EBP+12]\n");
	conteudoSaida.append("	mov dword [EBP-4],10\n");
	conteudoSaida.append("	cmp EAX, 0\n");
	conteudoSaida.append("	jge l\n");
	conteudoSaida.append("	neg EAX\n");
	conteudoSaida.append("l:  cdq\n");
	conteudoSaida.append("	div dword [EBP-4]\n");
	conteudoSaida.append("	add EDX, 0x30\n");
	conteudoSaida.append("	mov byte [EBX + ECX], DL\n");
	conteudoSaida.append("	inc ECX\n");
	conteudoSaida.append("	;sub EDX, EDX\n");
	conteudoSaida.append("	cmp EAX, 0\n");
	conteudoSaida.append("	jne l\n");
	conteudoSaida.append("	;inc ECX\n");
	conteudoSaida.append("	mov EAX, [EBP+8]\n");
	conteudoSaida.append("	cmp EAX, 0\n");
	conteudoSaida.append("	jge s\n");
	conteudoSaida.append("	mov byte [EBX + ECX ], '-'\n");
	conteudoSaida.append("	inc ECX\n");
	conteudoSaida.append("s:  mov EAX, ECX\n");
	conteudoSaida.append("	dec ECX\n");
	conteudoSaida.append("	push EBX\n");
	conteudoSaida.append("	push ECX\n");
	conteudoSaida.append("	call swap\n");
	conteudoSaida.append("	;mov byte [%2 + ECX], 0\n");
	conteudoSaida.append("	mov EDX, EAX\n");
	conteudoSaida.append("	mov ECX, EBX\n");
	conteudoSaida.append("	mov EAX, 4\n");
	conteudoSaida.append("	mov EBX, 1\n");
	conteudoSaida.append("	int 80h\n");
	conteudoSaida.append("	endl\n");
	conteudoSaida.append("	add EAX,1\n");
	conteudoSaida.append("	popa\n");
	conteudoSaida.append("	leave\n");
	conteudoSaida.append("ret 8\n");
	conteudoSaida.append("\n");
	conteudoSaida.append("\n");;

	conteudoSaida.append("section .data\n");

	conteudoSaida.append("	;linhas adicionadas automaticamente para avisar de erros durante a execução\n");
	conteudoSaida.append("	overflow101010:  db \"overflow\", 0xa\n");
	conteudoSaida.append("	TAMoverflow101010 EQU $-overflow101010\n");
	conteudoSaida.append("	numInvalido101010:  db \"Esperava-se um numero, um caracter invalido foi digitado\", 0xa\n");
	conteudoSaida.append("	TAMnumInvalido101010 EQU $-numInvalido101010\n");
	conteudoSaida.append("	endlineeee: db 0xa\n\n");

	conteudoSaida.append("section .bss\n");
	conteudoSaida.append("	AUX1212:  resb 100\n");

	return conteudoSaida;
}

