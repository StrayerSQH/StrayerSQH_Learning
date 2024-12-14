DATAS SEGMENT
    str DB 31 DUP(?)       ; 字符串变量，最大长度为30（包括结尾的空字符）
    newline DB 0DH, 0AH, '$'  ; 换行字符串
DATAS ENDS

STACKS SEGMENT
    DW 100H DUP(?)         ; 堆栈段定义
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES, DS:DATAS, SS:STACKS
START:
    MOV AX, DATAS          ; 将数据段地址加载到寄存器AX
    MOV DS, AX             ; 将数据段寄存器DS设置为AX的值

    ; 手动输入字符串
    MOV AH, 0AH            ; 使用0AH中断功能，接收字符串
    MOV DX, OFFSET str     ; 字符串存储的偏移地址
    INT 21H                ; 执行21H中断，等待用户输入字符串

    ; 输出字符串
    MOV AH, 09H            ; 使用09H中断功能，输出字符串
    MOV DX, OFFSET str     ; 字符串存储的偏移地址
    INT 21H                ; 执行21H中断，输出字符串

    ; 换行输出
    MOV AH, 09H            ; 使用09H中断功能，输出换行字符串
    MOV DX, OFFSET newline ; 换行字符串存储的偏移地址
    INT 21H                ; 执行21H中断，输出换行字符串

    ; 程序退出
    MOV AH, 4CH            ; 设置AH寄存器为4CH，表示程序退出
    INT 21H                ; 执行21H中断，退出程序

CODES ENDS
    END START
