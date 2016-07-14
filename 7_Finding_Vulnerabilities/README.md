Finding Vulnerabilities
=======================

When analyzing binaries, you may want to know if there are some vulnerabilities you may take care of.
When you have access to the source code, you usually begin to look for potential problems there, but it is also possible to look at the binary directly with the help of a binary analyzation tool.

It may be helpful to try to figure out how the source-code would look like, but this exercises should be possible to solve without it. You have to see, how the input-data is handled and how the memory changes depending on the input.


Exercise 1
----------

Try to find an argument-string, so that the program shows you the 'WIN' output.

The usage of this program is `./ex1`

Hint: look at the buffer sizes

[download binary (zip)](ex1.zip)

Exercise 2
----------

Again, try to find an argument-string, so that the program shows you the 'WIN' output.

The usage of this program is `./ex2 password`

Hint: look at the format-string documentation.

[download binary (zip)](ex2.zip)

