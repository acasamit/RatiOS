global gdt_load
extern gdtp
gdt_load:
    lgdt [gdtp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:next_label
next_label:
    ret

extern keyboard_handler
extern PIC_sendEOI
global   irq1
;.align   4

irq1:
    cli
    pushad
    cld
    call keyboard_handler
    mov eax, 1
    call PIC_sendEOI
    popad
    iret
