DATAS SEGMENT
    string1 'welcome to masm!''0'
    string2 'what is teacher saying''0'
    string3 'i can hardly understand''0'
    string4 'i would better to sleep''0'
    
    table dw string1, string2, string3, string4
     
DATAS ENDS

STACKS SEGMENT
    dw 8 dup(0)
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    mov ax, stacks
    mov ss, ax
    mov sp, 16
    
    mov cx, 4
    mov bp, 0
    
    s:
    	push cx
    	mov bx, table[bp]
    	mov si, 0
    	mov di, 0
    	
    	mov cx, 50
    	s1:
    		mov al, [bx+si]
    		cmp al, '0'
    		je ok
    		cmp al, ' '
    		je ok1
    		and al, 11011111B
    		mov [bx+si], al
    		ok1:
    			inc si
    			inc di
    			loop s1
    		ok:
    			add dp, 2
    			pop cx
    			loop s
    ;此处输入代码段代码
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
