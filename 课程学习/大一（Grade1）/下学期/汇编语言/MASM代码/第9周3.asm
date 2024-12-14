DATAS SEGMENT
    string db 'he is 324 years old'
    strlen equ $-string
    strtmp db strlen dup(0) 
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
    lea si, string
    lea di, strtmp
    mov cx, strlen
    cld
    
    lop:
    	lodsb
    	cmp al, '9'
    		ja l1
    	cmp al, '0'
    		jb l1
    	jmp next
    
    l1:
    	stosb
    next:
    	loop lop
    	lea si, strtmp
    	lea di, string
    	mov cx, strlen
    	cld
    	rep movsb
    	
    	mov dx, offset string
    	mov ah, 09h
    	int 21h
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
