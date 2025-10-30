ENTRY(main)

SECTIONS {
    . = 1M;
    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    . = ALIGN(0x1000);
    _start_kernel = .;
    _end_kernel = . + SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.bss);
}
