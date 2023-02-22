#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

int parse_args(int argc, char **argv);

char * get_type(Elf64_Word type);

char * get_perm(Elf64_Word perm);