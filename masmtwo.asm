assume cs:code, ds:data, ss:stack

stack segment  stack
    dw 100H dup (?)
stack ends

data segment
    dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;初始化数组，最大存储20个数，最后一位为数组大小位
    tip0 db 13,10,'Please input the total number you want to sort:$';提示读入排序的数字个数
    tip1 db 13,10,'Please input the numbers (separated by space):$';提示用户输入数字
    tip2 db 13,10,'Error! Please input the number between 0 and 99!$';若用户输入的数字不在0-99范围内，显示出错消息并退出
    tip3 db 13,10,'Error! Please input the total number between 0 and 20!$';若用户输入的数字总数不在0-20范围内，显示出错消息并退出
    tip4 db 13,10,'The sort result is:$';输出最终排序结果
data ends

code segment
;主程序部分---------------------------------------------------------------------------------

start: 
    mov ax, data
    mov ds, ax;准备工作，ds段指向data

    call total;调用读入数字总数的子程序
    call input;调用读入用户输入的数字的子程序
    call bubble;调用冒泡排序子程序
    call result;调用显示排序结果的子程序
    call exit;调用程序运行完退出程序的子程序

;子程序部分-----------------------------------------------------------------------------------
    
    ;total读入数字总数------------------------------------------------------------------------
    total:
        lea dx,tip0
        call dosoutstr;调用子程序dosoutstr提示读入排序的数字个数

        mov si,40;指示指针到数组的最后一位，准备读入数字总数

        call readtwo;调用子程序readtwo读取两位数
        
        mov ax,20
        cmp ax,[si]
        jl errort;判断输入数字是否小于20，不符合条件调用子程序errort,打印出错信息并退出

        mov ax,0
        cmp ax,[si]
        jg errort;判断输入数字是否大于0，不符合条件调用子程序errort,打印出错信息并退出
    ret

        ;errort错误消息------------------------------------------------------------------------
        errort:
            lea dx,tip3
            call dosoutstr
            call exit
        ret
        ;errort结束------------------------------------------------------------------------

    ;total结束-------------------------------------------------------------------------------

    ;input读入用户输入的数字------------------------------------------------------------------
    input:
        lea dx,tip1
        call dosoutstr;打印提示消息，提示用户输入排序的数字

        mov cx,[si];通过[si]中的总数设置循环次数
        mov si,0;si回到数组起始点，准备读入数据

        ;inputr开始循环------------------------------------------------------------------------
        inputr:
            push cx;保存循环总数

            call readtwo;调用子程序readtwo读取两位数

            mov ax,99
            cmp ax,[si]
            jl error;判断输入数字是否小于99，不符合条件调用子程序error,打印出错信息并退出

            mov ax,0
            cmp ax,[si]
            jg error;判断输入数字是否大于0，不符合条件调用子程序error,打印出错信息并退出

            call dosinput;调用子程序dosinput吸收空格符号

            add si,2;si指向数组的下一个元素
            pop cx;恢复循环总数

        loop inputr;循环操作
        ;inputr结束------------------------------------------------------------------------

    ret

        ;error错误消息---------------------------------------------------------------------
        error:
            lea dx,tip2
            call dosoutstr
            call exit
        ret
        ;error结束----------------------------------------------------------------------

    ;input结束------------------------------------------------------------------------

    ;bubble对数组元素进行冒泡排序---------------------------------------------------------
    bubble:
        mov si,40;指示指针到数组的最后一位，准备提取数字总数

        mov cx,[si];通过[si]中的总数设置循环次数
        sub cx,1;循环次数减1

        ;sort排序循环----------------------------------------------------------------------
        sort:  
            push cx;保存外层循环次数，控制外循环的cx值恰就是内层循环次数
            mov si,0;指向数组第一个元素，相当于内层循环时j=0

            ;inn内层循环----------------------------------------------------------------------
            inn: 
                mov ax,[si]
                cmp ax,[si+2];[si]与[si+2]比较

                jbe jumpswap;[si]<=[si+2]时不交换，跳转到jumpswap子程序
                xchg ax,[si+2];[si]>[si+2]时交换

                mov [si],ax;最终[si]与[si+2]交换完成
                
                ;jumpswap跳过交换-------------------------------------------------------------
                jumpswap: 
                    add si,2;不交换数字，直接进入下一次判断
                ;jumpswap结束------------------------------------------------------------------

            loop inn;内层循环
            ;inn内层循环结束-----------------------------------------------------------------

            pop cx;恢复外层循环的cx

        loop sort;外层循环
        ;sort结束----------------------------------------------------------------------

    ret
    ;bubble结束------------------------------------------------------------------------
    
    ;result输出排序结果-----------------------------------------------------------------
    result:
        lea dx,tip4
        call dosoutstr;调用子程序dosoutstr提示输入排序结果

        mov si,40;指示指针到数组的最后一位，准备提取数字总数
        mov cx,[si];通过[si]中的总数设置循环次数

        mov si,0;指向数组第一个元素

        mov bl,10;借助bl获得商和余数，从而获得完整的两位数

        ;resulto遍历数组中的所有元素并输出---------------------------------------------
        resulto:
            push cx;保存循环总数

            mov ax,[si];ax获得数组中的数
            div bl;al获得商,ah获得余数

            mov dl,al;al的商送到dl中准备输出
            add dl,30h;将ASCII码转换为十进制

            mov dh,ah;将ah中的余数暂存在dh中，防止dos中断时数据丢失

            call dosoutput;调用子程序dosoutput输出dl中的商，即为输出两位数中的高位数

            mov dl,dh;dh中的商送到dl中准备输出
            add dl,30h;将ASCII码转换为十进制

            call dosoutput;调用子程序dosoutput输出dl中的余数，即为输出两位数中的低位数

            mov dl,0h
            call dosoutput;调用子程序dosoutput输出一个空格

            add si,2;指针指向下一个元素
            pop cx;恢复循环次数

        loop resulto;循环操作
        ;resulto结束---------------------------------------------

    ret
    ;result结束-----------------------------------------------------------------

    ;exit程序运行结束退出
    exit:
        mov ax, 4c00h
        int 21h;退出程序
    ;exit结束-----------------------------------------------------------------------

    ;通用辅助子程序-----------------------------------------------------------------

        ;dosinput输入字符
        dosinput:
            mov ah,01h
            int 21h
        ret

        ;dosoutput输出字符
        dosoutput:
            mov ah,02h
            int 21h
        ret

        ;dosoutstr输出字符串
        dosoutstr:
            mov ah,09h
            int 21h
        ret

        ;readtwo读取两位数
        readtwo:
            mov bl,10;借助bl与读入的数字相乘，获得高位数

            call dosinput;调用子程序dosinput读入高位数
                
            sub al,30h;将ASCII码转换为十进制
            mul bl;高位数乘10
            mov [si],ax;将高位数送入数组

            call dosinput;调用子程序dosinput读入低位数

            sub al,30h;将ASCII码转换为十进制
            add [si],al;将低位数送入数组
        ret
    ;通用辅助子程序结束-------------------------------------------------------

;主程序部分结束----------------------------------------------------------------
code ends

end start