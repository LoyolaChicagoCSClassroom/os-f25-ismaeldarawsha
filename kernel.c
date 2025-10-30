// kernel.c

#include "paging.h"

// Declare external symbol for end of kernel
extern uint32_t _end_kernel;

void main() {
    uint32_t esp;
    asm("mov %%esp, %0" : "=r" (esp));  // Get stack pointer

    // Temporary page structure for identity mapping
    struct ppage tmp;
    tmp.next = NULL;

    // Identity map kernel code (0x100000 -> &_end_kernel)
    for (uint32_t addr = 0x100000; addr < (uint32_t)&_end_kernel; addr += 4096) {
        tmp.physical_addr = addr;
        map_pages((void *)addr, &tmp, pd);
    }

    // Identity map the stack
    for (uint32_t addr = esp & 0xFFFFF000; addr >= esp - 4096 * 8; addr -= 4096) {
        tmp.physical_addr = addr;
        map_pages((void *)addr, &tmp, pd);
    }

    // Identity map the video buffer (0xB8000)
    tmp.physical_addr = 0xB8000;
    map_pages((void *)0xB8000, &tmp, pd);

    // Load the page directory into CR3 register
    loadPageDirectory(pd);

    // Enable Paging by modifying CR0 register
    asm("mov %cr0, %eax\n"
        "or $0x80000001, %eax\n"
        "mov %eax, %cr0");

    // Continue kernel initialization...
}

