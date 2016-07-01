_extern2: DW C
MOV C, AX
SUB AX, BX
JS _a<b ;A<B
MOV C, BX
SUB BX, CX
JS _<c ;B<C
WRITE CX
RET
_a<b: MOV AX, C
SUB AX, CX
JS _<c ;A<C
WRITE CX
RET 
_<c: WRITE C
RET