#include "ian-proj1.h"

int main(int argc, char **argv) {

    int fd = parse_args(argc, argv);

    if (elf_version(EV_CURRENT) == EV_NONE) {
        puts("Elf version is not compatible");
        return 1;
    }

    Elf * elf;
    // Getting the elf file
    if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL) {
        puts("Error opening elf file");
        return 1;
    }
    if (elf_kind(elf) != ELF_K_ELF) {
        puts("Not an ELF file");
        return 1;
    }

    int arch;
    // Getting the architecture of the elf file
    if ((arch = gelf_getclass(elf)) == ELFCLASSNONE) {
        puts("Error unknown architecture");
        return 1;
    }

    printf("%s : %d - bit ELF object\n", argv[1],arch == ELFCLASS32 ? 32 : 64);

    size_t n;
    // Getting the number of segments
    if (elf_getphdrnum(elf, &n) != 0) {
        puts("Error while getting number of segments");
        return 1;
    }

    GElf_Phdr phdr;
    char * type, * perm;

    // Printing the segments and their information
    printf("%-7s %-12s %-10s %-4s %-s\n", "Segment", "Type", "Raw type", "Perm", "Sections");

    for (size_t i = 0; i < n; ++i) {
        if (gelf_getphdr(elf, i, &phdr) != &phdr) {
            puts("Error getting program header table");
            return 1;
        }
        type = get_type(phdr.p_type);
        perm = get_perm(phdr.p_flags);
        printf("%-7ld %-12s %-#10x %-4s %-s\n", i, type, phdr.p_type, perm, "todo");
    }

    puts("");

    size_t shstridx;
    // Getting the section header string table index
    if (elf_getshdrstrndx(elf, &shstridx) != 0) {
        puts("Error getting section header string table index");
        return 1;
    }

    Elf_Scn * section = NULL;
    GElf_Shdr shdr;
    char * name;

    printf("%-7s %s\n", "Section", "Name");

    for (int i = 0; (section = elf_nextscn(elf, section)) != NULL; ++i) {
        // Getting the section header
        if (gelf_getshdr(section, &shdr) != &shdr) {
            puts("Error getting section header");
            return 1;
        }
        // Getting the section name
        if ((name = elf_strptr(elf, shstridx, shdr.sh_name)) == NULL) {
            puts("Error getting section name");
            return 1;
        }

        printf("%-7d %s\n", (i+1), name);
    }

    elf_end(elf);
    close(fd);
    return 0;
}
