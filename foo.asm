;于渊《自己动手写操作系统》
;(ld的'-s'选项为"strip all")
;
;$ nasm -f elf foo.asm -o foo.o
;$ gcc -c bar.c -o bar.o
;$ ld -s hello.o bar.o -o foobar
;$ ./foobar
;the 2nd one
;$

extern choose;int choose(int a,int b);
[section .data];数据在此
num1st  dd  3
num2nd  dd  4
[section .text];代码在此

global _start   ;我们必须导出 _start 这个入口，以便让链接器识别
global myprint  ;导出这个函数为了让bar.c使用

_start:
    push    dword  [num2nd] ;'.
    push    dword  [num1st] ; |
    call    choose          ; |choose(num1st,num2nd);
    add     esp,8           ;/
    mov ebx,0
    mov eax,1
    int 0x00

myprint:
    mov edx,[esp+8];
    mov ecx,[esp+4];
    mov ebx,1
    mov eax,4
    int 0x80
    ret