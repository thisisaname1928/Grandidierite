ENTRY(kmain)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS {
    . = 0xffffffff80000000;

    .text : { 
        *(.text.entry)
        *(.text) 
    }

    .rodata (READONLY) : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }

    /DISCARD/ : { *(.comment) *(.eh_frame) }

    end = .;
}