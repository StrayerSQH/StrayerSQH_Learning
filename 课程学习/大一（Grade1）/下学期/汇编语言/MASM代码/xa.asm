DATAS SEGMENT
    ;�˴��������ݶδ���  
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    MOV AL, 10101010b
  
; ���BL�Ĵ�������ѡ�����������Ǹ���ϰ�ߣ�  
	MOV BL, 0  
  
; ��ʼ��ȡ����λ  
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
