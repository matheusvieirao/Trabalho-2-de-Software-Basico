SECTION TEXT
		LOAD		3
		ADD			NUM
		STORE		R
		OUTPUT		R
		STOP

SECTION BSS
	R:		SPACE
SECTION DATA
	NUM:	CONST	-234