extern scanf, printf

section .data
f_in_double: dd `%lf`, 0
f_out_double: dd `%lf\n`, 0

section .text
global f1
global f2
global f3
    
;f1 = e^x + 2 = 2^(x*log2(e)) + 2
f1:
    push ebp
    mov ebp, esp
    sub esp, 16
    finit 
    fld qword[ebp+8]        ;st0 = x                           
    fldl2e                  ;st0 = log2(e) ,  st1 = x               
    fmul                    ;st0 = x*log2(e)                    
    fld1                    ;st0 = 1 ,  st1 = x*log2(e)        
    fld st1                 ;st0 = x*log2(e) , st1 = 1           
    fprem                   ;st0 = {x*log2(e)} , st1 = 1           
    f2xm1                   ;st0 = 2^{x*log2(e)} - 1 , st1 = 1     
    fadd          ;st0 = 2^{x*log2(e)}                                 
    fscale                  ;st0 = (2^{x*log2(e)} + 1)

    mov dword[esp], 2       
    fild dword[esp]
                                
    fadd         
    
    fstp qword[esp]
    mov eax, dword[esp+4]
    mov edx, dword[esp]
    add esp, 16
    leave
    ret
   
;f2 = -2x + 8 
f2:
   push ebp
   mov ebp, esp
   sub esp, 16
   finit
   fld qword[ebp+8];        ;st0 = x
   mov dword[esp], -2
   fild dword[esp]          ;st0 = -2 , st1 = x
   fmul                     ;st0 = -2x
                            
   mov dword[esp], 8
   fild dword[esp]          ;st0 = 8 , st1 = -2x
   fadd                     ;st0 = -2x + 8
   
   fstp qword[esp]
   mov eax, dword[esp+4]
   mov edx, dword[esp]
   
   add esp, 16
   leave
   ret 

;f3 = -5/x
f3:
    push ebp
    mov ebp, esp
    sub esp, 16
    mov dword[esp], -5      
    fild dword[esp]         ;st0 = -5
    fld qword[ebp+8]        ;st0 = x , st1 = -5
    fdiv                    ;st0 = -5/x
                            
    fstp qword[esp]
    mov eax, dword[esp+4]
    mov edx, dword[esp]
    add esp, 16
    leave
    ret
    
    