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
    MOV AL, 0xAA  
  
; 清除BL寄存器（可选，但在这里是个好习惯）  
MOV BL, 0  
  
; 开始提取奇数位  
MOV CL, 4         ; 初始化计数器，因为我们要处理4次（针对AL的4个奇数位）  
SHL AL, 1         ; 将AL的所有位左移一位，以便我们可以从最低位开始  
  
NEXT_BIT:  
    ; 检查最低位（现在实际上是原来的奇数位）  
    AND AL, 0x01  ; 屏蔽除了最低位之外的所有位  
    JZ SKIP       ; 如果最低位是0，则跳过存储  
  
    ; 将最低位（奇数位）存储到BL的适当位置  
    ; 我们需要先将BL的当前位置清零，然后设置它  
    SHL BL, 1     ; 将BL的所有位左移一位以准备存储下一个奇数位  
    OR BL, 0x01   ; 设置BL的最低位（现在存储的是AL的原始奇数位）  
  
SKIP:  
    ; 准备处理下一个奇数位  
    SHR AL, 2     ; 将AL的所有位右移两位（因为我们已经检查了最低位，并且下一个奇数位在它后面两位）  
    DEC CL        ; 减少计数器  
    JNZ NEXT_BIT  ; 
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
