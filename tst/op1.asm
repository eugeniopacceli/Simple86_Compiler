_extern1: DW B
MOV B, AX
SUB AX, BX
JS _a<b
MOV AX, B
SUB AX, CX
JS _<c
WRITE B
RET
_a<b: MOV B, BX
SUB BX, CX
JS _<c
WRITE B
RET
_<c: WRITE CX
RET