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
    MOV AL, 0xAA  
  
; ���BL�Ĵ�������ѡ�����������Ǹ���ϰ�ߣ�  
MOV BL, 0  
  
; ��ʼ��ȡ����λ  
MOV CL, 4         ; ��ʼ������������Ϊ����Ҫ����4�Σ����AL��4������λ��  
SHL AL, 1         ; ��AL������λ����һλ���Ա����ǿ��Դ����λ��ʼ  
  
NEXT_BIT:  
    ; ������λ������ʵ������ԭ��������λ��  
    AND AL, 0x01  ; ���γ������λ֮�������λ  
    JZ SKIP       ; ������λ��0���������洢  
  
    ; �����λ������λ���洢��BL���ʵ�λ��  
    ; ������Ҫ�Ƚ�BL�ĵ�ǰλ�����㣬Ȼ��������  
    SHL BL, 1     ; ��BL������λ����һλ��׼���洢��һ������λ  
    OR BL, 0x01   ; ����BL�����λ�����ڴ洢����AL��ԭʼ����λ��  
  
SKIP:  
    ; ׼��������һ������λ  
    SHR AL, 2     ; ��AL������λ������λ����Ϊ�����Ѿ���������λ��������һ������λ����������λ��  
    DEC CL        ; ���ټ�����  
    JNZ NEXT_BIT  ; 
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
