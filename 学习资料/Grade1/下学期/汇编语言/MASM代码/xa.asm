DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    MOV AL, 10101010b
  
; 清除BL寄存器（可选，但在这里是个好习惯）  
	MOV BL, 0  
  
; 开始提取奇数位  
	MOV CX, 5
	LOOP S
	
	S:
		SHR AL, 1
		PUSH AX
		AND AL, 00000001b
		ADD BL, AL
		CMP CX, 1
			JNE not_end
			JMP end_program
	
	not_end:
		SHL BL, 1
		POP AX
		SHR AL, 1
		LOOP S
		
	end_program:
	    MOV AH,4CH
    	INT 21H
		CODES ENDS
    	END START
