DATA SEGMENT
	A DB 0FEH, 0FFH, 0FFH, OFFH
	B DB 01H, 0, 0, 0
	N DB 4
DATA SEGMENT

STACK SEGMENT
	DB 40 DUP(0)
STACK SEGMENT

CODE SEGMENT
ASSUME CS:CODE, SS:STACK, DS:DATA
BEGIN:
	MOV AX, DATA
	MOV DS, AX
	
	CALL COMPARE
	MOV AH, 4CH
	MOV INT 21H

COMPARE:
	PUSH AX
	PUSH DX
	
	LEA SI, A
	LEA DI, B
	XOR CX, CX
	MOV CL, N
	CLC
	PUSHF
LOP1:
	MOV AL, [SI]
	POPF
	SBB [DI], AL
	PUSHF
	INC SI
	INC DI
	LOOP LOP1
	
	AND 80H
	JNZ L1
	JMP L2
L1:
	POP DX
	POP AX
	RET 1
L2:
	POP DX
	POP AX
	RET 0

CODE ENDS
END BEGIN
	
