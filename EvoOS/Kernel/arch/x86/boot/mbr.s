BITS 16

start:
    jmp main

read_disk:
    mov ah, 0x42       ; Function 42h - Extended Read Sectors From Drive
    mov al, NUMBER_OF_SECTORS
    mov ch, 0x00       ; Cylinder number
    mov dh, 0x00       ; Head number
    mov dl, 0x80       ; Boot device (e.g., first hard disk)
    mov rdi, KERNEL_LOAD_ADDRESS ; Destination address to load the sectors
    int 0x13           ; BIOS interrupt to read disk
    jc disk_error      ; Jump to disk_error if read error occurred
    ret

main:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, KERNEL_STACK_ADDRESS ; Set stack pointer to KERNEL_STACK_ADDRESS

    call read_disk     ; Call the disk read function to load subsequent code

    jmp KERNEL_ENTRY_POINT     ; Jump to the loaded kernel entry point

disk_error:
    ; Handle disk read error here

    ; Infinite loop
    jmp $

; Constants
KERNEL_LOAD_ADDRESS     equ 0x10000    ; Address where the kernel is loaded
KERNEL_ENTRY_POINT      equ 0x10000    ; Entry point of the kernel
KERNEL_STACK_ADDRESS    equ 0x9000     ; Address of the stack for the kernel
NUMBER_OF_SECTORS       equ 1          ; Number of sectors to load

times 510 - ($ - $$) db 0
dw 0xAA55   ; Bootloader signature
