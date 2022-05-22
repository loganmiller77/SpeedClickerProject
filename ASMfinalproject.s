.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can not contain a main function: the C file will
; define main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore () and be included in a comment delimited list below.
.global _write_0, _write_1, _wait_100microseconds, _wait_1millisecond



_wait_100microseconds:
              ;2 cycles for function call
    repeat #1594    ;1 cycle for repeat and the rest are nop
    nop
    return        ;3 cycles for the return


_wait_1millisecond:
            ;2cycles for function call
    bclr LATA,#0
        repeat #15994
    nop
    return        ;3 cycles for function call


_write_0:
   ;2cycles
   bset LATA,#0     ;1cycle
   repeat #4     ;1plus 5 cycles
   nop
   bclr LATA,#0    ;1cycle
   repeat #5   ;7 total cycles
   nop
   return      ;return and call is 5 cycles

 _write_1:
    ;2cycles
    bset LATA,#0  ;1cycle
    repeat #8 ;1plus 9
    nop
    bclr LATA,#0  ;1cycle
    repeat #1  ;3 total cycles
    nop
    return   ;5 for the call of next and return


