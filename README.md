# Binary Code Analysis - Project 1
The program takes the name of the ELF binary and outputs the information about the segments and sections that the binary contains.

## Example
```bash
$ ./ian-proj1 hello
hello : 64 - bit ELF object
Segment Type         Raw type   Perm Offset  Size   Sections
0       PHDR         0x6        R--  64      728
1       INTERP       0x3        R--  792     28    .interp
2       LOAD         0x1        R--  0       1576  .interp .note.gnu.property .note.gnu.build-id .note.ABI-tag .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
3       LOAD         0x1        R-X  4096    373   .init .plt .plt.got .plt.sec .text .fini
4       LOAD         0x1        R--  8192    244   .rodata .eh_frame_hdr .eh_frame
5       LOAD         0x1        RW-  11704   600   .init_array .fini_array .dynamic .got .data
6       DYNAMIC      0x2        RW-  11720   496   .dynamic
7       NOTE         0x4        R--  824     48    .note.gnu.property
8       NOTE         0x4        R--  872     68    .note.gnu.build-id .note.ABI-tag
9       GNU_PROPERTY 0x6474e553 R--  824     48    .note.gnu.property
10      GNU_EH_FRAME 0x6474e550 R--  8208    52    .eh_frame_hdr
11      GNU_STACK    0x6474e551 RW-  0       0
12      GNU_RELRO    0x6474e552 R--  11704   584   .init_array .fini_array .dynamic .got
```

