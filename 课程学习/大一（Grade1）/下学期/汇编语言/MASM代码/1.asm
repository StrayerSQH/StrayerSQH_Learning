DATAS SEGMENT
    ;�˴��������ݶδ���  
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX, 0123h
    MOV bx, 0456h
    add ax, bx
    add ax, ax
    ;�˴��������δ���
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
