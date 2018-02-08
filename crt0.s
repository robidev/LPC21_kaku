/*
     uC/OSII interface

     crt0.s:
            startup-code en interrupt afhandeling

     opmerkingen:
                 -MEMMAP is op SRAM ingesteld: interrupt-vectors worden in SRAM geplaatst.
                 -Alleen IRQ en Reset exceptions zijn hier uitgewerkt. De andere vectors
                  verwijzen naar een C-functie in exceptions.c
                 -"A re-entrant interrupt handler (at least its 'top-level') *must* be written in assembler."
                  (aldus ARM). En ja, wij zijn re-entrant.
                 -Dit bestand is nogal afhankelijk van het Linkerscript; wil je precies begrijpen wat hier gebruikt,
                  dan moet je het linkerscript dus ook goed doornemen.
                 -Het systeem start in System-modus.
                 -__enable_interrupts en __disable_interrupts werken niet in user-modus.
*/

    .extern main                @ 'prototype' van de main

    .text
    .code 32
    .align 0

    @ De volgende worden in het linkerscript gedefineerd(daar wordt bepaald
    @ waar wat staat in het geheugen)
    .extern __bss_beg__ 
    .extern __bss_end__
    .extern __stack_end__
    .extern __data_beg__
    .extern __data_end__
    .extern __data_beg_src__

    .global _start
    .global endless_loop

    @ Definitie van ARM operating modes
    .set INIT_FIQ_MODE,         0x11
    .set INIT_IRQ_MODE,         0x12
    .set INIT_SUPERVISOR_MODE,  0x13
    .set INIT_ABORT_MODE,       0x17
    .set INIT_UNDEFINED_MODE,   0x1B
    .set INIT_SYSTEM_MODE,      0x1F

_start:                         @ HET startpunt; hier begint de code
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ opstart
ResetHandler:
    @ We beginnen met stack-initialisatie, voor elke mode een
    @ Grootes van stack zijn gedefineerd in het linkerscript
    msr     cpsr_c, #INIT_FIQ_MODE
    ldr     sp, =__stack_end_fiq__
    msr     cpsr_c, #INIT_IRQ_MODE
    ldr     sp, =__stack_end_irq__
    msr     cpsr_c, #INIT_SUPERVISOR_MODE
    ldr     sp, =__stack_end_supervisor__
    msr     cpsr_c, #INIT_ABORT_MODE
    ldr     sp, =__stack_end_abort__
    msr     cpsr_c, #INIT_UNDEFINED_MODE
    ldr     sp, =__stack_end_undefined__
    msr     cpsr_c, #INIT_SYSTEM_MODE
    ldr     sp, =__stack_end__

    sub     sl, sp, #512        @ StackLimit zit 512 bits boven de StackPointer

    mov     a2, #0              @ FramePointer..
    mov     fp, a2              @ ..op nul

    @ En dan nu, ongebruikt geheugen op nul stellen, 
    @ oftewel, clear .BSS (zie ook eens http://en.wikipedia.org/wiki/BSS)
    @
    @ Volgens de C-standaard moeten nieuwe variables NULL zijn (als in 'int i;')
    @ De compiler doet dit zelf niet, maar gaat ervanuit dat dit door het systeem wordt geregeld
    @ Laten wij nu eens het systeem zijn... dus, hier ongebruikt geheugen wissen.
    @
    ldr     r1, .LC1            @  __bss_beg__ laden; gedefineerd in het linkerscript
    ldr     r3, .LC2            @  __bss_end__ laden;        ''
    subs    r3, r3, r1          @ bepaal lengte van BSS (__bss_end__ - __bss_beg__)
    beq     .end_clear_loop     @ Als er geen ongeinitialiseerd geheugen is, is er hier 
                                @ ook niets te doen.. branch naar eind v. functie

    mov     r2, #0              @ En we vullen met...
.clear_loop:
    strb    r2, [r1], #1        @ Vul byte in r1, en schuif meteen door naar het volgende byte
    subs    r3, r3, #1          @ Lengte van BSS minus 1
    bgt     .clear_loop         @ Zijn we al klaar ?
.end_clear_loop:

    @ En vervolgens kopieren we de initialisatie-waardes voor .data van flash naar RAM
    @
    @ init-waardes voor variables worden in flash opgeslagen, en tijdens startup op de
    @ juiste plek(in RAM dus) geladen.
    @
    @ En inderdaad, dat laatste gebeurt hier
    @
    ldr     r1, .LC3            @ __data_beg__ laden; ook deze staat in het linkerscript
    ldr     r2, .LC4            @ __data_beg_src__ laden;   ''
    ldr     r3, .LC5            @ __data_end__ laden;       ''
    subs    r3, r3, r1          @ En bepaal lengte van dit blok (__data_end__ - __data_beg__)
    beq     .end_set_loop       @ Als er geen .data blok is om over te kopieren, is er hier 
                                @ ook niets te doen.. branch naar eind v. functie
.set_loop:
    ldrb    r4, [r2], #1        @ Byte inlezen van flash (en __data_beg_src__++)..
    strb    r4, [r1], #1        @ ..en opslaan in RAM(en __data_beg__++)
    subs    r3, r3, #1          @ hoeveelheid over te kopieren data min een
    bgt     .set_loop           @ tot er niets meer van over is
.end_set_loop:

    @ Eindelijk klaar..
    @
    @ O, wacht.. die interrupts moeten ook nog
    ldr     r0, =0xE01FC040     @ MEMMAP
    #ifdef RUN_FROM_RAM
    mov     r1, #2              @ interruptvectors staan in SRAM
    #elif defined(RUN_FROM_ROM)
    mov     r1, #1              @ interruptvectors staan in flash
    #else
        #error Invalid run-modus selected. Chose 'RUN_FROM_ROM' or 'RUN_FROM_RAM'
    #endif
    str     r1, [r0]
    ldr     r0, =0xFFFFF034     @ VICDefVectAddr
    ldr     r1, =__HaltIrq      @ IRQ-handler voor non-vectored interrupts
    str     r1, [r0]

    @ Dan toch eindelijk; roep de main() functie van de C-code aan
    @ main() krijgt geen argumenten mee:
    mov     r0, #0              @ argc leeg..
    mov     r1, #0              @ .. en argv ook
    ldr     lr, =ExitMain       @ return-adres (Voor als de main toch stopt..)
    b       main                @ Roep main aan
ExitMain:
    @ Woeps, da's niet goed; de main hoort niet te stoppen!
    @ Foutje van de programmeur dus. We blijven hier in een oneindige lus draaien
    @ om vreemde acties te voorkomen.
    mov     a1, r0              @ return-code v.d. main geven we door
    b       __HaltExit          @ Deze gooit een melding over de uart
endless_loop:                   @ Stel dat de bovenstaande functie toch stopt,..
    b       endless_loop        @ ..blijf dan hier hangen.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Interrupts uitschakelen
    .global __disable_interrupts
__disable_interrupts:
    mrs     r0, CPSR            @ status-reg inladen
    orr     r0, r0, #0xC0       @ IRQ en FIQ bits hoog (dus beiden uit)
    msr     CPSR_c, r0          @ status-reg weer terugzetten
    mov     pc, lr              @ en exit functie

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Interrupts inschakelen
    .global __enable_interrupts
__enable_interrupts:
    mrs     r0, CPSR            @ status-reg inladen
    bic     r0, r0, #0x80       @ IRQ bit laag (dus aan)
                                @
                                @ LET OP: we zetten de FIQ niet aan!!!
                                @
    msr     CPSR_c, r0          @ status-reg weer terugzetten
    mov     pc, lr              @ en exit functie

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ IRQ afhandeling
IRQHandler:
    stmfd   sp!, {r0-r3,r12,lr}     @ registers op stack
    ldr     lr, =ExitISR            @ return-adres (voor later)
    ldr     r0, =0xFFFFF030         @ VICVectAddr
    ldr     pc, [r0]                @ spring naar (C-)functie van VICVectAddr
ExitISR:
    ldr     r1, =0xFFFFF030         @ VICVectAddr
    str     r0, [r1]                @ update priority hardware
    ldmfd   sp!, {r0-r3,r12,lr}
    subs    pc, lr, #4              @ return van IRQ-interrupt

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Undefinded instruction
UndefHandler:
    b       __HaltUndef

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Software interrupt
SWIHandler:
    b       __HaltSwi

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prefetch abort
PabortHandler:
    subs    lr, lr, #4                  @ Zet returnadres goed
    b       __HaltPabort

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Data abort
DabortHandler:
    subs    lr, lr, #8                  @ Zet returnadres goed
    b       __HaltDabort

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ FIQ exception
FIQHandler:
    subs    lr, lr, #4                  @ Zet returnadres goed
    b       __HaltFiq

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Definitie van constanten.
@
@   "For Thumb, constants must be after the code since only
@    positive offsets are supported for PC relative addresses."
@
@ dusch...

    .align 0
.LC1: 
.word   __bss_beg__
.LC2:
.word   __bss_end__
.LC3:
.word   __data_beg__
.LC4:
.word   __data_beg_src__
.LC5:
.word   __data_end__

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Exception-table
.section .startup,"ax"
    .code 32 
    .align 0

    ldr pc, =ResetHandler       @ reset vector
    ldr pc, =UndefHandler       @ undefined instruction
    ldr pc, =SWIHandler         @ software interrupt
    ldr pc, =PabortHandler      @ prefetch abort
    ldr pc, =DabortHandler      @ data abort
    nop                         @ signature, gebruikt door flashloaders enz.
    ldr pc, =IRQHandler         @ IRQ
    ldr pc, =FIQHandler         @ IRQ

    .end

