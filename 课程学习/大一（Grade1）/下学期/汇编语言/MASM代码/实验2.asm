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
    MOV  WORD PTR [0100H], 5564H
MOV  WORD PTR [0102H], 0C279H
MOV  AX, [0100H]
PUSH  AX
ADD   AL, [0102H]��˵����־λ�����ȷ����
ADC   AH, [0103H]��˵��ָ��ִ��ԭ��������־λ
MOV  [0104H], AX
POP    AX
SUB   AL, [0102H] ��˵����־λ�����ȷ����
SBB   AH, [0103H]��˵��ָ��ִ��ԭ��������־λ
MOV  [0106H], AX
��˵������ִ����Ϻ��ֵ�Ԫ[0104H]��[0106H]�е����ݡ�

    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
