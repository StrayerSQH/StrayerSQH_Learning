ASSUME CS:CODES, DS:DATAS, SS:STACKS
DATAS SEGMENT
	string1 db 'Who is your daddy'
	string2 db 'PENTY KILL'
	string3 db 'hahaha'
DATAS ENDS

STACKS SEGMENT
    dw 8 dup(0)
STACKS ENDS

CODES SEGMENT
    START:
    MOV AX,DATAS
    MOV DS,AX
    MOV AX, STACKS
    MOV SS, AX
    MOV SP, 16
    MOV CX, 6
    
    MOV BX, 0
    
    S1:
    	MOV AL, [BX + 001BH];001BH����string1��string2���ܳ���Ϊ27
    	AND AL, 11011111B;��СдתΪ��д
    	MOV [BX + 001BH], AL
    	INC BX
    	LOOP S1
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
