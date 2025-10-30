// paging.c

#include "paging.h"

// Declare global page directory and page table
struct page_directory_entry pd[1024] __attribute__((aligned(4096)));
struct page pt[1024] __attribute__((aligned(4096)));

// Function to map pages from physical memory to virtual memory
void *map_pages(void *vaddr, struct ppage *pglist, struct page_directory_entry *pd) {
    uint32_t vaddr_start = (uint32_t)vaddr;
    uint32_t pt_idx = vaddr_start >> 22;  // Page directory index (bits 22-31)
    uint32_t pt_entry = (vaddr_start >> 12) & 0x3FF;  // Page table index (bits 12-21)

    // Check if the page directory entry is valid
    if (!pd[pt_idx].present) {
        // Allocate a new page table if not present
        struct page *new_pt = &pt[pt_idx];
        pd[pt_idx].frame = ((uint32_t)new_pt) >> 12;  // Set the frame address
        pd[pt_idx].present = 1;  // Mark as present
        pd[pt_idx].rw = 1;  // Allow read-write
        pd[pt_idx].user = 0;  // Kernel space only
    }

    // Map the physical address to the virtual address
    struct page *page_entry = &pt[pt_idx];
    page_entry[pt_entry].frame = ((uint32_t)pglist->physical_addr) >> 12;
    page_entry[pt_entry].present = 1;
    page_entry[pt_entry].rw = 1;
    page_entry[pt_entry].user = 0;  // Kernel space only

    return (void *)vaddr;
}

// Load page directory into CR3 register
void loadPageDirectory(struct page_directory_entry *pd) {
    asm("mov %0, %%cr3"
        :
        : "r"(pd)
        :);
}

