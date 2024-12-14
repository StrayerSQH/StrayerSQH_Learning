DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX, 0123h
    MOV bx, 0456h
    add ax, bx
    add ax, ax
    ;此处输入代码段代码
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
