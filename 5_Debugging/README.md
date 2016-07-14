Debugging
=========

With the help of a disassembler, it is possible to understand what a binary is doing. But it is sometimes more helpful to see how the program behaves while running. This is where a debugger can be used. With it, you can figure out why a program crashes and look what is in the registers and the memory at a given point of the execution (very helpful to see with which parameters a function is called). It is also possible to step though each single instruction, so it is easier to find out where the error is really happening.

Exercise 1
----------

The provided program makes a small password-check and displays if the input was the right password or not. Try to figure out the right password.

Hint: check the c function-calls and their parameters

The usage of this program is `./ex1 password`

[download binary (zip)](ex1.zip)

Exercise 2
----------

This example crashes when it is executed. Try to figure out where this happens and what error the developer made.

The usage of this program is `./ex2`

[download binary (zip)](ex2.zip)

Exercise 3
----------

In this example, the program gets a number as input, does some calculation with it and should exit with the code 0.

However, there are some bugs, which let the program crash, or you may also receive the wrong exit-code. So try to find out which input actually works and returns 0.

The usage of this program is `./ex3 number`

[download binary (zip)](ex3.zip)

