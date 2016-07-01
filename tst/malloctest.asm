DW _ind
DW _ind2
MOV _ind, 0x3
MOV _ind2, 0x2
MOV AX, _ind2
ADD AX, _ind
WRITE AX ;Fim
HLT