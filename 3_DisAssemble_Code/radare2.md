
Solutions for Radare2
=====================

To assemble / disassemble binaries, we can use the tool `rasm2` from the radare2 suite.

To get an overview of the capabilities of rasm2, we can look at the command line parameters using `rasm2 -h`

```
Usage: rasm2 [-CdDehLBvw] [-a arch] [-b bits] [-o addr] [-s syntax]
             [-f file] [-F fil:ter] [-i skip] [-l len] 'code'|hex|-
 -a [arch]    Set architecture to assemble/disassemble (see -L)
 -b [bits]    Set cpu register size (8, 16, 32, 64) (RASM2_BITS)
 -c [cpu]     Select specific CPU (depends on arch)
 -C           Output in C format
 -d, -D       Disassemble from hexpair bytes (-D show hexpairs)
 -e           Use big endian instead of little endian
 -f [file]    Read data from file
 -F [in:out]  Specify input and/or output filters (att2intel, x86.pseudo, ...)
 -h           Show this help
 -i [len]     ignore/skip N bytes of the input buffer
 -k [kernel]  Select operating system (linux, windows, darwin, ..)
 -l [len]     Input/Output length
 -L           List supported asm plugins + features:
               a___ asm, _d__ disasm, __A_ analyzer, ___e ESIL
 -o [offset]  Set start address for code (default 0)
 -O [file]    Output file name (rasm2 -Bf a.asm -O a)
 -s [syntax]  Select syntax (intel, att)
 -B           Binary input/output (-l is mandatory for binary input)
 -v           Show version information
 -w           What's this instruction for? describe opcode
 If '-l' value is greater than output length, output is padded with nops
 If the last argument is '-' reads from stdin
```

Exercise 1
----------

The supported architectures can be listed using the command:

`rasm2 -L`

This gives us following list:


```
_dA_  8          8051        PD      8051 Intel CPU
_dA_  16 32      arc         GPL3    Argonaut RISC Core
adAe  16 32 64   arm         BSD     Capstone ARM disassembler
_dA_  16 32 64   arm.gnu     GPL3    Acorn RISC Machine CPU
_d__  16 32      arm.winedbg LGPL2   WineDBG's ARM disassembler
_dA_  16 32      avr         GPL     AVR Atmel
adAe  16 32      bf          LGPL3   Brainfuck
_dA_  16         cr16        LGPL3   cr16 disassembly plugin
_dAe  32         cris        GPL3    Axis Communications 32-bit embedded processor
_dA_  16         csr         PD      Cambridge Silicon Radio (CSR)
adA_  32 64      dalvik      LGPL3   AndroidVM Dalvik
ad__  16         dcpu16      PD      Mojang's DCPU-16
_dA_  32 64      ebc         LGPL3   EFI Bytecode
_dAe  16         gb          LGPL3   GameBoy(TM) (z80-like)
_dA_  16         h8300       LGPL3   H8/300 disassembly plugin
_d__  32         hppa        GPL3    HP PA-RISC
_d__             i4004       LGPL3   Intel 4004 microprocessor
_dA_  8          i8080       BSD     Intel 8080 CPU
adA_  32         java        Apache  Java bytecode
_d__  8          lh5801      LGPL3   SHARP LH5801 disassembler
_dA_  16 32      m68k        BSD     Motorola 68000
_dA_  32         malbolge    LGPL3   Malbolge Ternary VM
adAe  16 32 64   mips        BSD     Capstone MIPS disassembler
adAe  32 64      mips.gnu    GPL3    MIPS CPU
_d__  16 32 64   msil        PD      .NET Microsoft Intermediate Language
_dA_  16         msp430      LGPL3   msp430 disassembly plugin
_dAe  32         nios2       GPL3    NIOS II Embedded Processor
_dA_  32 64      ppc         BSD     Capstone PowerPC disassembler
_dA_  32 64      ppc.gnu     GPL3    PowerPC
ad__             rar         LGPL3   RAR VM
_dA_  32         sh          GPL3    SuperH-4 CPU
_dAe  32 64      sparc       BSD     Capstone SPARC disassembler
_dA_  32 64      sparc.gnu   GPL3    Scalable Processor Architecture
_d__  16         spc700      LGPL3   spc700, snes' sound-chip
_d__  32         sysz        BSD     SystemZ CPU disassembler
_dA_  32         tms320      LGPLv3  TMS320 DSP family
_dA_  32         v850        LGPL3   v850 disassembly plugin
_dA_  32         ws          LGPL3   Whitespace esotheric VM
_dAe  16 32 64   x86         BSD     Capstone X86 disassembler
a___  32 64      x86.nz      LGPL3   x86 handmade assembler
ad__  32         x86.olly    GPL2    OllyDBG X86 disassembler
a___  32         x86.tab     LGPL3   x86 table lookup assembler
_dAe  16 32 64   x86.udis    BSD     udis86 x86-16,32,64
_dAe  32         xcore       BSD     Capstone XCore disassembler
adA_  8          z80         NC-GPL2 Zilog Z80
_d__  8          z80.cr      LGPL    Zilog Z80
```

The columns are:
- flags
	- a : assemble
	- d : disassemble
	- A : analyze
	- e : emulate
- bits
- name
- license
- description

Exercise 2
----------

Using the output from exercise 1, we know that the name of the needed plugins for this example are
- `x86`
- `arm`

Translating a single instruction is pretty easy, we can just use the command 
`rasm2 'mov eax,0xdeadbeef'` to assemble into x86 code. However, sometimes it may be better to specify the architecture and bits explicitly (For example, when writing scripts for automatization): 
`rasm2 -a x86 -b 32 'mov eax,0xdeadbeef'`

This gives us `b8efbeadde` as result.

To assemble more than 1 command, we can use a semicolon (`;`) to separate the commands:

`rasm2 'push eax; push ebx; pop eax; pop ebx'`

The assembly for this code-junk is now `5053585b`

We can also specify to read from stdin using:

`echo 'push eax; push ebx; pop eax; pop ebx' | rasm2 -f -`

Which is very nice, because so we can also use for example a python script instead of the `echo` to write to stdin.

Exercise 3
----------

Disassembling a binary is similar to exercise 2, but with the additional flag `-d`:

`rasm2 -a x86 -d 'B4 00 B0 13 CD 10'`

Gives us
```
mov ah, 0
mov al, 0x13
int 0x10
```
