assume cs:code, ds:data, ss:stack
stack segment  stack
    dw 100H dup (?)
stack ends
data segment
    dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    tip0 db 13,10,'Please input the total number you want to sort:$'
    tip1 db 13,10,'Please input the numbers (separated by space:)$'
    tip2 db 13,10,'Error! Please input the number between 0 and 99!$'
    tip3 db 13,10,'The sort result is:$'
data ends
code segment
start: 
    mov ax, data
    mov ds, ax

    ; lea dx,tip0
    ; mov ah,09h
    ; int 21h;打印第一条提示消息,提示用户输入希望排序的数字总数


    ; mov ah,01h
    ; int 21h;读入需要排序的数据总数
    ; ;sub al,30h;将读入的ASCII码转化为十进制数



    lea dx,tip1
    mov ah,09h
    int 21h;打印提示消息，提示用户输入排序的数字

    ; mov ah,0
    ; mov ch,0
    mov cx,15;循环次数
    mov si,0;指示指针
    mov bl,10;两位数中的高位数
input:
    push cx

    mov ah,01h
    int 21h;输入数字，读入高位数
    
    sub al,30h;将ASCII码转换为十进制
    mul bl;高位数乘10
    mov [si],ax;将高位数送入数组

    mov ah,01h
    int 21h;读入低位数

    sub al,30h;将ASCII码转换为十进制
    add [si],al;将低位数送入数组

    mov ah,01h
    int 21h;吸收空格符号


    add si,2
    pop cx
    loop input;循环操作

;------------------------------------------------------------------------
;冒泡排序

    mov cx, 14   ;外层循环14趟
sort:  
    push cx          ;保存外层循环次数
         ;注意到外层第1次循环，内循环执行9次，外层第2次循环，内循环执行8次，...控制外循环的cx值恰就是内层循环次数
    mov si, 0        ;相当于内层循环时j=0
inner: 
    mov ax, [si]     ;(ax)即a[j]
    cmp ax, [si+2]   ;a[j]与a[j+1]比较
    jbe noswap       ;a[j]<=a[j+1]时不交换，等同于a[j]>a[j+1]时交换
    xchg ax, [si+2]  ;交换
    mov [si], ax     ;最终效果是a[j]与a[j+1]交换了
noswap: 
    add si, 2        ;要到下一个数了，相当于j++
    loop inner       ;内层循环
    pop cx           ;恢复外层循环的cx
    loop sort        ;外层循环

;----------------------------------------------------------------------------------------------------------
    lea dx,tip3
    mov ah,09h
    int 21h;打印第三条提示消息

    mov cx,15;循环15次打印数字
    mov si,0;指示指针
    mov bl,10;两位数分别打印，为除以10后的商和余数
s:
    push cx

    mov ax,[si]
    div bl;al获得商,ah获得余数

    mov dl,al;al的商送到dl中准备输出
    add dl,30h;将ASCII码转换为十进制

    mov dh,ah;将ah中的余数暂存在dh中

    mov ah,02h
    int 21h;输出dl中的商

    mov dl,dh;dh中的商送到dl中准备输出
    add dl,30h;将ASCII码转换为十进制

    mov ah,02h
    int 21h;输出dh中的余数

    mov dl,0h
    mov ah,02h
    int 21h;输出一个空格

    add si,2
    pop cx
    loop s;循环操作


    mov ax, 4c00h
    int 21h;退出程序
code ends

end start