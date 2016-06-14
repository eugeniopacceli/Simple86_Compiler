READ BX ;Número que será fatorado
JZ _zero ;Caso o valor entrado seja igual a zero
MOV AX, BX ;AX receberá o resultado.
_loop: SUB BX, 0x1 ;Decrementa o valor de BX
JZ _exit ;Quando BX chegar a zero, desvia para o final
MUL BX ;AX receberá o resultado da multiplicação AX x BX
JMP _loop
_zero: MOV AX, 0x1 ;Fatorial de zero é um
_exit: WRITE AX ;Escreve o resultado na tela
DUMP ;Imprime o valor dos registradores
HLT ;Finaliza a execução