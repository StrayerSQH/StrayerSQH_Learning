DATAS SEGMENT
    str DB 31 DUP(?)       ; �ַ�����������󳤶�Ϊ30��������β�Ŀ��ַ���
    newline DB 0DH, 0AH, '$'  ; �����ַ���
DATAS ENDS

STACKS SEGMENT
    DW 100H DUP(?)         ; ��ջ�ζ���
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES, DS:DATAS, SS:STACKS
START:
    MOV AX, DATAS          ; �����ݶε�ַ���ص��Ĵ���AX
    MOV DS, AX             ; �����ݶμĴ���DS����ΪAX��ֵ

    ; �ֶ������ַ���
    MOV AH, 0AH            ; ʹ��0AH�жϹ��ܣ������ַ���
    MOV DX, OFFSET str     ; �ַ����洢��ƫ�Ƶ�ַ
    INT 21H                ; ִ��21H�жϣ��ȴ��û������ַ���

    ; ����ַ���
    MOV AH, 09H            ; ʹ��09H�жϹ��ܣ�����ַ���
    MOV DX, OFFSET str     ; �ַ����洢��ƫ�Ƶ�ַ
    INT 21H                ; ִ��21H�жϣ�����ַ���

    ; �������
    MOV AH, 09H            ; ʹ��09H�жϹ��ܣ���������ַ���
    MOV DX, OFFSET newline ; �����ַ����洢��ƫ�Ƶ�ַ
    INT 21H                ; ִ��21H�жϣ���������ַ���

    ; �����˳�
    MOV AH, 4CH            ; ����AH�Ĵ���Ϊ4CH����ʾ�����˳�
    INT 21H                ; ִ��21H�жϣ��˳�����

CODES ENDS
    END START
