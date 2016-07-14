Solutions for Radare2
=====================

To find out how the serial is checked, and how to generate our own serial we will use a combination of debugging and step-by-step reconstruction of the code. 

The concept of the reverse-engineering of these 3 exercises is basically the same:
- First, we will try to find out how and where the serial is verified
- Then, we will try to rebuild the function and verify it by using the same input in our function and the debugger and compare the result
- At last, we 'reverse' the verification to generate a valid serial and test it

_Note: Because the main-program is the same for each of the 2 exercises, it is only analyzed an explained in the first one._

Exercise 1
----------

We start the binary in radare2 with `r2 -Ad ./ex1 user 12345` and then view the main-function (`s main`, `pdf`)

```
/ (fcn) sym.main 159
|          ; var int local_2      @ ebp-0x8
|          ; var int local_3      @ ebp-0xc
|          ; var int local_4      @ ebp-0x10
|          ; DATA XREF from 0x08048367 (entry0)
|          ;-- main:
|          ;-- sym.main:
|          0x080484ac    8d4c2404       lea ecx, dword [esp + 4]        ; 0x4  ; 4
|          0x080484b0    83e4f0         and esp, 0xfffffff0
|          0x080484b3    ff71fc         push dword [ecx - 4]
|          0x080484b6    55             push ebp
|          0x080484b7    89e5           mov ebp, esp
|          0x080484b9    53             push ebx
|          0x080484ba    51             push ecx
|          0x080484bb    83ec10         sub esp, 0x10
|          0x080484be    89cb           mov ebx, ecx
|          0x080484c0    833b03         cmp dword [ebx], 3              ; [0x3:4]=-1 ; 3
|      ,=< 0x080484c3    741d           je 0x80484e2                  
|      |   0x080484c5    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=-1 ; 4
|      |   0x080484c8    8b00           mov eax, dword [eax]
|      |   0x080484ca    83ec08         sub esp, 8
|      |   0x080484cd    50             push eax
|      |   0x080484ce    68d0850408     push str.Usage:__s_username_serial_n ; "Usage: %s username serial." @ 0x80485d0
|      |   0x080484d3    e828feffff     call sym.imp.printf             ; sub.printf_48_2fc+0x4 ;sub.printf_48_2fc(unk, unk) ; sym.imp.printf
|      |   0x080484d8    83c410         add esp, 0x10
|      |   0x080484db    b8ffffffff     mov eax, sym.imp.__gmon_start__ ; sym.imp.__gmon_start__
|     ,==< 0x080484e0    eb5f           jmp 0x8048541                 
|     ||   ; JMP XREF from 0x080484c3 (sym.main)
|     |`-> 0x080484e2    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=-1 ; 4
|     |    0x080484e5    83c008         add eax, 8
|     |    0x080484e8    8b00           mov eax, dword [eax]
|     |    0x080484ea    83ec04         sub esp, 4
|     |    0x080484ed    6a0a           push 0xa                       ; 10
|     |    0x080484ef    8d55f0         lea edx, dword [ebp-local_4]
|     |    0x080484f2    52             push edx
|     |    0x080484f3    50             push eax
|     |    0x080484f4    e827feffff     call sym.imp.strtoul ;sym.imp.strtoul(unk, unk, unk)
|     |    0x080484f9    83c410         add esp, 0x10
|     |    0x080484fc    8945f4         mov dword [ebp-local_3], eax
|     |    0x080484ff    8b4304         mov eax, dword [ebx + 4]        ; [0x4:4]=-1 ; 4
|     |    0x08048502    83c004         add eax, 4
|     |    0x08048505    8b00           mov eax, dword [eax]
|     |    0x08048507    83ec08         sub esp, 8
|     |    0x0804850a    ff75f4         push dword [ebp-local_3]
|     |    0x0804850d    50             push eax
|     |    0x0804850e    e838ffffff     call sym.checkSerial ;sym.checkSerial(unk, unk)
|     |    0x08048513    83c410         add esp, 0x10
|     |    0x08048516    85c0           test eax, eax
|    ,===< 0x08048518    7412           je 0x804852c                  
|    ||    0x0804851a    83ec0c         sub esp, 0xc
|    ||    0x0804851d    68eb850408     push str.Serial_WRONG_         ; "Serial WRONG!" @ 0x80485eb
|    ||    0x08048522    e8e9fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|    ||    0x08048527    83c410         add esp, 0x10
|   ,====< 0x0804852a    eb10           jmp 0x804853c                 
|   ||     ; JMP XREF from 0x08048518 (sym.main)
|   |`---> 0x0804852c    83ec0c         sub esp, 0xc
|   | |    0x0804852f    68f9850408     push str.Serial_OK_            ; "Serial OK!" @ 0x80485f9
|   | |    0x08048534    e8d7fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|   | |    0x08048539    83c410         add esp, 0x10
|   |      ; JMP XREF from 0x0804852a (sym.main)
|   `----> 0x0804853c    b800000000     mov eax, 0
|     |    ; JMP XREF from 0x080484e0 (sym.main)
|     `--> 0x08048541    8d65f8         lea esp, dword [ebp-local_2]
|          0x08048544    59             pop ecx
|          0x08048545    5b             pop ebx
|          0x08048546    5d             pop ebp
|          0x08048547    8d61fc         lea esp, dword [ecx - 4]
\          0x0804854a    c3             ret
```

What do we have here:
- Check, if there are 2 arguments (`0x080484c0`)
- Conversion of `argv[2]` to an unsignedlong-value and store it in `local_3` (`0x080484e2` - `0x080484fc`)
- Call the `sym.checkSerial` function with parameters `argv[1]` and `local_3` (`0x080484fc` - `0x08048513`)
- Depending on the result we display if the serial is right or wrong (`0x08048516` - `0x0804853c`)

So, we could theoretically change the conditional jump at `0x08048518` to display the "Serial OK!" message even if the serial is invalid, but we want to make a serial generator here.

It also seems, that we cannot 'sniff' a valid serial here, we take a look at `sym.checkSerial` (`s sym.checkSerial`, `pdf`)

```
/ (fcn) sym.checkSerial 97
|          ; arg int arg_2        @ ebp+0x8
|          ; arg int arg_3        @ ebp+0xc
|          ; var int local_1      @ ebp-0x4
|          ; var int local_2      @ ebp-0x8
|          ; CALL XREF from 0x0804850e (sym.main)
|          ;-- sym.checkSerial:
|          0x0804844b    55             push ebp
|          0x0804844c    89e5           mov ebp, esp
|          0x0804844e    83ec10         sub esp, 0x10
|          0x08048451    c745fc000000.  mov dword [ebp-local_1], 0
|          0x08048458    8d450c         lea eax, dword [ebp + 0xc]      ; 0xc  ; 12
|          0x0804845b    8945f8         mov dword [ebp-local_2], eax
|      ,=< 0x0804845e    eb3d           jmp 0x804849d                 
|      |   ; JMP XREF from 0x080484a5 (sym.checkSerial)
|      |   0x08048460    8b55fc         mov edx, dword [ebp-local_1]
|      |   0x08048463    8b45f8         mov eax, dword [ebp-local_2]
|      |   0x08048466    01c2           add edx, eax
|      |   0x08048468    8b4dfc         mov ecx, dword [ebp-local_1]
|      |   0x0804846b    8b45f8         mov eax, dword [ebp-local_2]
|      |   0x0804846e    01c8           add eax, ecx
|      |   0x08048470    0fb600         movzx eax, byte [eax]
|      |   0x08048473    89c1           mov ecx, eax
|      |   0x08048475    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|      |   0x08048478    0fb600         movzx eax, byte [eax]
|      |   0x0804847b    31c8           xor eax, ecx
|      |   0x0804847d    8802           mov byte [edx], al
|      |   0x0804847f    83450801       add dword [ebp + 8], 1
|      |   0x08048483    8b45fc         mov eax, dword [ebp-local_1]
|      |   0x08048486    8d5001         lea edx, dword [eax + 1]        ; 0x1  ; 1
|      |   0x08048489    89d0           mov eax, edx
|      |   0x0804848b    c1f81f         sar eax, 0x1f
|      |   0x0804848e    c1e81e         shr eax, 0x1e
|      |   0x08048491    01c2           add edx, eax
|      |   0x08048493    83e203         and edx, 3
|      |   0x08048496    29c2           sub edx, eax
|      |   0x08048498    89d0           mov eax, edx
|      |   0x0804849a    8945fc         mov dword [ebp-local_1], eax
|      |   ; JMP XREF from 0x0804845e (sym.checkSerial)
|      `-> 0x0804849d    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|          0x080484a0    0fb600         movzx eax, byte [eax]
|          0x080484a3    84c0           test al, al
|          0x080484a5    75b9           jne 0x8048460                 
|          0x080484a7    8b450c         mov eax, dword [ebp + 0xc]      ; [0xc:4]=-1 ; 12
|          0x080484aa    c9             leave
\          0x080484ab    c3             ret
```

So this looks very promising. There are no additional calls to other functions, only some calculations based on the username (`arg_2`) and serial (`arg_3`) are made. The function returns the value in the password-variable, and since we know that this value has to be 0 to succeed, we must try to figure out what happens there.

We also have 2 local variables, so we could now try to figure out what they are doing. But since we have a very small program here, we skip this and try to convert the program block-wise to pseudo c-code.

```
0x08048451    c745fc000000.  mov dword [ebp-local_1], 0
0x08048458    8d450c         lea eax, dword [ebp + 0xc]      ; 0xc  ; 12
0x0804845b    8945f8         mov dword [ebp-local_2], eax
0x0804845e    eb3d           jmp 0x804849d     
```
This is only the initialization of the local variables and a jump to some test if the function is finished
```
local_1 = 0
local_2 = arg_2
goto test	
```

```
0x08048460    8b55fc         mov edx, dword [ebp-local_1]
0x08048463    8b45f8         mov eax, dword [ebp-local_2]
0x08048466    01c2           add edx, eax
0x08048468    8b4dfc         mov ecx, dword [ebp-local_1]
0x0804846b    8b45f8         mov eax, dword [ebp-local_2]
0x0804846e    01c8           add eax, ecx
0x08048470    0fb600         movzx eax, byte [eax]
0x08048473    89c1           mov ecx, eax
0x08048475    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
0x08048478    0fb600         movzx eax, byte [eax]
0x0804847b    31c8           xor eax, ecx
0x0804847d    8802           mov byte [edx], al
```
Here the `local_1`th byte of the serial(`local_2`) is loaded and XORed with a part of the username (`arg_2`). this also gives us a hint why the address of the serial (`arg_3`) is stored in a local variable (`local_2`) -> this may be because the serial is `unsigned long` and the serial may be `unsigned char` for easier indexing.
```
start:
local_2[local_1] = local_2[local_1] ^ *arg_2 ; 1 byte
```

```
0x0804847f    83450801       add dword [ebp + 8], 1
0x08048483    8b45fc         mov eax, dword [ebp-local_1]
0x08048486    8d5001         lea edx, dword [eax + 1]     ; edx = local_1+1
0x08048489    89d0           mov eax, edx                 ; eax = local_1+1
0x0804848b    c1f81f         sar eax, 0x1f                ; arithmethic shift, 31
0x0804848e    c1e81e         shr eax, 0x1e				  ; shift, 30
0x08048491    01c2           add edx, eax
0x08048493    83e203         and edx, 3					  ; edx%=4
0x08048496    29c2           sub edx, eax
0x08048498    89d0           mov eax, edx
0x0804849a    8945fc         mov dword [ebp-local_1], eax
```

This part may be a little confusing, but it is in fact an optimized version of a simple modulo operation of a signed integer. The only interesting part here is at line `0x08048493`, the rest is for preserving the sign.

```
arg_2++
local_1 = (local_1+1)%4
```

```
0x0804849d    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
0x080484a0    0fb600         movzx eax, byte [eax]
0x080484a3    84c0           test al, al
0x080484a5    75b9           jne 0x8048460                 
0x080484a7    8b450c         mov eax, dword [ebp + 0xc]      ; [0xc:4]=-1 ; 12
0x080484aa    c9             leave
0x080484ab    c3             ret
```
Here we have the test for the loop.
```
test:
if(arg_2 != 0)
	goto start

return arg_3
```


With this information, we can reconstruct a c-program of the verification:
```
int checkSerial(char *name, unsigned long serial)
{
	int local_1 = 0;
	unsigned char *local_2 = (unsigned car*)&serial;
	goto test;
start:
	local_2[local_1] = local_2[local_1] ^ *name ;
	name++;
	local_1 = (local_1+1)%4;
test:
	if(name != 0)
		goto start;
	
	return serial;
	}
```

We could now rename the local variables and restructure it, but when we look at the function, we see that the given serial is only XORed with the username ... so basically, when we input 0 as the serial, we would get the right serial number as the return value.

Let's give it a try:
- restart radare with `r2 -Ad ./ex1 user 0`
- set a breakpoint right after `sym.checkSerial` (`db 0x08048513`)
- check the content of `eax` (`dr`)
```
eip = 0x08048513
oeax = 0xffffffff
eax = 0x72657375
ebx = 0xffaacd50
ecx = 0x00000000
edx = 0x00000000
esp = 0xffaacd10
ebp = 0xffaacd38
esi = 0x00000003
edi = 0xf76b4000
eflags = 0x00000246
```
- input this value as serial (`./ex1 user 1919251317`)

```
Serial OK!
```

Exercise 2
----------

Since the main-function is basically the same as in exercise 1, we could just simply try to input 0 as serial again and grab the return value of `sym.checkSerial` as our serialnumber.

```
./ex1 user 2911637029

Serial WRONG!
```

Ok, that would have been a little bit too easy ... so we have to take a look at `sym.checkSerial` again (in Visual Mode - Graph View `V`, `V`):

```
                                                                                            =--------------------------=
                                                                                            |_   0x080484d8            |
                                                                                           _| mov eax, dword [ebp + 8] |
                                                                                         _/ | movzx eax, byte [eax]    |
                                                                                       _/   |/test al, al              |
                                                                                     _/     ||jne 0x8048493            |
                                                                                   _/       =--------------------------=
                            =-----------------------------=                      _/         /   _t f_____
                            |    0x0804844b               |                    _/           | _/         \___________
                            | push ebp                    |                  _/            /_/                       \__________
                            | mov ebp, esp                |                _/             _|                                  =----------------------------=
                            | sub esp, 0x10               |              _/             _/ |                                  |    0x080484e2              |
                            | mov word [ebp - 2], 0x1234  |            _/             _/  /                                   | mov eax, dword [ebp + 0xc] |
                            | mov word [ebp - 4], 0xabcd  |          _/             _/    |                                   | leave                      |
                            | lea eax, dword [ebp + 0xc]  |        _/             _/     /                                    | ret                        |
                            | mov dword [ebp - 8], eax    |      _/        =-----------------------------=                    =----------------------------=
                            | lea eax, dword [ebp + 0xc]  |    _/          | -[ 0x08048493 ]-            |
                            | mov dword [ebp - 0xc], eax  |   /            | mov eax, dword [ebp - 8]    |
                            | mov eax, dword [ebp - 8]    | _/             | lea edx, dword [eax + 2]    |
                            | movzx eax, word [eax]       |/               | mov eax, dword [ebp - 8]    |
                            | xor ax, word [ebp - 2]    _/|                | add eax, 2|                 |
                            | mov edx, eax            _/  |                | movzx ecx, word [eax]       |
                            | mov eax, dword [ebp - 8]    |                | mov eax, dword [ebp + 8]    |
                            | mov word [eax], dx  _/      |                | movzx eax, byte [eax]       |
                            | mov eax, dword [ebp - 8]    |                | cbw      |                  |
                            | lea edx, dword [eax + 2]    |                | add eax, ecx                |
                            | mov eax, dword [ebp - 8]    |                | mov word [edx], ax          |
                            | add eax, 2  _/              |                | mov eax, dword [ebp - 0xc]  |
                            | movzx eax, word [eax]       |                | movzx eax, byte [eax]       |
                            | xor ax, word [ebp - 4]      |                | mov edx, eax                |
                            | mov word [edx], ax          |                | mov eax, dword [ebp + 8]    |
                            | jmp 0x80484d8               |                | movzx eax, byte [eax]       |
                            =-----------------------------=                | xor eax, edx                |
                               v                                           | mov edx, eax                |
                                                                           | mov eax, dword [ebp - 0xc]  |
                                                                           | mov byte [eax], dl          |
                                                                           | mov eax, dword [ebp - 0xc]  |
                                                                           | add eax, 1                  |
                                                                           | movzx edx, byte [eax]       |
                                                                           | sub edx, 1                  |
                                                                           | mov byte [eax], dl          |
                                                                           | add dword [ebp + 8], 1      |
                                                                           =-----------------------------=
                                                                              v

```

We also see from the listing, that there are 2 more variables compared to the last exercise:
```
arg int arg_2        @ ebp+0x8                                                                                                                                                                        
arg int arg_3        @ ebp+0xc                                                                                                                                                                        
arg int arg_1165     @ ebp+0x1234                                                                                                                                                                     
arg int arg_10995_1  @ ebp+0xabcd                                                                                                                                                                     
var int local_2      @ ebp-0x8                                                                                                                                                                        
var int local_3      @ ebp-0xc       
```

Now let's see what happens block-per-block:
`0x0804844b`
```
0x0804844b    55             push ebp
0x0804844c    89e5           mov ebp, esp
0x0804844e    83ec10         sub esp, 0x10
0x08048451    66c745fe3412   mov word [ebp - 2], 0x1234      ; [0x1234:2]=0xffff 
0x08048457    66c745fccdab   mov word [ebp - 4], 0xabcd      ; [0xabcd:2]=0xffff 
0x0804845d    8d450c         lea eax, dword [ebp + 0xc]      ; 0xc  ; 12
0x08048460    8945f8         mov dword [ebp-local_2], eax
0x08048463    8d450c         lea eax, dword [ebp + 0xc]      ; 0xc  ; 12
0x08048466    8945f4         mov dword [ebp-local_3], eax
0x08048469    8b45f8         mov eax, dword [ebp-local_2]
0x0804846c    0fb700         movzx eax, word [eax]
0x0804846f    663345fe       xor ax, word [ebp - 2]
0x08048473    89c2           mov edx, eax
0x08048475    8b45f8         mov eax, dword [ebp-local_2]
0x08048478    668910         mov word [eax], dx
0x0804847b    8b45f8         mov eax, dword [ebp-local_2]
0x0804847e    8d5002         lea edx, dword [eax + 2]        ; 0x2  ; 2
0x08048481    8b45f8         mov eax, dword [ebp-local_2]
0x08048484    83c002         add eax, 2
0x08048487    0fb700         movzx eax, word [eax]
0x0804848a    663345fc       xor ax, word [ebp - 4]
0x0804848e    668902         mov word [edx], ax
0x08048491    eb45           jmp 0x80484d8                
```
This is the initialization of the variables. Since the operations on local_2 are of the size word, we assume that the type is unsigned short
```
unsigned short word1 = 0x1234;
unsigned short word2 = 0xabcd;
unsigned short *local_2 = (unsigned short*) &serial;
local_3 = &serial;

local_2[0] = local_2[0]^word1; // 0x08048469 - 0x08048478
local_2[1] = local_2[1]^word2; // 0x0804847b - 0x0804848e

goto test;
```

`0x08048493`
```
0x08048493    8b45f8         mov eax, dword [ebp-local_2]
0x08048496    8d5002         lea edx, dword [eax + 2]        ; 
0x08048499    8b45f8         mov eax, dword [ebp-local_2]
0x0804849c    83c002         add eax, 2
0x0804849f    0fb708         movzx ecx, word [eax]
0x080484a2    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
0x080484a5    0fb600         movzx eax, byte [eax]
0x080484a8    6698           cbw							; convert byte to word
0x080484aa    01c8           add eax, ecx
0x080484ac    668902         mov word [edx], ax
0x080484af    8b45f4         mov eax, dword [ebp-local_3]
0x080484b2    0fb600         movzx eax, byte [eax]
0x080484b5    89c2           mov edx, eax
0x080484b7    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
0x080484ba    0fb600         movzx eax, byte [eax]
0x080484bd    31d0           xor eax, edx
0x080484bf    89c2           mov edx, eax
0x080484c1    8b45f4         mov eax, dword [ebp-local_3]
0x080484c4    8810           mov byte [eax], dl
0x080484c6    8b45f4         mov eax, dword [ebp-local_3]
0x080484c9    83c001         add eax, 1
0x080484cc    0fb610         movzx edx, byte [eax]
0x080484cf    83ea01         sub edx, 1
0x080484d2    8810           mov byte [eax], dl
0x080484d4    83450801       add dword [ebp + 8], 1
```
Here is the serial verification algorithm, we can also assume, that local_3 is an `unsigned char` pointer to serial
```
loop:
local_2[1] = local_2[1] + *username; // 0x08048493 - 0x080484ac
local_3[0] = local_3[0] ^ *username; // 0x080484af - 0x080484c4
local_3[1]--; // 0x080484c6 - 0x080484d2
username++; // 0x080484d4
```

`0x080484d8`
```
0x080484d8    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
0x080484db    0fb600         movzx eax, byte [eax]
0x080484de    84c0           test al, al
0x080484e0    75b1           jne 0x8048493       
```
This is our loop-condition again
```
test:

if(*username!=0)
	goto loop;
```

`0x080484e2`
```
0x080484e2    8b450c         mov eax, dword [ebp + 0xc]      ; [0xc:4]=-1 ; 12
0x080484e5    c9             leave
0x080484e6    c3             ret
```
And here we return the serial number.
```
return serial;
```

So the checkSerial function is something like:
```
unsigned long checkSerial(char *username, unsigned long serial)
{
	unsigned short word1 = 0x1234;
	unsigned short word2 = 0xabcd;
	unsigned short *local_2 = (unsigned short*) &serial;
	unsigned char *local_3 = (unsigned char *) &serial;

	local_2[0] = local_2[0]^word1;
	local_2[1] = local_2[1]^word2;

	goto test;

loop:
	local_2[1] = local_2[1] + *username;
	local_3[0] = local_3[0] ^ *username;
	local_3[1]--;
	username++;

test:

	if(*username!=0)
		goto loop;

	return serial;
}
```

A serial-generator would look like:
```
unsigned long generateSerial(char *username)
{
	unsigned long serial=0;
	unsigned short word1 = 0x1234;
	unsigned short word2 = 0xabcd;
	unsigned short *local_2 = (unsigned short*) &serial;
	unsigned char *local_3 = (unsigned char *) &serial;	

	char *u=username;
	while(*username) username++;

goto test;
loop:
	username--;
	local_3[1]++;
	local_3[0] = local_3[0] ^ *username;
	local_2[1] = local_2[1] - *username;

test:
	if(username!=u)
		goto loop;

	local_2[0] = local_2[0]^word1;
	local_2[1] = local_2[1]^word2;
	return serial;
}

```

Using this function, we can generate a serial and test it:
```
./ex2 user 1435244069
Serial OK!
```
