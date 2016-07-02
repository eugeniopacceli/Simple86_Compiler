_mod1: DW _B
MOV _B, AX
SUB AX, BX
JS _e1a<b
MOV AX, _B
SUB AX, CX
JS _e1<c
WRITE _B
RET
_e1a<b: MOV _B, BX
SUB BX, CX
JS _e1<c
WRITE _B
RET
_e1<c: WRITE CX
RET