DW _A
READ AX
READ BX
READ CX
MOV _A, AX
READ AX ;OP
SUB AX, 0x0001
JZ _extern1
SUB AX, 0x0001
JZ _extern2
SUB AX, 0x0001
JZ _extern3
JMP _exit
_extern1:
MOV AX, _A
EXTERN _mod1
JMP _exit
_extern2:
MOV AX, _A
EXTERN _mod2
JMP _exit
_extern3:
MOV AX, _A
EXTERN _mod3
JMP _exit
_exit: HLT