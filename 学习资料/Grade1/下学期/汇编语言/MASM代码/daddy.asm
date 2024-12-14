ASSUME CS:CODES,DS:DATAS,SS:STACKS

DATAS SEGMENT
    string1 db 'who is your daddy'
    string2 db 'PENTA KILL' 
DATAS ENDS

STACKS SEGMENT
    dw 8 dup(0)
STACKS ENDS

CODES SEGMENT
START:
    MOV AX,DATAS
    MOV DS,AX
    MOV AX STACKS
    MOV SS, AX
    MOV SP, 16
    MOV CX, 17
    MOV BX, OFFSET STRING1
    
  S1:
  	MOV al, [bx]
  	and al, 11011111b
  	mov [bx], al
  	inc bx
  	LOOP S1
  	
  	mov cx, 10
  	mov bx, offset string2
  	
  s2:
  	mov al, [bx]
  	or al, 00100000b
  	mov [bx], al
  	inc bx
  	loop s2
  	
    ;此处输入代码段代码
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
