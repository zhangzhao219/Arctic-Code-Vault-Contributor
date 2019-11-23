assume cs:code,ds:data;将cs对应与代码段code,ds对应于数据段data
data segment
    ;MyArray dw 1,1,1;用16位整数的变量将16位补码数存储到数组中
    ;MyArray dw 1,1,0
    ;MyArray dw 1,0,1
    ;MyArray dw 0,1,1
    MyArray dw 0,1,2 
data ends

code segment

start:
    mov ax,data
    mov ds,ax;通过ax作为中介将data数据送到数据段ds中

first:
    mov dl,30h;将输出值dl设为0
    mov ax,ds:MyArray[0]
    cmp ax,ds:MyArray[2]
    jz Addfirst;
    jnz second;如果不相等将第二个数与第三个数进行比较

Addfirst:
    mov al,dl
    inc al
    mov dl,al

second:
    
    mov ax,ds:MyArray[0]
    cmp ax,ds:MyArray[4]
    jz Addsecond;如果相等再次调用Add_dl实现dl加1
    jnz third;如果不相等将第二个数与第三个数进行比较

Addsecond:
    mov al,dl
    inc al
    mov dl,al

third:
    mov al,dl
    cmp al,32h
    jz Output
    mov ax,ds:MyArray[2]
    cmp ax,ds:MyArray[4]
    jz Adddl
    jnz Output

Adddl:
    mov al,dl
    inc al
    mov dl,al

Output:
    mov ah,02h
    int 21h
    mov ah,4ch
    int 21h

code ends

end start
