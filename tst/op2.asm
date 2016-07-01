_mod2: DW C
MOV C, AX
SUB AX, BX
JS _e2a<b ;A<B
MOV C, BX
SUB BX, CX
JS _e2<c ;B<C
WRITE CX
RET
_e2a<b: MOV AX, C
SUB AX, CX
JS _e2<c ;A<C
WRITE CX
RET 
_e2<c: WRITE C
RET
