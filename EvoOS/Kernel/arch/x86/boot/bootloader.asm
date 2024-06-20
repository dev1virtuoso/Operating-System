BITS 64

; Constants
KERNEL_LOAD_ADDRESS     equ 0x100000    ; Address where the kernel is loaded
KERNEL_ENTRY_POINT      equ KERNEL_LOAD_ADDRESS    ; Entry point of the kernel
KERNEL_STACK_ADDRESS    equ 0x90000    ; Address of the stack for the kernel
NUMBER_OF_SECTORS       equ 1         ; Number of sectors to load

start:
    ; Set up segment registers
    mov ax, 0x10       ; Kernel code segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Load kernel into memory
    mov rbx, KERNEL_LOAD_ADDRESS
    mov ah, 0x42       ; Function 42h - Extended Read Sectors From Drive
    mov al, NUMBER_OF_SECTORS
    mov ch, 0x00       ; Cylinder number
    mov dh, 0x00       ; Head number
    mov dl, 0x80       ; Boot device (e.g., first hard disk)
    mov rdi, rbx       ; Destination address to load the sectors
    int 0x13           ; BIOS interrupt to read disk

    ; Set up GDT
    lgdt [gdt_descriptor]

    ; Set up segment registers again
    mov ax, 0x10       ; Kernel code segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Enter protection mode
    mov eax, cr0
    or eax, 0x1        ; Set PE (Protection Enable) bit
    mov cr0, eax

    ; Set up page tables and enable paging
    mov eax, KERNEL_PAGE_DIRECTORY
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000 ; Set PG (Paging) bit
    mov cr0, eax

    ; Set up registers and stack for kernel entry point
    mov rsp, KERNEL_STACK_ADDRESS
    mov rbp, 0         ; Clear rbp register if necessary

    ; Jump to the kernel entry point
    jmp KERNEL_ENTRY_POINT

; Constants
gdt_start:
    dq 0x0000000000000000    ; Null descriptor
    ; Kernel code segment descriptor
    dq 0x00AF9A000000FFFF    ; Limit=FFFFh, Base=0, Type=Code, DPL=0, Present=1, Long Mode=1
    ; Kernel data segment descriptor
    dq 0x00AF92000000FFFF    ; Limit=FFFFh, Base=0, Type=Data, DPL=0, Present=1, Long Mode=1
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of the GDT
    dq gdt_start                ; Address of the GDT

; Page Tables
KERNEL_PAGE_DIRECTORY:
    ; Define your page directory entries here

; Placeholder for page tables if necessary
