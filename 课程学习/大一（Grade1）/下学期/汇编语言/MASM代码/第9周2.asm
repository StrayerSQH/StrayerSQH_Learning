DATAS SEGMENT
    string1 db 'this course is over'
    string2 db 40 dup(0)
DATAS ENDS

STACKS SEGMENT
    db 16 dup(0)
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    mov es, ax
    mov ax, stacks
    mov ss, ax
    mov sp, 16
    
    lea si, string1
    lea di, string2
    
    mov cx, di
    sub cx, si
    cld
    ;cldָ�DF��־����Ϊ0����֤di��si����
    ;stdָ�DF��־����Ϊ1����֤di��si�ݼ�
    
    rep movsb
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
