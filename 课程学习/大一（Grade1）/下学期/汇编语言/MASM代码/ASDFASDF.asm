DATA SEGMENT
	NUM DW 12H�� 35H, 20H, 60H
	LEN EUQ $-NUM
	COUNT DB ?
DATA ENDS

STACK SEGMENT
	DW 40H DUP(0)
STACK ENDS

CODE SEGMENT
ASSUME CS:CODE, DS:DATA, SS:STACK
BEGIN:MOV AX, DATA
	  MOV DS, AX
	  LEA SI, NUM
	  CALL NUMCOUNT
	  
	  MOV AH, 4CH
	  INT 21H

NUMCOUNT:PUSH AX
		 PUSH DX
	  	 MOV CL, LEN
	  	 S:AND [SI], 01H
	  	   JZ L1
	  	   JMP L2
	  	 POP DX
	  	 POP AX
	  	 RET
L1:INC SI
   LOOP S
 
L2:INC COUNT
   INC SI
   LOOP S	  

CODE ENDS
END BEGIN 	
