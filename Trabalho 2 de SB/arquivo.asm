TRIANGULO: EQU 1

SECTION TEXT
		INPUT		B
		INPUT		H
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV		DOIS
		STORE		R
		OUTPUT	R
		STOP

SECTION BSS
	B:		SPACE
	H:		SPACE
	R:		SPACE

SECTION DATA
	DOIS:	CONST		2
