Solutions for Radare2
=====================

To understand what a binary does, we have to load it into Radare2. In this exercised, we don't need debugging yet, so the command is just a simple `r2 binary`

The first thing to do will always be to look at the properties of the binary using the command 

`i` - information

There we can see what platform and architecture this binary is for and also some other information (programming language, security-flags, ...)

Another important command is to analyze the binary. (note: This could also be done with the command line parameter `r2 binary -A`)

`aa` - analyze all


Exercise 1
----------

First we look at the properties of the binaries (using the `i` command):

File     | Bits | Architecture | OS
---------|------|--------------|--------
ex1_1.so |  64  | AMD x86-64   | linux
ex1_2.so |  32  | Intel 80386  | linux
ex1_3.so |  32  | i386         | windows
ex1_4.so |  64  | AMD 64       | windows

Here we see, that this are Windows and Linux binaries compiled for 32 and 64bit architectures.

The next step is to look at the function `func`. To achieve this, we need to analyze the code with `aa` and then look for our function-name in the list given by **a**nalyze **f**unction **l**ist `afl`

Since this list could be rather long, we can use the grep-command to find the function:

`afl~func`


To print the code of the function, we use **p**rint **d**isassembled **f**unction with the found function name:

`pdf @ functionname`

### ex1_1.so
`sym.func` seems to be the right one.

so, `pdf @ sym.func` gives us:

```
/ (fcn) sym.func 22
|          ; arg int arg_2_3      @ rbp+0x13
|          ; var int local_0_4    @ rbp-0x4
|          ; var int local_1      @ rbp-0x8
|          ;-- sym.func:
|          0x00000620    55             push rbp
|          0x00000621    4889e5         mov rbp, rsp
|          0x00000624    897dfc         mov dword [rbp-local_0_4], edi
|          0x00000627    8975f8         mov dword [rbp-local_1], esi
|          0x0000062a    8b45fc         mov eax, dword [rbp-local_0_4]
|          0x0000062d    3945f8         cmp dword [rbp-local_1], eax    ; [0x13:4]=256
|          0x00000630    0f4d45f8       cmovge eax, dword [rbp-local_1]
|          0x00000634    5d             pop rbp
\          0x00000635    c3             ret
```

### ex1_2.so
again, `sym.func` is the function we want:

```
/ (fcn) sym.func 15
|          ; arg int arg_2        @ ebp+0x8
|          ; arg int arg_3        @ ebp+0xc
|          ; arg int arg_4_3      @ ebp+0x13
|          ;-- sym.func:
|          0x000004b0    55             push ebp
|          0x000004b1    89e5           mov ebp, esp
|          0x000004b3    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=0
|          0x000004b6    39450c         cmp dword [ebp + 0xc], eax      ; [0x13:4]=256
|          0x000004b9    0f4d450c       cmovge eax, dword [ebp + 0xc]   ; [0xc:4]=0
|          0x000004bd    5d             pop ebp
\          0x000004be    c3             ret
```

### ex1_3.so
Now, we have a different function name (`sym.ex1_3.so_func`)

```
/ (fcn) sym.ex1_3.so_func 15
|          ; arg int arg_2        @ ebp+0x8
|          ; arg int arg_3        @ ebp+0xc
|          ; arg int arg_4_3      @ ebp+0x13
|          ;-- sym.ex1_3.so_func:
|          0x677c14b0    55             push ebp
|          0x677c14b1    89e5           mov ebp, esp
|          0x677c14b3    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=4
|          0x677c14b6    39450c         cmp dword [ebp + 0xc], eax      ; [0x13:4]=0
|          0x677c14b9    0f4d450c       cmovge eax, dword [ebp + 0xc]   ; [0xc:4]=0xffff 
|          0x677c14bd    5d             pop ebp
\          0x677c14be    c3             ret
```

### ex1_4.so

Here the name is similar to the one above (`sym.ex1_4.so_func`)

```
/ (fcn) sym.ex1_4.so_func 22
|          ; arg int arg_2        @ rbp+0x10
|          ; arg int arg_2_3      @ rbp+0x13
|          ; arg int arg_3        @ rbp+0x18
|          ;-- sym.ex1_4.so_func:
|          0x65dc1420    55             push rbp
|          0x65dc1421    4889e5         mov rbp, rsp
|          0x65dc1424    894d10         mov dword [rbp + 0x10], ecx     ; [0x10:4]=184
|          0x65dc1427    895518         mov dword [rbp + 0x18], edx     ; [0x18:4]=64
|          0x65dc142a    8b4510         mov eax, dword [rbp + 0x10]     ; [0x10:4]=184
|          0x65dc142d    394518         cmp dword [rbp + 0x18], eax     ; [0x13:4]=0
|          0x65dc1430    0f4d4518       cmovge eax, dword [rbp + 0x18]  ; [0x18:4]=64
|          0x65dc1434    5d             pop rbp
\          0x65dc1435    c3             ret

```

### Interpretation

If we look at the disassembled code, we see that they follow a similar structure:

1. base- and stack-pointer preparation
2. 64-bit only: copy registers into local variables on stack
3. load a value into eax
4. compare eax with another value
5. conditional move (`cmovge` - Move if Greater Than or Equal to Zero)
6. restore base-pointer and return

A hint what is going on here are the [calling conventions](https://en.wikipedia.org/wiki/X86_calling_conventions) for the different binaries (only the relevant are listed):

Architecture | OS      | Parameters in registers              | Parameter order on stack
-------------|---------|--------------------------------------|-------------------------
IA-32        | linux   |                                      | right to left
IA-32        | windows |                                      | right to left
x86-64       | linux   | RDI, RSI, RDX, RCX, R8, R9, XMM0–7   | right to left
x86-64       | windows | RCX/XMM0, RDX/XMM1, R8/XMM2, R9/XMM3 | right to left

The return value is in the `eax` register.

Now we see that the 2 function-parameters from `int func(int, int)` are either on the stack or in registers (depending on the architecture/OS). Since the difference in the code is only the copying of these parameters on the stack and the logic afterwards is the same we may assume that the base-logic of these 4 functions is the same. 
When we now look at the 2 lines `mov eax, ...` and `cmp dword ...`, we see that this is a comparison between the 2 parameters of the function. When the value of the local value is greater than the one already in `eax`, the instruction `cmovge` puts this local value on `eax`, which is also the return value.

So basically, a C-like code of this function would look like:
```
int func(int a, int b)
{
	if(a > b)
	{
		a = b;
	}
	return a;
}
```

or shorter:

```
int func(int a, int b)
{
	return (a > b) ? a : b;
}
```


Exercise 2
----------

Now we have another program fo inspect. First, we start Radare2 with `r2 ex2 -A` and look at the properties with `i`:

```
type     EXEC (Executable file)
file     ex2
fd       6
size     0x1334
blksz    0x0
mode     r--
block    0x100
format   elf
pic      false
canary   false
nx       true
crypto   false
va       true
bintype  elf
class    ELF32
lang     c
arch     x86
bits     32
machine  Intel 80386
os       linux
subsys   linux
endian   little
stripped false
static   false
linenum  true
lsyms    true
relocs   true
rpath    NONE
binsz    3673
```

Here we see, that the binary is a 32-bit Linux binary.

For the next step, let's see the functions (`afl`):

```
0x080482c0  34  1  entry0
0x080482a0  6  1  sym.imp.__libc_start_main
0x080482a6  10  2  fcn.080482a6
0x08048290  12  1  section..plt
0x0804829c  10  1  sub.__libc_start_main_192_29c
0x080482b0  6  1  section..plt.got
0x080482b8  42  1  section_end..plt.got
0x080482f0  4  1  sym.__x86.get_pc_thunk.bx
0x08048300  43  4  sym.deregister_tm_clones
0x0804832c  57  4  fcn.0804832c
0x08048365  41  3  fcn.08048365
0x08048390  43  8  sym.frame_dummy
0x080483bb  135  11  sym.main
0x08048450  93  4  sym.__libc_csu_init
0x080484ad  5  1  fcn.080484ad
0x080484b2  22  1  section_end..text
0x0804826c  35  3  section..init
0x0804828f  13  1  section_end..init
```

Also let's see where our entry-point is with `iM`.

```
[Main]
vaddr=0x080483bb paddr=0x000003bb
```
which is `sym.main`

Since this names gives us no additional info what is going on in the binary we take a look at the main-function (`pdf @ sym.main`):

```
/ (fcn) sym.main 135
|           ; arg int arg_0_3      @ ebp+0x3
|           ; arg int arg_3        @ ebp+0xc
|           ; var int local_1      @ ebp-0x4
|           ; DATA XREF from 0x080482d7 (entry0)
|           ;-- main:
|           ;-- sym.main:
|           0x080483bb    55             push ebp
|           0x080483bc    89e5           mov ebp, esp
|           0x080483be    83ec10         sub esp, 0x10
|           0x080483c1    c745fc000000.  mov dword [ebp-local_1], 0
|           0x080483c8    837d0803       cmp dword [ebp + 8], 3         ; [0x3:4]=0x1010146 
|       ,=< 0x080483cc    7407           je 0x80483d5                 
|       |   0x080483ce    b8ffffffff     mov eax, sym.imp.__gmon_start__ ; sym.imp.__gmon_start__
|      ,==< 0x080483d3    eb6b           jmp 0x8048440                
|      ||   ; JMP XREF from 0x080483cc (sym.main)
|      ||   ; JMP XREF from 0x0804843e (sym.main)
|      |`-> 0x080483d5    8b450c         mov eax, dword [ebp + 0xc]     ; [0xc:4]=0
|      |    0x080483d8    83c004         add eax, 4
|      |    0x080483db    8b10           mov edx, dword [eax]
|      |    0x080483dd    8b45fc         mov eax, dword [ebp-local_1]
|      |    0x080483e0    01d0           add eax, edx
|      |    0x080483e2    0fb600         movzx eax, byte [eax]
|      |    0x080483e5    84c0           test al, al
|     ,===< 0x080483e7    751b           jne 0x8048404                
|     ||    0x080483e9    8b450c         mov eax, dword [ebp + 0xc]     ; [0xc:4]=0
|     ||    0x080483ec    83c008         add eax, 8
|     ||    0x080483ef    8b10           mov edx, dword [eax]
|     ||    0x080483f1    8b45fc         mov eax, dword [ebp-local_1]
|     ||    0x080483f4    01d0           add eax, edx
|     ||    0x080483f6    0fb600         movzx eax, byte [eax]
|     ||    0x080483f9    84c0           test al, al
|    ,====< 0x080483fb    7507           jne 0x8048404                
|    |||    0x080483fd    b800000000     mov eax, 0
|   ,=====< 0x08048402    eb3c           jmp 0x8048440                
|   |||     ; JMP XREF from 0x080483e7 (sym.main)
|   |||     ; JMP XREF from 0x080483fb (sym.main)
|   |``---> 0x08048404    8b450c         mov eax, dword [ebp + 0xc]     ; [0xc:4]=0
|   |  |    0x08048407    83c008         add eax, 8
|   |  |    0x0804840a    8b10           mov edx, dword [eax]
|   |  |    0x0804840c    8b45fc         mov eax, dword [ebp-local_1]
|   |  |    0x0804840f    01d0           add eax, edx
|   |  |    0x08048411    0fb600         movzx eax, byte [eax]
|   |  |    0x08048414    84c0           test al, al
|  ,======< 0x08048416    7507           jne 0x804841f                
|  ||  |    0x08048418    b801000000     mov eax, 1
| ,=======< 0x0804841d    eb21           jmp 0x8048440                
| ||        ; JMP XREF from 0x08048416 (sym.main)
| |`------> 0x0804841f    8b450c         mov eax, dword [ebp + 0xc]     ; [0xc:4]=0
| | |  |    0x08048422    83c004         add eax, 4
| | |  |    0x08048425    8b10           mov edx, dword [eax]
| | |  |    0x08048427    8b45fc         mov eax, dword [ebp-local_1]
| | |  |    0x0804842a    01d0           add eax, edx
| | |  |    0x0804842c    0fb600         movzx eax, byte [eax]
| | |  |    0x0804842f    84c0           test al, al
| ========< 0x08048431    7507           jne 0x804843a                
| | |  |    0x08048433    b802000000     mov eax, 2
| ========< 0x08048438    eb06           jmp 0x8048440                
|           ; JMP XREF from 0x08048431 (sym.main)
| --------> 0x0804843a    8345fc01       add dword [ebp-local_1], 1
| | |  |    0x0804843e    eb95           jmp 0x80483d5                
| | |  |    ; JMP XREF from 0x08048438 (sym.main)
| | |  |    ; JMP XREF from 0x0804841d (sym.main)
| | |  |    ; JMP XREF from 0x08048402 (sym.main)
| | |  |    ; JMP XREF from 0x080483d3 (sym.main)
| `-`--`--> 0x08048440    c9             leave
\           0x08048441    c3             ret
```

What we see here is that there are quite a few jumps in the function, but no call to another function.

We can now try to figure out what is going on here, but to make this easier for us we can use a different display mode to view the function.

First, we change into the visual mode with `V`. Now we can change the display mode to see the disassembly (cycle through the modes with `p` or `P`). In this mode, we can scroll through the listing with the cursor keys. We can now either find `sym.main` by our self or just jump to it with (press `v` to list the functions and select `sym.main` with the cursor. Then press `g` to jump to it)

At the beginning of the function, we see a listing of arguments given to the function and local variables.
```
|           ; arg int arg_0_3      @ ebp+0x3
|           ; arg int arg_3        @ ebp+0xc
|           ; var int local_1      @ ebp-0x4
```

When we look at `local_1`, we see that it is incremented at the bottom before we jump back to the beginning. else it is only used to load the variable into `eax`. So `local_1` will most like be a counter and we want to rename it for more comfortable reading.

To achieve this, we press `:`, so we can get a command line and then enter `afvn local_1 counter`. Now we can see `counter` instead of `local_1` in the listing of the function.

The two arguments are not very helpful here, because they do not occur in the listing. But since we know that the program usage is `./ex2 string1 string2`, we have 2 arguments (`int argc` and `char *argv[]`)

Now we look at the stack-layout of x86 c programs:

Address   | Content
----------|---------------
...       | ...
ebp + 0xC | argument 2
ebp + 0x8 | argument 1
ebp + 0x4 | return address
ebp       | saved ebp
ebp - 0x4 | local var 1
ebp - 0x8 | local var 2
...       | ...


`int argc` would be `[ebp + 8]` and `char *argv[]` would be `[ebp + C]`

A good way to get an overview of the function is to enter the graph mode by pressing `V` in the Visual mode.

The basic usage in this mode is:

`w`/`s`/`a`/`d` navigate (faster with `shift`)  
`cursor` move node (faster with `shift`)  
`t`/`f`/`u` walk around the nodes  

The graph will look like this (sorry, no color here -> just use radare2):

```
     =-------------------------=
     |    0x080483bb           |
     | push ebp                |
     | mov ebp, esp            |
     | sub esp, 0x10           |
     | mov dword [ebp - 4], 0  |
     | cmp dword [ebp + 8], 3  |
     | je 0x80483d5            |
     =-------------------------=
          t f
          | |
          | '---------.
          |           |
          |    =---------------------------------=
          |    | -[ 0x080483ce ]-                |
          |    | mov eax, sym.imp.__gmon_start__ |
          |    | jmp 0x8048440                   |
          |    =---------------------------------=
          |       v
          |       |
        .---------'
        |-------------------------------------------------------------.
     =----------------=    |       =----------------------------=     |
     |    0x08048440  |    |       |    0x080483e9     |        |     |
     | leave          |    |       | mov eax, dword [ebp + 0xc] |     |
     | ret|           |    |       | add eax, 8        |        |     |
     =----------------=    |       | mov edx, dword [eax]       |     |
          |                |       | mov eax, dword [ebp - 4]   |     |
          |                |       | add eax, edx      |        |     |
          |                |       | movzx eax, byte [eax]      |     |
          |                |       | test al, al       |        |     |
          |                |       | jne 0x8048404     |        |     |
          |                |       =----------------------------=     |
          |                |            t f            |              |
          |                |            | |            |              |
          |         .-------------------' '---------------------------|-----------------------------------------------------------------.
          |    .----|      |             .-------------|------. .-----|-----------------------------------.                             |
          |    =----------------------------=          | =----------------------------=            =----------------=            =----------------=
          |    |    0x08048404           |  |          | |    0x0804841f              |            |    0x08048418  |            |    0x080483fd  |
          |    | mov eax, dword [ebp + 0xc] |          | | mov eax, dword [ebp + 0xc] |            | mov eax, 1     |            | mov eax, 0     |
          |    | add eax, 8|             |  |          | | add eax, 4 |               |            | jmp 0x8048440  |            | jmp 0x8048440  |
          |    | mov edx, dword [eax]    |  |          | | mov edx, dword [eax]       |            =----------------=            =----------------=
          |    | mov eax, dword [ebp - 4]|  |          | | mov eax, dword [ebp - 4]   |               v                             v
          |    | add eax, edx            |  |          `-|-add eax, edx---------------|---------------------------------------------'
          |    | movzx eax, byte [eax]   |  |            | movzx eax, byte [eax]      |
          |    | test al, al             |  |            | test al, al                |
          |    | jne 0x804841f           |  |            | jne 0x804843a              |
          |    =----------------------------=            =----------------------------=
          |    |    t f    |             |                    t f
          |    |    `-`----|----------------------------------|-|
          .----------------|----------------------.-------------'
          |    |           |                      |
     =------------------------=            =----------------=
     |    0x0804843a       |  |            |    0x08048433  |
     | add dword [ebp - 4], 1 |            | mov eax, 2     |
     | jmp 0x80483d5       |  |            | jmp 0x8048440  |
     =------------------------=            =----------------=
        v |    |           |                  v
        | |    |           `------------------'
        | |    |           |
        | |    |           |
     =----------------------------=
     |    0x080483d5       |      |
     | mov eax, dword [ebp + 0xc] |
     | add eax, 4          |      |
     | mov edx, dword [eax]|      |
     | mov eax, dword [ebp - 4]   |
     | add eax, edx        |      |
     | movzx eax, byte [eax]      |
     | test al, al         |      |
     | jne 0x8048404       |      |
     =----------------------------=
          t f  |           |
          `-`--------------'

```

With this, we may now try to figure out what this program does.

Let's start with the first node
```
=-------------------------=
| -[ 0x080483bb ]-        |
| push ebp                |
| mov ebp, esp            |
| sub esp, 0x10           |
| mov dword [ebp - 4], 0  |
| cmp dword [ebp + 8], 3  |
| je 0x80483d5            |
=-------------------------=
```

Here we first have the usual `epb`/`esp` stuff, which is done automatically in C. Then `counter` is initialized with 0 and `argc` is compared with 3. When we now follow the jump when `argc` is not 3 (press `f`), we see that the program quits.

The code-part `mov eax, sym.imp.__gmon_start__ ` may be confusing, but looking at the instruction, we see that `b8ffffffff` means `mov eax, 0xffffffff`, which is `-1`, because we return an integer.

So, this would be identical to the following C-snippet (assuming the counter is a 32-bit integer):

```
int main(int argc, char* argv[])
{
	int counter = 0;
	if(argc == 3)
	{
		...
	}
	return;
}

```

Now we continue with the next node after `argc` is true:

```
=----------------------------=
| -[ 0x080483d5 ]-           |   
| mov eax, dword [ebp + 0xc] |
| add eax, 4                 |
| mov edx, dword [eax]       |
| mov eax, dword [ebp - 4]   |
| add eax, edx               |
| movzx eax, byte [eax]      |
| test al, al                |
| jne 0x8048404              |
=----------------------------=
```

We might notice, that there are 3 similar blocks like this (different `jne`-address and/or different addition to `eax` in the second line)

To make notes while analyzing the block we can make comments in the visual mode with `;`

```
mov eax, dword [ebp + 0xc]    ; [0xc:4]=0 ; load char* argv[]                                                                                                            
add eax, 4                    ; get the address of argv[1]                                                                                                                                                                                         
mov edx, dword [eax]          ; load argv[1]                                                                                                                              
mov eax, dword [ebp-local_1]  ; load i                                                                                                                                    
add eax, edx                  ; load the address of argv[1][i]                                                                                                            
movzx eax, byte [eax]         ; load the byte of argv[1][i] into eax                                                                                                      
test al, al                   ; check if zero                                                                                                                             
jne 0x8048404                 ;[3]                     
```

Which is equally to the c-code `if (argv[1][i]==0)` or `argv[2][i]==0` for some of the other similar blocks).

Using this we can step through the graph to rebuild a c-code:


```
int main(int argc, char* argv[])
{
	int counter = 0;
	if(argc == 3)
	{
start:
		if (argv[1][counter]==0)
		{
			if (argv[2][counter]==0)
			{
				return 0;
			}
			else
			{
				counter++;
				goto start;
			}
		}
		else
		{
			if (argv[2][counter]==0)
			{
				return 1;
			}
			else
			{
				if (argv[1][counter]==0)
				{
					return 2;
				}
				else
				{
					counter++;
					goto start;
				}
				
			}
		}

	}
	return -1;
}

```

after some cleaning-up we get:

```
int main(int argc, char* argv[])
{
	int counter = 0;
	if(argc == 3)
	{
		while(1)
		{
			if (argv[1][counter]==0)
			{
				if (argv[2][counter]==0)
				{
					return 0;
				}
			}
			else
			{
				if (argv[2][counter]==0)
				{
					return 1;
				}
				else
				{
					if (argv[1][counter]==0)
					{
						return 2;
					}
				}
			}
		}

	}
	return -1;
}

```

So this program returns which of the 2 given strings is smaller (or `0` if they are equally long) and `-1` when the wrong amount of arguments is provided.

Exercise 3
----------

_Please note: Since stepping through all of the instructions like in example 2, we try now a more abstract approach. Our main-goal is to figure out what is going on in the program, so it is sufficient that we only analyze the parts, which are unclear and need deeper inspection._

Again, we load the binary with `r2 ex3 -A` and look at the properties `i` and what the name of the main-function is.

It is a 32-bit Linux binary and the name of the main-function is again `sym.main`

The function-listing (`afl`) is now:
```
0x08048360  34  1  entry0
0x08048340  6  1  sym.imp.__libc_start_main
0x08048346  10  2  fcn.08048346
0x08048300  12  1  section..plt
0x0804830c  10  1  sub.printf_108_30c
0x08048316  10  1  fcn.08048316
0x08048320  6  1  sym.imp.tolower
0x08048326  10  1  fcn.08048326
0x08048330  6  1  sym.imp.strlen
0x08048336  10  1  fcn.08048336
0x08048350  6  1  section..plt.got
0x08048358  42  1  section_end..plt.got
0x08048390  4  1  sym.__x86.get_pc_thunk.bx
0x080483a0  43  4  sym.deregister_tm_clones
0x080483cc  57  4  fcn.080483cc
0x08048405  41  3  fcn.08048405
0x08048430  43  8  sym.frame_dummy
0x0804845b  38  1  sym.swap
0x08048481  92  6  sym.getLetterCount
0x080484dd  88  6  sym.getMOLetter
0x08048535  235  7  sym.main
0x08048620  93  4  sym.__libc_csu_init
0x0804867d  5  1  fcn.0804867d
0x08048682  22  1  section_end..text
0x080482d0  35  3  section..init
0x080482f3  25  1  section_end..init
```

Well, here we have some new interesting sounding functions:
- `sym.swap`
- `sym.getLetterCount`
- `sym.getMOLetter`

Now let's take a look at the main-function. using the commands `s sym.main` to search the function and `pdf` to display it:

```
/ (fcn) sym.main 235
|          ; var int local_2      @ ebp-0x8
|          ; var int local_3      @ ebp-0xc
|          ; var int local_4      @ ebp-0x10
|          ; DATA XREF from 0x08048377 (entry0)
|          ;-- main:
|          ;-- sym.main:
|          0x08048535    8d4c2404       lea ecx, dword [esp + 4]        ; 0x4 
|          0x08048539    83e4f0         and esp, 0xfffffff0
|          0x0804853c    ff71fc         push dword [ecx - 4]
|          0x0804853f    55             push ebp
|          0x08048540    89e5           mov ebp, esp
|          0x08048542    53             push ebx
|          0x08048543    51             push ecx
|          0x08048544    83ec10         sub esp, 0x10
|          0x08048547    89cb           mov ebx, ecx
|          0x08048549    833b02         cmp dword [ebx], 2              ; [0x2:4]=0x101464c 
|      ,=< 0x0804854c    7420           je 0x804856e                  
|      |   0x0804854e    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|      |   0x08048551    8b00           mov eax, dword [eax]
|      |   0x08048553    83ec08         sub esp, 8
|      |   0x08048556    50             push eax
|      |   0x08048557    68a0860408     push str.Program_usage:__s_string ; "Program usage: %s string" @ 0x80486a0
|      |   0x0804855c    e8affdffff     call sym.imp.printf             ; sub.printf_108_30c+0x4 ;sub.printf_108_30c(unk, unk) ; sym.imp.printf
|      |   0x08048561    83c410         add esp, 0x10
|      |   0x08048564    b8ffffffff     mov eax, sym.imp.__gmon_start__ ; sym.imp.__gmon_start__
|     ,==< 0x08048569    e9a8000000     jmp 0x8048616                 
|     ||   ; JMP XREF from 0x0804854c (sym.main)
|     |`-> 0x0804856e    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|     |    0x08048571    83c004         add eax, 4
|     |    0x08048574    8b00           mov eax, dword [eax]
|     |    0x08048576    83ec0c         sub esp, 0xc
|     |    0x08048579    50             push eax
|     |    0x0804857a    e8b1fdffff     call sym.imp.strlen ;sym.imp.strlen(unk)
|     |    0x0804857f    83c410         add esp, 0x10
|     |    0x08048582    8945f0         mov dword [ebp-local_4], eax
|     |    0x08048585    c745f4000000.  mov dword [ebp-local_3], 0
|    ,===< 0x0804858c    eb31           jmp 0x80485bf                 
|          ; JMP XREF from 0x080485ce (sym.main)
|   .----> 0x0804858e    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|   |||    0x08048591    83c004         add eax, 4
|   |||    0x08048594    8b00           mov eax, dword [eax]
|   |||    0x08048596    8b55f0         mov edx, dword [ebp-local_4]
|   |||    0x08048599    83ea01         sub edx, 1
|   |||    0x0804859c    2b55f4         sub edx, dword [ebp-local_3]
|   |||    0x0804859f    01c2           add edx, eax
|   |||    0x080485a1    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|   |||    0x080485a4    83c004         add eax, 4
|   |||    0x080485a7    8b08           mov ecx, dword [eax]
|   |||    0x080485a9    8b45f4         mov eax, dword [ebp-local_3]
|   |||    0x080485ac    01c8           add eax, ecx
|   |||    0x080485ae    83ec08         sub esp, 8
|   |||    0x080485b1    52             push edx
|   |||    0x080485b2    50             push eax
|   |||    0x080485b3    e8a3feffff     call sym.swap ;sym.swap(unk, unk)
|   |||    0x080485b8    83c410         add esp, 0x10
|   |||    0x080485bb    8345f401       add dword [ebp-local_3], 1
|   ||     ; JMP XREF from 0x0804858c (sym.main)
|   |`---> 0x080485bf    8b45f0         mov eax, dword [ebp-local_4]
|   | |    0x080485c2    89c2           mov edx, eax
|   | |    0x080485c4    c1ea1f         shr edx, 0x1f
|   | |    0x080485c7    01d0           add eax, edx
|   | |    0x080485c9    d1f8           sar eax, 1
|   | |    0x080485cb    3b45f4         cmp eax, dword [ebp-local_3]
|   `====< 0x080485ce    7fbe           jg 0x804858e                  
|     |    0x080485d0    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|     |    0x080485d3    83c004         add eax, 4
|     |    0x080485d6    8b00           mov eax, dword [eax]
|     |    0x080485d8    83ec08         sub esp, 8
|     |    0x080485db    50             push eax
|     |    0x080485dc    68b9860408     push str.Reversed:__s_n        ; "Reversed: %s." @ 0x80486b9
|     |    0x080485e1    e82afdffff     call sym.imp.printf             ; sub.printf_108_30c+0x4 ;sub.printf_108_30c(unk, unk) ; sym.imp.printf
|     |    0x080485e6    83c410         add esp, 0x10
|     |    0x080485e9    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
|     |    0x080485ec    83c004         add eax, 4
|     |    0x080485ef    8b00           mov eax, dword [eax]
|     |    0x080485f1    83ec0c         sub esp, 0xc
|     |    0x080485f4    50             push eax
|     |    0x080485f5    e8e3feffff     call sym.getMOLetter ;sym.getMOLetter(unk)
|     |    0x080485fa    83c410         add esp, 0x10
|     |    0x080485fd    0fbec0         movsx eax, al
|     |    0x08048600    83ec08         sub esp, 8
|     |    0x08048603    50             push eax
|     |    0x08048604    68c8860408     push str.Letter_with_the_most_occurrency_is:__c_n ; "Letter with the most occurrency is: %c." @ 0x80486c8
|     |    0x08048609    e802fdffff     call sym.imp.printf             ; sub.printf_108_30c+0x4 ;sub.printf_108_30c(unk, unk) ; sym.imp.printf
|     |    0x0804860e    83c410         add esp, 0x10
|     |    0x08048611    b800000000     mov eax, 0
|     |    ; JMP XREF from 0x08048569 (sym.main)
|     `--> 0x08048616    8d65f8         lea esp, dword [ebp-local_2]
|          0x08048619    59             pop ecx
|          0x0804861a    5b             pop ebx
|          0x0804861b    5d             pop ebp
|          0x0804861c    8d61fc         lea esp, dword [ecx - 4]
\          0x0804861f    c3             ret
```

Now we have calls to additional functions, some of them are c-functions (`sym.imp.printf` and `sym.imp.strlen`) and some are in the same binary (`sym.getMOLetter` and `sym.swap`)

The strings of the `printf` function are giving us very good hints what is going on.
First, we have a help-text to display the correct usage of the program. Then we will display the text 'Reversed: ' followed by a string and then we will display the letter with the most occurrences.

Similar to the second exercise, we have some local variables, which we can try to figure out what their purpose is:
- `local_4` is set to the result of strlen (0x08048582), so it should be better named length
- `local_3` is initialized with 0 (0x08048585), gets increased by 1 (0x080485bb), and gets compared with `length/2` (0x080485cb), so that must be a counter of something like a for-loop
- `local_2` is not clear, but since it is at the clean-up part of the program, we can ignore it (seems to be used to save ecx, ebx and ebp to ebp-0x8, ebp-0x4, ebp)

There variables can be renamed with `afvn name newname`

We might also notice, that `ebx` is used quiet often, and is only set once in the beginning to the address of `argc`

Also, we have some code-blocks with
```
mov eax, dword [ebx + 4]        ; [0x4:4]=0x10101 
add eax, 4
mov register, dword [eax]
```
which is like `register = argv[1]`

So what is the main-function doing now?

- At the beginning, `argc` is compared with `2`, and if it fails, a help-text about the program usage is displayed and the program quits. (0x08048549)
- Then, we initialize `length` with `strlen(argv[1])` and `counter` with `0`. (0x0804856e - 0x08048585)
- Afterwards we jump to a comparison if `counter` is smaller than `length/2`, and if it succeed we call `sym.swap` with `argv[1][counter]` and `argv[1][length-1-counter]`. From the function name, we can guess that the variables just get swapped in place, since we pass the addresses of the variables and not the value.
- When the loop is finished, we print `argv[1]`, which is now reversed.
- Next, `sym.getMOLetter` is called and the result is displayed with the format-text `Letter with the most occurrence is: %c`. This may give us the conclusion, that `getMOLetter` is returning the character of the letter with the highest occurrence.

So, with this, the task of this exercise is completed. We could also analyze the functions itself, but it is not really necessary, because we already know what is going on. However, when we inspect some malware or binaries with some obfuscation, we would still hate to take a look inside of these content, because we can never be sure that the functions does what the name says.

