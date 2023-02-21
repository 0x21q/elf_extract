#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

int parse_args(int argc, char **argv);
char * get_type(Elf64_Word type);

int parse_args(int argc, char **argv) {
    if (argc != 2) {
        puts("Usage: ./ian-proj1 <ELF binary>");
        exit(1);
    }
    int fd;
    if ((fd = open(argv[1], O_RDONLY, 0)) < 0) {
        puts("Error opening file");
        exit(1);
    }
    return fd;
}

char * get_type(Elf64_Word type) {
    switch (type) {
        case 0:
            return "NULL";
        case 1:
            return "LOAD";
        case 2:
            return "DYNAMIC";
        case 3:
            return "INTERP";
        case 4:
            return "NOTE";
        case 5:
            return "SHLIB";
        case 6:
            return "PHDR";
        case 7:
            return "TLS";
        default:
            return "-";
    }
}

char * get_perm(Elf64_Word perm) {
    switch(perm) {
        case 0:
            return "none";
        case 1:
            return "--X";
        case 2:
            return "-W-";
        case 3:
            return "-WX";
        case 4:
            return "R--";
        case 5:
            return "R-X";
        case 6:
            return "RW-";
        case 7:
            return "RWX";
        default:
            return "-";
    }
}

int main(int argc, char **argv) {
    Elf * elf;
    int arch;
    size_t n;

    int fd = parse_args(argc, argv);

    if (elf_version(EV_CURRENT) == EV_NONE) {
        puts("Elf version is not compatible");
        return 1;
    }

    if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL) {
        puts("Error opening elf file");
        return 1;
    }
    if (elf_kind(elf) != ELF_K_ELF) {
        puts("Not an ELF file");
        return 1;
    }

    if ((arch = gelf_getclass(elf)) == ELFCLASSNONE) {
        puts("Error getting architecture");
        return 1;
    }

    printf("%s : %d - bit ELF object\n", argv[1],arch == ELFCLASS32 ? 32 : 64);

    if (elf_getphdrnum(elf, &n) != 0) {
        puts("Error while getting number of segments");
        return 1;
    }

    GElf_Phdr phdr;

    printf("%-8s %-15s %-5s %-s\n", "Segment", "Type", "Perm", "Sections");

    for(size_t i = 0; i < n; ++i) {
        if (gelf_getphdr(elf, i, &phdr) != &phdr) {
            puts("Error getting program header table");
            return 1;
        }
        char * type = !strcmp(get_type(phdr.p_type), "-") ? "-" : get_type(phdr.p_type);
        char * perm = get_perm(phdr.p_flags);
        printf("%-8ld %-15s %-5s %-s\n", i, type, perm, "sections");
    }

    close(fd);
    return 0;
}
