DATAS SEGMENT
    string1 db 'welcome to masm!'
    string2 db 50 dup(0)
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    ;�˴��������δ���
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
