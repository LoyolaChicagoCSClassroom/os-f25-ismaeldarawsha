#include "page.h"

struct ppage physical_page_array[128];
struct ppage *free_list = 0;

// Initialize free page list
void init_pfa_list(void) {
    for (int i = 0; i < 128; i++) {
        physical_page_array[i].physical_addr = (void *)(i * 0x200000); // 2MB per page
        physical_page_array[i].next = (i < 127) ? &physical_page_array[i + 1] : 0;
        physical_page_array[i].prev = (i > 0) ? &physical_page_array[i - 1] : 0;
    }
    free_list = &physical_page_array[0];
}

// Allocate npages from the free list
struct ppage *allocate_physical_pages(unsigned int npages) {
    struct ppage *allocd_list = free_list;
    struct ppage *curr = allocd_list;

    for (unsigned int i = 0; i < npages - 1 && curr; i++) {
        curr = curr->next;
    }

    if (!curr) return 0; // Not enough pages

    free_list = curr->next;
    if (free_list)
        free_list->prev = 0;

    curr->next = 0;
    return allocd_list;
}

// Free a list of pages back to the free list
void free_physical_pages(struct ppage *ppage_list) {
    if (!ppage_list) return;

    struct ppage *curr = ppage_list;
    while (curr->next)
        curr = curr->next;

    curr->next = free_list;
    if (free_list)
        free_list->prev = curr;

    free_list = ppage_list;
}
