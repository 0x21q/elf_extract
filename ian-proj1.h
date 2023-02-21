#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

int parse_args(int argc, char **argv);

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

char * get_type(Elf64_Word type);

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

char * get_perm(Elf64_Word perm);

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