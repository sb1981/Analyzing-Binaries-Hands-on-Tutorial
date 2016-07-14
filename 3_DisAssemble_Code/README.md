# (Dis)Assemble Code

An integral part of a binary analyzation framework is to convert machine-code in a more human readable form. Normally, this is done on-the-fly when using the program. However, sometimes you may need to do this manually. It is also very useful to convert some assembly code fragments into machine code, for example when you want to write patches for binaries.

## Exercise 1

Try to find out what platforms your framework supports for assemble and disassemble code

## Exercise 2

Compile the following code-fragments into the machine-code (x86):

`mov eax, 0xDEADBEEF` 

and

`push eax`  
`push ebx`  
`pop eax`  
`pop ebx`


## Exercise 3

Decompile the following machine-codes back into assembler-code:

`B4 00 B0 13 CD 10` into x86 assembly code
