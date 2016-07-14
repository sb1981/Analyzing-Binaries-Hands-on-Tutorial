Solutions for Radare2
=====================

As we saw in the exercises for '2. Understanding Algorithms', finding out what a binary is doing can be very time-consuming and only guessing what a program does is also not the best way to analyze a binary. So we use a debugger in addition to the features of Radare2, we used before.

To start radare2 in debugger-mode we have to add a -d as parameter. In addition with the analyzation parameter we now have `r2 -A -d binary`. When radare2 starts, it displays some information about the process like pid and tid.

It is also possible to connect radare2 to other debuggers, but for our purposes this is not needed now.

The basic commands in the debugger are:

`db address` Set breakpoint  
`db -address` Remove breakpoint  
`do` Reopen program  
`dc` Continue  
`ds` Step in  
`dso` Step out  
`dss` Skip instruction  
`dp` Shows debugged process, child processes and threads  
`dm` Shows memory map  
`dbt` Shows backtrace

_Please note that not all trivial tasks will be shown in the solution (for example finding the main-function), since this is handled in other exercises. However, the commands used are still given._

Exercise 1
----------

Ok, there would be an easy solution for this by just using `ltrace ./ex1 test` to find the password, but let's use the radare2 debugger :)

Let's now start radare2 with `r2 -A -d ./ex2`. This gives us the following information (pid and tid may differ):
```
Process with PID 6324 started...
PID = 6324
pid = 6324 tid = 6324
r_debug_select: 6324 6324
Using BADDR 0x8048000
Asuming filepath ./ex1
bits 32
pid = 6324 tid = 6324
```

So now we just try to find out what happens when we run the program with `dc`:
```
Usage: ./ex1 password
r_debug_select: 6324 1
```

Ok, this was not going well, perhaps we should pass some arguments to the program. This can be done with the help of `rarun2`:

`r2 -Ad ./ex1 testpass`

Running the program now gives us this output:

Perhaps we are lucky and find the password as a string in the data-section of the binary with `iz`:

```
vaddr=0x08048600 paddr=0x00000600 ordinal=000 sz=20 len=19 section=.rodata type=a string=Usage: %s password\n
vaddr=0x08048614 paddr=0x00000614 ordinal=001 sz=15 len=14 section=.rodata type=a string=Wrong password
vaddr=0x08048623 paddr=0x00000623 ordinal=002 sz=12 len=11 section=.rodata type=a string=Password OK
```

Well, that was obviously not such an easy solution, even if we look at all strings in the binary with `izz` we won't find anything useful.

Perhaps we get lucky when we look at the binary (`afl` `s main` `pdf`)

The most interesting line here is:
```
 0x08048537    e8c4fdffff     call sym.imp.strcmp             ; sub.strcmp_92_2fc+0x4 ;sub.strcmp_92_2fc(unk, unk) ; sym.imp.strcmp
```
Which is a string-comparison and depending on the result it displays either `Password OK` or `Wrong password`

So lets set a breakpoint here with `db 0x08048537` and run the program with `dc`. This should give us the following output, which indicates that we hit the breakpoint:
```
hit breakpoint at: 8048537
r_debug_select: 6759 1
```

Since this is a 32-bit Linux binary, the arguments for the function-call are on the stack. So we look at the strings at the last 2 addresses on the stack (print 0-terminated string - `psz @ address`):

```
[0x08048537]> psz @ [esp]
testpass
[0x08048537]> psz @ [esp+4]
GoodJob!
```

We know, that `testpass` was our given argument, so `GoodJob!` is our solution!

Exercise 2
----------

Now we have to find out why the program crashes. So let's just load it with `r2 -Ad ./ex2` and start it (`dc`) to see what happens.

```
Array 1: 1 2 3 4 5 6 7 8 9 
Reversed array 1: 9 8 7 6 5 4 3 2 1 

[+] SIGNAL 11 errno=0 addr=0x47 code=1 ret=0
r_debug_select: 6784 1
[+] signal 11 aka SIGSEGV received 0
```

Well, this doesn't look good, but at least some of the code was executed. But first we take a look at the error itself. It says `signal 11 aka SIGSEGV`, which is a segment fault. So we might be out of boundaries here.

Now we look where the error happens with `pdf`. This is in the function `sym.reverse` at address `0x080485ec`.

```
/ (fcn) sym.reverse 117
|          ; arg int arg_2        @ ebp+0x8
|          ; arg int arg_3        @ ebp+0xc
|          ; var int local_1      @ ebp-0x4
|          ; var int local_2      @ ebp-0x8
|          ; var int local_3      @ ebp-0xc
|          ; CALL XREF from 0x080484eb (sym.main)
|          ; CALL XREF from 0x08048541 (sym.main)
|          ;-- sym.reverse:
|          0x080485c5    55             push ebp
|          0x080485c6    89e5           mov ebp, esp
|          0x080485c8    83ec10         sub esp, 0x10
|          0x080485cb    c745fc000000.  mov dword [ebp-local_1], 0
|          0x080485d2    8b450c         mov eax, dword [ebp + 0xc]      ; [0xc:4]=-1 ; 12
|          0x080485d5    83e801         sub eax, 1
|          0x080485d8    8945f8         mov dword [ebp-local_2], eax
|      ,=< 0x080485db    eb52           jmp 0x804862f                 
|      |   ; JMP XREF from 0x08048635 (sym.reverse)
|      |   0x080485dd    8b45fc         mov eax, dword [ebp-local_1]
|      |   0x080485e0    8d1485000000.  lea edx, dword [eax*4]
|      |   0x080485e7    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|      |   0x080485ea    01d0           add eax, edx
|      |   ;-- eip:
|      |   0x080485ec    8b00           mov eax, dword [eax]
|      |   0x080485ee    8945f4         mov dword [ebp-local_3], eax
|      |   0x080485f1    8b45fc         mov eax, dword [ebp-local_1]
|      |   0x080485f4    8d1485000000.  lea edx, dword [eax*4]
|      |   0x080485fb    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|      |   0x080485fe    01c2           add edx, eax
|      |   0x08048600    8b45f8         mov eax, dword [ebp-local_2]
|      |   0x08048603    8d0c85000000.  lea ecx, dword [eax*4]
|      |   0x0804860a    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|      |   0x0804860d    01c8           add eax, ecx
|      |   0x0804860f    8b00           mov eax, dword [eax]
|      |   0x08048611    8902           mov dword [edx], eax
|      |   0x08048613    8b45f8         mov eax, dword [ebp-local_2]
|      |   0x08048616    8d1485000000.  lea edx, dword [eax*4]
|      |   0x0804861d    8b4508         mov eax, dword [ebp + 8]        ; [0x8:4]=-1 ; 8
|      |   0x08048620    01c2           add edx, eax
|      |   0x08048622    8b45f4         mov eax, dword [ebp-local_3]
|      |   0x08048625    8902           mov dword [edx], eax
|      |   0x08048627    8345fc01       add dword [ebp-local_1], 1
|      |   0x0804862b    836df801       sub dword [ebp-local_2], 1
|      |   ; JMP XREF from 0x080485db (sym.reverse)
|      `-> 0x0804862f    8b45fc         mov eax, dword [ebp-local_1]
|          0x08048632    3b45f8         cmp eax, dword [ebp-local_2]
|          0x08048635    75a6           jne 0x80485dd                 
|          0x08048637    90             nop
|          0x08048638    c9             leave
\          0x08048639    c3             ret
```

So let's find out how this function is working. Obviously, it is some sort of reverse algorithm and has 2 arguments and 3 local variables.

Then we set a breakpoint at the start of the function (but after the ebp saving) and try to figure out what's in these arguments and variables (`db 0x080485c8` `do` `dc`)

We can view the actual values of the registers with `dr`
```
eip = 0x080485c8
oeax = 0xffffffff
eax = 0xff9d00b4
ebx = 0x00000000
ecx = 0x7ffffffe
edx = 0xf7775870
esp = 0xff9d0068
ebp = 0xff9d0068
esi = 0x00000001
edi = 0xf7774000
eflags = 0x00000296
```

Since we know that the 2 arguments are on the stack, we can view the contents with `px 8 @ 0xff9d0068+0x8` (print 8 bytes (2 items on the stack) in hex-format from address 0xff9d0068+0x8)

```
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xff9d0070  b400 9dff 0900 0000                      ........        
```

The first item (`arg_2`) is most likely a pointer to `0xff9d00b4` an the second (`arg_3`) a number (`9`)

Now we take a look what the first item points to (`px @ [0xff9d0068+0x8]`)

```
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xff9d00b4  0100 0000 0200 0000
```

Well ... here we have the numbers 1 and 2 in the first 2 32-bit values. Let's try to display 9 (arg_3) 32-bit values here (`px 9*4 @ [0xff9d0068+0x8]`).

```
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xff9d00b4  0100 0000 0200 0000 0300 0000 0400 0000  ................
0xff9d00c4  0500 0000 0600 0000 0700 0000 0800 0000  ................
0xff9d00d4  0900 0000                                ....            
```

When we remember, the program output was 
```
Array 1: 1 2 3 4 5 6 7 8 9 
Reversed array 1: 9 8 7 6 5 4 3 2 1 
```
before the program crashed. So it seems, that `arg_2` is an array with numbers and `arg_3` is number of items in this array.

We also see, that the program also outputs the reversed array, so let's continue the program and see what happens (`dc`)

```
Array 1: 1 2 3 4 5 6 7 8 9 
Reversed array 1: 9 8 7 6 5 4 3 2 1 

hit breakpoint at: 80485c8
```

Now that is interesting, there was no `SIGSEGV`, but instead we hit our breakpoint again. This means, that we function successfully completes with the arguments above. So now, we take a look at the arguments again (ebp was the same).

`px 8 @ 0xff9d0068+0x8`
```
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xff9d0070  8c00 9dff 0a00 0000                      ........        
```

`px 0xa*4 @ [0xff9d0068+0x8]`
```
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xff9d008c  0100 0000 0200 0000 0300 0000 0400 0000  ................
0xff9d009c  0500 0000 0600 0000 0700 0000 0800 0000  ................
0xff9d00ac  0900 0000 0000 0000                      ........        
```

and continue the execution (`dc`)

Ah, now we have the crash ... but why?

Well, the only difference is that we now have 10 numbers instead of the 9 in the first execution of this function. To figure out what happened, we must go a little deeper here. Let's take a look on the local variables:

- `local_1` - is initialized with `0` (`0x080485cb`)
- `local_2` - is initialized with `length-1` (`0x080485d2 -  0x080485d8`)
- `local_3` - is used for some swapping of variables of the array, indexed by `local_1` and `local_2` (` 0x080485dd` - `0x08048625`)

We also see, that `local_1` and `local_2` are incremented/decremented and then there is a comparison if they are equal
(`0x08048627` - `0x08048635`).

And here we might have our problem, the conditional jump at `0x08048635` is a `jne`, but this can cause problems, because this only works on odd lengths of arrays. We also can confirm this behavior with the data we have extracted above. To fix this, the `jne` would have to be replaced by a `js`.

Actually, we can already do this here:
- set the breakpoint at the beginning of the function (`db 0x080485c8`)
- start the program `dc`
- step over the first execution of the function `dc`
- set a breakpoint at the faulty jump (`db 0x08048635`)
- continue and check, if we are at the right position (`dc` `dr` `pdf`)
- replace the instruction (`wx 7ca6` - `jne` is `75` / `jne` is `7c`) and verify the changes (`pdf`)
- remove the breakpoint (`db -0x08048635`)
- continue the program `dc`

```
[0x080485ec]> do
Wait event received by different pid 7167
Wait event received by different pid 7168
Process with PID 7170 started...
PID = 7170
File dbg://./ex2 reopened in read-write mode
r_debug_select: 7170 7170
Asuming filepath ./ex2
pid = 7170 tid = 7170
[0xf773ca90]> dc
hit breakpoint at: 80485d8
r_debug_select: 7170 1
[0x080485d8]> dc
Array 1: 1 2 3 4 5 6 7 8 9 
Reversed array 1: 9 8 7 6 5 4 3 2 1 

hit breakpoint at: 80485d8
[0x080485d8]> db  0x08048635
[0x080485d8]> dc
hit breakpoint at: 8048635
[0x08048635]> wx 7ca6
[0x08048635]> db  -0x08048635
[0x08048635]> dc
Array 2: 1 2 3 4 5 6 7 8 9 0 
Reversed array 2: 0 9 8 7 6 5 4 3 2 1 
[0xf773ad49]> 
```

Exercise 3
----------

So, the usage of this program is `./ex3 number`, so why we don't start it with a string instead `r2 -Ad ./ex3 test` and then start it `dc`

```
r_debug_select: 7326 1
[+] signal 8 aka SIGFPE received 0
```
Ok, we now get a floating point exception. Let's take a look where this happens:

`
[0xf77cda90]> pdf
Cannot find function at 0xf77cda90
`

Ok, this didn't work as we planned ... perhaps we should take another approach. First we reload the program with `do`. Then we will go through the program step by step, so we need a breakpoint at the beginning of the main function (`db main` and verify with `s main`, `pdf`). Now we start the program (`dc`)
```
hit breakpoint at: 804842b
r_debug_select: 7335 1
```

To make this more comfortable, we switch to the visual mode - debug view (`V`, `p`)

Here we can scroll through the binary with the cursor keys. Let's try this to get a hint what the program does.

We see a help-text at the beginning, then a call to a function called `sym.getInt` and then some calculation and returning the result. So let's see what `sym.getInt`

The commands for debugging in visual mode are (remember, by pressing `;`, you can get a console to input radare2 commands):

`F2` toggle breakpoints  
`F7` / `s` single-step  
`F8` / `S` step-over  
`F9` continue    
`c` changes to of exit cursor mode  

Now we use step-over (`F8`) to see if anything interesting happens in the main-function.

`sym.getInt` gets `argv[1]` as input-parameter and returns `0x00012136` for the input `test`. So the input is not really checked inside the function.

Now let's start radare2 again, but this time with a more useful argument for the program (`q`, `r2 -Ad ./ex3 1234`, `s main`, `db main`, `dc`, `V`, `p`)

`sym.getInt` returns `0x000004d2`, which is `1234` in decimal. so the function works for the right input. The result is then stored in `local_3`

Next, there happens some 'magic' with bit-operators, which would be very painful to figure it out by our self. So we check, what is stored in the local-variables.

  Position | Variable |     Value
-----------|----------|-------------------
0x08048475 | local_3  |  0x000004d2 (1234)
0x08048490 | local_4  |  0x00000001 (1)
0x0804849a | local_3  |  0x000004d2 (1234)
0x080484ab | local_4  |  0x00000000 (0)

crash at the division at `0x080484b2`.

Now we start over again, but with another number. We try a larger one, because local_4 gets derived from local_3, so we may guess, that there is perhaps a division there.

  Position | Variable |     Value
-----------|----------|-------------------
0x08048475 | local_3  |  0x00003039 (12345)
0x08048490 | local_4  |  0x0000000C (12)
0x0804849a | local_3  |  0x00000404 (1028)
0x080484ab | local_4  |  0x00000000 (0)

  Position | Variable |     Value
-----------|----------|-------------------
0x08048475 | local_3  |  0x0001e240 (123456)
0x08048490 | local_4  |  0x0000007B (123)
0x0804849a | local_3  |  0x000003eb (1003)
0x080484ab | local_4  |  0x00000001 (1)

wow, this was actually working! (return code 0), why?

We try to make some c-code (check the instructions to get some additional info beside the debug-output from the 3 different arguments):

```
local_3 = getInt(argv[1]);
local_4 = local_3/1000;
local_3 /= local_4;
local_4 = local_3%2;
```

Then we also have some block for loading a character of `argv[1]` and multiply it with `local_3`, which is then the return value.

```
local_3 *= argv[1][6];
```

So for the program to finish with return code 0 and without crashing, the number must have exactly 7 digits (so the first potential division-error is also handled with, because there the number must be > 1000), and the input/1000 also have to be an odd number. Interestingly, we could also input some other characters than numbers, and the program may also finish successfully :)