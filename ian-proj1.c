#include "ian-proj1.h"

// Parses the arguments
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

// Returns the type string based on the p_type number
char * get_type(Elf64_Word type) {
    switch (type) {
        case 0x0:
            return "NULL";
        case 1:
            return "LOAD";
        case 0x2:
            return "DYNAMIC";
        case 0x3:
            return "INTERP";
        case 0x4:
            return "NOTE";
        case 0x5:
            return "SHLIB";
        case 0x6:
            return "PHDR";
        case 0x7:
            return "TLS";
        case 0x6474e550:
            return "GNU_EH_FRAME";
        case 0x6474e551:
            return "GNU_STACK";
        case 0x6474e552:
            return "GNU_RELRO";
        case 0x6474e553:
            return "GNU_PROPERTY";
        default:
            return "-";
    }
}

// Returns the permission string based on the permission number
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

    // Parses the arguments
    int fd = parse_args(argc, argv);

    if (elf_version(EV_CURRENT) == EV_NONE) {
        puts("Elf version is not compatible");
        return 1;
    }

    Elf * elf;

    // Opens the elf file
    if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL) {
        puts("Error opening elf file");
        return 1;
    }

    // Determines if the file is archive, elf or none
    if (elf_kind(elf) != ELF_K_ELF) {
        puts("Not an ELF file");
        return 1;
    }

    int arch;

    // Gets the architecture of the elf file
    if ((arch = gelf_getclass(elf)) == ELFCLASSNONE) {
        puts("Error unknown architecture");
        return 1;
    }

    printf("%s : %d - bit ELF object\n", argv[1],arch == ELFCLASS32 ? 32 : 64);

    size_t shstridx;

    // Gets the section header string table index
    if (elf_getshdrstrndx(elf, &shstridx) != 0) {
        puts("Error getting section header string table index");
        return 1;
    }

    size_t n;

    // Gets the number of segments
    if (elf_getphdrnum(elf, &n) != 0) {
        puts("Error while getting number of segments");
        return 1;
    }

    // Defines needed structs and variables
    GElf_Phdr phdr;
    char * type, * perm;
    Elf_Scn * section = NULL;
    GElf_Shdr shdr;
    char * name;

    // Prints the segments and their information
    printf("%-7s %-12s %-10s %-4s %-7s %-6s %-s\n", "Segment", "Type", "Raw type", "Perm", "Offset", "Size", "Sections");

    // Loops through the segments in the program header table
    for (size_t i = 0; i < n; ++i) {

        if (gelf_getphdr(elf, i, &phdr) != &phdr) {
            puts("Error getting program header table");
            return 1;
        }

        type = get_type(phdr.p_type);
        perm = get_perm(phdr.p_flags);

        // Prints the segments and their information
        printf("%-7ld %-12s %-#10x %-4s %-7ld %-6ld",i, type, phdr.p_type, perm, phdr.p_offset, phdr.p_filesz);

        // Loops through the sections in the section header table for each segment
        while ((section = elf_nextscn(elf, section)) != NULL) {

            // Gets the section header struct
            if (gelf_getshdr(section, &shdr) != &shdr) {
                puts("Error getting section header");
                return 1;
            }

            // If the current section is not in the current segment, continue
            if (shdr.sh_offset >= phdr.p_offset+phdr.p_filesz || shdr.sh_offset+shdr.sh_size <= phdr.p_offset) {
                continue;
            }

            // Gets the section name if it is in the current segment
            if ((name = elf_strptr(elf, shstridx, shdr.sh_name)) == NULL) {
                puts("Error getting section name");
                return 1;
            }
            else {
                printf("%s ", name); // Prints the section name
            }
        }
        puts("");
    }

    elf_end(elf);
    close(fd);

    return 0;
}
