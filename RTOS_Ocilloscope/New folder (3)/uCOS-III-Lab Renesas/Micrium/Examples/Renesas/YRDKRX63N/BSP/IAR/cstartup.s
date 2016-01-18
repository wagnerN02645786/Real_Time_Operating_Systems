/*---------------------------------------------------------------------------*/
/*                            - cstartup.s -                                 */
/*                                                                           */
/*       This module contains the code executed before the C "main"          */
/*       function is called.                                                 */
/*       The code can be tailored to suit special hardware needs.            */
/*                                                                           */
/*       Copyright 2009-2010 IAR Systems AB.                                 */
/*                                                                           */
/*       Entries:                                                            */
/*         __iar_program_start       C entry point                           */
/*                                                                           */
/*       $Revision: 2041 $                                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef __ROPI__
#error __ROPI__ not defined
#endif

;------------------------------------------------------------------------------
; Reset vector.
;------------------------------------------------------------------------------
; The C startup sequence.
;
; Calls __low_level_init to perform early user initialization, after which the
; static data areas are initialized if necessary. Then 'main' and finally 'exit'
; are called.
;------------------------------------------------------------------------------
        module    _CSTARTUP
        section   ISTACK:DATA:NOROOT
        section   USTACK:DATA:NOROOT
        section   .inttable:CONST:NOROOT
        section   .text:CODE:NOROOT(2)
        extern    _main
        extern    _exit
        extern    ___iar_data_init2
        public    __iar_program_start
        require   __iar_main_call
        extern    __iar_reset_vector
        require   __iar_reset_vector
        extern    _vector_table
        require   _vector_table
        require   __cinit
__iar_program_start:
;------------------------------------------------------------------------------
; After a reset, execution will start here.
;------------------------------------------------------------------------------
        code
        mvtc      #(SFE ISTACK), ISP  ; istack pointer

;------------------------------------------------------------------------------
; If the application swithes to user mode, the user stack pointer will
; need initialization.
;------------------------------------------------------------------------------
        ;mvtc      #(SFE USTACK), USP  ; stack pointer

;------------------------------------------------------------------------------
; If the application does not enable interrupts in low_level_init, then the
; initialization of INTB can wait until just before the call to main. This
; allows interrupt handlers and the vector to be placed in RAM.
;------------------------------------------------------------------------------
        mvtc      #(SFB .inttable), INTB ; vector address

        section   .text:CODE:NOROOT
        public    __iar_subn_handling
        code
__iar_subn_handling:
        mvtc      #0, fpsw            ; enable subnormal handling

        section   .text:CODE:NOROOT
        public    __iar_call_low_level_init
        extern    ___low_level_init
        code

;------------------------------------------------------------------------------
; If hardware must be initiated from assembly or if interrupts
; should be on when reaching main, this is the place to insert
; such code.
;------------------------------------------------------------------------------

__iar_call_low_level_init:
        bsr.a   ___low_level_init
        cmp     #0, r1
        beq     __iar_main_call

;------------------------------------------------------------------------------
; Data initialization.
;
; Initialize the static data areas.
;------------------------------------------------------------------------------
        section .text:CODE:NOROOT
        code
__cinit:
        bsr     ___iar_data_init2

;------------------------------------------------------------------------------
; Call C++ static constructors.
;------------------------------------------------------------------------------

        section .text:CODE:NOROOT
        public  __iar_cstart_call_ctors
        extern  ___call_ctors
        extern  SHT$$PREINIT_ARRAY$$Base
        extern  SHT$$INIT_ARRAY$$Limit
        code
__iar_cstart_call_ctors:
        mov.l   #SHT$$PREINIT_ARRAY$$Base, r1
        mov.l   #SHT$$INIT_ARRAY$$Limit, r2
        bsr.a   ___call_ctors

;------------------------------------------------------------------------------
; Call 'main' C function.
;------------------------------------------------------------------------------

        section .text:CODE:NOROOT
        public  __iar_cstart_end
        public  __iar_main_call

;------------------------------------------------------------------------------
; Switch to user mode here if desired.
;------------------------------------------------------------------------------

__iar_main_call:

;       Enable Interrupt
        MOV.L       #010000H,R5
        MVTC        R5,PSW

#if 0
;       Enter user mode
;       Change PSW PMbit (SuperVisor->User)
        MVFC        PSW,R1
        OR          #100000H,R1
        PUSH.L      R1
        MVFC        PC,R1
        ADD         #0AH,R1
        PUSH.L      R1
        RTE
        NOP
        NOP
#endif


        code
        bsr.a     _main
        bra.a     _exit
__iar_cstart_end:

        end
