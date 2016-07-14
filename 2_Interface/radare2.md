Radare2 Interface
=================

Radare2 is a software suite for analyzing binaries, which consist of the several tools (see below) around the 'main-program' radare2.

In comparison to IDA, it is quiet hard to learn, because it basically only a command-line interface and no real GUI to work with. There are some improvements, like the Visual-Mode or the Web-GUI, but they don't offer the same functionality as the command-line interface (yet).

There also exists a GUI frontend called 'Bokken', which is available [here](https://inguma.eu/projects/bokken). However, the solutions to the following exercises are done in plain Radare2.

Radare2 is Open-Source and very actively developed, so there are still some features added or changed. 
It is available on all major platforms and is also included in security-distribution like Kali Linux, so is is very handy if you know the basic usage of this software suite.

Useful links
------------

Homepage - [Official Radare2 Page](http://radare.org/)

Book - [The official Radare2-Book](https://www.gitbook.com/book/radare/radare2book/details)

Book - [Radare Explorations] (https://www.gitbook.com/book/monosource/radare2-explorations/details)

Tutorial - [Radare 2 in 0x1E minutes](https://blog.techorganic.com/2016/03/08/radare-2-in-0x1e-minutes/)

[Cheatsheet](https://github.com/pwntester/cheatsheets/blob/master/radare2.md)


Radare2 Suite
-------------

You can find a detailed description about the usage of the programs [here](https://github.com/pwntester/cheatsheets/blob/master/radare2.md#radare2-suite-commands)

Tool    | Description
--------|------------
rax2    | Base Convertion
rahash  | Entropy, hashes and checksums
radiff  | File diffing
rasm2   | Assembly-Disassembly
rafind2 | Search
ragg2   | Shellcode generator, C/opcode compiler
rabin2  | Executable analysis: symbols, imports, strings ...
rarun2  | Launcher to run programs with different environments, args, stdin, permissions, fds
r2pm    | Radare2 Package Manager will install external plugins, tools and extras


Command Line Parameters
----------

`r2 [parameter] binary [arguments]`

`-A` analyze executable at load time  
`-L` list of supported IO plugins  
`-c'cmds'` run r2 and execute commands  
`-q` exit after processing commands  
`-d` start debug mode  

Basic Commands
------------

`i` list information about the binary  
`V` switch to Visual Mode  
`aa` analyze all  
`afl` list functions  
`pdf @ offset` print disassembly function  
`iM` view entrypoint  
`afn newname address` rename function  
`afvn name newname` rename local variable  
`afan name newname` rename function argument  
`wx @ offset` write hex values in offset  
`psz n @ offset` print n zero terminated String  
`px n @ offset` print hexdump (or just x) of n bytes  
`s offset` move cursor to address or symbol 
`Ps name` save project
`Po name` load project

### Debug

`db address` set breakpoint  
`db -address` remove breakpoint  
`do` reopen program  
`dc` continue  
`ds` step in  
`dso` step out  
`dss` skip instruction  
`dp` shows debugged process, child processes and threads  
`dm` shows memory map  
`dbt` shows backtrace  
`dr` print registers  


Visual Mode
-----------

`p` / `P` cycle through visual modes
`v` analyze mode
`V` enter graph mode
`q` exit visual mode

Graph Mode
-----------

`w`/`s`/`a`/`d` navigate (faster with `shift`)  
`cursor` move node (faster with `shift`)  
`t`/`f`/`u` walk around the nodes  
`O` show bytes/flags  
`z`/`Z` debug within them  
`x`/`X` jump around xrefs  

### Debug

`F2` toggle breakpoints  
`F7` / `s` single-step  
`F8` / `S` step-over  
`F9` continue  


Misc
----

### Stack

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
