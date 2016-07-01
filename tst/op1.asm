_mod1: DW B
MOV B, AX
SUB AX, BX
JS _e1a<b
MOV AX, B
SUB AX, CX
JS _e1<c
WRITE B
RET
_e1a<b: MOV B, BX
SUB BX, CX
JS _e1<c
WRITE B
RET
_e1<c: WRITE CX
RET
