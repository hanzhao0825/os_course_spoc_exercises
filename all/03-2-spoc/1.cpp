#include <stdio.h>

int mem[4096];
int pdbr = 0x220;

int main() {
    for (int i = 0; i < 128; i++) {
        char c1[11], c2[11];
        scanf("%s %s:", c1, c2);
        for (int j = 0; j < 32; j++) {
            scanf("%x", &mem[32 * i + j]);
        }
    }
    int query;
    while (scanf("%d", &query)) {
        printf("Virtual Address %x:\n", query);
        int pdindex = (query >> 10) & 0x1F;
        int ptindex = (query >> 5) & 0x1F;
        int offset = query & 0x1F;
        int pde = mem[pdbr + pdindex];
        int pdevalid = (pde >> 7) & 1;
        int pt = pde & 0x7F;
        printf("  --> pde index:0x%x  pde contents:(valid %d, pfn 0x%x)\n", pdindex, pdevalid, pt);
        if (!pdevalid) {
            printf("    --> Fault (page directory entry not valid)\n\n");
            continue;
        }
        int pte = mem[((pde & 0x7F) << 5) + ptindex];
        int ptevalid = (pte >> 7) & 1;
        int pfn = pte & 0x7F;
        printf("    --> pte index:0x%x  pte contents:(valid %d, pfn 0x%x)\n", ptindex, ptevalid, pfn);
        if (!ptevalid) {
            printf("      --> Fault (page table entry not valid)\n\n");
            continue;
        }
        int pa = pfn << 5 | offset;
        printf("      --> Translates to Physical Address 0x%x --> Value: %x\n\n", pa, mem[pa]);
    }
}